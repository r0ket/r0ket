#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/print.h"
#include "filesystem/ff.h"
#include "usb/usbmsc.h"


FATFS FatFs;
/**************************************************************************/

void init(void)
{
    systickInit(SYSTICKSPEED);

    gpioSetValue (RB_LED0, 0); 
    gpioSetValue (RB_LED1, 0); 
    gpioSetValue (RB_LED2, 0); 
    gpioSetValue (RB_LED3, 0); 
    IOCON_PIO1_11 = 0x0;
    gpioSetDir(RB_LED3, gpioDirection_Output);
}

void format(void)
{
    int res;
    lcdPrintln("Mount DF:");
    res=f_mount(0, &FatFs);
    lcdPrintln(f_get_rc_string(res));
    lcdRefresh();

    lcdPrintln("Formatting DF...");
    res=f_mkfs(0,1,0);
    lcdPrintln(f_get_rc_string(res));
    lcdRefresh();
}

void msc(int timeout)
{
    lcdPrintln("MSC Enabled.");
    lcdRefresh();
    delayms_power(300);
    usbMSCInit();
    while(timeout--)
        delayms(1000);
    lcdPrintln("MSC Disabled.");
    usbMSCOff();
    lcdRefresh();
}

void isp(void)
{
    lcdPrintln("Enter ISP!");
    lcdRefresh();
    ISPandReset();
}
	
void main_initial(void) {
    init();
    format();
    msc(10);
    isp();
}

void tick_initial(void){
    static int foo=0;
    static int toggle=0;
    incTimer();
    if(foo++>80){
        toggle=1-toggle;
        foo=0;
        gpioSetValue (RB_LED0, toggle); 
    };
};
