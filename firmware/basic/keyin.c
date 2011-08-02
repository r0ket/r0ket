#include <sysinit.h>
#include "basic/basic.h"

uint8_t getInputRaw(void) {
    uint8_t result = BTN_NONE;

    if (gpioGetValue(RB_BTN3)==0) {
        result |= BTN_UP;
    }

    if (gpioGetValue(RB_BTN2)==0) {
        result |= BTN_DOWN;
    }

    if (gpioGetValue(RB_BTN4)==0) {
        result |= BTN_ENTER;
    }

    if (gpioGetValue(RB_BTN0)==0) {
        result |= BTN_LEFT;
    }

    if (gpioGetValue(RB_BTN1)==0) {
        result |= BTN_RIGHT;
    }

    return result;
}

uint8_t getInput(void) {
    uint8_t key = BTN_NONE;

    key=getInputRaw();
    /* XXX: we should probably debounce the joystick.
            Any ideas how to do this properly?
            For now wait for any release.
     */
    if(key != BTN_NONE)
        while(key==getInputRaw())
            work_queue();

    return key;
}

uint8_t getInputWait(void) {
    uint8_t key;
    while ((key=getInputRaw())==BTN_NONE)
        work_queue();
    delayms_queue(10); /* Delay a little more to debounce */
    return key;
};

uint8_t getInputWaitTimeout(int timeout) {
    uint8_t key;
    int end=_timectr+timeout*(1000/SYSTICKSPEED);
    while ((key=getInputRaw())==BTN_NONE){
        if(_timectr>end)
            break;
        work_queue();
    };
    delayms_queue(10); /* Delay a little more to debounce */
    return key;
};

uint8_t getInputWaitRepeat(void) {
    static uint8_t oldkey=BTN_NONE;
    static int repeatctr=0;
    uint8_t key=getInputRaw();

    if (key != BTN_NONE && key==oldkey){
        int dtime;
        if(!repeatctr)
            dtime=600;
        else if(repeatctr<5)
            dtime=250;
        else if(repeatctr<25)
            dtime=150;
        else if(repeatctr<50)
            dtime=80;
        else
            dtime=20;
        repeatctr++;
        int end=_timectr+(dtime/SYSTICKSPEED);
        while(_timectr<end && key==getInputRaw())
            work_queue();
        key=getInputRaw();
        if (key==oldkey)
            return key;
    };

    repeatctr=0;
    while ((key=getInputRaw())==BTN_NONE){
        work_queue();
    };
    delayms_queue(10); /* Delay a little more to debounce */
    oldkey=key;
    return key;
};

void getInputWaitRelease(void) {
    while (getInputRaw()!=BTN_NONE)
        work_queue();
    delayms_queue(10); /* Delay a little more to debounce */
};


