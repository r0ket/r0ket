#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "usetable.h"

/**************************************************************************/

// Improved version by Ikarus:
// Fixed 4.xxV --> 4.0xxV output problem
// & added graphical battery.

// hLine and vLine code from Juna, nougad and fu86
// (camp 2011, CTHN Village) 
void hLine(int y, int x1, int x2, bool pixel);
void vLine(int x, int y1, int y2, bool pixel);

void rectFill(int x, int y, int width, int heigth, bool pixel);

void ram(void) {
    int v,mv,c;
    do{
        lcdClear();
        lcdPrintln("Battery status:");
        c = gpioGetValue(RB_PWR_CHRG);
        mv = GetVoltage();
        v = mv/1000;
        
        // Draw battery frame.
        hLine(20, 14, 72, true);
        hLine(40, 14, 72, true);
        vLine(14, 20, 40, true);
        vLine(72, 20, 25, true);
        vLine(72, 35, 40, true);
        hLine(25, 72, 78, true);
        hLine(35, 72, 78, true);
        vLine(78, 25, 35, true);
        
        // Print and draw status.
        if(!c){
            lcdNl();
            DoString(17, 26, "Charging");
        }else if (mv<3550){
            lcdPrintln("  Charge NOW!");
        }else if (mv<3650){
            lcdPrintln("  Charge soon");
            rectFill(16, 22, 12, 16, true);
        }else if (mv<4000){
            lcdPrintln("      OK");
            rectFill(16, 22, 12, 16, true);
            rectFill(30, 22, 12, 16, true);
        }else if (mv<4120){
            lcdPrintln("     Good");
            rectFill(16, 22, 12, 16, true);
            rectFill(30, 22, 12, 16, true);
            rectFill(44, 22, 12, 16, true);
        }else{
            lcdPrintln("     Full");
            rectFill(16, 22, 12, 16, true);
            rectFill(30, 22, 12, 16, true);
            rectFill(44, 22, 12, 16, true);
            rectFill(58, 22, 12, 16, true);
        };

        // Print voltage.
        lcdNl();
        lcdNl();
        lcdNl();
        lcdNl();
        lcdPrint("    ");
        lcdPrint(IntToStr(v,2,0));
        lcdPrint(".");
        lcdPrint(IntToStr(mv%1000, 3, F_ZEROS | F_LONG));
        lcdPrintln("V");
        // Print if not charging.
        if(c){
            lcdPrintln("(not charging)");
        };
        lcdRefresh();
    } while ((getInputWaitTimeout(242))==BTN_NONE);
}

void hLine(int y, int x1, int x2, bool pixel) {
    for (int i=x1; i<=x2; ++i) {
        lcdSetPixel(i, y, pixel);
    }
}

void vLine(int x, int y1, int y2, bool pixel) {
    for (int i=y1; i<=y2; ++i) {
        lcdSetPixel(x, i, pixel);
    }
}

void rectFill(int x, int y, int width, int heigth, bool pixel) {
    for (int i=y; i<=y+heigth; ++i) {
        for (int j=x; j<=x+width; ++j) {
            lcdSetPixel(j, i, pixel);
        }
    }
}
