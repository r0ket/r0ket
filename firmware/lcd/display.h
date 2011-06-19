#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#include <sysdefs.h>

/*
#badge    <s> 96x68
#badge    <s> das farbige hat 98x70 
#badge    <s> das laengliche 128x32 
*/

#define RESX 96
#define RESY 68
#define RESY_B 9


/* Display buffer */
extern uint8_t lcdBuffer[RESX*RESY_B];

void lcdInit(void);
void lcdFill(char f);
void lcdDisplay(uint32_t shift);
void lcdInvert(void);
void lcdSetPixel(char x, char y, bool f);
bool lcdGetPixel(char x, char y);

#endif
