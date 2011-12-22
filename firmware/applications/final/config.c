#include <sysinit.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/print.h"
#include "lcd/render.h"
#include "lcd/display.h"

#include "filesystem/ff.h"

#include <string.h>

/**************************************************************************/
//# MENU config
void changer(void){
    if(execute_file("config.int")){
        lcdClear();
        lcdPrintln("config");
        lcdPrintln("l0dable");
        lcdPrintln("not found");
        lcdRefresh();
        getInputWait();
        getInputWaitRelease();
    }
}

