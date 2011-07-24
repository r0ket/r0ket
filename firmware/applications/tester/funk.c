#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/lcd.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"

#include <string.h>

#include "funk/rftransfer.h"
#include "funk/openbeacon.h"

/**************************************************************************/

#define BEACON_CHANNEL 81
#define BEACON_MAC     "\x1\x2\x3\x2\1"

uint32_t const testkey[4] = {
    0xB4595344,0xD3E119B6,0xA814D0EC,0xEFF5A24E
};

int enctoggle=0;

void f_init(void){
    nrf_init();

    struct NRF_CFG config = {
        .channel= BEACON_CHANNEL,
        .txmac= BEACON_MAC,
        .nrmacs=1,
        .mac0=  BEACON_MAC,
        .maclen ="\x10",
    };

    nrf_config_set(&config);
    lcdPrintln("Done.");
};

void f_status(void){
    int dx=0;
    int dy=8;
    uint8_t buf[4];

    buf[0]=C_R_REGISTER | R_CONFIG;
    buf[1]=0;
    buf[2]=0;
    buf[3]=0;
    dx=DoString(0,dy,"S:"); 
    dx=DoCharX(dx,dy,buf[0]);
    dx=DoCharX(dx,dy,buf[1]);
    dx=DoCharX(dx,dy,buf[2]);
    dx=DoCharX(dx,dy,buf[3]);
    dy+=8;
    nrf_cmd_rw_long(buf,2);
    dx=DoString(0,dy,"R:");
    dx=DoCharX(dx,dy,buf[0]);
    dx=DoCharX(dx,dy,buf[1]);
    dx=DoCharX(dx,dy,buf[2]);
    dx=DoCharX(dx,dy,buf[3]);
    dy+=8;

    int status=nrf_cmd_status(C_NOP);
    dx=DoString(0,dy,"St:"); DoCharX(dx,dy,status);dy+=8;
};

void f_recv(void){
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int len;
    do{

        lcdClear();
        len=nrf_rcv_pkt_time_encr(1000,sizeof(buf),buf,enctoggle?testkey:NULL);

        if(len==0){
            lcdPrintln("No pkt (Timeout)");
        };
        lcdPrint("Size:");lcdPrintInt(len);lcdNl();
        lcdPrint("1:");lcdPrintIntHex( *(int*)(buf+ 0) ); lcdNl();
        lcdPrint("2:");lcdPrintIntHex( *(int*)(buf+ 4) ); lcdNl();
        lcdPrint("3:");lcdPrintIntHex( *(int*)(buf+ 8) ); lcdNl();
        lcdPrint("4:");lcdPrintIntHex( *(int*)(buf+12) ); lcdNl();

        len=crc16(buf,14);
        lcdPrint("crc:");lcdPrintShortHex(len); lcdNl();
        lcdDisplay();
    }while ((getInputRaw())==BTN_NONE);

};

void f_cfg(void){
    struct NRF_CFG config;

    nrfconfig cfg=&config;

    nrf_config_get(cfg);

    lcdPrint("ch:");lcdPrintInt( cfg->channel ); lcdNl();
    lcdPrint("nr:");lcdPrintInt( cfg->nrmacs ); lcdNl();

    lcdPrint("0:");
    lcdPrintCharHex(cfg->mac0[0]);
    lcdPrintCharHex(cfg->mac0[1]);
    lcdPrintCharHex(cfg->mac0[2]);
    lcdPrintCharHex(cfg->mac0[3]);
    lcdPrintCharHex(cfg->mac0[4]);
    lcdNl();
    lcdPrint("1:");
    lcdPrintCharHex(cfg->mac1[0]);
    lcdPrintCharHex(cfg->mac1[1]);
    lcdPrintCharHex(cfg->mac1[2]);
    lcdPrintCharHex(cfg->mac1[3]);
    lcdPrintCharHex(cfg->mac1[4]);
    lcdNl();
    lcdPrint("2345:");
    lcdPrintCharHex(cfg->mac2345[0]);
    lcdPrintCharHex(cfg->mac2345[1]);
    lcdPrintCharHex(cfg->mac2345[2]);
    lcdPrintCharHex(cfg->mac2345[3]);
    lcdNl();
    lcdPrint("tx:");
    lcdPrintCharHex(cfg->txmac[0]);
    lcdPrintCharHex(cfg->txmac[1]);
    lcdPrintCharHex(cfg->txmac[2]);
    lcdPrintCharHex(cfg->txmac[3]);
    lcdPrintCharHex(cfg->txmac[4]);
    lcdNl();
    lcdPrint("len:");
    lcdPrintCharHex(cfg->maclen[0]);
    lcdPrintCharHex(cfg->maclen[1]);
    lcdPrintCharHex(cfg->maclen[2]);
    lcdPrintCharHex(cfg->maclen[3]);
    lcdPrintCharHex(cfg->maclen[4]);
    lcdNl();
};

void f_cfg_set(void){
    struct NRF_CFG config = {
        .channel= 13,
        .txmac= "R0KET",
        .nrmacs=1,
        .mac0=  "R0KET",
        .maclen ="\x10",
    };

    nrf_config_set(&config);
};

void f_enctog(void){
    enctoggle=1-enctoggle;
    if(enctoggle)
        lcdPrintln("Encrypt ON!");
    else
        lcdPrintln("encrypt off!");

};

void f_send(void){
    uint8_t status;

    status = openbeaconSend();
    lcdPrint("Status:");
    lcdPrintCharHex(status);
    lcdNl();
};
