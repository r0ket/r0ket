#include <sysinit.h>

#include "basic/basic.h"
#include "basic/config.h"

#include <string.h>

/**************************************************************************/

#include "serial.gen"

void main_serial(void) {
    GLOBAL(daytrig)=10;
    GLOBAL(lcdbacklight)=10;
    handleMenu(&mainmenu);
    gpioSetValue (RB_LED3, 1); 
    ISPandReset();
};

void serial(void);
