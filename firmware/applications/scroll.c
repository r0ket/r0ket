#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/display.h"
#include "lcd/allfonts.h"

void backlightInit(void);

/**************************************************************************/

void main_scroll(void) {
    int dx=0;
    char key;
    backlightInit();
    font_direction = FONT_DIR_LTR; // LeftToRight is the default

    font=&Font_7x8;
    dx=DoString(0,0,"Hello World");
   
    while (1) {
        lcdDisplay();
////        delayms(10);

        key= getInput();

        // Easy flashing
        if(key==BTN_ENTER){
            DoString(0,8,"Enter ISP!");
            lcdDisplay();
            ISPandReset();
        }
		if(key==BTN_RIGHT){
			lcdShift(1,0,true);
		}
		if(key==BTN_LEFT){
			lcdShift(-1,0,true);
		}
		if(key==BTN_UP){
			lcdShift(0,1,true);
		}
		if(key==BTN_DOWN){
			lcdShift(0,-1,true);
		}

        //font = &Font_Ubuntu36pt;
    }
    return;
}

void tick_scroll(void){
    return;
};
