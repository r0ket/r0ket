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



uint8_t mac[5] = {1,2,3,2,1};

void ram(void)
{
    char file[13];
    selectFile(file,"TXT");
    sendFile(file); 
}

void sendR(uint8_t *rx, uint8_t *ry)
{
    uint8_t exp[2 + 4*NUMWORDS + 2];
    exp[0] = 'R';
    for(int i=0; i<4*NUMWORDS; i++)
        exp[2+i] = rx[i];
    exp[1] = 'X';
    nrf_snd_pkt_crc(32, exp);
    delayms(10);
    exp[1] = 'Y';
    for(int i=0; i<4*NUMWORDS; i++)
        exp[2+i] = ry[i]; 
    nrf_snd_pkt_crc(32, exp);
    delayms(10);
}

int receiveKey(uint8_t type, uint8_t *x, uint8_t *y)
{
    uint8_t buf[32];
    uint8_t n;
    
    n = nrf_rcv_pkt_time(1000, 32, buf);
    if( n == 32 && buf[0] == type && buf[1] == 'X' ){
        for(int i=0; i<NUMWORDS*4; i++)
            x[i] = buf[i+2];
        n = nrf_rcv_pkt_time(100, 32, buf);
        if( n == 32 && buf[0] ==type && buf[1] == 'Y' ){
            for(int i=0; i<NUMWORDS*4; i++)
                y[i] = buf[i+2];
            return 0;
        }
    }
    return -1;
}

int receivePublicKey(uint8_t *px, uint8_t *py)
{
    return receiveKey('P',px,py);
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
    nrf_snd_pkt_crc(32, buf);
    delayms(10);
}

int receiveMac(uint8_t *mac)
{
    uint8_t buf[32]; 
    uint8_t n;

    n = nrf_rcv_pkt_time(100, 32, buf);
    if( n == 32 && buf[0] == 'M' && buf[1] == 'C' ){
        for(int i=0; i<5; i++)
            mac[i] = buf[i+2];
        return 0;
    }
    return -1;
}

int receiveKeys(uint8_t *px, uint8_t *py, uint8_t *mac)
{
    uint8_t done = 0;
    char key;
    while( !done ){
        lcdClear();
        lcdPrintln("Recv. PUBKEY");
        lcdPrintln("Down=Abort");
        lcdRefresh();
        key = getInput();
        delayms(20);
        if( key == BTN_DOWN ){
            return -1;
        }
        if( receivePublicKey(px,py) )
            continue;
        if( receiveMac(mac) )
            continue;
        lcdClear();
        lcdPrintln("Got PUBKEY"); lcdRefresh(); 
        lcdPrintln("Hash:");
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


void sendFile(char *filename)
{
    uint8_t px[4*NUMWORDS];
    uint8_t py[4*NUMWORDS];
    uint8_t mac[5];
    if( receiveKeys(px, py, mac) )
        return;
    
    uint8_t done = 0;
    uint8_t key;

    uint8_t k1[16], k2[16], rx[4*NUMWORDS], ry[4*NUMWORDS];
    
    lcdClear();
    lcdPrintln("Creating key"); lcdRefresh();
    ECIES_encyptkeygen(px, py, k1, k2, rx, ry);
    
    while( !done ){
        lcdPrintln("Sending file");lcdRefresh();
        sendR(rx,ry);
        delayms(3000);
        filetransfer_send((uint8_t*)filename, 0, mac, (uint32_t*)k1);
        lcdPrintln("Done");
        lcdPrintln("Right=OK");
        lcdPrintln("Left=Retry");
        lcdRefresh();
        while(1){
            key = getInput();
            delayms(20);
            if( key == BTN_LEFT ){
                break;
            }else if( key == BTN_RIGHT ){
                done = 1;
                break;
            }
        } 
        lcdClear();
    }
}

