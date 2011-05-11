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
extern uint8_t buffer[RESX*RESY_B];

void write(uint8_t cd, uint8_t data);
void read(uint8_t data);
void init(void);
void fill(char f);
void display(uint32_t shift);

#endif
