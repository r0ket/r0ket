
#include "core/sysinit.h"

#include "basic/basic.h"

#include "lcd/render.h"

#include "pmu/pmu.h"

void ReinvokeISP(void);

/**************************************************************************/

void wrapper(void);

int simulator_main(void) {

  // Configure cpu and mandatory peripherals
  systemInit();

  // initialise basic badge functions
  rbInit();
  
  lcdInit(); // display

  lcdFill(0);
  lcdDisplay();

  wrapper(); // see module/ subdirectory
}

static uint32_t ledstate[4];

int simGetLED(int led) {
  return ledstate[led];
}

void simSetLED(int led,uint32_t bitVal) {  
  ledstate[led]=bitVal;
  simSetLEDHook(led);
}

