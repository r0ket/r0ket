#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/print.h"

#include <string.h>

/**************************************************************************/

#include "tester.gen"

static const struct MENU mainmenu = {"Mainmenu", mentry};

void main_tester(void) {
    handleMenu(&mainmenu);
    gotoISP();
};

void no_tick_tester(void){
    static int foo=0;
    static int toggle=0;
	if(foo++>80){
        toggle=1-toggle;
		foo=0;
        gpioSetValue (RB_LED0, toggle); 
	};
    return;
};


