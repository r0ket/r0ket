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

#define SERIAL_NONE 0
#define SERIAL_ESCAPE   '\\'
#define SERIAL_STOP     '0'
#define SERIAL_PACKETLEN    128

struct NRF_CFG config = {
    .channel= CHANNEL,
    .txmac= MAC,
    .nrmacs=1,
    .mac0=  MAC,
    .maclen ="\x20",
};

uint8_t serialmsg_message[SERIAL_PACKETLEN];
uint8_t serialmsg_len = 0;

void serialmsg_init(void);
uint8_t serialmsg_put(uint8_t data);
char snd_pkt_no_crc(int size, uint8_t * pkt);
void dump_encoded(int len, uint8_t *data);

#define INPUTLEN 99
/**************************************************************************/

void main_bridge(void)
{
    GLOBAL(daytrig)=10;
    GLOBAL(lcdbacklight)=10;
    GLOBAL(privacy) = 3;
    char input[64];
    char led1=0;
    char led2=0;

    usbCDCInit();
    delayms(500);
    nrf_init();
    nrf_config_set(&config);
    
    nrf_rcv_pkt_start(R_CONFIG_EN_CRC);
    while(1){
        int l, i, status;
        CDC_OutBufAvailChar (&l);
        if(l>0){
            gpioSetValue (RB_LED0, led1);led1=1-led1;
            CDC_RdOutBuf (input, &l);
            for(i=0; i<l; i++){
                uint8_t cmd = serialmsg_put(input[i]);
                if( cmd != SERIAL_NONE ){
                    switch( cmd ){
                        case '1':
                            // can we loose packets here?
                            nrf_rcv_pkt_end();
                            status=snd_pkt_no_crc(serialmsg_len, serialmsg_message);
                            //status=nrf_snd_pkt_crc(serialmsg_len, serialmsg_message);
                            nrf_rcv_pkt_start(R_CONFIG_EN_CRC);
                        break;
                        case '3':
                            memcpy(config.txmac, serialmsg_message, 5);
                            nrf_write_long(C_W_REGISTER|R_TX_ADDR,5,config.txmac);
                        break;
                        case '4':
                            memcpy(config.mac0, serialmsg_message, 5);
                            nrf_write_long(C_W_REGISTER|R_RX_ADDR_P0,5,config.mac0);
                            nrf_write_reg(R_EN_RXADDR,1);
                        break;
                        case '5':
                            config.channel=serialmsg_message[0];
                            nrf_set_channel(config.channel);
                            nrf_cmd(C_FLUSH_RX);
                        break;
                        case '6':
                            config.maclen[0]=serialmsg_message[0];
                            nrf_write_reg(R_RX_PW_P0,config.maclen[0]);
                        break;
                        case '7':
                            puts("\\7");
                            char s[sizeof(uint32_t)+1];
                            *((uint32_t*)s) =GetUUID32();
                            s[sizeof(uint32_t)]=0;
                            puts(s);
                            puts("\\0");
                        break;
                        case '8': /* set mac width */
                            nrf_write_reg(R_SETUP_AW,serialmsg_message[0]);
                        break;
                        case '9': // Dis/Enable CRC
                            nrf_write_reg(R_CONFIG, R_CONFIG_PRIM_RX|R_CONFIG_PWR_UP|
                                    ((serialmsg_message[0]&1)?R_CONFIG_EN_CRC :0)|
                                    ((serialmsg_message[0]&2)?R_CONFIG_CRCO :0)
                                    
                                    );
                            /* maybe add enhanced shockburst stuff here */
                            nrf_cmd(C_FLUSH_RX);
                            nrf_write_reg(R_STATUS,0);
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
            gpioSetValue (RB_LED2, led2);led2=1-led2;
            puts("\\1");
            dump_encoded(len, buf);
            puts("\\0");
        }
    }
}

void dump_encoded(int len, uint8_t *data)
{
    int i=0,j=0;
    uint8_t buf[SERIAL_PACKETLEN*2];
    for(i=0; i<len; i++){
        if( data[i] == SERIAL_ESCAPE ){
            buf[j++] = SERIAL_ESCAPE;
        }
        buf[j++] = data[i];
    }
    CDC_WrInBuf((char*)buf, &j);
}

void tick_bridge(void){
    return;
}

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
}

void serialmsg_init(void)
{
    serialmsg_len = 0;
}

//returns the message type or SERIAL_NONE
uint8_t serialmsg_put(uint8_t data)
{
    static uint8_t escaped = 0;
    static uint8_t msgtype = SERIAL_NONE;

    if( data == SERIAL_ESCAPE && escaped == 0 ){
        //a control code will follow
        escaped = 1;
        return SERIAL_NONE;
    }else if( escaped ){
        escaped = 0;
        if( data != SERIAL_ESCAPE ){
            if( data == SERIAL_STOP ){
                uint8_t tmp = msgtype;
                msgtype = SERIAL_NONE;
                return tmp;
            }
            msgtype = data;
            serialmsg_len=0;
            return SERIAL_NONE;
        }
    }
    serialmsg_message[serialmsg_len++] = data;

    //prevent a buffer overflow
    if( serialmsg_len == SERIAL_PACKETLEN )
        serialmsg_len--;

    return SERIAL_NONE;
}


