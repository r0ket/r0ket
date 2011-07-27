#include <sysinit.h>

#include "basic/basic.h"
#include "basic/byteorder.h"

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

#define REMOTE_CHANNEL 91
#define REMOTE_MAC     "REM0T"

#if CFG_USBMSC
#error "MSC is defined"
#endif

#if !CFG_USBCDC
#error "CDC is not defined"
#endif

/**************************************************************************/

uint32_t const remotekey[4] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
};

void r_init(void){
    nrf_init();

    struct NRF_CFG config = {
        .channel= REMOTE_CHANNEL,
        .txmac= REMOTE_MAC,
        .nrmacs=1,
        .mac0=  REMOTE_MAC,
        .maclen ="\x10",
    };

    nrf_config_set(&config);
};

void s_init(void){
    usbCDCInit();
    nrf_init();

    struct NRF_CFG config = {
        .channel= REMOTE_CHANNEL,
        .txmac= REMOTE_MAC,
        .nrmacs=1,
        .mac0=  REMOTE_MAC,
        .maclen ="\x10",
    };

    nrf_config_set(&config);
};

void r_recv(void){
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int len;

    nrf_rcv_pkt_start();

    getInputWaitRelease();

    while(!getInputRaw()){
        delayms(100);
        len=nrf_rcv_pkt_poll_dec(sizeof(buf),buf,remotekey);

        if(len<=0)
            continue;

        if(buf[1]=='C'){ // Cursor
            puts("C ");
            puts(IntToStrX( buf[2],2 ));
            puts(" ");
            puts(IntToStrX( uint8ptouint32(buf+4), 8 ));
            puts(" ");
            puts(IntToStrX( uint8ptouint32(buf+8), 8 ));
        }else{
            puts("U ");
//          puts("[");puts(IntToStrX(len,2));puts("] ");
            puts(IntToStrX( *(int*)(buf+  0),2 ));
            puts(" ");
            puts(IntToStrX( *(int*)(buf+  1),2 ));
            puts(" ");
            puts(IntToStrX( *(int*)(buf+  2),2 ));
            puts(" ");
            puts(IntToStrX( *(int*)(buf+  3),2 ));
            puts(" ");
            puts(IntToStrX( uint8ptouint32(buf+4),8 ));
            puts(".");
            puts(IntToStrX( uint8ptouint32(buf+8),8 ));
            puts(" ");
            puts(IntToStrX( uint8ptouint32(buf+10),4 ));
        };
        puts("\r\n");

    };

    nrf_rcv_pkt_end();

}



void r_s1(void){
    static int ctr=1;
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int status;

    buf[0]=0x10; // Length: 16 bytes
    buf[1]='1'; // Proto
    buf[2]=0x00;
    buf[3]=0x00; // Unused

    uint32touint8p(ctr++,buf+4);

    uint32touint8p(0x5ec,buf+8);

    buf[12]=0xff; // salt (0xffff always?)
    buf[13]=0xff;
    status=nrf_snd_pkt_crc_encr(16,buf,remotekey);

    lcdClear();
    lcdPrint("F-St:"); lcdPrintInt(status); 
    lcdDisplay();

};
void r_s2(void){
    static int ctr=1;
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int status;

    buf[0]=0x10; // Length: 16 bytes
    buf[1]='C'; // Proto
    buf[2]=0x00;
    buf[3]=0x00; // Unused

    uint32touint8p(ctr++,buf+4);

    uint32touint8p(0x5ec,buf+8);

    buf[12]=0xff; // salt (0xffff always?)
    buf[13]=0xff;
    status=nrf_snd_pkt_crc_encr(16,buf,remotekey);

    buf[0]=0x10; // Length: 16 bytes
    buf[1]='I'; // Proto
    buf[2]=0x00;
    buf[3]=0x00; // Unused

    uint32touint8p(ctr++,buf+4);

    uint32touint8p(0x5ec,buf+8);

    buf[12]=0xff; // salt (0xffff always?)
    buf[13]=0xff;
    status=nrf_snd_pkt_crc_encr(16,buf,remotekey);

    lcdClear();
    lcdPrint("F-St:"); lcdPrintInt(status); 
    lcdDisplay();

};

void r_send(void){
    int ctr=1;
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int len;
    int status;

    while(1){

        buf[0]=0x10; // Length: 16 bytes
        buf[1]='C'; // Proto
        buf[2]=getInputRaw();
        buf[3]=0x00; // Unused

        ctr++;
        *(int*)(buf+4)=ctr;

        /*
        buf[4]=0x00; // ctr
        buf[5]=0x00; // ctr
        buf[6]=0x00; // ctr
        buf[7]=ctr++; // ctr
        */

        buf[8]=0x0; // Object id
        buf[9]=0x0;
        buf[10]=0x05;
        buf[11]=0xec;

        buf[12]=0xff; // salt (0xffff always?)
        buf[13]=0xff;

        lcdClear();
        lcdPrint("Key:"); lcdPrintInt(buf[2]); lcdNl();
        if(buf[2]==BTN_ENTER)
            break;

        status=nrf_snd_pkt_crc_encr(16,buf,remotekey);

        lcdPrint("F-St:"); lcdPrintInt(status); 
        lcdDisplay();

        len=nrf_rcv_pkt_time_encr(10,sizeof(buf),buf,remotekey);
        if(len>0){
            lcdPrint("Got!");
            lcdDisplay();
            break;
        };
    };
};

