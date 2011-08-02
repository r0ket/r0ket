#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/print.h"

#include <string.h>

/**************************************************************************/

#include "tester.gen"

void main_tester(void) {
    handleMenu(&mainmenu);
    gotoISP();
};

void tick_tester(void);
