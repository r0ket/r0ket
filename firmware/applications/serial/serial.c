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
#include "usbcdc/util.h"

#include <string.h>

#define BEACON_CHANNEL 81
#define BEACON_MAC     "\x1\x2\x3\x2\1"

uint32_t const testkey[4] = {
    0xB4595344,0xD3E119B6,0xA814D0EC,0xEFF5A24E
};

#if CFG_USBMSC
#error "MSC is defined"
#endif

#if !CFG_USBCDC
#error "CDC is not defined"
#endif

/**************************************************************************/


void ser_enable(void) {
    usbCDCInit();
};

void ser_disable(void) {
    usbCDCOff();
};

#define myLEN 10
void ser_read(){
    uint8_t buf[myLEN+1];
    int l=myLEN;

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

void ser_say(){
        puts("hello world\r\n");
};

void f_init(){
    nrf_init();
    struct NRF_CFG config = {
        .channel= BEACON_CHANNEL,
        .txmac= BEACON_MAC,
        .nrmacs=1,
        .mac0=  BEACON_MAC,
        .maclen ="\x10",
    };

    nrf_config_set(&config);
};

void f_beacon(void){
    struct NRF_CFG config = {
        .channel= BEACON_CHANNEL,
        .txmac= BEACON_MAC,
        .nrmacs=1,
        .mac0=  BEACON_MAC,
        .maclen ="\x10",
    };

    nrf_config_set(&config);
};

int enctoggle=0;

void f_enctog(){
    enctoggle=1-enctoggle;
    lcdClear();
    lcdPrint("Encryption:");
    if(enctoggle)
        lcdPrintln("ON");
    else
        lcdPrintln("Off");
};

const char* IntToStrX(unsigned int num, unsigned int mxlen){
#define LEN 32
	static char s[LEN+1];
	char * o=s;
	int len;
	s[LEN]=0;
	for (len=(LEN-1);len>=(LEN-mxlen);len--){
		s[len]=(num%16)+'0';
		if(s[len]>'9')
			s[len]+='A'-'9'-1;
		num/=16;
	};
	return &s[len+1];
};

void f_recser(void){
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int len;

    getInputWaitRelease();

    do{
        len=nrf_rcv_pkt_time_encr(1000,sizeof(buf),buf,enctoggle?testkey:NULL);

        if(len==0){
            puts("(Timeout)\r\n");
        };
        puts("pkt: ");
        puts("[");puts(IntToStrX(len,2));puts("] ");
        puts(IntToStrX( *(int*)(buf+  0),2 ));
        puts(" ");
        puts(IntToStrX( *(int*)(buf+  1),2 ));
        puts(" ");
        puts(IntToStrX( *(int*)(buf+  2),2 ));
        puts(" ");
        puts(IntToStrX( *(int*)(buf+  3),2 ));
        puts(".");
        puts(IntToStrX( *(int*)(buf+  4),8 ));
        puts(".");
        puts(IntToStrX( *(int*)(buf+  8),8 ));
        puts(".");
        puts(IntToStrX( *(int*)(buf+ 12),4 ));
        puts(" [");

        len=crc16(buf,14);
        puts(IntToStrX(len,4)); puts("]\r\n");
        delayms(10);
    }while ((getInputRaw())==BTN_NONE);

};
