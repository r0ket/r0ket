#if 0
#include "../firmware/lcd/display.c"
#else

#include "../firmware/lcd/display.h"
#include "simulator.h"

uint8_t lcdBuffer[RESX*RESY_B];
int lcd_layout = 0;
const int TYPE_DATA=0;


void lcdInit(void) {
  fprintf(stderr,"lcdInit(void)\n");
}


void lcdFill(char f){
  int x;
  for(x=0;x<RESX*RESY_B;x++) {
    lcdBuffer[x]=f;
  }
};

void lcdSetPixel(char x, char y, bool f){
  char y_byte = (RESY-(y+1)) / 8;
  char y_off = (RESY-(y+1)) % 8;
  char byte = lcdBuffer[y_byte*RESX+(RESX-(x+1))];
  if (f) {
    byte |= (1 << y_off);
  } else {
    byte &= ~(1 << y_off);
  }
  lcdBuffer[y_byte*RESX+(RESX-(x+1))] = byte;
}

bool lcdGetPixel(char x, char y){
  char y_byte = (RESY-(y+1)) / 8;
  char y_off = (RESY-(y+1)) % 8;
  char byte = lcdBuffer[y_byte*RESX+(RESX-(x+1))];
  return byte & (1 << y_off);
}

void lcdDisplay() {
  simlcdDisplayUpdate();
}

inline void lcdInvert(void) {
  lcdToggleFlag(LCD_INVERTED);
}

void lcdToggleFlag(int flag) {
  lcd_layout=lcd_layout ^ flag;
}

#endif
