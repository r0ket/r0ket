#include <sysinit.h>

/**************************************************************************/

void module_default(void) {
    systickInit(10);
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

    return;
};
