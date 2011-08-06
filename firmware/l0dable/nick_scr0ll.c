#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/print.h"
#include "lcd/render.h"
#include "lcd/display.h"

#include "basic/config.h"

#include "usetable.h"

#define one ((uint32_t)1)

typedef uint8_t uchar;

static unsigned long iter=0;

void ram(void) {
    getInputWaitRelease();

    static int nickx=2,nicky=10;
    signed char movy=1;
    static int nickwidth,nickheight;
    static int nickoff=10;
    static char delay=10;
    static char speedmode=0;
    static char movx=1;
    static char LCDSHIFTX_EVERY_N=2;
    static char LCDSHIFTY_EVERY_N=2;

    lcdClear();
    setExtFont(GLOBAL(nickfont));
    nicky=1; 
    nickwidth=DoString(nickx,nicky,GLOBAL(nickname));
    if(nickwidth<50)nickoff=30;
    nickheight=getFontHeight();
    nicky=(RESY-getFontHeight())/2;


    char stepmode=0;
    while (1) {
        ++iter;
        lcdDisplay();
        lcdClear();
        // draw_area(); // xor life pattern again to restore original display content
        // Old shift code. Can't handle longer Nicks...
        // if(iter%LCDSHIFT_EVERY_N==0) lcdShift(1,-2,1);
        // if(iter%LCDSHIFT_EVERY_N==0) { nickx=(nickx+1)%100-nickwidth; nicky=(nicky+1)%50;}
        if(iter%LCDSHIFTX_EVERY_N==0) { nickx-=movx; 
        if(nickx<(-1*nickwidth-nickoff))nickx=0; }
        // if(iter%LCDSHIFTY_EVERY_N==0) { nicky+=movy;
        // if(nicky<1 || nicky>RESY-nickheight) movy*=-1; }
        DoString(nickx,nicky,GLOBAL(nickname));
        DoString(nickx+nickwidth+nickoff,nicky,GLOBAL(nickname));
        if(nickwidth<RESX) DoString(nickx+2*(nickwidth+nickoff),nicky,GLOBAL(nickname));
	char key=stepmode?getInputWait():getInputRaw();
	stepmode=0;
	switch(key) {
	case BTN_ENTER:
	  return;
	case BTN_RIGHT:
	  getInputWaitRelease();
          speedmode=(speedmode+1)%6;
          delay=15;
          switch(speedmode) {
            case 0:
              movx=1; LCDSHIFTX_EVERY_N=1; LCDSHIFTY_EVERY_N=1; break;
            case 1:
              movx=1; LCDSHIFTX_EVERY_N=2; LCDSHIFTY_EVERY_N=2; break;
            case 2:
              movx=1; LCDSHIFTX_EVERY_N=3; LCDSHIFTY_EVERY_N=4; break;
            case 4:
              movx=2; LCDSHIFTX_EVERY_N=1; LCDSHIFTY_EVERY_N=1; break;
            case 5:
              movx=3; LCDSHIFTX_EVERY_N=1; LCDSHIFTY_EVERY_N=1; break;
          }
	  break; 
	case BTN_DOWN:
	  stepmode=1;
	  getInputWaitRelease();
	  break;
	case BTN_LEFT:
          return;
	case BTN_UP:
	  return;
	}
        delayms_queue_plus(delay,0);
    }
    return;
}
