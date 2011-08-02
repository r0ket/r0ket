#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/lcd.h"
#include "lcd/print.h"
#include "lcd/allfonts.h"

#include "filesystem/ff.h"
#include "filesystem/select.h"
#include "funk/nrf24l01p.h"
#include "usb/usbmsc.h"

#include <string.h>

/**************************************************************************/

#define MAXNICK 20
char nickname[MAXNICK];

void nick_init(void){
    FIL file[2];            /* File objects */
    UINT readbytes;
    int res;

    res=f_open(&file[0], "nick.cfg", FA_OPEN_EXISTING|FA_READ);
    lcdPrint("open:");
    lcdPrintln(f_get_rc_string(res));
    if(res){
        lcdPrintln("no nick.cfg...");
        return;
    };

    res = f_read(&file[0], nickname, MAXNICK-1, &readbytes);
    lcdPrint("read:");
    lcdPrintln(f_get_rc_string(res));
    if(res){
        return;
    };

    nickname[MAXNICK-1]=0;

    // truncate at newline (or other control chars)
    for(int i=0;i<MAXNICK;i++){
        if(nickname[i]<32)nickname[i]=0;
    };

    lcdPrint("<");
    lcdPrint(nickname);
    lcdPrint(">");
    lcdNl();

    res=f_close(&file[0]);
    lcdPrint("close:");
    lcdPrintln(f_get_rc_string(res));
    if(res){
        return;
    };

    lcdPrintln("Done.");
};

char fontname[15];

void f_nick(void){
    static char ctr=0;
    char key;
    static signed char x=10;
    static signed char y=10;
    while (1) {
        lcdClear();
        lcdFill(255);

        setExtFont(fontname);

        DoString(x,y,nickname);

        setIntFont(&Font_7x8);
        lcdSetCrsr(50,50);
        lcdPrintInt(x);
        lcdPrint("x");
        lcdPrintInt(y);

        lcdDisplay();
        __asm volatile ("WFI");
        __asm volatile ("WFI");
        __asm volatile ("WFI");
        __asm volatile ("WFI");

        key= getInputRaw();
        if(key & BTN_UP){
            --y;//if(--y<0) y=0;
        };
        if (key & BTN_DOWN){
            ++y;//if(++y>=RESY) y=RESY-1;
        };
        if (key & BTN_LEFT){
            --x;//if(--x<0) x=0;
        };
        if (key & BTN_RIGHT){
            ++x;//if(++x>=RESX) x=RESX-1;
        };
        if (key == BTN_ENTER){
            lcdClear();
            lcdPrintln("Done.");
            lcdDisplay();
            ctr++;
            break;
        };
    };
};

void f_font(void){

    if( selectFile(fontname,"F0N") != 0){
        lcdPrintln("No file selected.");
        return;
    };

    lcdClear();
    lcdPrintln(fontname);
    setExtFont(fontname);
    lcdPrintln("PUabc€");
    setIntFont(&Font_7x8);
    lcdPrintln("done.");
    lcdDisplay();
    while(!getInputRaw())delayms(10);
};


void f_speedtest(void){
    lcdClear();
    lcdPrintln("Speedtest");
    lcdNl();
    lcdNl();
    lcdPrintln("running...");
    
    int start=getTimer();
    int ctr=0;
    while(ctr++<1000){
        lcdRefresh();
    };
    start=getTimer()-start;
    lcdPrint("Ticks:");
    lcdPrintln(IntToStr(start,6,0));
    lcdPrint("Tickspd:");
    lcdPrintln(IntToStr(SYSTICKSPEED,3,0));
    lcdRefresh();
    getInputWait();
    getInputWaitRelease();
};

void f_speedtest2(void){
    lcdClear();
    lcdPrintln("Speedtest");
    lcdNl();
    lcdNl();
    lcdPrintln("running...");
    
    int start=getTimer();
    int ctr=0;
    int tm;
    while(!getInputRaw()){
        ctr++;
        tm=((getTimer()-start)*SYSTICKSPEED)*1000/ctr;
        DoInt(0,32,tm);
        lcdRefresh();
    };
    getInputWaitRelease();
};
