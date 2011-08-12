#ifdef __APPLE__
#define lcdRefresh _hideaway_lcdRefresh
#endif
#define lcdDisplay _hideaway_lcdDisplay
#define lcdInit _hideaway_lcdInit
#include "../../../firmware/lcd/display.c"
#undef lcdDisplay
#undef lcdInit
#ifdef __APPLE__
#undef lcdRefresh
#endif

#include "simulator.h"

void lcdDisplay() {
  simlcdDisplayUpdate();
}

#ifdef __APPLE__
void lcdRefresh() {
    lcdDisplay();
}
#endif

void lcdInit() {
}
