#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdint.h>

void simlcdDisplayUpdate();

int simButtonPressed(int button);

int simGetLED(int led);
void simSetLED(int led,uint32_t bitVal);
void simSetLEDHook(int led);


int simulator_main(void);

#endif
