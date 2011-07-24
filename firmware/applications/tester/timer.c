#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/lcd.h"
#include "lcd/print.h"
#include "lcd/allfonts.h"

#include "filesystem/ff.h"
#include "filesystem/select.h"
#include "funk/nrf24l01p.h"
#include "usb/usbmsc.h"

#include <string.h>

/**************************************************************************/

void s_ticks(void) {
    int dx=0;
    int dy=8;
    lcdClear();
    dx=DoString(0,dy,"Ticks:");
    dx=DoString(0,dy+16,"Qdepth:");
    while ((getInputRaw())==BTN_NONE){
        DoInt(0,dy+8,_timectr);
        DoInt(dx,dy+16,(the_queue.qend-the_queue.qstart+MAXQENTRIES)%MAXQENTRIES);
        lcdDisplay();
        __asm volatile ("WFI");
    };
    dy+=16;
    dx=DoString(0,dy,"Done.");
};

void b_one(void){
    gpioSetValue (RB_LED2, 0); 
    delayms(100);
    gpioSetValue (RB_LED2, 1); 
    delayms(100);
    gpioSetValue (RB_LED2, 0); 
};

void do_qone(void) {
    work_queue();
};

void do_q(void) {
    idle_queue(500);
};

void push_qone(void) {
    push_queue(&b_one);
};

