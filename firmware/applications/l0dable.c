#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "lcd/lcd.h"
#include "lcd/print.h"
#include "usb/usbmsc.h"

/**************************************************************************/
void gotoISP(void) {
    DoString(0,0,"Enter ISP!");
    lcdDisplay();
    ISPandReset();
}

void msc_menu(void){
    DoString(0,8,"MSC Enabled.");
    lcdDisplay();
    usbMSCInit();
    while(!getInputRaw())delayms(10);
    DoString(0,16,"MSC Disabled.");
    usbMSCOff();
};

extern void (*ram)(void);

static const struct MENU mainmenu = {"Mainmenu", {
    {"Run Loadable",   &ram},
    {"Invoke ISP",  &gotoISP},
    {"MSC",   &msc_menu},
    {NULL,NULL}
}};



/**************************************************************************/

void main_l0dable(void) {
    lcdClear();
    lcdDisplay();
    handleMenu(&mainmenu);
    gotoISP();
};
