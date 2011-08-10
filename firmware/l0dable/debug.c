#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "usb/usbmsc.h"

#include "core/iap/iap.h"

#include "funk/nrf24l01p.h"
#include "funk/mesh.h"

#include "usetable.h"

/**************************************************************************/
void ChkFunk(void);
void ChkLight(void);
void ChkBattery(void);
void m_time(void);
void Qstatus(void);
void getsp(void);
void uptime(void);
void uuid(void);

static const struct MENU submenu_debug={ "debug", {
	{ "ChkBattery", &ChkBattery},
	{ "ChkLight", &ChkLight},
	{ "MeshInfo", &m_time},
	{ "ChkFunk", &ChkFunk},
	{ "Qstatus", &Qstatus},
//	{ "ShowSP", &getsp},
	{ "Uptime", &uptime},
	{ "Uuid", &uuid},
	{NULL,NULL}
}};

void ram(void) {
	handleMenu(&submenu_debug);
};

//# MENU debug ChkLight
void ChkLight(void) {
    int dx=0;
    int dy=8;
    dx=DoString(0,dy,"Light:");
    DoString(0,dy+16,"Night:");
    while ((getInputRaw())==BTN_NONE){
        DoInt(dx,dy,GetLight());
        DoInt(dx,dy+16,isNight());
        DoInt(dx,dy+8,GLOBAL(daytrig));
        lcdDisplay();
        delayms_queue(100);
    };
    dx=DoString(0,dy+24,"Done.");
}

//# MENU debug ChkBattery
void ChkBattery(void) {
    do{
        lcdClear();
        lcdPrintln("Voltage:");
        lcdPrintln(IntToStr(GetVoltage(),5,0));
        lcdNl();
        lcdPrintln("Chrg_stat:");
        if(gpioGetValue(RB_PWR_CHRG)){
            lcdPrintln("not charging");
        }else{
            lcdPrintln("    CHARGING");
        };
        lcdRefresh();
        delayms_queue(100);
    } while ((getInputRaw())==BTN_NONE);
}

//# MENU debug Uptime
void uptime(void) {
    int t;
    int h;
    char flag;
    while ((getInputRaw())==BTN_NONE){
        lcdClear();
        lcdPrintln("Uptime:");
        t=(_timectr)/(1000/SYSTICKSPEED);
        h=t/60/60;
        flag=F_ZEROS;
        if(h>0){
            lcdPrint(IntToStr(h,2,flag));
            lcdPrint("h");
            flag|=F_LONG;
        };
        h=t/60%60;
        if(h>0){
            lcdPrint(IntToStr(h,2,flag));
            lcdPrint("m");
            flag|=F_LONG;
        };
        h=t%60;
        if(h>0){
            lcdPrint(IntToStr(h,2,flag));
            lcdPrint("s");
        };
        lcdNl();
        lcdNl();
        lcdPrintln("Ticks:");
        lcdPrint(IntToStr(_timectr,10,0));
        lcdRefresh();
        delayms_queue(222);
    };
    lcdPrintln("done.");
}

//# MENU debug Uuid
void uuid(void) {
    IAP_return_t iap_return;
    iap_return = iapReadSerialNumber();
    lcdClear();
    lcdPrintln("UUID:");
    lcdPrintln(IntToStrX(iap_return.Result[0],8));
    lcdPrintln(IntToStrX(iap_return.Result[1],8));
    lcdPrintln(IntToStrX(iap_return.Result[2],8));
    lcdPrintln(IntToStrX(iap_return.Result[3],8));
    lcdNl();
    lcdPrintln("Beacon ID:");
    lcdPrintln(IntToStrX(GetUUID32(),8));
    lcdRefresh();
    while(!getInputRaw())work_queue();
}

//# MENU debug Qstatus
void Qstatus(void) {
    int dx=0;
    int dy=8;
    lcdClear();
    dx=DoString(0,dy+16,"Qdepth:");
    while ((getInputRaw())!=BTN_ENTER){
        DoInt(dx,dy+16,(the_queue.qend-the_queue.qstart+MAXQENTRIES)%MAXQENTRIES);
        lcdDisplay();
        if(getInputRaw()!=BTN_NONE)
            work_queue();
        else
            delayms(10);
    };
    dy+=16;
    dx=DoString(0,dy,"Done.");
};

//# MENU debug ShowSP
void getsp(void) {
    int dx=0;
    int dy=8;
    int x;
    lcdClear();
    dx=DoString(0,dy,"SP:");
    while ((getInputRaw())==BTN_NONE){
        __asm(  "mov %0, sp\n" : "=r" (x) :);
        DoIntX(0,dy+8,x);
        lcdDisplay();
        delayms_queue(50);
    };
    dy+=16;
    dx=DoString(0,dy,"Done.");
};

void m_time_details(int select) {
    getInputWaitRelease();

    while(getInputWaitTimeout(50) != BTN_LEFT){
        lcdClear();
        MPKT *mpkt = &meshbuffer[select];
        uint8_t *pkt = mpkt->pkt;

        if (!mpkt->flags & MF_USED)
            lcdPrint("<unused>");
        else {
            lcdPrint("Type: ");
            char c[2] = {0, 0};
            c[0] = mpkt->pkt[0];
            lcdPrint(c);
            lcdNl();

            lcdPrint("Gen: ");
            lcdPrintInt(MO_GEN(pkt) & 0xff);
            lcdNl();

            lcdPrint("Time: ");
            lcdPrintInt(MO_TIME(pkt) & 0xffff);
            lcdNl();
            lcdPrint("Body: ");
            lcdNl();

            int x = 0;
            for(uint8_t *body = MO_BODY(pkt); body < pkt + 32; body++) {
                if (*body >= ' ' && *body < 128) {
                    if (x > 12) {
                        lcdNl();
                        x = 0;
                    }

                    char c[2] = {*body, 0};
                    lcdPrint(c);
                    x++;
                } else {
                    if (x > 10) {
                        lcdNl();
                        x = 0;
                    }

                    lcdPrint("\\");
                    lcdPrintInt(*body & 0xff);
                    x += 2;
                    if (*body > 9)
                        x++;
                    if (*body > 99)
                        x++;
                }
            }
        }
            
        lcdRefresh();
    }
}

void m_time(void){
    struct tm* tm;
    int select=0;
    char c[2]={0,0};
    getInputWaitRelease();
    delayms(100);
    while(1) {
        lcdClear();
        tm= mygmtime(getSeconds());
        lcdPrint(IntToStr(tm->tm_hour,2,F_LONG));
        lcdPrint(":");
        lcdPrint(IntToStr(tm->tm_min,2,F_LONG|F_ZEROS));
        lcdPrint(":");
        lcdPrint(IntToStr(tm->tm_sec,2,F_LONG|F_ZEROS));
        lcdNl();
        lcdPrint(IntToStr(tm->tm_mday,2,F_LONG));
        lcdPrint(".");
        lcdPrint(IntToStr(tm->tm_mon+1,2,0));
        lcdPrint(".");
        lcdPrint(IntToStr(tm->tm_year+YEAR0,4,F_LONG|F_ZEROS));
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

        lcdPrint(" ");
        for(int i=0; i<MESHBUFSIZE; i++) {
            lcdPrint(i == select ? "^" : " ");
        }
        lcdNl();

        lcdPrint("Gen:");
        lcdPrintInt(meshgen);
        lcdNl();
        lcdPrint("Inc:");
        lcdPrintInt(meshincctr);
        lcdNl();
        lcdPrint("Nice:");
        lcdPrintInt(meshnice);
        lcdNl();
        lcdRefresh();

        uint8_t key = getInputWaitTimeout(50);
        switch(key) {
        case BTN_UP:
            select--;
            if (select < 0)
                select = MESHBUFSIZE - 1;
            break;
        case BTN_DOWN:
            select++;
            if (select >= MESHBUFSIZE)
                select = 0;
            break;
        case BTN_RIGHT:
        case BTN_ENTER:
            m_time_details(select);
            break;
        case BTN_LEFT:
            // Exit
            return;
        }
        if (key != BTN_NONE)
            getInputWaitRelease();
    }
};

void ChkFunk(){
    lcdPrint("st: ");
    lcdPrintln(IntToStrX(nrf_read_reg(R_STATUS),2));
    lcdPrint("fifost:");
    lcdPrintln(IntToStrX(nrf_read_reg(R_FIFO_STATUS),2));
    lcdPrint("cfg:");
    lcdPrintln(IntToStrX(nrf_read_reg(R_CONFIG),2));
    lcdPrintln("Resets:");
    lcdPrintln(IntToStr(_nrfresets,3,0));
    lcdRefresh();
    while(!getInputRaw())work_queue();
};

