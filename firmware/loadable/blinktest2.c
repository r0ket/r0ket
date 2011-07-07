#include <sysinit.h>
#include "basic/basic.h"
void ram(void);
void setLED(int i);

void ram(void){
	for (int x=0;x<20;x++){
        setLED(x);
		delayms(50);
	};
};

void setLED(int i)
{
		gpioSetValue (RB_LED1, i%2); 
}


