#include <sysinit.h>

#include "core/cpu/cpu.h"
#include "core/pmu/pmu.h"

#include "basic/basic.h"
#include "lcd/render.h"
#include "filesystem/ff.h"

__attribute__ ((used, section("crp"))) const uint32_t the_crp=0x87654321;

/**************************************************************************/

void wrapper(void);

int main(void) {
    // Configure cpu and mandatory peripherals
    cpuInit();                                // Configure the CPU
// we do it later
//    systickInit(CFG_SYSTICK_DELAY_IN_MS);     // Start systick timer
// cpuInit already calls this
//    gpioInit();                               // Enable GPIO 
    pmuInit();                                // Configure power management
    adcInit();                                // Config adc pins to save power

    // initialise basic badge functions
    rbInit();

    fsInit();
  
    lcdInit(); // display

    //lcdFill(0);
    lcdDisplay();

	wrapper(); // see module/ subdirectory
}
