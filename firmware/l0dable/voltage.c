#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "usetable.h"

/**************************************************************************/

// Improved version by Ikarus:
// Added graphical battery.

// hLine and vLine code from Juna, nougad and fu86
// (camp 2011, CTHN Village)
void hLine(int y, int x1, int x2, bool pixel);
void vLine(int x, int y1, int y2, bool pixel);

void drawCommonThings(int c);
void rectFill(int x, int y, int width, int heigth, bool pixel);

void ram(void) {
    int v,mv,c;
    char old_state=-1;
    do{
        c = gpioGetValue(RB_PWR_CHRG);
        mv = GetVoltage();

        // Print state and draw battery (only if state changed).
        if(!c && old_state != 0){
            drawCommonThings(c);
            DoString(17, 29, "Charging");
            old_state = 0;
        }else if (c && mv<3550 && old_state != 1){
            drawCommonThings(c);
            lcdPrintln("  Charge NOW!");
            old_state = 1;
        }else if (c && mv<3650 && mv>=3550 && old_state != 2){
            drawCommonThings(c);
            lcdPrintln("  Charge soon");
            rectFill(16, 25, 12, 16, true);
            old_state = 2;
        }else if (c && mv<4000 && mv>=3650 && old_state != 3){
            drawCommonThings(c);
            lcdPrintln("      OK");
            rectFill(16, 25, 12, 16, true);
            rectFill(30, 25, 12, 16, true);
            old_state = 3;
        }else if (c && mv<4120 && mv>=4000 && old_state != 4){
            drawCommonThings(c);
            lcdPrintln("     Good");
            rectFill(16, 25, 12, 16, true);
            rectFill(30, 25, 12, 16, true);
            rectFill(44, 25, 12, 16, true);
            old_state = 4;
        }else if (c && mv>=4120 && old_state != 5){
            drawCommonThings(c);
            lcdPrintln("     Full");
            rectFill(16, 25, 12, 16, true);
            rectFill(30, 25, 12, 16, true);
            rectFill(44, 25, 12, 16, true);
            rectFill(58, 25, 12, 16, true);
            old_state = 5;
        }

        // Print voltage. (Every frame).
        lcdSetCrsr(0, 50);
        v = mv/1000;
        lcdPrint("    ");
        lcdPrint(IntToStr(v,2,0));
        lcdPrint(".");
        lcdPrint(IntToStr(mv%1000, 3, F_ZEROS | F_LONG));
        lcdPrintln("V");
        lcdSetCrsr(0, 0);
        lcdRefresh();
    } while ((getInputWaitTimeout(242))==BTN_NONE);
}

void drawCommonThings(int c) {
    lcdClear();
    // Print header.
    lcdPrintln("Battery status:");

    // Draw battery frame.
    hLine(23, 14, 72, true);
    hLine(43, 14, 72, true);
    vLine(14, 23, 43, true);
    vLine(72, 23, 28, true);
    vLine(72, 38, 43, true);
    hLine(28, 72, 78, true);
    hLine(38, 72, 78, true);
    vLine(78, 28, 38, true);

    // Print if not charging.
    lcdSetCrsr(0, 60);
    if(c){
        lcdPrintln("(not charging)");
    };
    lcdSetCrsr(0, 10);
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
        hLine(i, x, x+width, pixel);
    }
}
