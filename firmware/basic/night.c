#include <sysinit.h>

#include "basic/basic.h"
#include "basic/config.h"

#define SAMPCT (4)
uint32_t light=150*SAMPCT;
char _isnight=1;

#define threshold GLOBAL(daytrig)
#define RANGE GLOBAL(daytrighyst)

void LightCheck(void){
    int iocon;
    char iodir;

    int value;

    iocon=IOCON_PIO1_11;
//    iodir=gpioGetDir(RB_LED3);
    iodir= (GPIO_GPIO1DIR & (1 << (RB_LED3) ))?1:0;

    gpioSetDir(RB_LED3, gpioDirection_Input);
    IOCON_PIO1_11 = IOCON_PIO1_11_FUNC_AD7|IOCON_PIO1_11_ADMODE_ANALOG;
    light-=light/SAMPCT;
	light += (adcRead(7)/2);
    
    gpioSetDir(RB_LED3, iodir);
    IOCON_PIO1_11=iocon;

    if(threshold==0){ // uninitialized?
        threshold=320;
    };

    if(_isnight && light/SAMPCT>(threshold+RANGE))
        _isnight=0;

    if(!_isnight && light/SAMPCT<threshold)
        _isnight=1;
};

uint32_t GetLight(void){
    return light/SAMPCT;
};

char isNight(void){
    return _isnight;
};
