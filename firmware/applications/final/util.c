#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"
#include "lcd/image.h"

#include "usb/usbmsc.h"

#include "filesystem/ff.h"
#include "filesystem/select.h"

#include "core/iap/iap.h"

/**************************************************************************/


//# MENU usb_storage
void msc_menu(void){
    lcdClear();
    lcdPrintln("MSC Enabled.");
    lcdRefresh();
    usbMSCInit();
    while(!getInputRaw())delayms(10);
    DoString(0,16,"MSC Disabled.");
    usbMSCOff();
    fsReInit();
};

void blink_led0(void){
    gpioSetValue (RB_LED0, 1-gpioGetValue(RB_LED0));
};

void tick_alive(void){
    static int foo=0;

    if(GLOBAL(alivechk)==0)
        return;

	if(foo++>500/SYSTICKSPEED){
		foo=0;
        if(GLOBAL(alivechk)==2)
            push_queue(blink_led0);
        else
            blink_led0();
	};
    return;
};


// //# MENU img img
void t_img(void){
    char fname[FILENAMELEN];
    selectFile(fname, "LCD");
    lcdLoadImage(fname);
    lcdRefresh();
    getInputWait();
};

// //# MENU img anim
void t_ani(void){
    char fname[FILENAMELEN];
    selectFile(fname, "LCD");
    lcdShowAnim(fname,1000);
};

