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

void init_mesh(void){
    nrf_init();
    initMesh();
};

void m_tset(void){
    _timet=1311961112;
};

//# MENU debug MeshInfo
void m_time(void){
    struct tm* tm;
    char c[2]={0,0};
    getInputWaitRelease();
    delayms(100);
    do{
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


int choose(char * texts, int8_t menuselection){
    uint8_t numentries = 0;
    uint8_t visible_lines = 0;
    uint8_t current_offset = 0;

    char*p=texts;

    do{
        lcdPrintln(p);
        while(*p)p++;
        numentries++;p++;
    }while(*p);
    numentries--;

    visible_lines = (RESY/getFontHeight())-1; // subtract title line

    while (1) {
        // Display current menu page
        lcdClear();
        lcdPrintln(texts);
        p=texts;
        while(*p++);
        for(int i=0;i<current_offset;i++)
            while(*p++);
        for (uint8_t i = current_offset; i < (visible_lines + current_offset) && i < numentries; i++) {
            if (i == menuselection)
                lcdPrint("*");
            lcdSetCrsrX(14);
            lcdPrintln(p);
            while(*p++);
        }
        lcdRefresh();

        switch (getInputWait()) {
            case BTN_UP:
                menuselection--;
                if (menuselection < current_offset) {
                    if (menuselection < 0) {
                        menuselection = numentries-1;
                        current_offset = ((numentries-1)/visible_lines) * visible_lines;
                    } else {
                        current_offset -= visible_lines;
                    }
                }
                break;
            case BTN_DOWN:
                menuselection++;
                if (menuselection > (current_offset + visible_lines-1) || menuselection >= numentries) {
                    if (menuselection >= numentries) {
                        menuselection = 0;
                        current_offset = 0;
                    } else {
                        current_offset += visible_lines;
                    }
                }
                break;
            case BTN_LEFT:
                return -1;
            case BTN_RIGHT:
            case BTN_ENTER:
                return menuselection;
        }
        getInputWaitRelease();
    }
    /* NOTREACHED */
}



/***********************************************************************/

char *meshmsgs(void){
    static char msgtypes[MESHBUFSIZE+1];
    memset(msgtypes,'_',MESHBUFSIZE);
    msgtypes[MESHBUFSIZE]=0;
    uint8_t lo=0;
    uint8_t hi;

    for(int o=0;o<MESHBUFSIZE;o++){
        hi=0xff;
        for(int i=0;i<MESHBUFSIZE;i++){
            if(meshbuffer[i].flags&MF_USED){
                if(MO_TYPE(meshbuffer[i].pkt)>lo)
                    if(MO_TYPE(meshbuffer[i].pkt)<hi)
                        hi=MO_TYPE(meshbuffer[i].pkt);
            };
        };
        if(hi==0xff){
            msgtypes[o]=0;
            break;
        };
        msgtypes[o]=hi;
        lo=hi;
    };

    return msgtypes;
};



extern MPKT meshbuffer[MESHBUFSIZE];
//# MENU mesh Messages
void m_choose(){
    char list[99];
    int i=0;

    while(1){
    char *p=list;
    strcpy(p,"Note");
    while(*p++);

    char *mm=meshmsgs();
    char *tmm=mm;
    while(*mm){
        switch(*mm){
            case('A'):
                strcpy(p,"Message");
                break;
            case('E'):
                strcpy(p,"Kourou");
                break;
            case('F'):
                strcpy(p,"Baikonur");
                break;
            case('T'):
                strcpy(p,"Time");
                break;
            case('i'):
                strcpy(p,"Invaders");
                break;
            default:
                p[0]=*mm;
                p[1]=0;
        };
        while(*p++);
        mm++;
    };
    p[0]=0;

    i=choose(list,i);
    if(i<0)
        return;
    lcdClear();

    int j=0;
    for(int z=0;z<MESHBUFSIZE;z++)
        if(meshbuffer[z].flags&MF_USED)
            if(MO_TYPE(meshbuffer[z].pkt)==tmm[i])
                j=z;

    switch(tmm[i]){
        case('A'):
            lcdPrintln("Message");
            break;
        case('E'):
            lcdPrintln("Kourou");
            break;
        case('F'):
            lcdPrintln("Baikonur");
            break;
        case('T'):
            lcdPrintln("Time");
            break;
        case('i'):
            lcdPrintln("Invaders");
            break;
    };
    if(tmm[i]>='a' && tmm[i]<='z'){
        lcdPrintln(IntToStr(MO_TIME(meshbuffer[j].pkt),10,0));
    }else{
        struct tm *tm= mygmtime(MO_TIME(meshbuffer[j].pkt));
        lcdPrint(IntToStr(tm->tm_hour,2,F_LONG));
        lcdPrint(":");
        lcdPrint(IntToStr(tm->tm_min,2,F_LONG|F_ZEROS));
        lcdPrint(":");
        lcdPrint(IntToStr(tm->tm_sec,2,F_LONG|F_ZEROS));
        lcdNl();
    };
    char *foo=(char *)MO_BODY(meshbuffer[j].pkt);
    while(strlen(foo)>13){
        int q;
        for(q=0;q<13;q++){
            if(foo[q]==' ')
                break;
        };
        foo[q]=0;
        lcdPrintln(foo);
        foo[q]=' ';
        foo+=q+1;
    };
    lcdPrintln(foo);
    lcdRefresh();
    getInputWaitRelease();
    };
};


void tick_mesh(void){
    mesh_systick();
};

