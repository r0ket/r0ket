#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/allfonts.h"

#include "core/cpu/cpu.h"
#include "core/timer32/timer32.h"

#define FREQUENZ(x) ((72E6/x)/2)

void TIMER32_0_IRQHandler(void){
TMR_TMR32B0IR = TMR_TMR32B0IR_MR0;
static int time=0;
if (time==0){time=1;} else {time=0;}
  gpioSetValue (RB_LED2, time);
  gpioSetValue (RB_SPI_SS3, time);
};
void backlightInit(void);

/**************************************************************************/

void main_lilafisch(void) {
    int dx=0;
    uint32_t ton=1000, timer=0; //(nicht 0!)
//    int zaehler=0;
    char key;
    lcdToggleFlag(LCD_MIRRORX);
    backlightInit();
    font_direction = FONT_DIR_LTR; // LeftToRight is the default
    dx=DoString(0,0,"init timer"); lcdDisplay(0);
    timer32Init(0, (72E6/5E3)/2); //timer einschalten, auf 5kHz(?) setzen
    dx=DoString(0,0,"enable timer"); lcdDisplay(0);
    timer32Enable(0);
    dx=DoString(0,0,"timer done"); lcdDisplay(0);

    while (1) {
        ton ++;
        if (ton==5000){
          ton=1000;
        }
        timer = (72000000UL/ton/2);
        DoInt(0,0,timer);
        TMR_TMR32B0MR0 = timer; //FREQUENZ(ton);
        if (TMR_TMR32B0TC > timer){    //schneller fix wenn ton zurueckgesetzt wird, aber timer weiterlaeuft
           TMR_TMR32B0TC=0;
        }
        lcdDisplay(0);
        delayms(10);

        key= getInput();
        font=&Font_7x8;

        // Easy flashing
        if(key==BTN_LEFT){
            timer32Disable(0);
            DoString(0,8,"Enter ISP!");
            lcdDisplay(0);
            ISPandReset(5);
        }

        //font = &Font_Ubuntu36pt;
        //dx=DoString(0,0,"Hello World");

    }
    return;
}

void tick_lilafisch(void){
    return;
};
