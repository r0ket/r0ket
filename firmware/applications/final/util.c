#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "usb/usbmsc.h"

#include "core/iap/iap.h"

/**************************************************************************/


//# MENU msc
void msc_menu(void){
    lcdClear();
    lcdPrintln("MSC Enabled.");
    lcdRefresh();
    usbMSCInit();
    while(!getInputRaw())delayms_queue(10);
    DoString(0,16,"MSC Disabled.");
    usbMSCOff();
    fsReInit();
};

