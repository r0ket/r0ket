#include <sysinit.h>

#include "basic/basic.h"


void VoltageCheck(void){
	uint32_t results;

	results = adcRead(1);
	results *= 10560;
	results /= 1024;

	if( results < 3500 ){
	    gpioSetValue (RB_PWR_GOOD, 0); 
	    gpioSetValue (RB_LCD_BL, 0); 
	    SCB_SCR |= SCB_SCR_SLEEPDEEP;
	    PMU_PMUCTRL = PMU_PMUCTRL_DPDEN_DEEPPOWERDOWN;
	    __asm volatile ("WFI");
    };
};
