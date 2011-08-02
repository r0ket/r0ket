#include <sysinit.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/lcd.h"
#include "lcd/print.h"
#include "lcd/allfonts.h"

#include "filesystem/ff.h"
#include "filesystem/select.h"
#include "funk/nrf24l01p.h"
#include "usb/usbmsc.h"

#include <string.h>

/**************************************************************************/

void show_ticks(void) {
    int dx=0;
    int dy=8;
    lcdClear();
    dx=DoString(0,dy,"Ticks:");
    while ((getInputRaw())==BTN_NONE){
        DoInt(0,dy+8,_timectr);
        lcdDisplay();
    };
    dy+=16;
    dx=DoString(0,dy,"Done.");
};


void chrg_stat(void) {
    int stat;
    while ((getInputRaw())==BTN_NONE){
        lcdClear();
        lcdPrintln("Chrg_stat:");
        stat=gpioGetValue(RB_PWR_CHRG);
        lcdPrint(IntToStr(stat,3,0));
        lcdNl();
        lcdRefresh();
    };
    lcdPrintln("Done.");
};
void adc_light(void) {
    int dx=0;
    int dy=8;
    dx=DoString(0,dy,"Light:");
    DoString(0,dy+16,"Night:");
    while ((getInputRaw())==BTN_NONE){
        DoInt(dx,dy,GetLight());
        DoInt(dx,dy+16,isNight());
        DoInt(dx,dy+8,GLOBAL(daytrig));
        lcdDisplay();
    };
    dy+=8;
    dx=DoString(0,dy,"Done.");
};

void uptime(void) {
    int t;
    int h;
    char flag;
    while ((getInputRaw())==BTN_NONE){
        lcdClear();
        lcdPrintln("Uptime:");
        t=getTimer()/(1000/SYSTICKSPEED);
        h=t/60/60;
        flag=F_ZEROS;
        if(h>0){
            lcdPrint(IntToStr(h,2,flag));
            lcdPrint("h");
            flag|=F_LONG;
        };
        h=t/60%60;
        if(h>0){
            lcdPrint(IntToStr(h,2,flag));
            lcdPrint("m");
            flag|=F_LONG;
        };
        h=t%60;
        if(h>0){
            lcdPrint(IntToStr(h,2,flag));
            lcdPrint("s");
        };
        lcdNl();
        lcdRefresh();
        delayms_queue(200);
    };
    lcdPrintln("done.");
};

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
    getInputWaitRelease();
    getInputWait();
    DoString(0,16,"MSC Disabled.");
    usbMSCOff();
    fsReInit();
};
