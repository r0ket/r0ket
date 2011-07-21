#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/display.h"
#include "lcd/allfonts.h"
#include "lcd/print.h"

void backlightInit(void);

/**************************************************************************/

void main_scroll(void) {
    int dx=0;
    char key;
    backlightInit();
    bool wrap=true;
    int ctr=0;

    font=&Font_7x8;
    dx=DoString(0,0,"Hello World");
   
    while (1) {
        lcdDisplay();
////        delayms(10);

        key= getInputRaw();

        // Easy flashing
        if((key&(BTN_ENTER|BTN_LEFT))==(BTN_ENTER|BTN_LEFT)){
            DoString(0,8,"Enter ISP!");
            lcdDisplay();
            ISPandReset();
        }
        if(key&BTN_ENTER){
            lcdPrintInt(ctr++);
            lcdPrintln(".");
            while(getInputRaw())delayms(10);
        };
		if(key&BTN_RIGHT){
			lcdShift(1,0,wrap);
		}
		if(key&BTN_LEFT){
			lcdShift(-1,0,wrap);
		}
		if(key&BTN_UP){
			lcdShift(0,1,wrap);
		}
		if(key&BTN_DOWN){
			lcdShift(0,-1,wrap);
		}

        //font = &Font_Ubuntu36pt;
    }
    return;
}

void tick_scroll(void){
    return;
};
