#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/backlight.h"
#include "lcd/allfonts.h"

#include "funk/nrf24l01p.h"

/**************************************************************************/

void f_init(void){
    nrf_init();
};

void f_status(void){
    int status;
    int dx=0;
    status=nrf_cmd_status(C_NOP);
    dx=DoString(0,8,"Status:");
    DoIntX(dx,8,status);
};

void f_recv(void){
};

void gotoISP(void) {
    DoString(0,0,"Enter ISP!");
    lcdDisplay(0);
    ISPandReset(5);
}

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

const struct MENU_DEF menu_ISP =    {"Invoke ISP",  &gotoISP};
const struct MENU_DEF menu_init =   {"F Init",   &f_init};
const struct MENU_DEF menu_status = {"F Status", &f_status};
const struct MENU_DEF menu_rcv =    {"F Recv",   &f_recv};
const struct MENU_DEF menu_nop =    {"---",   NULL};

static menuentry menu[] = {
    &menu_init,
    &menu_status,
    &menu_rcv,
    &menu_nop,
    &menu_ISP,
    NULL,
};

static const struct MENU mainmenu = {"Mainmenu", menu};

void handleMenu(const struct MENU *the_menu) ;

void main_funk(void) {

    backlightInit();

    while (1) {
        lcdFill(0); // clear display buffer
        lcdDisplay(0);
        handleMenu(&mainmenu);
        gotoISP();
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
                while (getInput()==BTN_NONE) delayms(10);
                break;
            case BTN_ENTER:
                lcdFill(0);
                DoString(0,0,"Called....");
                lcdDisplay(0);
                if (the_menu->entries[menuselection]->callback!=NULL)
                    the_menu->entries[menuselection]->callback();
                lcdDisplay(0);
                while (getInput()==BTN_NONE) delayms(10);

                break;
            default:
                /* no button pressed */
                break;
        }

    }

    return;
}


void tick_funk(void){
    static int foo=0;
    static int toggle=0;
	if(foo++>50){
        toggle=1-toggle;
		foo=0;
        gpioSetValue (RB_LED0, toggle); 
	};
    return;
};

