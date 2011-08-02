#include <sysinit.h>

#include "basic/basic.h"
#include "basic/byteorder.h"
#include "basic/config.h"

#include "funk/nrf24l01p.h"
#include "funk/openbeacon.h"


#define B_INTERVAL 100

/**************************************************************************/

void do_openbeacon(){
    openbeaconSend();
}

void init_beacon(void){
    nrf_init();
    openbeaconSetup();
};

void tick_beacon(void){
    static int beaconctr=0;
    if(GLOBAL(privacy)>0)
        return;

    if(beaconctr--<0){
        push_queue(&do_openbeacon);
        beaconctr=B_INTERVAL/SYSTICKSPEED/2;
        beaconctr+=getRandom()%(beaconctr*2);
    };
};

