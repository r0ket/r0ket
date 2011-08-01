#include <sysinit.h>

#include "basic/basic.h"

uint32_t results=5000;

void VoltageCheck(void){

	results = adcRead(1);
	results *= 10560;
	results /= 1024;

	if( results < 3500 ){
        nrf_off();
	    gpioSetValue (RB_PWR_GOOD, 0); 
	    gpioSetValue (RB_LCD_BL, 0); 
	    SCB_SCR |= SCB_SCR_SLEEPDEEP;
	    PMU_PMUCTRL = PMU_PMUCTRL_DPDEN_DEEPPOWERDOWN;
	    __asm volatile ("WFI");
    };
};

uint32_t GetVoltage(void){
    return results;
};
