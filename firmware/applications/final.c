#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/print.h"

#include <string.h>

/**************************************************************************/

#include "final.gen"

static const struct MENU mainmenu = {"Menu", mentry};

void initNick();
void fancyNickname();

void main_final(void) {
    //checkFirstBoot();

    initNick();
    while(1){
#ifndef FINAL
        if(getInputRaw()==BTN_LEFT)
            ISPandReset();
#endif
        if(getInput()){
            handleMenu(&mainmenu);
            getInputWaitRelease();
            initNick();
        };
        fancyNickname();
        delayms_queue(100);
    };
};

void tick_final(void);
