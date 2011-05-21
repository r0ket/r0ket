#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"

#include "pmu/pmu.h"
#include "eeprom/eeprom.h"

void ReinvokeISP(void);

/**************************************************************************/

void wrapper(void);

int main(void) {
    // Configure cpu and mandatory peripherals
    systemInit();

    //enable clocks to adc and watchdog
    pmuInit();

    // initialise basic badge functions
    rbInit();
  
    lcdInit(); // display
  
    adcInit();

    lcdFill(0);
    lcdDisplay(0);

	wrapper(); // see module/ subdirectory
}
