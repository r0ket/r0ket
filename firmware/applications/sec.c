#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/allfonts.h"

void backlightInit(void);

/**************************************************************************/

void main_sec(void) {

    backlightInit();

    //disable the JTAG on PIO3_3
    IOCON_PIO3_3 = 0x10;

    int yctr=8;
    int dx=0;

    yctr=18;

    uint8_t trigger;

    trigger=20;

    uint32_t ctr=0;
    char key;
    while (1) {
        ctr++;

        lcdDisplay();
        delayms(10);

        key= getInput();
        if(key==BTN_UP){
            trigger +=1;
        }else if (key ==BTN_DOWN){
            trigger -=1;
        };

        font=&Font_7x8;
        dx=DoString(0,0,"Trig:");
        dx=DoInt(dx,0,trigger);
        DoString(dx,0," ");

        // Easy flashing
        if(key==BTN_LEFT){
            DoString(0,8,"Enter ISP!");
            lcdDisplay();
            ISPandReset();
        };

        // Display nickname
        font = &Font_Ubuntu36pt;
        dx=DoString(0,0,"Sec");

        // Blink LED
        if(ctr++>trigger){
            ctr=0;
            if (gpioGetValue(RB_LED2) == CFG_LED_OFF){
                gpioSetValue (RB_LED2, CFG_LED_ON); 
            } else {
                gpioSetValue (RB_LED2, CFG_LED_OFF); 
            };
        };

        // Print Voltage
        font = &Font_7x8;
        dx=DoString(0,yctr+28,"Voltage:");
        DoInt(dx,yctr+28,GetVoltage());

    }
    return;
}

void tick_sec(void){
    static int foo=0;
    static int toggle=0;
	if(foo++>50){
        toggle=1-toggle;
		foo=0;
        gpioSetValue (RB_LED0, toggle); 
	};
    return;
};
