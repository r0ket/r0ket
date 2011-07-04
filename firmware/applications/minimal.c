#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/allfonts.h"

void backlightInit(void);

/**************************************************************************/

void main_minimal(void) {
    //int yctr=8;
    int dx=0;
    char key;
    int ctr = 0;
    backlightInit();
    font_direction = FONT_DIR_LTR; // LeftToRight is the default
    //yctr=18;
    int toggle = 0;
    volatile int foo;
    volatile char foo2[] = "12345";
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
        }else if(key==BTN_RIGHT){
            gpioSetValue (RB_LED1, toggle);
            foo = strspn(foo2, "123456789");
            gpioSetValue (RB_LED2, toggle);
            toggle = 1 - toggle;
        }

        // Display nickname
        //font = &Font_Ubuntu36pt;
        dx=DoString(0,0,"Test");
        dx=DoInt(dx,0,ctr++);
    }
    return;
}

void tick_minimal(void){
    return;
};
