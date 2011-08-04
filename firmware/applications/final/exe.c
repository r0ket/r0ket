#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "lcd/lcd.h"
#include "lcd/allfonts.h"
#include "lcd/print.h"
#include "usb/usbmsc.h"
#include "filesystem/ff.h"
#include "filesystem/select.h"
#include "filesystem/execute.h"

/**************************************************************************/

//# MENU execute
void execute_menu(void){
    while(getInput()!=BTN_NONE);
    executeSelect("C0D");
    lcdRefresh();
};

//# MENU |devel
void execute_devel(void){
    while(getInput()!=BTN_NONE);
    executeSelect("INT");
    lcdRefresh();
};
