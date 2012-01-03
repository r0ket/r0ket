#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"
#include "funk/mesh.h"

#include "usetable.h"

/**************************************************************************/
void m_time(void);

void ram(void) {
	m_time();
};

void m_time_details(int select) {
    getInputWaitRelease();

    while(getInputWaitTimeout(50) == BTN_NONE){
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
            for(uint8_t *body = MO_BODY(pkt); body < (pkt + 30); body++) {
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
    int active=0;
    struct tm* tm;
    int select=0;
    char c[2]={0,0};
    getInputWaitRelease();
    delayms(100);
    while(1) {
        lcdClear();
        lcdPrint("  ");
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

	if(active){
	    lcdPrint(" ");
	    for(int i=0; i<MESHBUFSIZE; i++) {
		lcdPrint(i == select ? "^" : " ");
	    }
	    lcdNl();
	}else{
	    lcdPrintln(" up to select");
	};

        lcdPrint("Gen:");
        lcdPrint(IntToStrX(meshgen,2));
	lcdPrint(" ");
	lcdPrint(GLOBAL(nickname));
        lcdNl();
        lcdPrint("Inc:");
        lcdPrintInt(meshincctr);
        lcdNl();
        lcdPrint("Nice:");
        lcdPrintInt(meshnice);
	lcdPrint(" R:");
	lcdPrint(IntToStrX(getrelease()%0xfff,3));
        lcdNl();
        lcdRefresh();

	uint8_t key = getInputWaitTimeout(100);
	if(active==1){
	    switch(key) {
		case BTN_LEFT:
		    select--;
		    if (select < 0)
			select = MESHBUFSIZE - 1;
		    break;
		case BTN_RIGHT:
		    select++;
		    if (select >= MESHBUFSIZE)
			select = 0;
		    break;
		case BTN_ENTER:
		    m_time_details(select);
		    break;
		case BTN_UP:
		    active=0;
		    break;
		case BTN_DOWN:
		    // Exit
		    return;
	    }
	}else{
	    switch(key) {
		case BTN_UP:
		    active=1;
		    break;
		default:
		    return;
	    };
	};
	if (key != BTN_NONE)
		getInputWaitRelease();
    }
};
