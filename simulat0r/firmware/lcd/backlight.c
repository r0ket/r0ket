#include "basic/basic.h"
    
uint32_t brightness = 100;
    
void backlightInit(void) {
}
    
int backlightSetBrightness(uint32_t percentage) {
  if ((percentage < 0) || (percentage > 100)) {
    /* brightness must be a value between 1 and 100 */
    return -1;
  }
    
  brightness = percentage;    
  return 0;
}
    
uint32_t backlightGetBrightness(void) {
  return brightness;
}
