#include <sysinit.h>

#include "basic/basic.h"

#define RANGE (10)
#define HYST (4)
uint32_t light=300*HYST;
char _isnight=1;

void LightCheck(void){
    int iocon;
    char iodir;

    int value;

    iocon=IOCON_PIO1_11;
//    iodir=gpioGetDir(RB_LED3);
    iodir= (GPIO_GPIO1DIR & (1 << (RB_LED3) ))?1:0;

    gpioSetDir(RB_LED3, gpioDirection_Input);
    IOCON_PIO1_11 = IOCON_PIO1_11_FUNC_AD7|IOCON_PIO1_11_ADMODE_ANALOG;
    light-=light/HYST;
	light += adcRead(7);
    
    gpioSetDir(RB_LED3, iodir);
    IOCON_PIO1_11=iocon;

    static uint16_t threshold=0;
    if(threshold==0){
        // read threshold from config
        threshold=320 * HYST;
    };

    if(_isnight && light>(threshold+RANGE*HYST))
        _isnight=0;

    if(!_isnight && light<threshold)
        _isnight=1;
};

uint32_t GetLight(void){
    return light/HYST;
};

char isNight(void){
    return _isnight;
};
