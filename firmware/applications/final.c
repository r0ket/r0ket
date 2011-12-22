#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/print.h"

#include <string.h>

#include "basic/ecc.h"

#include "basic/config.h"
#include "filesystem/execute.h"

/**************************************************************************/

#include "final.gen"

void init_nick();
void fancyNickname();

void main_final(void) {

    init_nick();
	if(GLOBAL(privacy)>2){ //firstboot
        execute_file("initanim.c0d");
        if(execute_file("1boot.int")){
            lcdPrintln("Badge SETUP");
            lcdPrintln("error.");
            lcdPrintln("Features may");
            lcdPrintln("be broken.");
            lcdRefresh();
            getInputWait();
            getInputWaitRelease();
            GLOBAL(privacy)=0;
        }else{
            saveConfig();
        };
    };
    //checkFirstBoot();
    init_final();
    menuflags|=MENU_TIMEOUT;

    while(1){
        if(GLOBAL(develmode))
            if(getInputRaw()==BTN_LEFT)
                ISPandReset();
        if(getInput()){
            handleMenu(&mainmenu);
            getInputWaitRelease();
            init_nick();
        };
        work_queue();
        fancyNickname();
    };
};

void tick_final(void);
