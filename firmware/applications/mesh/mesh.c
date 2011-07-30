#include <sysinit.h>
#include <string.h>
#include <time.h>

#include "basic/basic.h"
#include "basic/byteorder.h"

#include "lcd/lcd.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"

#include <string.h>

#define MESH_CHANNEL 85
#define MESH_MAC     "MESHB"

/**************************************************************************/

uint32_t const meshkey[4] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

#define MESHBUFSIZE 10
#define MESHPKTSIZE 32
typedef struct {
    uint8_t pkt[32];
    char flags;
} MPKT;

MPKT meshbuffer[MESHBUFSIZE];

#define MF_FREE (0)
#define MF_USED (1<<0)

time_t _timet=0;

// Timezones suck. Currently we only do it all in localtime.
// I know it's broken. Sorry
time_t getSeconds(void){
    return _timet+(getTimer()*SYSTICKSPEED/1000);
};

// **********************************************************************

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

    for(int i=0;i<MESHBUFSIZE;i++){
        meshbuffer[i].flags=MF_FREE;
    };
    memset(meshbuffer[0].pkt,0,MESHPKTSIZE);
    meshbuffer[0].pkt[0]='T';
    uint32touint8p(getSeconds(),meshbuffer[0].pkt+2);
    meshbuffer[0].flags=MF_USED;
};

void m_tset(void){
    _timet=1311961112;
};

void m_cleanup(void){
    time_t now=getSeconds();
    for(int i=1;i<MESHBUFSIZE;i++){
        if(meshbuffer[i].flags&MF_USED){
            if (uint8ptouint32(meshbuffer[i].pkt+2)<now){
                meshbuffer[i].flags=MF_FREE;
            };
        };
    };
};

#define YEAR0           1900                    /* the first year */
#define EPOCH_YR        1970            /* EPOCH = Jan 1 1970 00:00:00 */
#define SECS_DAY        (24L * 60L * 60L)
#define LEAPYEAR(year)  (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year)  (LEAPYEAR(year) ? 366 : 365)

int _ytab[2][12] = {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

struct tm * mygmtime(register const time_t time) {
        static struct tm br_time;
        register struct tm *timep = &br_time;
        register unsigned long dayclock, dayno;
        int year = EPOCH_YR;

        dayclock = (unsigned long)time % SECS_DAY;
        dayno = (unsigned long)time / SECS_DAY;

        timep->tm_sec = dayclock % 60;
        timep->tm_min = (dayclock % 3600) / 60;
        timep->tm_hour = dayclock / 3600;
        timep->tm_wday = (dayno + 4) % 7;       /* day 0 was a thursday */
        while (dayno >= YEARSIZE(year)) {
                dayno -= YEARSIZE(year);
                year++;
        }
        timep->tm_year = year - YEAR0;
        timep->tm_yday = dayno;
        timep->tm_mon = 0;
        while (dayno >= _ytab[LEAPYEAR(year)][timep->tm_mon]) {
                dayno -= _ytab[LEAPYEAR(year)][timep->tm_mon];
                timep->tm_mon++;
        }
        timep->tm_mday = dayno + 1;
        timep->tm_isdst = 0;

        return timep;
}

#define M_SENDINT 500
#define M_RECVINT 1000
#define M_RECVTIM 100


void m_recv(void){
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int len;
    int recvend=M_RECVTIM/SYSTICKSPEED+getTimer();

    static int toggle=0;
    gpioSetValue (RB_LED2, toggle); 
    toggle=1-toggle;

    m_cleanup();

    nrf_rcv_pkt_start();
    do{
        len=nrf_rcv_pkt_poll_dec(sizeof(buf),buf,meshkey);

        // Receive
        if(len<=0){
            delayms_power(10);
            continue;
        };

        int i;
        for(i=0;i<MESHBUFSIZE;i++){
            if((meshbuffer[i].flags&MF_USED)==0)
                break;
        };
        if(i==MESHBUFSIZE){ // Buffer full. Ah well. Kill a random packet
            i=1; // XXX: GetRandom()?
        };
        memcpy(meshbuffer[i].pkt,buf,MESHPKTSIZE);
        meshbuffer[i].flags=MF_USED;

        if(buf[0]=='T'){
            gpioSetValue (RB_LED1, 0); 
            time_t toff=uint8ptouint32(buf+2)-(getTimer()*SYSTICKSPEED/1000);
            if(toff>_timet) // Do not live in the past.
                _timet = toff;
        }else if (buf[0]>='A' && buf[0] <'T'){ // Truncate ascii packets.
            meshbuffer[i].pkt[MESHPKTSIZE-3]=0;
        };
    }while(getTimer()<recvend);
    nrf_rcv_pkt_end();
}

void m_send(void){
    int ctr=0;
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int status;

    // Update [T]ime packet

    uint32touint8p(getSeconds(),meshbuffer[0].pkt+2);
    for (int i=0;i<MESHBUFSIZE;i++){
        if(!meshbuffer[i].flags&MF_USED)
            continue;
        ctr++;
        memcpy(buf,meshbuffer[i].pkt,MESHPKTSIZE);
        status=nrf_snd_pkt_crc_encr(MESHPKTSIZE,buf,meshkey);
        //Check status? But what would we do...
    };
};

void m_info(void){
    char ctr=0;
    getInputWaitRelease();
    lcdClear();
    for (int i=0;i<MESHBUFSIZE;i++){
        if(!meshbuffer[i].flags&MF_USED)
            continue;
        ctr++;
    };
    lcdPrint("MeshQ:");
    lcdPrintInt(ctr);
    lcdNl();
    lcdDisplay();
};


void tick_mesh(void){
    static int ctr=0;
    ctr++;
    if((ctr % (M_RECVINT/SYSTICKSPEED))==0){
        push_queue(&m_recv);
    };

    if((ctr % (M_SENDINT/SYSTICKSPEED))==0){
        push_queue(&m_send);
    };
};


void m_time(void){
    struct tm* tm;
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
        lcdRefresh();
        delayms_queue(50);
    }while ((getInputRaw())==BTN_NONE);
};
