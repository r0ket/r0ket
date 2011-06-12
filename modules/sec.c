#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/allfonts.h"

void ReinvokeISP(void);
void EnableWatchdog(uint32_t ms);
void delayms(uint32_t ms);

/**************************************************************************/

void module_sec(void) {
    //Make PIO1_11 an analog input
    gpioSetDir(RB_LED3, gpioDirection_Input);
    IOCON_PIO1_11 = 0x41;

    backlightInit();

    uint32_t j=0;

    //disable the JTAG on PIO3_3
    IOCON_PIO3_3 = 0x10;

    int yctr=8;
    int dx=0;

    font_direction = FONT_DIR_LTR; // LeftToRight is the default
    font = &Font_8x8;

    static FONT fonts[] = {
	 &Font_7x8, 
	 &Font_Ubuntu18pt, // 3 byte-font
	 &Font_8x8,
    };

    int fontctr=0;
    yctr=18;

    uint8_t trigger;

#define SEND
#ifdef SEND
  trigger=200;
  gpioSetDir(RB_LED0, gpioDirection_Output);
  IOCON_JTAG_TDI_PIO0_11 = 0x11;
#else
  trigger=380;
  gpioSetDir(RB_LED0, gpioDirection_Input);
  IOCON_JTAG_TDI_PIO0_11 = 0x42;
#endif


    uint32_t ctr=0;
    while (1) {
	ctr++;
	uint32_t results;
	lcdDisplay(j);
	delayms(10);

	font=fonts[fontctr];

	if(gpioGetValue(RB_BTN3)==0){
		while(gpioGetValue(RB_BTN3)==0);
		trigger +=10;
	};
	if(gpioGetValue(RB_BTN2)==0){
		while(gpioGetValue(RB_BTN2)==0);
		trigger -=10;
	};
	dx=DoString(0,0,"Trig:");
	dx=DoInt(dx,0,trigger);
	DoString(dx,0," ");

	if(gpioGetValue(RB_BTN0)==0){
		while(gpioGetValue(RB_BTN0)==0);
		DoString(0,8,"Enter ISP!");
		lcdDisplay(0);
		EnableWatchdog(1000*5);
		ReinvokeISP();
	};

    font = &Font_Ubuntu36pt;
	dx=DoString(0,0,"Sec");
#ifdef SEND
	if(ctr++>trigger/10){
		ctr=0;
		if (gpioGetValue(RB_LED0) == CFG_LED_OFF){
			gpioSetValue (RB_LED0, CFG_LED_ON); 
//			DoString(dx,14,"ON!");
		} else {
			gpioSetValue (RB_LED0, CFG_LED_OFF); 
//			DoString(dx,14,"off");
		};
	};
#else
	results = adcRead(0);
	DoInt(dx,20,results);

	if(results>trigger){
		DoString(dx,30,"YES!");
	}else{
		DoString(dx,30," no ");
	};

#endif
	font = &Font_7x8;

	results = adcRead(1);
	dx=DoString(0,yctr+28,"Voltage:");
	results *= 10560;
	results /= 1024;
	DoInt(dx,yctr+28,results);

	if( results < 3500 ){
	    DoString(0,yctr+30,"Shutdown");
	    gpioSetValue (RB_PWR_GOOD, 0); 
	    gpioSetValue (RB_LCD_BL, 0); 
	    SCB_SCR |= SCB_SCR_SLEEPDEEP;
	    PMU_PMUCTRL = PMU_PMUCTRL_DPDEN_DEEPPOWERDOWN;
	    __asm volatile ("WFI");
	}else{
	    //DoString(0,yctr+30,"OK           ");
		;
	}

    }

    return;
}
