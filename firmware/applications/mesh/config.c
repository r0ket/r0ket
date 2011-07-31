#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/print.h"
#include "lcd/display.h"

#include "filesystem/ff.h"

#include <string.h>

/**************************************************************************/

void readcfg(void) {
    readConfig();
};

void savecfg(void){
    saveConfig();
};

void applycfg(void){
    applyConfig();
};

void changer(void){
    uint8_t numentries = 0;
    signed char menuselection = 0;
    uint8_t visible_lines = 0;
    uint8_t current_offset = 0;

    for (int i=0;the_config[i].name!=NULL;i++){
        numentries++;
    };

    visible_lines = ((RESY/getFontHeight())-1)/2;

    while (1) {
        // Display current menu page
        lcdClear();
        lcdPrint("Config");
        
        lcdSetCrsrX(60);
        lcdPrint("[");
        lcdPrint(IntToStr(current_offset/visible_lines,1,0));
        lcdPrint("/");
        lcdPrint(IntToStr(numentries/visible_lines,1,0));
        lcdPrint("]");
        lcdNl();

        lcdNl();

        for (uint8_t i = current_offset; i < (visible_lines + current_offset) && i < numentries; i++) {
            if(i==0){
                lcdPrintln("Save changes:");
                if (i == menuselection)
                    lcdPrint("*");
                lcdSetCrsrX(14);
                if (i == menuselection)
                    lcdPrintln("YES");
                else
                    lcdPrintln("no");
            }else{
                lcdPrintln(the_config[i].name);
                if (i == menuselection)
                    lcdPrint("*");
                lcdSetCrsrX(14);
                lcdPrint("<");
                lcdPrint(IntToStr(the_config[i].value,3,F_LONG));
                lcdPrintln(">");
            };
        }
        lcdRefresh();

        switch (getInputWait()) {
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
                if(the_config[menuselection].value >
                        the_config[menuselection].min)
                    the_config[menuselection].value--;
                if(the_config[menuselection].value > the_config[menuselection].max)
                    the_config[menuselection].value=
                        the_config[menuselection].max;
                applyConfig();
                break;
            case BTN_RIGHT:
                if(the_config[menuselection].value <
                        the_config[menuselection].max)
                    the_config[menuselection].value++;
                if(the_config[menuselection].value < the_config[menuselection].min)
                    the_config[menuselection].value=
                        the_config[menuselection].min;
                applyConfig();
                break;
            case BTN_ENTER:
                if(menuselection==0)
                    saveConfig();
                return;
        }
        getInputWaitRelease();
    }
    /* NOTREACHED */
}
