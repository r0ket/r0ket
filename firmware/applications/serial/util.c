#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/lcd.h"
#include "lcd/print.h"
#include "lcd/allfonts.h"

#include "filesystem/ff.h"
#include "filesystem/select.h"
#include "funk/nrf24l01p.h"

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


void gotoISP(void) {
    DoString(0,0,"Enter ISP!");
    lcdDisplay();
    ISPandReset();
}
