#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/allfonts.h"

void backlightInit(void);

#include "loadable/blinktest.h"

void execute (const void *function, uint16_t length){
    void (*dst)(void);

    dst=(void (*)(void)) 0x10001c00;

    DoString(0,8,"Enter RAM!");
    DoIntX(0,16,(uint32_t)function);
    DoIntX(0,24,(uint32_t)dst);
    lcdDisplay(0);

    for(int i=0;i<length;i++){
        ((char*)dst)[i]=((char*)function)[i];
    };

    dst=(void (*)(void)) ((uint32_t)(dst) | 1); // Enable Thumb mode!
    dst();
};

/**************************************************************************/

void module_executor(void) {

    backlightInit();

    //disable the JTAG on PIO3_3
    IOCON_PIO3_3 = 0x10;

    int yctr=8;
    int dx=0;

    font_direction = FONT_DIR_LTR; // LeftToRight is the default

    yctr=18;

    uint8_t trigger;

    trigger=20;

    uint32_t ctr=0;
    char key;
    while (1) {
        ctr++;

        lcdDisplay(0);
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
            lcdDisplay(0);
            ISPandReset(5);
        };

        // RAM execute
        if(key==BTN_RIGHT){
            execute(loadable_blinktest,loadable_blinktest_size);
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

void tick_executor(void){
    static int foo=0;
    static int toggle=0;
	if(foo++>50){
        toggle=1-toggle;
		foo=0;
        gpioSetValue (RB_LED0, toggle); 
	};
    return;
};



