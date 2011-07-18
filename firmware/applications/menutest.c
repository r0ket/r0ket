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

const struct MENU_DEF menu_incBL = {"Backlight++", &incBacklight};
const struct MENU_DEF menu_decBL = {"Backlight--", &decBacklight};
const struct MENU_DEF menu_ISP = {"Invoke ISP", &gotoISP};
const struct MENU_DEF menu_Ep = {"p1e4", NULL};
const struct MENU_DEF menu_Eq = {"p1e5", NULL};
const struct MENU_DEF menu_Er = {"p1e6", NULL};
const struct MENU_DEF menu_Es = {"p1e7", NULL};
const struct MENU_DEF menu_Et = {"p2e1", NULL};
const struct MENU_DEF menu_Eu = {"p2e2", NULL};
const struct MENU_DEF menu_Ev = {"p2e3", NULL};

static menuentry menu[] = {
    &menu_incBL,
    &menu_decBL,
    &menu_ISP,
    &menu_Ep,
    &menu_Eq,
    &menu_Er,
    &menu_Es,
    &menu_Et,
    &menu_Eu,
    &menu_Ev,
    &menu_Ev,
    &menu_ISP,
    NULL,
};

static const struct MENU mainmenu = {"Mainmenu", menu};

void main_menutest(void) {

    backlightInit();

    font_direction = FONT_DIR_LTR; // LeftToRight is the default

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

