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

struct MENU_DEF {
    char *text;
    void (*callback)(void);
};

typedef const struct MENU_DEF * menuentry;

struct MENU {
    char *title;
    menuentry *entries;
};

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

void handleMenu(const struct MENU *the_menu) ;
void delayms(int);

uint8_t getInput(void) {
    uint8_t result = BTN_NONE;

    if (gpioGetValue(RB_BTN3)==0) {
        while(gpioGetValue(RB_BTN3)==0);
        result += BTN_UP;
    }

    if (gpioGetValue(RB_BTN2)==0) {
        while(gpioGetValue(RB_BTN2)==0);
        result += BTN_DOWN;
    }

    if (gpioGetValue(RB_BTN4)==0) {
        while(gpioGetValue(RB_BTN4)==0);
        result += BTN_ENTER;
    }

    if (gpioGetValue(RB_BTN0)==0) {
        while(gpioGetValue(RB_BTN0)==0);
        result += BTN_LEFT;
    }

    if (gpioGetValue(RB_BTN1)==0) {
        while(gpioGetValue(RB_BTN1)==0);
        result += BTN_RIGHT;
    }

    if (result == (BTN_LEFT+BTN_TOP+BTN_ENTER)){ /* Development hack */
        gotoISP();
    }

    return result;
}


void module_menutest(void) {

    backlightInit();

    font_direction = FONT_DIR_LTR; // LeftToRight is the default

    while (1) {
        lcdDisplay(0);
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

void handleMenu(const struct MENU *the_menu) {
    uint8_t back = 0;
    int8_t menuselection = 0;
    uint8_t numentries = 0;
    uint8_t visible_lines = 0;
    uint8_t current_offset = 0;

    if (the_menu == NULL) return;

    font = &Font_7x8;

    for (numentries = 0; the_menu->entries[numentries] != NULL; numentries++);

    visible_lines = RESY/font->u8Height;

    if (visible_lines < 2) return;

    visible_lines--; // subtract title line

    while (!back) {
        uint8_t line = 0;

        lcdFill(0); // clear display buffer

        DoString(0, line, the_menu->title);
        line += font->u8Height;

        for (uint8_t i = current_offset; i < (visible_lines + current_offset) && i < numentries; i++) {
            DoString(14, line, the_menu->entries[i]->text);
            if (i == menuselection) {
                DoString(0, line, "* ");
            }
            line += font->u8Height;
        }

        lcdDisplay(0);

        switch (getInput()) {
            case BTN_UP:
                menuselection--;
                if (menuselection < current_offset) {
                    if (menuselection < 0) {
                        menuselection = numentries-1;
                        current_offset = ((numentries-1)/visible_lines) * visible_lines;
                    } else {
                        current_offset -= visible_lines;
                    }
                }
                break;
            case BTN_DOWN:
                menuselection++;
                if (menuselection > (current_offset + visible_lines-1) || menuselection >= numentries) {
                    if (menuselection >= numentries) {
                        menuselection = 0;
                        current_offset = 0;
                    } else {
                        current_offset += visible_lines;
                    }
                }
                break;
            case BTN_LEFT:
                return;
            case BTN_RIGHT:
                if (the_menu->entries[menuselection]->callback!=NULL)
                    the_menu->entries[menuselection]->callback();
                break;
            case BTN_ENTER:
                if (the_menu->entries[menuselection]->callback!=NULL)
                    the_menu->entries[menuselection]->callback();
                break;
            default:
                /* no button pressed */
                break;
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

