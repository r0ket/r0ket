#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/lcd.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"

#include "usbcdc/usb.h"
#include "usbcdc/usbcore.h"
#include "usbcdc/usbhw.h"
#include "usbcdc/cdcuser.h"
#include "usbcdc/cdc_buf.h"

#include <string.h>

#if CFG_USBMSC
#error "MSC is defined"
#endif

#if !CFG_USBCDC
#error "CDC is not defined"
#endif

/**************************************************************************/


void f_ser(void) {
    usbCDCInit();
};

void f_disconnect(void) {
    usbCDCOff();
};

#define LEN 10
void f_sread(){
    uint8_t buf[LEN+1];
    int l=LEN;

    lcdPrint("Bytes:");
    CDC_OutBufAvailChar (&l);
    lcdPrintInt(l);
    lcdNl();

    lcdPrint("read:");
    CDC_RdOutBuf (buf, &l);
    lcdPrintInt(l);
    lcdNl();

    buf[l]=0;
    lcdPrintln(buf);
};

void f_echo(){
    uint8_t buf[2] = {0,0};
    int l;
    while(1){
        CDC_OutBufAvailChar(&l);
        if( l ){
            l = 1;
            CDC_RdOutBuf (buf, &l);
            puts(buf);
        }
        //puts("hello world\r\n");
        //delayms(1);
    }
};

void f_say(){
        puts("hello world\r\n");
};
