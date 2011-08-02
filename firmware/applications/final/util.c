#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/print.h"

#include "usb/usbmsc.h"

#include "core/iap/iap.h"

/**************************************************************************/

void ChkLight(void) {
    int dx=0;
    int dy=8;
    dx=DoString(0,dy,"Light:");
    DoString(0,dy+16,"Night:");
    while ((getInputRaw())==BTN_NONE){
        DoInt(dx,dy,GetLight());
        DoInt(dx,dy+16,isNight());
        DoInt(dx,dy+8,GLOBAL(daytrig));
        lcdDisplay();
        delayms_queue(100);
    };
    dx=DoString(0,dy+24,"Done.");
}

void ChkBattery(void) {
    do{
        lcdClear();
        lcdPrintln("Voltage:");
        lcdPrintln(IntToStr(GetVoltage(),5,0));
        lcdNl();
        lcdPrintln("Chrg_stat:");
        if(gpioGetValue(RB_PWR_CHRG)){
            lcdPrintln("1");
        }else{
            lcdPrintln("0");
        };
        lcdRefresh();
        delayms_queue(100);
    } while ((getInputRaw())==BTN_NONE);
}

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
        lcdNl();
        lcdPrintln("Ticks:");
        lcdPrint(IntToStr(_timectr,10,0));
        lcdRefresh();
        delayms_queue(200);
    };
    lcdPrintln("done.");
}

void uuid(void) {
    IAP_return_t iap_return;
    iap_return = iapReadSerialNumber();
    lcdClear();
    lcdPrintln("UUID:");
    lcdPrintIntHex(iap_return.Result[0]); lcdNl();
    lcdPrintIntHex(iap_return.Result[1]); lcdNl();
    lcdPrintIntHex(iap_return.Result[2]); lcdNl();
    lcdPrintIntHex(iap_return.Result[3]); lcdNl();
    lcdNl();
    lcdPrintln("Beacon ID:");
    lcdPrintln(IntToStrX(GetUUID32(),4));
    lcdRefresh();
}
