#include <sysinit.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "basic/basic.h"
#include "lcd/render.h"
#include "lcd/allfonts.h"
#include "basic/ecc.h"
#include "funk/nrf24l01p.h"
#include "filesystem/ff.h"
#include "filesystem/diskio.h"
#include "funk/filetransfer.h"
#include "lcd/print.h"
#include <string.h>
#include "funk/nrf24l01p.h"
#include "funk/filetransfer.h"
#include "funk/rftransfer.h"
#include "basic/basic.h"
#include "basic/xxtea.h"
#include "filesystem/ff.h"
#include "lcd/print.h"
#include "usetable.h"


uint8_t mac[5] = {1,2,3,2,1};
    struct NRF_CFG config = {
        .channel= 81,
        .txmac= "\x1\x2\x3\x2\x1",
        .nrmacs=1,
        .mac0=  "\x1\x2\x3\x2\x1",
        .maclen ="\x20",
    };

void ram(void)
{
    nrf_config_set(&config);

    if( sendKeys() )
        return;

    char priv[42];
    UINT readbytes;
    FIL file;

    if( f_open(&file, "priv.key", FA_OPEN_EXISTING|FA_READ) ){
        return;
    }
    if( f_read(&file, priv, 41, &readbytes) || readbytes != 41 ){
        return;
    }
    f_close(&file);
    priv[41] = 0;

    uint8_t done = 0;
    uint8_t key;
    uint8_t k1[16], k2[16], rx[4*NUMWORDS], ry[4*NUMWORDS];
 
    while( !done ){
        lcdClear();
        lcdPrintln("Receiving file");
        lcdPrintln("Down=Abort");
        lcdRefresh();
        key = getInput();
        delayms(20);
        if( key == BTN_DOWN ){
            return -1;
        }
        if( receiveR(rx,ry) )
            continue;
        lcdPrintln("Creating key");
        lcdRefresh();
        ECIES_decryptkeygen(rx, ry, k1, k2, priv);
        if( filetransfer_receive(mac,(uint32_t*)k1) < 0 )
            continue;
        lcdPrintln("Right=OK");
        lcdPrintln("Left=Retry");
        lcdPrintln("Down=Abort");
        lcdRefresh();

        while(1){
            key = getInput();
            delayms(20);
            if( key == BTN_LEFT ){
                break;
            }else if( key == BTN_RIGHT ){
                done = 1;
                break;
            }else if( key == BTN_DOWN ){
                return -1;
            }
        }
    }
}

void sendPublicKey(void)
{
    uint8_t exp[2 + 4*NUMWORDS + 2];
    char buf[42];
    UINT readbytes;
    FIL file;

    if( f_open(&file, "pubx.key", FA_OPEN_EXISTING|FA_READ) ){
        lcdPrint("pubx.key");
        lcdRefresh();
        while(1);
        return;
    }
    if( f_read(&file, buf, 41, &readbytes) || readbytes != 41 ){
        lcdPrint("read x");
        lcdRefresh();
        while(1);
        return;
    }
    f_close(&file);
    buf[41] = 0;

    exp[0] = 'P';
    bitstr_parse_export((char*)exp+2, buf);
    exp[1] = 'X';
    nrf_config_set(&config);
    nrf_snd_pkt_crc(32, exp);
    delayms(10);

    if( f_open(&file, "puby.key", FA_OPEN_EXISTING|FA_READ) ){
        lcdPrint("puby.key");
        lcdRefresh();
        while(1);
 
        return;
    }
    if( f_read(&file, buf, 41, &readbytes) || readbytes != 41 ){
        lcdPrint("read x");
        lcdRefresh();
        while(1);
 
        return;
    }
    f_close(&file);
    buf[41] = 0;

    exp[1] = 'Y';
    bitstr_parse_export((char*)exp+2, buf);
    nrf_config_set(&config);
    nrf_snd_pkt_crc(32, exp);
    delayms(10);
}


int receiveKey(uint8_t type, uint8_t *x, uint8_t *y)
{
    uint8_t buf[32];
    uint8_t n;
    
    nrf_config_set(&config);
    n = nrf_rcv_pkt_time(1000, 32, buf);
    if( n == 32 && buf[0] == type && buf[1] == 'X' ){
        for(int i=0; i<NUMWORDS*4; i++)
            x[i] = buf[i+2];
    nrf_config_set(&config);
        n = nrf_rcv_pkt_time(100, 32, buf);
        if( n == 32 && buf[0] ==type && buf[1] == 'Y' ){
            for(int i=0; i<NUMWORDS*4; i++)
                y[i] = buf[i+2];
            return 0;
        }
    }
    return -1;
}

int receiveR(uint8_t *rx, uint8_t *ry)
{
    return receiveKey('R',rx,ry);
}

void sendMac(void)
{
    uint8_t buf[32];
    buf[0] = 'M';
    buf[1] = 'C';
    buf[2] = mac[0];
    buf[3] = mac[1];
    buf[4] = mac[2];
    buf[5] = mac[3];
    buf[6] = mac[4];
    nrf_config_set(&config);
    nrf_snd_pkt_crc(32, buf);
    delayms(10);
}

int sendKeys(void)
{
    uint8_t done = 0;
    char key;
    while( !done ){
        lcdClear();
        lcdPrintln("Sending PUBKEX");lcdRefresh();
        sendPublicKey();
        sendMac();
        lcdPrintln("Done");
        lcdPrintln("Right=OK");
        lcdPrintln("Left=Retry");
        lcdPrintln("Down=Abort");
        lcdRefresh();

        while(1){
            key = getInput();
            delayms(20);
            if( key == BTN_LEFT ){
                break;
            }else if( key == BTN_RIGHT ){
                done = 1;
                break;
            }else if( key == BTN_DOWN ){
                return -1;
            }
        }
    }
    return 0;
}

int filetransfer_receive(uint8_t *mac, uint32_t const k[4])
{
    uint8_t buf[MAXSIZE+1];
    uint16_t size;
    uint8_t n;

    UINT written = 0;
    FIL file;
    FRESULT res;
    //uint8_t macbuf[5];
    //nrf_get_rx_max(0,5,macbuf);

    uint8_t metadata[32];

    //nrf_set_rx_mac(0, 32, 5, mac);
    nrf_config_set(&config);
    n = nrf_rcv_pkt_time_encr(3000, 32, metadata, k);
    if( n != 32 )
        return 1;       //timeout
    //nrf_set_rx_mac(0, 32, 5, macbuf);
    //lcdPrintln("got meta"); lcdRefresh();
    metadata[19] = 0; //enforce termination
    size = (metadata[20] << 8) | metadata[21];

    if( size > MAXSIZE ) {lcdPrintln("too big"); lcdRefresh(); while(1);}
    if( size > MAXSIZE ) return 1; //file to big
    //if(fileexists(metadata)) return 1;   //file already exists
    
    //lcdPrint("open"); lcdPrintln((const char*)metadata); lcdRefresh();
    res = f_open(&file, (const char*)metadata, FA_OPEN_ALWAYS|FA_WRITE);

    //lcdPrintln("file opened"); lcdRefresh();
    //if( res ) {lcdPrintln("res"); lcdPrint(f_get_rc_string(res)); lcdRefresh(); while(1);}
    if( res ){ lcdPrintln("file error"); lcdRefresh(); while(1);}
    if( res )
        return res;
    
    uint16_t wordcount = (size+3)/4;
    
    //nrf_set_rx_mac(0, 32, 5, mac);
    //lcdPrintln("get file"); lcdRefresh();
    int fres = rftransfer_receive(buf, wordcount*4, 1000);
    if( fres == -1 ){
        lcdPrintln("checksum wrong");
    }else if( fres == -2 ){
        lcdPrintln("timeout");
    }else{
        //lcdPrintln("got file");
    }
    lcdRefresh();
    if( fres < 0 )
        return 1;
    //nrf_set_rx_mac(0, 32, 5, macbuf);

    xxtea_decode_words((uint32_t *)buf, wordcount, k);
    
    res = f_write(&file, buf, size, &written);
    f_close(&file);
    if( res )
        return res;
    if( written != size )
        return 1;           //error while writing
    lcdClear();
    lcdPrintln("Received"); lcdPrintln((const char*)metadata); lcdRefresh();

    return 0;
}

#define MAXPACKET   32
int16_t rftransfer_receive(uint8_t *buffer, uint16_t maxlen, uint16_t timeout)
{
    uint8_t buf[MAXPACKET];
    uint8_t state = 0;
    uint16_t pos = 0, seq = 0, size = 0, rand = 0, crc = 0;
    int n,i;
    unsigned int currentTick = systickGetTicks();
    unsigned int startTick = currentTick;
    
    while(systickGetTicks() < (startTick+timeout) ){//this fails if either overflows
    nrf_config_set(&config);
        n = nrf_rcv_pkt_time(1000, MAXPACKET, buf);
        switch(state){
            case 0:
                if( n == 32 && buf[0] == 'L' ){
                    size = (buf[1] << 8) | buf[2];
                    rand =  (buf[3] << 8) | buf[4];
                    seq = 0;
                    pos = 0;
                    if( size <= maxlen ){
                        //lcdClear();
                        //lcdPrint("got l="); lcdPrintInt(size);
                        //lcdPrintln(""); lcdRefresh();
                        state = 1;
                    }
                }
            break;
            case 1:
                if( n == 32 && buf[0] == 'D' && ((buf[3]<<8)|buf[4])==rand ){
                    //lcdPrint("got d"); lcdRefresh();
                    if( seq == ((buf[1]<<8)|buf[2]) ){
                        //lcdPrintln(" in seq"); lcdRefresh();
                            for(i=5; i<n-2 && pos<size; i++,pos++){
                                buffer[pos] = buf[i];
                            }
                            seq++;
                    }
                }
                if( pos == size ){
                    //lcdPrintln("got all"); lcdRefresh();
                    crc = crc16(buffer, size);
                    state = 2;
                }
            break;
            case 2:
                if( n == 32 && buf[0] == 'C' && ((buf[3]<<8)|buf[4])==rand){
                    //lcdPrint("got crc"); lcdRefresh();
                    if( crc == ((buf[1]<<8)|buf[2]) ){
                        //lcdPrintln(" ok"); lcdRefresh();
                        return size;
                    }else{
                        //lcdPrintln(" nok"); lcdRefresh();
                        return -1;
                    }
                }
            break;
        };
    }
    //lcdPrintln("Timeout"); lcdRefresh();
    return -2;
}
#if 0
int ECIES_decryptkeygen(uint8_t *rx, uint8_t *ry,
             uint8_t k1[16], uint8_t k2[16], const char *privkey)
{
  elem_t Rx, Ry, Zx, Zy;
  exp_t d;
  bitstr_import(Rx, (char*)rx);
  bitstr_import(Ry, (char*)ry);
  if (ECIES_embedded_public_key_validation(Rx, Ry) < 0)
    return -1;
  bitstr_parse(d, privkey);
  point_copy(Zx, Zy, Rx, Ry);
  point_mult(Zx, Zy, d);
  point_double(Zx, Zy);                             /* cofactor h = 2 on B163 */
  if (point_is_zero(Zx, Zy))
    return -1;
  ECIES_kdf((char*)k1,(char*) k2, Zx, Rx, Ry);
  return 0;
}
#endif
