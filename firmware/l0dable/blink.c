#include <sysinit.h>

#include "basic/basic.h"

#include "usetable.h"

void ram(void){
	for (int x=0;x<20;x++){
		gpioSetValue (RB_LED1, x%2); 
		delayms(50);
	};
    gpioSetValue (RB_LED1, 0); 
};
