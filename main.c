#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"

#include "pmu/pmu.h"

void ReinvokeISP(void);

/**************************************************************************/

void wrapper(void);

int main(void) {
    // Configure cpu and mandatory peripherals
    systemInit();

    // initialise basic badge functions
    rbInit();
  
    lcdInit(); // display

    lcdFill(0);
    lcdDisplay(0);

	wrapper(); // see module/ subdirectory
}
