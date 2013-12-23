#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"
#include "filesystem/ff.h"

#include "usetable.h"

/**************************************************************************/

#define MAC_TABLE_SIZE 50
#define ENTRYLEN 7
#define PACKETLEN   25

struct NRF_CFG config = {
    .channel= 81,
    .txmac= "\x0\x0\x0\x0\x0",
    .nrmacs=1,
    .mac0=  "\x55\x0\x0\x0\x0",
    .maclen ="\x19",
};

void shift_buffer_left(uint8_t *buffer, uint8_t n, uint8_t shift);
void shift_buffer_right(uint8_t *buffer, uint8_t n, uint8_t shift);
void configure_nrf(uint8_t channel);

void ram(void) {
    char channel=25;
    char key;
    unsigned char buf[PACKETLEN];
    unsigned char buf2[PACKETLEN];
    uint8_t count = 0;

    #if 0
    do {
        lcdClear();
        lcdPrintln("Key-Scan:");
        lcdPrint("Ch: ");
        lcdPrint(IntToStr(channel,3,0));
        lcdPrintln("");
        lcdRefresh();
        key = getInputWait();
        if ( key == BTN_UP ){
            if ( channel < 127 )
                channel++;
        } else if ( key == BTN_DOWN ){
            if ( channel > 0 )
                channel--;
        }
        delayms(100);
    }while (key!=BTN_ENTER);
    #endif

    configure_nrf(channel);

    while(true) {
        char n;
        n = nrf_rcv_pkt_poll(sizeof(buf), buf);
        if(n!=sizeof(buf)){
            if(n==0)
                continue;
            lcdPrint("Err");
            lcdPrintln(IntToStr(n,3,0));
            lcdRefresh();
            continue;
        };

        //lcdPrintln(IntToStr(n,3,0));
        //lcdRefresh();
        //getInputWait();
        //getInputWaitRelease();
        uint8_t shift;
        bool found = false;
        uint8_t dir;
        for(dir = 0; dir < 2; dir++) {
            for(shift = 0; shift < 4; shift++) {
                memcpy(buf2, buf, sizeof(buf));
                
                //lcdPrint(IntToStr(buf2[0],2,F_HEX));
                //lcdPrint(IntToStr(buf2[1],2,F_HEX));
                //lcdPrint(IntToStr(buf2[2],2,F_HEX));
                //lcdPrint(IntToStr(buf2[3],2,F_HEX));
                //lcdPrint(IntToStr(buf2[4],2,F_HEX));
                //lcdPrintln("");

                
                // shift out additional preamble bits
                if(dir == 0) {
                    shift_buffer_left(buf2, sizeof(buf2), shift);
                } else {
                    shift_buffer_right(buf2, sizeof(buf2), shift);
                }
                //lcdPrintln("c"); lcdRefresh();
                // we need to shift the payload one bit
                // to the left as there is a 9 bit
                // field in the shockburst+ header
                
                // Byte number 6 contains one bit
                // of the PCF as the MSB and 7 bit
                // of the first payload byte.
                //
                // Byte number 6 + 16 contains 7 bit
                // of the checksum
                //
                // Byte number 6 + 17 contains the LSB
                // of the checksum as MSB.
                shift_buffer_left(buf2 + 6, 17, 1);
                
                /*
                lcdPrint(IntToStr(buf2[4],2,F_HEX));
                lcdPrint(IntToStr(buf2[3],2,F_HEX));
                lcdPrint(IntToStr(buf2[2],2,F_HEX));
                lcdPrint(IntToStr(buf2[1],2,F_HEX));
                lcdPrint(IntToStr(buf2[0],2,F_HEX));
                lcdPrintln("");

                lcdPrint(IntToStr(buf2[6 + 10 + 4],2,F_HEX));
                lcdPrint(IntToStr(buf2[6 + 10 + 0],2,F_HEX));
                lcdPrint(IntToStr(buf2[6 + 10 + 1],2,F_HEX));
                lcdPrint(IntToStr(buf2[6 + 10 + 2],2,F_HEX));
                lcdPrint(IntToStr(buf2[6 + 10 + 3],2,F_HEX));
                lcdPrintln("");
                lcdRefresh();
                */
                // We expect the MAC at byte number
                // 6 + 10 to 6 + 14
                //
                // The mac is with the following byte order
                // at the beginning: 4 3 2 1 0
                //
                // We expect the following byte order of the
                // mac in the paylod:
                // 1 2 3 4 0

                if(buf2[6 + 10 + 0] == buf2[3] &&
                        buf2[6 + 10 + 1] == buf2[2] &&
                        buf2[6 + 10 + 2] == buf2[1] &&
                        buf2[6 + 10 + 3] == buf2[0] &&
                        buf2[6 + 10 + 4] == buf2[4]) {
                    // This originated from a keyboard
                    found = true;
                    break;
                }
            }
            if(found == true){
                break;
            }
        }
        //volatile uint32_t i;
        //for(i=0; i< 1000000; i++);
        if(found) {
            lcdPrint(IntToStr(count++&0x0F,1,F_HEX));
            lcdPrint(IntToStr(dir,1,F_HEX));
            lcdPrint(IntToStr(shift,1,F_HEX));
            lcdPrint(" ");
            lcdPrint(IntToStr(buf2[4],2,F_HEX));
            lcdPrint(IntToStr(buf2[3],2,F_HEX));
            lcdPrint(IntToStr(buf2[2],2,F_HEX));
            lcdPrint(IntToStr(buf2[1],2,F_HEX));
            lcdPrint(IntToStr(buf2[0],2,F_HEX));
            lcdPrintln("");
            lcdRefresh();
        }
        
        key = getInputRaw();
        if ( key == BTN_UP ){
            if ( channel < 127 )
                channel++;
            configure_nrf(channel);
        } else if ( key == BTN_DOWN ){
            if ( channel > 0 )
                channel--;
            configure_nrf(channel);
        }

        delayms(100);
    };

    lcdPrintln("done.");
    lcdRefresh();

    FIL file;            /* File object */
    UINT writebytes;
    UINT allwrite=0;
    int res;
    res=f_open(&file, "KBD.MAC", FA_OPEN_ALWAYS|FA_WRITE);
    if(res){
        return;
    };
    res = f_write(&file, &buf2[4], 1, &writebytes);
    allwrite+=writebytes;
    res = f_write(&file, &buf2[3], 1, &writebytes);
    allwrite+=writebytes;
    res = f_write(&file, &buf2[2], 1, &writebytes);
    allwrite+=writebytes;
    res = f_write(&file, &buf2[1], 1, &writebytes);
    allwrite+=writebytes;
    res = f_write(&file, &buf2[0], 1, &writebytes);
    allwrite+=writebytes;
    if(res){
        return;
    };
    res=f_close(&file);
    if(res){
        return;
    };

    lcdPrint(IntToStr(allwrite,1,0));
    lcdPrintln("b saved.");
    lcdRefresh();

    getInputWaitRelease();
    getInputWait();
}


void shift_buffer_left(uint8_t *buf, uint8_t n, uint8_t shift)
{
    while(shift--) {
        uint8_t i;
        for (i = 0; i < n; i++){
            buf[i]=(buf[i] << 1) | (buf[i+1] >> 7);
        };
        buf[n-1] &= 0xFE;
    }
}

void shift_buffer_right(uint8_t *buf, uint8_t n, uint8_t shift)
{
    while(shift--) {
        int8_t i;
        for (i = n - 1; i >= 0; i--){
            buf[i]=(buf[i] >> 1) | (buf[i-1] << 7);
        };
        
        // Fill with pattern
        buf[0] &= 0x7F;
        if((buf[0] & 0x40) == 0) {
            buf[0] |= 0x80;
        }
    }
}

void configure_nrf(uint8_t channel)
{
    lcdClear();
    lcdPrintln("Key-Scan:");
    lcdPrint("Ch: ");
    lcdPrint(IntToStr(channel,3,0));
    lcdPrintln("");
    lcdRefresh();

    config.channel=channel;
    nrf_config_set(&config);

    nrf_write_reg(R_SETUP_AW,0 /*R_SETUP_AW_2*/);
    nrf_rcv_pkt_start(0);   //Do not enable the CRC
}

