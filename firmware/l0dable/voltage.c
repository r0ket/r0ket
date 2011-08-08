#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "usetable.h"

/**************************************************************************/

void ram(void) {
    int v,mv;
    do{
        lcdClear();
        lcdPrintln("Battery status:");
        mv=GetVoltage();
        v=mv/1000;

        lcdNl();
        if (mv<3550){
            lcdPrintln("  Charge NOW!");
        }else if (mv<3650){
            lcdPrintln("  Charge soon");
        }else if (mv<4000){
            lcdPrintln("      OK");
        }else if(mv<4200){
            lcdPrintln("     Good");
        }else{
            lcdPrintln("     Full");
        };

        lcdNl();
        lcdPrint(" ");
        lcdPrint(IntToStr(v,2,0));
        lcdPrint(".");
        lcdPrint(IntToStr(mv%1000,3,F_ZEROS));
        lcdPrintln("V");

        lcdNl();
        if(gpioGetValue(RB_PWR_CHRG)){
            lcdPrintln("(not charging)");
        }else{
            lcdPrintln("CHARGING");
        };
        lcdRefresh();
    } while ((getInputWaitTimeout(242))==BTN_NONE);
}
