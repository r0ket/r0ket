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


FATFS FatFs[_VOLUMES];		/* File system object for logical drive */
/**************************************************************************/

uint8_t mac[5] = {1,2,3,2,1};
char *Px = "1c56d302cf642a8e1ba4b48cc4fbe2845ee32dce7";
char *Py = "45f46eb303edf2e62f74bd68368d979e265ee3c03";
char *Priv ="0e10e787036941e6c78daf8a0e8e1dbfac68e26d2";

void sendPublicKey(char *px, char *py)
{
    uint8_t exp[2 + 4*NUMWORDS];
    exp[0] = 'P';
    bitstr_parse_export((char*)exp+2, px);
    exp[1] = 'X';
    nrf_snd_pkt_crc(30, exp);
    delayms(10);
    exp[1] = 'Y';
    bitstr_parse_export((char*)exp+2, py);
    nrf_snd_pkt_crc(30, exp);
    delayms(10);
}

void sendR(uint8_t *rx, uint8_t *ry)
{
    uint8_t exp[2 + 4*NUMWORDS];
    exp[0] = 'R';
    for(int i=0; i<4*NUMWORDS; i++)
        exp[2+i] = rx[i];
    exp[1] = 'X';
    nrf_snd_pkt_crc(30, exp);
    delayms(10);
    exp[1] = 'Y';
    for(int i=0; i<4*NUMWORDS; i++)
        exp[2+i] = ry[i]; 
    nrf_snd_pkt_crc(30, exp);
    delayms(10);
}

int receiveKey(uint8_t type, uint8_t *x, uint8_t *y)
{
    uint8_t buf[30];
    uint8_t n;
    
    n = nrf_rcv_pkt_time(1000, 30, buf);
    lcdPrint("pkt:"); lcdPrintInt(n);lcdPrintln(""); lcdRefresh();
    if( n == 30 && buf[0] == type && buf[1] == 'X' ){
        for(int i=0; i<NUMWORDS*4; i++)
            x[i] = buf[i+2];
        n = nrf_rcv_pkt_time(100, 30, buf);
        if( n == 30 && buf[0] ==type && buf[1] == 'Y' ){
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

int receiveR(uint8_t *rx, uint8_t *ry)
{
    return receiveKey('R',rx,ry);
}

void sendMac(void)
{
    uint8_t buf[7];
    buf[0] = 'M';
    buf[1] = 'C';
    buf[2] = mac[0];
    buf[3] = mac[1];
    buf[4] = mac[2];
    buf[5] = mac[3];
    buf[6] = mac[4];
    nrf_snd_pkt_crc(30, buf);
    delayms(10);
}

int receiveMac(uint8_t *mac)
{
    uint8_t buf[30]; 
    uint8_t n;

    n = nrf_rcv_pkt_time(100, 30, buf);
    if( n == 30 && buf[0] == 'M' && buf[1] == 'C' ){
        for(int i=0; i<5; i++)
            mac[i] = buf[i+2];
        return 0;
    }
    return -1;
}

int sendKeys(void)
{
    uint8_t done = 0;
    char key;
    while( !done ){
        lcdClear();
        lcdPrintln("Sending key");lcdRefresh();
        sendPublicKey(Px,Py);
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
int receiveKeys(uint8_t *px, uint8_t *py, uint8_t *mac)
{
    uint8_t done = 0;
    char key;
    while( !done ){
        lcdClear();
        lcdPrintln("Receiving key");
        lcdPrintln("Down=Abort");
        lcdRefresh();
        key = getInput();
        delayms(20);
        if( key == BTN_DOWN ){
            return -1;
        }
        if( receivePublicKey(px,py) )
            continue;
        lcdPrintln("Got PUBKEY");
        lcdRefresh(); 
        if( receiveMac(mac) )
            continue;
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

void receiveFile(void)
{
    if( sendKeys() )
        return;

    uint8_t done = 0;
    uint8_t key;
    uint8_t k1[16], k2[16], rx[4*NUMWORDS], ry[4*NUMWORDS];
 
    while( !done ){
        lcdClear();
        lcdSetCrsr(0,0);
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
        lcdPrintln("Got R");
        lcdRefresh();
        ECIES_decryptkeygen(rx, ry, k1, k2, Priv);
        //if( filetransfer_receive(mac,k1) )
        //    continue;
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
    ECIES_encyptkeygen(px, py, k1, k2, rx, ry);
    
    while( !done ){
        lcdClear();
        lcdPrintln("Sending file");lcdRefresh();
        sendR(rx,ry);
        lcdPrintln("Sent R");
        lcdRefresh();
        delayms(3000);
        //filetransfer_send((uint8_t*)filename, 0, mac, (uint32_t*)k1);
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
    }
}

void main_vcard(void) {
    char key;
    nrf_init();
    f_mount(0, &FatFs[0]);
    nrf_set_rx_mac(0, 32, 5, mac);

    while (1) {
        key= getInput();
        delayms(20);

        // Easy flashing
        if(key==BTN_LEFT){
            DoString(0,8,"Enter ISP!");
            lcdDisplay(0);
            ISPandReset(5);
        }else if(key==BTN_UP){
            lcdClear();
            lcdPrintln("Generating...");
            lcdRefresh();
            sendFile("foobar.txt");
            //uint8_t k1[16], k2[16], Rx[4*NUMWORDS], Ry[4*NUMWORDS];
            //ECIES_encyptkeygen("1c56d302cf642a8e1ba4b48cc4fbe2845ee32dce7",
            //                "45f46eb303edf2e62f74bd68368d979e265ee3c03",
            //                k1, k2, Rx, Ry);
            //nrf_snd_pkt_crc(30, k1); 
            lcdPrintln("Done");
            lcdRefresh();
        }else if(key==BTN_DOWN){
            lcdClear();
            lcdPrintln("Generating...");
            lcdRefresh();
            receiveFile();
            //uint8_t k1[16], k2[16], Rx[4*NUMWORDS], Ry[4*NUMWORDS];
            //ECIES_encyptkeygen("1c56d302cf642a8e1ba4b48cc4fbe2845ee32dce7",
            //                "45f46eb303edf2e62f74bd68368d979e265ee3c03",
            //                k1, k2, Rx, Ry);
            //nrf_snd_pkt_crc(30, k1); 
            lcdPrintln("Done");
            lcdRefresh();
        }

                //encryption_decryption_demo("This is encrypted",
        //                "1c56d302cf642a8e1ba4b48cc4fbe2845ee32dce7", 
        //                "45f46eb303edf2e62f74bd68368d979e265ee3c03",
        //                "0e10e787036941e6c78daf8a0e8e1dbfac68e26d2");
    }
}

void tick_vcard(void){
    return;
};

