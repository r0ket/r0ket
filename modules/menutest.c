#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/backlight.h"
#include "lcd/allfonts.h"

void ReinvokeISP(void);

void incBacklight(void);
void decBacklight(void);
void gotoISP(void);

void handleMenu(void);

/**************************************************************************/

struct MENU_DEF {
    char *text;
    void (*callback)(void);
};

typedef const struct MENU_DEF * menuentry;

const struct MENU_DEF menu_incBL = {"Backlight++", &incBacklight};
const struct MENU_DEF menu_decBL = {"Backlight--", &decBacklight};
const struct MENU_DEF menu_ISP = {"Invoke ISP", &gotoISP};

static menuentry menu[] = {
    &menu_incBL,
    &menu_decBL,
    &menu_ISP,
};

void module_menutest(void) {

    backlightInit();

    font_direction = FONT_DIR_LTR; // LeftToRight is the default

    while (1) {
        lcdDisplay(0);
        delayms(10);

        lcdFill(0); // clear display buffer

        font = &Font_Orbitron14pt;

        DoString(0, 0, "Menü");

        if (gpioGetValue(RB_BTN4)==0) {
            while(gpioGetValue(RB_BTN4)==0);
            handleMenu();
        }
    }

    return;
}

void handleMenu(void) {
    uint8_t back = 0;
    uint8_t menuselection = 0;

    font = &Font_7x8;

    while (!back) {
        uint8_t line = 0;
        uint8_t col = 0;

        lcdDisplay(0);
        delayms(10);

        lcdFill(0); // clear display buffer

        DoString(0, line, "Menu");
        line += font->u8Height;

        if (gpioGetValue(RB_BTN1)==0) {
            while(gpioGetValue(RB_BTN1)==0);
            if (menuselection != 0) menuselection--;
        }

        if (gpioGetValue(RB_BTN0)==0) {
            while(gpioGetValue(RB_BTN0)==0);
            if (menuselection != 2)
                menuselection++;
            else
                menuselection = 0; // wrap around
        }

        for (uint8_t i = 0; i < 3; i++) {
            DoString(14, line + i*8, menu[i]->text);
            if (i == menuselection) {
                DoString(0, line + i*8, "* ");
            }
        }

        if (gpioGetValue(RB_BTN4)==0) {
            while(gpioGetValue(RB_BTN4)==0);
            menu[menuselection]->callback();
        }

        if (gpioGetValue(RB_BTN2)==0) {
            while(gpioGetValue(RB_BTN2)==0);
            back = 1;
        }

        // fallback ISP via BTN3 ;)

        if (gpioGetValue(RB_BTN3)==0) {
            while(gpioGetValue(RB_BTN3)==0);
            gotoISP();
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
    lcdDisplay(0);
    EnableWatchdog(5000);
    ReinvokeISP();
}

