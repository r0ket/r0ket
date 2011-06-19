#include <sysinit.h>
#include "basic/basic.h"

uint8_t getInput(void) {
    uint8_t result = BTN_NONE;

    if (gpioGetValue(RB_BTN3)==0) {
        while(gpioGetValue(RB_BTN3)==0);
        result += BTN_UP;
    }

    if (gpioGetValue(RB_BTN2)==0) {
        while(gpioGetValue(RB_BTN2)==0);
        result += BTN_DOWN;
    }

    if (gpioGetValue(RB_BTN4)==0) {
        while(gpioGetValue(RB_BTN4)==0);
        result += BTN_ENTER;
    }

    if (gpioGetValue(RB_BTN0)==0) {
        while(gpioGetValue(RB_BTN0)==0);
        result += BTN_LEFT;
    }

    if (gpioGetValue(RB_BTN1)==0) {
        while(gpioGetValue(RB_BTN1)==0);
        result += BTN_RIGHT;
    }

    if (result == (BTN_LEFT+BTN_UP+BTN_ENTER)){ /* Development hack */
        ISPandReset(5);
    }

    return result;
}

