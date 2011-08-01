#include "simulator.h"
#include "../firmware/basic/basic.h"
#include "../firmware/basic/config.h"
#include "../firmware/lcd/display.h"

#include <unistd.h>

void simlcdDisplayUpdate() {
  write(1,"\033[H",3);
  const char* symbolOff=GLOBAL(lcdinvert)?"_":"@";
  const char* symbolOn=GLOBAL(lcdinvert)?"#":".";

  for(int y=0; y<RESY; ++y) {
    for(int x=0; x<RESX; ++x) {
      write(1,(lcdGetPixel((GLOBAL(lcdmirror) /* LCD_MIRRORX */ )?(RESX-x-1):x,(0 /* & LCD_MIRRORY */)?(RESY-y-1):y)?symbolOn:symbolOff),1);
    }
      write(1,("\n"),1);
  }
}

int simButtonPressed(int button) {
  return 1; // TODO
}

void simSetLEDHook(int led) {
  // TODO
}

int main(void) {
  simulator_main();
}
