#include <sysinit.h>
#include <string.h>
#include <time.h>

#include "basic/basic.h"
#include "basic/byteorder.h"

#include "lcd/lcd.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"

#include "funk/mesh.h"

#include <string.h>

/**************************************************************************/

void m_init(void){
    nrf_init();

    struct NRF_CFG config = {
        .channel= MESH_CHANNEL,
        .txmac= MESH_MAC,
        .nrmacs=1,
        .mac0=  MESH_MAC,
        .maclen ="\x20", // XXX: MESHPKTSIZE
    };

    nrf_config_set(&config);

    initMesh();
};

void m_tset(void){
    _timet=1311961112;
};

void m_time(void){
    struct tm* tm;
    char c[2]={0,0};
    getInputWaitRelease();
    delayms(100);
    do{
        lcdClear();
        tm= mygmtime(getSeconds());
        lcdPrintInt(tm->tm_hour);
        lcdPrint(":");
        lcdPrintInt(tm->tm_min);
        lcdPrint(":");
        lcdPrintInt(tm->tm_sec);
        lcdNl();
        lcdPrintInt(tm->tm_mday);
        lcdPrint(".");
        lcdPrintInt(tm->tm_mon+1);
        lcdPrint(".");
        lcdPrintInt(tm->tm_year+YEAR0);
        lcdNl();

        lcdNl();
        lcdPrint("<");

        for(int i=0;i<MESHBUFSIZE;i++){
            if(!meshbuffer[i].flags&MF_USED){
                c[0]='_';
            }else{
                c[0]=meshbuffer[i].pkt[0];
            };
            lcdPrint(c);
        };
        lcdPrintln(">");

        lcdPrint("Gen:");
        lcdPrintInt(meshgen);
        lcdNl();
        lcdRefresh();
        delayms_queue(50);
    }while ((getInputRaw())==BTN_NONE);
};


inline void blink(char a, char b){
    gpioSetValue (a,b, 1-gpioGetValue(a,b));
};



/***********************************************************************/


void m_cleanup(void){
    mesh_cleanup();
};

void m_recv(void){
    mesh_recvloop();
}

void m_send(void){
    mesh_sendloop();
};

void tick_mesh(void){
    mesh_systick();
};

