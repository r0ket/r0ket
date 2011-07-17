#include "rftransfer.h"
#include "nrf24l01p.h"
#include <basic/basic.h>
#include <core/systick/systick.h>

#define MAXPACKET   32
void rftransfer_send(uint16_t size, uint8_t *data)
{
    uint8_t buf[MAXPACKET];
    buf[0] = 'L';
    buf[1] = size >> 8;
    buf[2] = size & 0xFF;

    uint16_t rand = 5; //random_rand16();
    buf[3] = rand >> 8;
    buf[4] = rand & 0xFF;

    //nrf_snd_pkt_crc(5,buf);     //setup packet
    nrf_snd_pkt_crc(32,buf);     //setup packet
    delayms(20);
    uint16_t index = 0;
    uint8_t i;
    uint16_t crc = crc16(data,size);

    while(size){
        buf[0] = 'D';
        buf[1] = index >> 8;
        buf[2] = index & 0xFF;
        buf[3] = rand >> 8;
        buf[4] = rand & 0xFF;
        for(i=5; i<MAXPACKET && size>0; i++,size--){
            buf[i] = *data++;
        }
        index++;
        nrf_snd_pkt_crc(32,buf);     //data packet
        delayms(20);
    }

    buf[0] = 'C';
    buf[1] = crc >> 8;
    buf[2] = crc & 0xFF;
    buf[3] = rand >> 8;
    buf[4] = rand & 0xFF;
    //nrf_snd_pkt_crc(5,buf);     //crc packet
    nrf_snd_pkt_crc(32,buf);     //setup packet
    delayms(20);
}

uint16_t rftransfer_receive(uint8_t *buffer, uint16_t maxlen, uint16_t timeout)
{
    uint8_t buf[MAXPACKET];
    uint8_t state = 0;
    uint16_t pos = 0, seq = 0, size = 0, rand = 0, crc = 0;
    int n,i;
    unsigned int currentTick = systickGetTicks();
    unsigned int startTick = currentTick;
    
    while(currentTick < (startTick+timeout) ){//this fails if either overflows
        n = nrf_rcv_pkt_time(1000, MAXPACKET, buf);
        switch(state){
            case 0:
                if( n == 32 && buf[0] == 'L' ){
                    size = (buf[1] << 8) | buf[2];
                    rand =  (buf[3] << 8) | buf[4];
                    seq = 0;
                    pos = 0;
                    if( size <= maxlen ){
                        lcdClear();
                        lcdPrintln("got l"); lcdRefresh();
                        state = 1;
                    }
                }
            break;
            case 1:
                if( n == 32 && buf[0] == 'D' && ((buf[3]<<8)|buf[4])==rand ){
                    lcdPrint("got d"); lcdRefresh();
                    if( seq == ((buf[1]<<8)|buf[2]) ){
                        lcdPrintln(" in seq"); lcdRefresh();
                        //if( (pos + n - 5)<maxlen ){
                            //for(i=5; i<n; i++,pos++){
                            for(i=5; i<n && pos<size; i++,pos++){
                                buffer[pos] = buf[i];
                            }
                            seq++;
                        //}
                    }
                }
                if( pos == size ){
                    lcdPrintln("got all"); lcdRefresh();
                    crc = crc16(buffer, size);
                    state = 2;
                }
            break;
            case 2:
                if( n == 32 && buf[0] == 'C' && ((buf[3]<<8)|buf[4])==rand){
                    lcdPrint("got crc"); lcdRefresh();
                    if( crc == ((buf[1]<<8)|buf[2]) ){
                        lcdPrintln(" ok"); lcdRefresh();
                        return size;
                    }else{
                        state = 0;
                    }
                }
            break;
        };
    }
    return 0;
}

