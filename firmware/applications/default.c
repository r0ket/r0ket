#include <sysinit.h>
#include "basic/basic.h"

/**************************************************************************/

void main_default(void) {
    systickInit(10);

    if(getInputRaw()==BTN_ENTER){
        ISPandReset(7);
    };

    return;
};

// every 10 ms
void tick_default(void) {
    static int ctr;
    ctr++;
    if(ctr>100){
        VoltageCheck();
        ctr=0;
    };
    if(ctr%5==0){
        if(GetVoltage()<3600){
            IOCON_PIO1_11 = 0x0;
            gpioSetDir(RB_LED3, gpioDirection_Output);
            if( (ctr/5)%10 == 1 )
                gpioSetValue (RB_LED3, 1);
            else
                gpioSetValue (RB_LED3, 0);
        };
    };

    return;
};
