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

void getInputWaitRelease(void) {
    while (getInputRaw()!=BTN_NONE)
        work_queue();
    delayms_queue(10); /* Delay a little more to debounce */
};


