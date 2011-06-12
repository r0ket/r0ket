#include <display.h>
#include <sysdefs.h>
#include "lpc134x.h"
#include "gpio/gpio.h"

/**************************************************************************/
/*! 
    Approximates a 1 millisecond delay using "nop".  This is less
    accurate than a dedicated timer, but is useful in certain situations.

    The number of ticks to delay depends on the optimisation level set
    when compiling (-O).  Depending on the compiler settings, one of the
    two defined values for 'delay' should be used.
*/
/**************************************************************************/
void delayms(uint32_t ms)
{
  uint32_t delay = ms * ((CFG_CPU_CCLK / 100) / 45);      // Release Mode (-Os)
  // uint32_t delay = ms * ((CFG_CPU_CCLK / 100) / 120);  // Debug Mode (No optimisations)

  while (delay > 0)
  {
    __asm volatile ("nop");
    delay--;
  }
}

/**************************************************************************/
/* Utility routines to manage nokia display */
/**************************************************************************/

uint8_t lcdBuffer[RESX*RESY_B];
int inverted = 0;

/*
//TODO FIXME why doenst that work ?
#define CS 	RB_LCD_CS
#define SCK 	RB_SPI_SCK
#define SDA 	RB_SPI_MOSI
#define RST 	RB_LCD_RST
*/

#define CS 2,1
#define SCK 2,11
//#define SCK 2,8
#define SDA 0,9
//#define SDA 2,8
#define RST 2,2

void lcdWrite(uint8_t cd, uint8_t data)
{
    uint8_t i;

    gpioSetDir(SDA, 1);
    gpioSetValue(SCK, 0);
    //delayms(0);
    gpioSetValue(CS, 0);
    //delayms(0);

    gpioSetValue(SDA, cd);
    //delayms(0);
    gpioSetValue(SCK, 1);
    //delayms(0);
    
    for(i=0; i<8; i++){
        gpioSetValue(SCK, 0);
        //delayms(0);
        if( data & 0x80 )
            gpioSetValue(SDA, 1);
        else
            gpioSetValue(SDA, 0);
        data <<= 1;
        gpioSetValue(SCK, 1);
        //delayms(1);
    }
    gpioSetValue(CS, 0);
    //delayms(0);
}

void lcdRead(uint8_t data)
{
    uint8_t i;

    gpioSetDir(SDA, 1);
    gpioSetValue(SCK, 0);
    delayms(1);
    gpioSetValue(CS, 0);
    delayms(1);

    gpioSetValue(SDA, 0);
    delayms(1);
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

    gpioSetDir(SDA, 0);
    for(i=0; i<8; i++){
        gpioSetValue(SCK, 0);
        delayms(1);
        gpioSetValue(SCK, 1);
        delayms(1);
    }

    gpioSetValue(CS, 0);
    delayms(1);
}

void lcdInit(void)
{
    //IOCON_SWCLK_PIO0_10 = 0x51;
    gpioSetValue(RST, 1); 
    gpioSetValue(CS, 1);

    gpioSetDir(RST, 1);
    gpioSetDir(CS, 1);
    gpioSetDir(SCK, 1);
    
   
    delayms(100);
    gpioSetValue(RST, 0); 
    delayms(100);
    gpioSetValue(RST, 1); 
    delayms(100);

    lcdWrite(0,0xE2);
    delayms(5);
    lcdWrite(0,0xAF);
    lcdWrite(0,0xA4);
    lcdWrite(0,0x2F);
    lcdWrite(0,0xB0);
    lcdWrite(0,0x10);
    lcdWrite(0,0x00);

    uint16_t i;
    for(i=0; i<100; i++)
        lcdWrite(1,0x00);
}

void lcdFill(char f){
	int x;
	for(x=0;x<RESX*RESY_B;x++) {
		lcdBuffer[x]=f;
	}
};

void lcdDisplay(uint32_t shift)
{
    lcdWrite(0,0xB0);
    lcdWrite(0,0x10);
    lcdWrite(0,0x00);
    uint16_t i,page;
    for(page=0; page<RESY_B;page++) {
        for(i=0; i<RESX; i++) {
            if (inverted) {
                lcdWrite(1,~lcdBuffer[page*RESX+((i+shift)%RESX)]);
            } else {
                lcdWrite(1,lcdBuffer[page*RESX+((i+shift)%RESX)]);
            }
        }
    }
}

void lcdInvert(void) {
    inverted = ~inverted;
}

