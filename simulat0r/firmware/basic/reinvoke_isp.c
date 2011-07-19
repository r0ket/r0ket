#include <core/gpio/gpio.h>                                                                                 

void ReinvokeISP(void) {
}

void EnableWatchdog(uint32_t ms) {
}

void ISPandReset(int delay){
  EnableWatchdog(1000*delay);
  ReinvokeISP();
};
