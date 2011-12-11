#include <sysinit.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "basic/basic.h"
#include "lcd/render.h"
#include "lcd/allfonts.h"
#include "basic/config.h"
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
#include "core/ssp/ssp.h"

#if CFG_USBMSC
#error "MSC is defined"
#endif

#if !CFG_USBCDC
#error "CDC is not defined"
#endif

#define CHANNEL 81
#define MAC     "\x1\x2\x3\x2\x1"

#define UB_NONE 0
#define UB_ESCAPE   '\\'
#define UB_STOP     '0'
#define UB_PACKETLEN    128

struct NRF_CFG config = {
    .channel= CHANNEL,
    .txmac= MAC,
    .nrmacs=1,
    .mac0=  MAC,
    .maclen ="\x10",
};

uint8_t serialmsg_message[UB_PACKETLEN];
uint8_t serialmsg_len = 0;

void serialmsg_init(void);
uint8_t serialmsg_put(uint8_t data);
char snd_pkt_no_crc(int size, uint8_t * pkt);

#define INPUTLEN 99
/**************************************************************************/

void main_schneider(void)
{
    GLOBAL(daytrig)=10;
    GLOBAL(lcdbacklight)=10;
    char input[INPUTLEN+1];

    usbCDCInit();
    delayms(500);
    nrf_init();
    nrf_config_set(&config);
    
    nrf_rcv_pkt_start();
    while(1){
        int l=INPUTLEN, i, status;
        CDC_OutBufAvailChar (&l);
        if(l>0){
            CDC_RdOutBuf (input, &l);
            for(i=0; i<l; i++){
                uint8_t cmd = serialmsg_put(input[i]);
                if( cmd != UB_NONE ){
                    switch( cmd ){
                        case '1':
                            // can we loose packets here?
                            nrf_rcv_pkt_end();
                            status=snd_pkt_no_crc(serialmsg_len, serialmsg_message);
                            nrf_rcv_pkt_start();
                        break;
                        case '3':
                            memcpy(config.txmac, serialmsg_message, 5);
                            nrf_config_set(&config);
                        break;
                        case '4':
                            memcpy(config.mac0, serialmsg_message, 5);
                            nrf_config_set(&config);
                        break;
                        case '5':
                            config.channel=serialmsg_message[0];
                            nrf_config_set(&config);
                        break;
                        case '6':
                            config.maclen[0]=serialmsg_message[0];
                            nrf_config_set(&config);
                        break;
                    };
                    puts("\\2\\0");
                }
            }
        }
        int len;
        uint8_t buf[32];
        len=nrf_rcv_pkt_poll(sizeof(buf),buf);
        if( len > 0 ){
            puts("\\1");
            CDC_WrInBuf(buf, &len);
            puts("\\0");
        }
    }
}

void tick_schneider(void){
    return;
};

inline void xmit_spi(uint8_t dat) {
    sspSend(0, (uint8_t*) &dat, 1);
}

inline void rcv_spi(uint8_t *dat) {
    sspReceive(0, dat, 1);
}

#define CS_LOW()    gpioSetValue(RB_SPI_NRF_CS, 0)
#define CS_HIGH()   gpioSetValue(RB_SPI_NRF_CS, 1)
#define CE_LOW()    gpioSetValue(RB_NRF_CE, 0)
#define CE_HIGH()   gpioSetValue(RB_NRF_CE, 1)

char snd_pkt_no_crc(int size, uint8_t * pkt)
{
    if(size > MAX_PKT)
        size=MAX_PKT;

    nrf_write_reg(R_CONFIG,
            R_CONFIG_PWR_UP|  // Power on
            R_CONFIG_EN_CRC   // CRC on, single byte
            );
    
    CS_LOW();
    xmit_spi(C_W_TX_PAYLOAD);
    sspSend(0,pkt,size);
    CS_HIGH();

    CE_HIGH();
    delayms(1); // Send it.  (only needs >10ys, i think)
    CE_LOW();

    return nrf_cmd_status(C_NOP);
};

void serialmsg_init(void)
{
    serialmsg_len = 0;
}

//returns the message type or UB_NONE
uint8_t serialmsg_put(uint8_t data)
{
    static uint8_t escaped = 0;
    static uint8_t msgtype = UB_NONE;

    if( data == UB_ESCAPE && escaped == 0 ){
        //a control code will follow
        escaped = 1;
        return UB_NONE;
    }else if( escaped ){
        escaped = 0;
        if( data != UB_ESCAPE ){
            if( data == UB_STOP ){
                uint8_t tmp = msgtype;
                msgtype = UB_NONE;
                return tmp;
            }
            msgtype = data;
            serialmsg_len=0;
            return UB_NONE;
        }
    }
    serialmsg_message[serialmsg_len++] = data;

    //prevent a buffer overflow
    if( serialmsg_len == UB_PACKETLEN )
        serialmsg_len--;

    return UB_NONE;
}


