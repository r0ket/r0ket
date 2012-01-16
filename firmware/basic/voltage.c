#include <sysinit.h>

#include "basic/basic.h"
#include "funk/nrf24l01p.h"

static uint32_t voltage=5000;
static uint8_t chrg=1;

void VoltageCheck(void){

    chrg=gpioGetValue(RB_PWR_CHRG);
    //slow down the adc for our high impedance volatage devider
    ADC_AD0CR = ((CFG_CPU_CCLK / SCB_SYSAHBCLKDIV) / 100000 - 1 ) << 8;
	voltage = adcRead(1);
    //speed it up again
    ADC_AD0CR = ((CFG_CPU_CCLK / SCB_SYSAHBCLKDIV) / 1000000 - 1 ) << 8;

	voltage *= 10560;
	voltage /= 1024;
    
    //add the drop over the voltage switch
    voltage += 50;

    //battery is assumed empty if the volatge falls bellow 3.5V
	if( voltage < 3500 ){
        nrf_off();
	    gpioSetValue (RB_PWR_GOOD, 0); 
	    gpioSetValue (RB_LCD_BL, 0);

        //put the chip into deep power down
	    SCB_SCR |= SCB_SCR_SLEEPDEEP;
	    PMU_PMUCTRL = PMU_PMUCTRL_DPDEN_DEEPPOWERDOWN;
	    __asm volatile ("WFI");
    };
};

uint32_t GetVoltage(void){
    return voltage;
};

uint8_t GetChrgStat(void){
    return !chrg;
};
