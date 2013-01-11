#include <sysinit.h>
#include <string.h>
#include <time.h>

#include "basic/basic.h"
#include "basic/byteorder.h"
#include "basic/config.h"

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

static const char * expandmesh(char pkt){
    switch(pkt){
        case('A'): return "Message";
        case('E'): return "Saal 1";
        case('F'): return "Saal 2";
        case('G'): return "Saal 3";
        case('T'): return "Time";
        case('i'): return "Invaders";
        case('j'): return "Jump";
        case('r'): return "r0type";
        case('s'): return "Snake";
    };
    return "";
}

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
                if(*expandmesh(MO_TYPE(meshbuffer[i].pkt)))
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
}

#if 0
static inline uint32_t popcount(uint32_t *buf, uint8_t n){
    int cnt=0;
    do {
        unsigned m = *buf++;
        m = (m & 0x55555555) + ((m & 0xaaaaaaaa) >> 1);
        m = (m & 0x33333333) + ((m & 0xcccccccc) >> 2);
        m = (m & 0x0f0f0f0f) + ((m & 0xf0f0f0f0) >> 4);
        m = (m & 0x00ff00ff) + ((m & 0xff00ff00) >> 8);
        m = (m & 0x0000ffff) + ((m & 0xffff0000) >> 16);
        cnt += m;
    } while(--n);
    return cnt;
}
#endif

extern MPKT meshbuffer[MESHBUFSIZE];
//# MENU messages
void m_choose(){
    char list[99];
    int i=0;

    meshmsg=0;
    gpioSetValue (RB_LED1, 0); 

    while(1){
    char *p=list;
    strcpy(p,"Messages");
    while(*p++);

    char *mm=meshmsgs();
    char *tmm=mm;
    while(*mm){
        strcpy(p,expandmesh(*mm));

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

    lcdPrintln(expandmesh(tmm[i]));
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

#if 0
        if(tmm[i]=='Z'){
            lcdPrintln(IntToStrX(uint8ptouint32(meshbuffer[j].pkt+ 6),8));
            lcdPrintln(IntToStrX(uint8ptouint32(meshbuffer[j].pkt+10),8));
            lcdPrintln(IntToStrX(uint8ptouint32(meshbuffer[j].pkt+14),8));
            lcdPrintln(IntToStrX(uint8ptouint32(meshbuffer[j].pkt+18),8));
            lcdPrintln(IntToStrX(uint8ptouint32(meshbuffer[j].pkt+22),8));
            lcdPrintln(IntToStrX(uint8ptouint32(meshbuffer[j].pkt+26),8));
            lcdPrint(IntToStr(popcount((uint32_t*)(meshbuffer[j].pkt+6),6),3,0));
            lcdPrintln(" pts.");
            lcdRefresh();
            getInputWaitRelease();
            continue;
        }else 
#endif
        if(tmm[i]=='T'){
            lcdPrint(IntToStr(tm->tm_mday,2,F_LONG));
            lcdPrint(".");
            lcdPrint(IntToStr(tm->tm_mon+1,2,0));
            lcdPrint(".");
            lcdPrint(IntToStr(tm->tm_year+YEAR0,4,F_LONG|F_ZEROS));
            lcdNl();
            MO_BODY(meshbuffer[j].pkt)[0]=0;
        };
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
}


void tick_mesh(void){
    if(GLOBAL(privacy)<2)
        mesh_systick();
    if(_timectr%64)
        if(meshmsg){
            gpioSetValue (RB_LED1, 1); 
            meshmsg=0;
        };
}

