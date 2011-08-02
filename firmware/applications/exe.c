#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "lcd/lcd.h"
#include "lcd/allfonts.h"
#include "lcd/print.h"
#include "usb/usbmsc.h"
#include "filesystem/ff.h"
#include "filesystem/select.h"
#include "filesystem/execute.h"

/**************************************************************************/

void execute_menu(void){
    while(getInput()!=BTN_NONE);
    executeSelect("C0D");
    lcdRefresh();
};

void msc_menu(void){
    DoString(0,8,"MSC Enabled.");
    lcdDisplay();
    usbMSCInit();
    while(!getInputRaw())delayms(10);
    DoString(0,16,"MSC Disabled.");
    usbMSCOff();
};

void gotoISP(void) {
    DoString(0,0,"Enter ISP!");
    lcdDisplay();
    ISPandReset();
}

void lcd_mirror(void) {
    lcdToggleFlag(LCD_MIRRORX);
};

void adc_check(void) {
    int dx=0;
    int dy=8;
    // Print Voltage
    dx=DoString(0,dy,"Voltage:");
    while ((getInputRaw())==BTN_NONE){
        DoInt(dx,dy,GetVoltage());
        lcdDisplay();
    };
    dy+=8;
    dx=DoString(0,dy,"Done.");
};

/**************************************************************************/

static const struct MENU mainmenu = {"Mainmenu", {
    {"MSC",   &msc_menu},
    {"Exec",   &execute_menu},
    {"---",   NULL},
    {"Mirror",   &lcd_mirror},
    {"Akku",   &adc_check},
    {"Invoke ISP",  &gotoISP},
    {NULL,NULL}
}};


void main_exe(void) {

    lcdSetPixel(0,0,0);

    while (1) {
        lcdFill(0); // clear display buffer
        lcdDisplay();
        handleMenu(&mainmenu);
        gotoISP();
    }
};

void tick_exe(void){
    static int foo=0;
    static int toggle=0;
	if(foo++>50){
        toggle=1-toggle;
		foo=0;
        gpioSetValue (RB_LED0, toggle); 
	};
    return;
};


