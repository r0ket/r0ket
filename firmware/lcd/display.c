#include <string.h>

#include <display.h>
#include <sysdefs.h>
#include "lpc134x.h"
#include "core/ssp/ssp.h"
#include "gpio/gpio.h"
#include "basic/basic.h"
#include "basic/config.h"
#include "usb/usbmsc.h"

enum display_type {
	DISPLAY_TYPE_N1200 = 0,
	DISPLAY_TYPE_N1600 = 1,
};

enum command_type {
	COMMAND_TYPE_CMD = 0,
	COMMAND_TYPE_DATA = 1,
};

struct display_macro  {
	enum command_type type:1;
	unsigned int data:8;
	unsigned int delay_after:7;
} __attribute__((packed));

/* Small Nokia 1200 LCD docs:
 *           clear/ set
 *  on       0xae / 0xaf
 *  invert   0xa6 / 0xa7
 *  mirror-x 0xA0 / 0xA1
 *  mirror-y 0xc7 / 0xc8
 *
 *  0x20+x contrast (0=black - 0x2e)
 *  0x40+x offset in rows from top (-0x7f)
 *  0x80+x contrast? (0=black -0x9f?)
 *  0xd0+x black lines from top? (-0xdf?)
 *
 */
/* Decoded:
 * E2: Internal reset
 * AF: Display on/off: DON = 1
 * A1: undefined?
 * A4: all on/normal: DAL = 0
 * 2F: charge pump on/off: PC = 1
 * B0: set y address: Y[0-3] = 0
 * 10: set x address (upper bits): X[6-4] = 0
 */
static const struct display_macro INIT_N1200[] = {
		{COMMAND_TYPE_CMD, 0xE2, 5},
		{COMMAND_TYPE_CMD, 0xAF},
		{COMMAND_TYPE_CMD, 0xA1},
		{COMMAND_TYPE_CMD, 0x2F},
		{COMMAND_TYPE_CMD, 0xB0},
		{COMMAND_TYPE_CMD, 0x10},
};

static const struct display_macro PREPARE_N1200[] = {
		{COMMAND_TYPE_CMD, 0xB0},
		{COMMAND_TYPE_CMD, 0x10},
		{COMMAND_TYPE_CMD, 0x00},
};

/* Decoded:
 * CMD 29: DISPON
 * CMD BA: Data order (1)
 * CMD 11: sleep out
 * CMD 13: normal display mode on
 * CMD 37: set scroll entry point
 *   DAT 00:  scroll entry point
 * CMD 3A: interface pixel format
 *   DAT 05: 16 bit/pixel
 * CMD 2A: column address set
 *   DAT 0    : xs
 *   DAT 98-1 : xe
 * CMD 2B: page address set
 *   DAT 0    : ys
 *   DAT 70-1 : ye
 */
static const struct display_macro INIT_N1600[] = {
		{COMMAND_TYPE_CMD, 0x01, 10},
		{COMMAND_TYPE_CMD, 0x29},
		{COMMAND_TYPE_CMD, 0xBA},
		{COMMAND_TYPE_CMD, 0x11},
		{COMMAND_TYPE_CMD, 0x13},
		{COMMAND_TYPE_CMD, 0x37},
		{COMMAND_TYPE_DATA, 0x00},
		{COMMAND_TYPE_CMD, 0x3A},
		{COMMAND_TYPE_DATA, 0x05},
		{COMMAND_TYPE_CMD, 0x2A},
		{COMMAND_TYPE_DATA, 0x00},
		{COMMAND_TYPE_DATA, 98-1},
		{COMMAND_TYPE_CMD, 0x2B},
		{COMMAND_TYPE_DATA, 0},
		{COMMAND_TYPE_DATA, 70-1},
};

static const struct display_macro PREPARE_N1600[] = {
		{COMMAND_TYPE_CMD, 0x2C},
};


struct display_descriptor {
	/* Macro to execute in order to initialize the display from scratch */
	const struct display_macro * init_macro;
	int init_macro_length;

	/* Macro to execute to prepare the display for sending raw contents */
	const struct display_macro * prepare_macro;
	int prepare_macro_length;
};

const struct display_descriptor DISPLAY_DESCRIPTORS[] = {
		[DISPLAY_TYPE_N1200] = {
				INIT_N1200, sizeof(INIT_N1200)/sizeof(INIT_N1200[0]),
				PREPARE_N1200, sizeof(PREPARE_N1200)/sizeof(PREPARE_N1200[0])
		},
		[DISPLAY_TYPE_N1600] = {
				INIT_N1600, sizeof(INIT_N1600)/sizeof(INIT_N1600[0]),
				PREPARE_N1600, sizeof(PREPARE_N1600)/sizeof(PREPARE_N1600[0])
		},
};

/**************************************************************************/
/* Utility routines to manage nokia display */
/**************************************************************************/

uint8_t lcdBuffer[RESX*RESY_B];
uint32_t intstatus; // Caches USB interrupt state
                    // (need to disable MSC while displaying)
enum display_type displayType;
const struct display_descriptor *displayDescriptor;


static void lcd_select() {
#if CFG_USBMSC
    if(usbMSCenabled){
        intstatus=USB_DEVINTEN;
        USB_DEVINTEN=0;
    };
#endif
    /* the LCD requires 9-Bit frames */
    uint32_t configReg = ( SSP_SSP0CR0_DSS_9BIT     // Data size = 9-bit
                  | SSP_SSP0CR0_FRF_SPI             // Frame format = SPI
                  | SSP_SSP0CR0_SCR_8);             // Serial clock rate = 8
    SSP_SSP0CR0 = configReg;
    gpioSetValue(RB_LCD_CS, 0);
}

static void lcd_deselect() {
    gpioSetValue(RB_LCD_CS, 1);
    /* reset the bus to 8-Bit frames that everyone else uses */
    uint32_t configReg = ( SSP_SSP0CR0_DSS_8BIT     // Data size = 8-bit
                  | SSP_SSP0CR0_FRF_SPI             // Frame format = SPI
                  | SSP_SSP0CR0_SCR_8);             // Serial clock rate = 8
    SSP_SSP0CR0 = configReg;
#if CFG_USBMSC
    if(usbMSCenabled){
        USB_DEVINTEN=intstatus;
    };
#endif
}

static void lcdWrite(uint8_t cd, uint8_t data) {
    uint16_t frame = 0x0;

    frame = cd << 8;
    frame |= data;

    while ((SSP_SSP0SR & (SSP_SSP0SR_TNF_NOTFULL | SSP_SSP0SR_BSY_BUSY)) != SSP_SSP0SR_TNF_NOTFULL);
    SSP_SSP0DR = frame;
    while ((SSP_SSP0SR & (SSP_SSP0SR_BSY_BUSY|SSP_SSP0SR_RNE_NOTEMPTY)) != SSP_SSP0SR_RNE_NOTEMPTY);
    /* clear the FIFO */
    frame = SSP_SSP0DR;
}

#define CS 2,1
#define SCK 2,11
#define SDA 0,9
#define RST 2,2

uint8_t lcdRead(uint8_t data)
{
    uint32_t op211cache=IOCON_PIO2_11;
    uint32_t op09cache=IOCON_PIO0_9;
    uint32_t dircache=GPIO_GPIO2DIR;
    IOCON_PIO2_11=IOCON_PIO2_11_FUNC_GPIO|IOCON_PIO2_11_MODE_PULLUP;
    IOCON_PIO0_9=IOCON_PIO0_9_FUNC_GPIO|IOCON_PIO0_9_MODE_PULLUP;
    gpioSetDir(SCK, 1);

    uint8_t i;

    gpioSetDir(SDA, 1);
    gpioSetValue(SCK, 0);
    gpioSetValue(CS, 0);
    delayms(1);

    gpioSetValue(SDA, 0);
    gpioSetValue(SCK, 1);
    delayms(1);
    
    for(i=0; i<8; i++){
        gpioSetValue(SCK, 0);
        delayms(1);
        if( data & 0x80 )
            gpioSetValue(SDA, 1);
        else
            gpioSetValue(SDA, 0);
        data <<= 1;
        gpioSetValue(SCK, 1);
        delayms(1);
    }
    uint8_t ret = 0;

    gpioSetDir(SDA, 0);
    for(i=0; i<8; i++){
        gpioSetValue(SCK, 0);
        delayms(1);
        ret <<= 1;
        ret |= gpioGetValue(SDA);
        gpioSetValue(SCK, 1);
        delayms(1);
    }
    gpioSetValue(SCK, 0);

    gpioSetValue(CS, 1);
    gpioSetDir(SDA, 1);
    IOCON_PIO2_11=op211cache;
    IOCON_PIO0_9=op09cache;
    GPIO_GPIO2DIR=dircache;
    delayms(1);
    return ret;
}

static void lcdExecuteMacro(const struct display_macro *macro, int macro_length)
{
    for(int i=0; i<macro_length; i++) {
    	lcdWrite(macro[i].type, macro[i].data);
    	delayms(macro[i].delay_after);
    }
}

void lcdInit(void) {
    int id;

    sspInit(0, sspClockPolarity_Low, sspClockPhase_RisingEdge);

    gpioSetValue(RB_LCD_CS, 1);
    gpioSetValue(RB_LCD_RST, 1);

    gpioSetDir(RB_LCD_CS, gpioDirection_Output);
    gpioSetDir(RB_LCD_RST, gpioDirection_Output);

    delayms(100);
    gpioSetValue(RB_LCD_RST, 0);
    delayms(100);
    gpioSetValue(RB_LCD_RST, 1);
    delayms(100);

    id=lcdRead(220); // ID3
    
    if(id==14)
        displayType=DISPLAY_TYPE_N1600;
    else /* ID3 == 48 */
        displayType=DISPLAY_TYPE_N1200;
    
    displayDescriptor = DISPLAY_DESCRIPTORS + displayType;

    lcd_select();
    lcdExecuteMacro(displayDescriptor->init_macro, displayDescriptor->init_macro_length);
    lcd_deselect();
}

void lcdFill(char f){
    memset(lcdBuffer,f,RESX*RESY_B);
#if 0
    int x;
    for(x=0;x<RESX*RESY_B;x++) {
        lcdBuffer[x]=f;
    }
#endif
};

void lcdSetPixel(char x, char y, bool f){
    if (x<0 || x> RESX || y<0 || y > RESY)
        return;
    char y_byte = (RESY-(y+1)) / 8;
    char y_off = (RESY-(y+1)) % 8;
    char byte = lcdBuffer[y_byte*RESX+(RESX-(x+1))];
    if (f) {
        byte |= (1 << y_off);
    } else {
        byte &= ~(1 << y_off);
    }
    lcdBuffer[y_byte*RESX+(RESX-(x+1))] = byte;
}

bool lcdGetPixel(char x, char y){
    char y_byte = (RESY-(y+1)) / 8;
    char y_off = (RESY-(y+1)) % 8;
    char byte = lcdBuffer[y_byte*RESX+(RESX-(x+1))];
    return byte & (1 << y_off);
}


// Color display hepler functions
static void _helper_pixel16(uint16_t color){
    lcdWrite(COMMAND_TYPE_DATA,color>>8);
    lcdWrite(COMMAND_TYPE_DATA,color&0xFF);
}

static void _helper_hline(uint16_t color){
    for(int cx=0;cx<98;cx++)
        _helper_pixel16(color);
}

#define COLORPACK_RGB565(r,g,b) (((r&0xF8) << 8) | ((g&0xFC)<<3) | ((b&0xF8) >> 3))

static const uint16_t COLOR_FG =    COLORPACK_RGB565(0x00, 0x60, 0x00);
static const uint16_t COLOR_BG =    COLORPACK_RGB565(0xff, 0xff, 0xff);
static const uint16_t COLOR_FRAME = COLORPACK_RGB565(0x00, 0x00, 0x80);


void lcdDisplay(void) {
    char byte;
    lcd_select();

	lcdExecuteMacro(displayDescriptor->prepare_macro, displayDescriptor->prepare_macro_length);

    if(displayType==DISPLAY_TYPE_N1200){
      uint16_t i,page;
      for(page=0; page<RESY_B;page++) {
          for(i=0; i<RESX; i++) {
              if (GLOBAL(lcdmirror))
                  byte=lcdBuffer[page*RESX+RESX-1-(i)];
              else
                  byte=lcdBuffer[page*RESX+(i)];
  
              if (GLOBAL(lcdinvert))
                  byte=~byte;
      
              lcdWrite(COMMAND_TYPE_DATA,byte);
          }
      }
    } else { /* displayType==DISPLAY_TYPE_N1600 */
      uint16_t x,y;
      bool px;
 
      //top line of the frame...
      _helper_hline(COLOR_FRAME);
  
      for(y=RESY;y>0;y--){
          //left line of the frame
          _helper_pixel16(COLOR_FRAME);
  
          for(x=RESX;x>0;x--){
              if(GLOBAL(lcdmirror))
                  px=lcdGetPixel(RESX-x+1,y-1);
              else
                  px=lcdGetPixel(x-1,y-1);
                  
              if((!px)^(!GLOBAL(lcdinvert))) {
            	  _helper_pixel16(COLOR_FG); /* foreground */
              } else {
            	  _helper_pixel16(COLOR_BG); /* background */
              }
  
          }
          //right line of the frame
          _helper_pixel16(COLOR_FRAME);
      }
      
      //bottom line of the frame
      _helper_hline(COLOR_FRAME);
      }
    lcd_deselect();
}

void lcdRefresh() __attribute__ ((weak, alias ("lcdDisplay")));

inline void lcdInvert(void) {
    GLOBAL(lcdinvert)=!GLOBAL(lcdinvert);
}

void lcdSetContrast(int c) {
    lcd_select();
    if(displayType==DISPLAY_TYPE_N1200){
        if(c<0x1F)
            lcdWrite(COMMAND_TYPE_CMD,0x80+c);
    }else{ /* displayType==DISPLAY_TYPE_N1600 */
        if(c<0x40) {
            lcdWrite(COMMAND_TYPE_CMD,0x25);
            lcdWrite(COMMAND_TYPE_DATA,4*c);
        };
    }
    lcd_deselect();
};

void lcdSetInvert(int c) {
    lcd_select();
     /* it doesn't harm N1600, save space */
//  if(displayType==DISPLAY_N1200)
        lcdWrite(COMMAND_TYPE_CMD,(c&1)+0xa6);
    lcd_deselect();
};

/* deprecated */
void __attribute__((__deprecated__)) lcdToggleFlag(int flag) {
    if(flag==LCD_MIRRORX)
        GLOBAL(lcdmirror)=!GLOBAL(lcdmirror);
    if(flag==LCD_INVERTED)
        GLOBAL(lcdinvert)=!GLOBAL(lcdinvert);
}

void lcdShiftH(bool right, bool wrap) {
	uint8_t tmp;
	for (int yb = 0; yb<RESY_B; yb++) {
		if (right) {
			tmp = lcdBuffer[yb*RESX];
			memmove(lcdBuffer + yb*RESX,lcdBuffer + yb*RESX+1 ,RESX-1);
            lcdBuffer[yb*RESX+(RESX-1)] = wrap?tmp:0;
		} else {
			tmp = lcdBuffer[yb*RESX+(RESX-1)];
			memmove(lcdBuffer + yb*RESX+1,lcdBuffer + yb*RESX ,RESX-1);
			lcdBuffer[yb*RESX] = wrap?tmp:0;
		}
	}
}

void lcdShiftV8(bool up, bool wrap) {
	uint8_t tmp[RESX];
	if (!up) {
		if (wrap)
            memmove(tmp, lcdBuffer, RESX);
        else
            memset(tmp,0,RESX);
		memmove(lcdBuffer,lcdBuffer+RESX ,RESX*(RESY_B-1));
		memmove(lcdBuffer+RESX*(RESY_B-1),tmp,RESX);
	} else {
		if (wrap)
            memmove(tmp, lcdBuffer+RESX*(RESY_B-1), RESX);
        else
            memset(tmp,0,RESX);
		memmove(lcdBuffer+RESX,lcdBuffer ,RESX*(RESY_B-1));
		memmove(lcdBuffer,tmp,RESX);
	}
}

void lcdShiftV(bool up, bool wrap) {
	uint8_t tmp[RESX];
	if (up) {
		if (wrap) 
            memmove(tmp,lcdBuffer+((RESY_B-1)*RESX),RESX);
        else
            memset(tmp,0,RESX);
		for (int x = 0; x<RESX; x++){
			for (int y = RESY_B-1; y > 0; y--){
				lcdBuffer[x+(y*RESX)] = (lcdBuffer[x+(y*RESX)] << 1) |( lcdBuffer[x+((y-1)*RESX)] >> 7);
			}
			lcdBuffer[x] = ( lcdBuffer[x] << 1) | ((tmp[x]>>3)&1);
		}
				
	} else {
		if (wrap)
            memmove(tmp,lcdBuffer,RESX);
        else
            memset(tmp,0,RESX);
		for (int x = 0; x<RESX; x++){
			for (int y = 0; y < (RESY_B-1); y++){
				lcdBuffer[x+(y*RESX)] = (lcdBuffer[x+(y*RESX)] >> 1) |( lcdBuffer[x+((y+1)*RESX)] << 7);
			}
			lcdBuffer[x+((RESY_B-1)*RESX)] = ( lcdBuffer[x+((RESY_B-1)*RESX)] >> 1) | ((tmp[x]<<3)&8);
		}	
	}
}	

void lcdShift(int x, int y, bool wrap) {
	bool dir=true;

    if(x<0){
        dir=false;
        x=-x;
    };

    while(x-->0)
        lcdShiftH(dir, wrap);

    if(y<0){
        dir=false;
        y=-y;
    }else{
        dir=true;
    };

    while(y>=8){
        y-=8;
        lcdShiftV8(dir, wrap);
    };

    while(y-->0)
        lcdShiftV(dir, wrap);
}

