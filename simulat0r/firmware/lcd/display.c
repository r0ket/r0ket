#define lcdDisplay _hideaway_lcdDisplay
#define lcdInit _hideaway_lcdInit
#include "../../../firmware/lcd/display.c"
#undef lcdDisplay
#undef lcdInit

#include "simulator.h"

void lcdDisplay() {
  simlcdDisplayUpdate();
}

void lcdInit() {
}
