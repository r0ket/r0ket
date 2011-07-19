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

void lcd_mirror(void) {
    lcdToggleFlag(LCD_MIRRORX);
};

void lcd_invert(void) {
    lcdToggleFlag(LCD_INVERTED);
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

void msc_menu(void){
    DoString(0,8,"MSC Enabled.");
    lcdDisplay();
    usbMSCInit();
    while(!getInputRaw())delayms(10);
    DoString(0,16,"MSC Disabled.");
    usbMSCOff();
};

extern void (*ram)(void);

const struct MENU_DEF menu_ISP =    {"Invoke ISP",  &gotoISP};
const struct MENU_DEF menu_again = {"Run Loadable",   &ram};
const struct MENU_DEF menu_nop =    {"---",   NULL};
const struct MENU_DEF menu_msc =   {"MSC",   &msc_menu};
const struct MENU_DEF menu_volt =   {"Akku",   &adc_check};
const struct MENU_DEF menu_mirror = {"Mirror",   &lcd_mirror};
const struct MENU_DEF menu_invert = {"Invert",   &lcd_invert};

static menuentry menu[] = {
    &menu_again,
    &menu_ISP,
    &menu_nop,
    &menu_msc,
    &menu_mirror,
    &menu_invert,
    &menu_volt,
    NULL,
};

static const struct MENU mainmenu = {"Mainmenu", menu};


/**************************************************************************/

void main_loadable(void) {

    lcdFill(0); // clear display buffer
    lcdDisplay();
    handleMenu(&mainmenu);
    gotoISP();
};
