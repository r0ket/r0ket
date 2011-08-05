#include <sysinit.h>

#include "basic/basic.h"

uint32_t results=5000;
static uint8_t chrg=1;

void VoltageCheck(void){
};

uint32_t GetVoltage(void){
  return results;
};

uint8_t GetChrgStat(void){
  return !chrg;
}
