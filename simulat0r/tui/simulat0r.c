#include "simulator.h"
#include "../firmware/lcd/display.h"

#include <unistd.h>

extern int lcd_layout;

void simlcdDisplayUpdate() {
  write(1,"\033[H",3);
  for(int y=0; y<RESY; ++y) {
    for(int x=0; x<RESX; ++x) {
      if(lcd_layout & LCD_INVERTED) {
	write(1,(lcdGetPixel((lcd_layout & LCD_MIRRORX)?(RESX-x-1):x,(lcd_layout & LCD_MIRRORY)?(RESY-y-1):y)?"#":"_"),1);
      } else {
	write(1,(lcdGetPixel((lcd_layout & LCD_MIRRORX)?(RESX-x-1):x,(lcd_layout & LCD_MIRRORY)?(RESY-y-1):y)?".":"@"),1);
      }
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
