#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/fonts.h"
#include "lcd/render.h"

/**************************************************************************/

void handleMenu(const struct MENU *the_menu) {
    uint8_t back = 0;
    int8_t menuselection = 0;
    uint8_t numentries = 0;
    uint8_t visible_lines = 0;
    uint8_t current_offset = 0;

    if (the_menu == NULL) return;

//    font = &Font_7x8; // Font needs to be set externally?

    for (numentries = 0; the_menu->entries[numentries] != NULL; numentries++);

    visible_lines = (RESY/font->u8Height)-1; // subtract title line
#ifdef SAFETY
    if (visible_lines < 2) return;
#endif

    while (!back) {
        uint8_t line = 0;

        // Display current menu page
        lcdFill(0);
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
                lcdFill(0);
                DoString(0,0,"Called....");
                lcdDisplay(0);
                if (the_menu->entries[menuselection]->callback!=NULL)
                    the_menu->entries[menuselection]->callback();
                lcdDisplay(0);
                getInputWait();

                break;
            default:
                /* no button pressed */
                break;
        }

    }
    return;
}
