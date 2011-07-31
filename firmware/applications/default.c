#include <sysinit.h>
#include "basic/basic.h"

#include "lcd/lcd.h"
#include "lcd/fonts/smallfonts.h"
#include "lcd/print.h"
#include "filesystem/ff.h"
#include "usb/usbmsc.h"
#include "basic/random.h"

/**************************************************************************/

void main_default(void) {
    systickInit(SYSTICKSPEED);

    switch(getInputRaw()){
        case BTN_ENTER:
            ISPandReset();
            break;
        case BTN_DOWN:
            usbMSCInit();
            while(1)
                delayms_power(100);
            break;
    };

    readConfig();
    randomInit();

    return;
};

// every 10 ms
void tick_default(void) {
    static int ctr;
    ctr++;
    incTimer();
    if(ctr>1000/SYSTICKSPEED){
        if(!adcMutex){
            VoltageCheck();
            LightCheck();
            ctr=0;
        }else{
            ctr--;
        };
    };

    if(ctr>100/SYSTICKSPEED){
        if(isNight()){
            backlightSetBrightness(GLOBAL(lcdbacklight));
            lcdSetInvert(0);
        } else {
            backlightSetBrightness(0);
            if(GLOBAL(dayinvert))
                lcdSetInvert(1);
            else
                lcdSetInvert(0);
        }
    }

    if(ctr%(50/SYSTICKSPEED)==0){

        if(GetVoltage()<3600
#ifdef SAFE
                || GetVoltage() > 10000 // pin not connected
#endif
                ){
            IOCON_PIO1_11 = 0x0;
            gpioSetDir(RB_LED3, gpioDirection_Output);
            if( (ctr/(50/SYSTICKSPEED))%10 == 1 )
                gpioSetValue (RB_LED3, 1);
            else
                gpioSetValue (RB_LED3, 0);
        };
    };

    return;
};
