#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/allfonts.h"

void backlightInit(void);

/**************************************************************************/

void main_s(void) {
    uint8_t key;
    //lcdClear();
    while (1) {
        uint8_t contrast = lcdRead(0xDF);
        lcdInit(); // display
        lcdPrint("reg=");
        lcdPrintCharHex(contrast);
        lcdNl();
        lcdDisplay();
        while(1);
        delayms(500);

        //key= getInputWait();

        // Easy flashing
        if(key==BTN_LEFT){
            DoString(0,8,"Enter ISP!");
            lcdDisplay();
            ISPandReset();
        }
    }
}

void tick_minimal(void){
    return;
};
