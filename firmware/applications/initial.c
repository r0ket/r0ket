#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/print.h"
#include "filesystem/ff.h"
#include "usb/usbmsc.h"


/**************************************************************************/

void main_initial(void) {
    char key=BTN_NONE;
    gpioSetValue (RB_LED0, 0); 
    gpioSetValue (RB_LED1, 0); 
    gpioSetValue (RB_LED2, 0); 
    gpioSetValue (RB_LED3, 0); 
    IOCON_PIO1_11 = 0x0;
    gpioSetDir(RB_LED3, gpioDirection_Output);

    while(1){
        lcdClear();
        lcdPrintln("Init v.42");
        lcdNl();
        lcdPrintln("Left:  ISP()");
        lcdPrintln("Right: MSC()");
        lcdPrintln("Up:    FormatDF()");
        lcdPrintln("Down:  ???");
        lcdPrintln("Enter: LEDs()");
        lcdRefresh();
   
        key=getInputWait();

        if(key&BTN_ENTER){
            gpioSetValue (RB_LED0, 1); 
            gpioSetValue (RB_LED1, 1); 
            gpioSetValue (RB_LED2, 1); 
            gpioSetValue (RB_LED3, 1); 
            delayms_power(100);
            getInputWaitRelease();

            gpioSetValue (RB_LED0, 0); 
            gpioSetValue (RB_LED1, 0); 
            gpioSetValue (RB_LED2, 0); 
            gpioSetValue (RB_LED3, 0); 
            delayms_power(50);
        };
		if(key&BTN_RIGHT){
            lcdClear();
            lcdPrintln("MSC Enabled.");
            lcdRefresh();
            delayms_power(300);
            usbMSCInit();
            getInputWait();
            lcdPrintln("MSC Disabled.");
            usbMSCOff();
            lcdRefresh();
		}
		if(key&BTN_LEFT){
            lcdClear();
            lcdPrintln("Enter ISP!");
            lcdRefresh();
            ISPandReset();
		}
		if(key&BTN_UP){
            FATFS FatFs;
            int res;

            lcdClear();

            lcdPrintln("Mount DF:");
            res=f_mount(0, &FatFs);
            lcdPrintln(f_get_rc_string(res));
            lcdRefresh();

            lcdPrintln("Formatting DF...");
            res=f_mkfs(0,1,0);
            lcdPrintln(f_get_rc_string(res));
            lcdRefresh();
		}
		if(key&BTN_DOWN){
			;
		}

        getInputWaitRelease();
    };
}

void tick_initial(void){
    static int foo=0;
    static int toggle=0;
    if(foo++>80){
        toggle=1-toggle;
        foo=0;
        gpioSetValue (RB_LED0, toggle); 
    };
};
