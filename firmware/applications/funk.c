#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/backlight.h"
#include "lcd/allfonts.h"

#include "funk/nrf24l01p.h"

/**************************************************************************/

void f_init(void){
    nrf_init();
};

#define CS_LOW()    {gpioSetValue(RB_SPI_NRF_CS, 0); gpioSetValue(3,2,0);}
#define CS_HIGH()   {gpioSetValue(RB_SPI_NRF_CS, 1); gpioSetValue(3,2,1);}
#include "core/ssp/ssp.h"

void f_status(void){
    int dx=0;
    int dy=8;
    uint8_t buf[4];

    buf[0]=C_R_REGISTER | R_CONFIG;
    buf[1]=0;
    buf[2]=0;
    buf[3]=0;
    dx=DoString(0,dy,"S:"); DoIntX(dx,dy,*(int*)buf);dy+=8;
    nrf_cmd_rw_long(buf,2);
    dx=DoString(0,dy,"R:"); DoIntX(dx,dy,*(int*)buf);dy+=8;

    int status=nrf_cmd_status(C_NOP);
    dx=DoString(0,dy,"St:"); DoIntX(dx,dy,status);dy+=8;
};

void f_recv(void){
    int dx=0;
    int dy=8;
    uint8_t buf[32];
    int len;

    len=nrf_rcv_pkt_time(500,sizeof(buf),buf);

    if(len==0){
        dx=DoString(0,dy,"No pkt"); dy+=8;
        return;
    }else if (len ==-1){
        dx=DoString(0,dy,"Pkt too lg"); dy+=8;
        return;
    }else if (len ==-2){
        dx=DoString(0,dy,"No pkt error?"); dy+=8;
        return;
    };
    dx=DoString(0,dy,"Size:"); DoInt(dx,dy,len); dy+=8;
    dx=DoString(0,dy,"1:"); DoIntX(dx,dy,*(int*)(buf));dy+=8;
    dx=DoString(0,dy,"2:"); DoIntX(dx,dy,*(int*)(buf+4));dy+=8;
    dx=DoString(0,dy,"3:"); DoIntX(dx,dy,*(int*)(buf+8));dy+=8;
    dx=DoString(0,dy,"4:"); DoIntX(dx,dy,*(int*)(buf+12));dy+=8;

    len=crc16(buf,14);
    dx=DoString(0,dy,"c:"); DoIntX(dx,dy,len);dy+=8;

};

void f_send(void){
    static char ctr=1;
    int dx=0;
    int dy=8;
    uint8_t buf[32];
    int status;
    uint16_t crc;

    buf[0]=0x05; // ID
    buf[1]=0xEC; // ID
    buf[2]=0xff;
    buf[3]=0xff; // Send intensity

    buf[4]=0x00; // ctr
    buf[5]=0x00; // ctr
    buf[6]=0x00; // ctr
    buf[7]=ctr++; // ctr

    buf[8]=0xff;
    buf[9]=0xff;
    buf[10]=0xff;
    buf[11]=0xff;
    buf[12]=0xff;
    buf[13]=0xff;

    crc=crc16(buf,14);
    buf[14]=crc & 0xff; // CRC
    buf[15]=(crc >>8) & 0xff; // CRC

    status=nrf_snd_pkt_crc(16,buf);

    dx=DoString(0,dy,"St:"); DoInt(dx,dy,status); dy+=8;

};

void gotoISP(void) {
    DoString(0,0,"Enter ISP!");
    lcdDisplay(0);
    ISPandReset(5);
}

/**************************************************************************/

const struct MENU_DEF menu_ISP =    {"Invoke ISP",  &gotoISP};
const struct MENU_DEF menu_init =   {"F Init",   &f_init};
const struct MENU_DEF menu_status = {"F Status", &f_status};
const struct MENU_DEF menu_rcv =    {"F Recv",   &f_recv};
const struct MENU_DEF menu_snd =    {"F Send",   &f_send};
const struct MENU_DEF menu_nop =    {"---",   NULL};

static menuentry menu[] = {
    &menu_init,
    &menu_status,
    &menu_rcv,
    &menu_snd,
    &menu_nop,
    &menu_ISP,
    NULL,
};

static const struct MENU mainmenu = {"Mainmenu", menu};

void main_funk(void) {

    backlightInit();

    while (1) {
        lcdFill(0); // clear display buffer
        lcdDisplay(0);
        handleMenu(&mainmenu);
        gotoISP();
    }
};

void tick_funk(void){
    static int foo=0;
    static int toggle=0;
	if(foo++>50){
        toggle=1-toggle;
		foo=0;
        gpioSetValue (RB_LED0, toggle); 
	};
    return;
};


