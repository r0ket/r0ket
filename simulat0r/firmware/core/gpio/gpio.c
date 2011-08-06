#include <core/gpio/gpio.h>
#include "basic/basic.h"
#include "../simcore/simulator.h"

static bool compair(uint32_t a1, uint32_t a2, uint32_t b1, uint32_t b2) {
  return a1==b1 && a2==b2;
}

void gpioInit (void) {
  printf("void gpioInit (void)\n");
}

void gpioSetDir (uint32_t portNum, uint32_t bitPos, gpioDirection_t dir) {
  printf("void gpioSetDir (portNum %d, bitPos %d, dir %x)\n",portNum,bitPos,dir);
}

uint32_t gpioGetValue (uint32_t portNum, uint32_t bitPos) {
  if(compair(portNum, bitPos, RB_BTN3)) return simButtonPressed(BTN_UP);
  if(compair(portNum, bitPos, RB_BTN2)) return simButtonPressed(BTN_DOWN);
  if(compair(portNum, bitPos, RB_BTN4)) return simButtonPressed(BTN_ENTER);
  if(compair(portNum, bitPos, RB_BTN0)) return simButtonPressed(BTN_LEFT);
  if(compair(portNum, bitPos, RB_BTN1)) return simButtonPressed(BTN_RIGHT);

  if(compair(portNum, bitPos, RB_LED0)) return simGetLED(0);
  if(compair(portNum, bitPos, RB_LED1)) return simGetLED(1);
  if(compair(portNum, bitPos, RB_LED2)) return simGetLED(2);
  if(compair(portNum, bitPos, RB_LED3)) return simGetLED(3);

  fprintf(stderr,"Unimplemented gpioGetValue portNum %d %x bit %d\n",portNum, portNum, bitPos);
  return 0;
}

void gpioSetValue (uint32_t portNum, uint32_t bitPos, uint32_t bitVal) {
  if(compair(portNum, bitPos, RB_LED0)) return simSetLED(0,bitVal);
  if(compair(portNum, bitPos, RB_LED1)) return simSetLED(1,bitVal);
  if(compair(portNum, bitPos, RB_LED2)) return simSetLED(2,bitVal);
  if(compair(portNum, bitPos, RB_LED3)) return simSetLED(3,bitVal);

  fprintf(stderr,"Unimplemented gpioSetValue portNum %d %x bit %d\n",portNum, portNum, bitPos);
}

void gpioSetInterrupt (uint32_t portNum, uint32_t bitPos, gpioInterruptSense_t sense, gpioInterruptEdge_t edge, gpioInterruptEvent_t event) {
  printf("void gpioSetInterrupt (uint32_t portNum, uint32_t bitPos, gpioInterruptSense_t sense, gpioInterruptEdge_t edge, gpioInterruptEvent_t event)\n");
}

void gpioIntEnable (uint32_t portNum, uint32_t bitPos) {
  printf("void gpioIntEnable (uint32_t portNum, uint32_t bitPos)\n");
}

void gpioIntDisable (uint32_t portNum, uint32_t bitPos) {
  printf("void gpioIntDisable (uint32_t portNum, uint32_t bitPos)\n");
}

uint32_t  gpioIntStatus (uint32_t portNum, uint32_t bitPos) {
  printf("uint32_t  gpioIntStatus (uint32_t portNum, uint32_t bitPos)\n");
  return 0;
}

void gpioIntClear (uint32_t portNum, uint32_t bitPos) {
  printf("void gpioIntClear (uint32_t portNum, uint32_t bitPos)\n");
}

void gpioSetPullup (volatile uint32_t *ioconRegister, gpioPullupMode_t mode) {
  printf("void gpioSetPullup (volatile uint32_t *ioconRegister, gpioPullupMode_t mode)\n");
}

