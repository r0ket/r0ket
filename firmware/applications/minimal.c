#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/allfonts.h"

void backlightInit(void);

/**************************************************************************/

void main_minimal(void) {
    int dx=0;
    char key;
    backlightInit();
    font_direction = FONT_DIR_LTR; // LeftToRight is the default
    while (1) {

        lcdDisplay(0);
        delayms(10);

        key= getInput();
        font=&Font_7x8;

        // Easy flashing
        if(key==BTN_LEFT){
            DoString(0,8,"Enter ISP!");
            lcdDisplay(0);
            ISPandReset(5);
        }

        //font = &Font_Ubuntu36pt;
        dx=DoString(0,0,"Hello World");
    }
    return;
}

void tick_minimal(void){
    return;
};
