#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/backlight.h"
#include "lcd/allfonts.h"

#define BTN_NONE 0
#define BTN_UP   (1<<0)
#define BTN_DOWN (1<<1)
#define BTN_LEFT (1<<2)
#define BTN_RIGHT (1<<3)
#define BTN_ENTER (1<<4)

void ReinvokeISP(void);

void incBacklight(void);
void decBacklight(void);
void gotoISP(void);


/**************************************************************************/
static const struct MENU mainmenu = {"Mainmenu", {
 {"Backlight++", &incBacklight},
 {"Backlight--", &decBacklight},
 {"Invoke ISP", &gotoISP},
 {"p1e4", NULL},
 {"p1e5", NULL},
 {"p1e6", NULL},
 {"p1e7", NULL},
 {"p2e1", NULL},
 {"p2e2", NULL},
 {"p2e3", NULL},
 {NULL, NULL},
}};


void main_menutest(void) {

    backlightInit();

    while (1) {
        lcdDisplay();
        delayms(10);

        lcdFill(0); // clear display buffer

        font = &Font_Orbitron14pt;

        DoString(0, 0, "Menü");

        if (getInput() == BTN_ENTER) {
            handleMenu(&mainmenu);
        }
    }

    return;
}

void incBacklight(void) {
    uint32_t brightness = backlightGetBrightness();
    if (brightness < 100) {
        backlightSetBrightness(brightness + 10);
    }
}

void decBacklight(void) {
    uint32_t brightness = backlightGetBrightness();
    if (brightness > 0) {
        backlightSetBrightness(brightness - 10);
    }
}

void gotoISP(void) {
    DoString(0,58,"Enter ISP!");
    lcdDisplay();
    EnableWatchdog(5000);
    ReinvokeISP();
}

