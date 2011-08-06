#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/print.h"
#include "lcd/render.h"
#include "lcd/display.h"

#include "basic/config.h"

#include "usetable.h"

#define one ((uint32_t)1)
#define utf8boundary nick[4]+nick[6]

typedef uint8_t uchar;

static unsigned long iter=0;

void ram(void) {
    getInputWaitRelease();

    static int nickx=2,nicky=10;
    static int nickwidth,nickheight;
    static char *croppednickbase;
    static int nickoff=10;
    static char delay=15;
    static char speedmode=0;
    static char movx=1;
    static char LCDSHIFTX_EVERY_N=1;
    static char LCDSHIFTY_EVERY_N=1;
    static char charwidthbuffer[]={82,48,107,101,84,32,70,105,114,109,119,48,114,101,32,112,114,51,115,51,110,116,51,100,32,98,121,32,84,69,65,77,32,82,48,75,69,84,58,32,67,111,100,101,32,119,114,105,116,116,101,110,32,98,121,32,48,48,45,115,99,104,110,101,105,100,48,114,44,32,115,48,99,44,32,98,48,114,105,115,44,32,105,103,103,48,44,32,108,48,108,97,102,105,115,99,104,44,32,98,115,120,44,32,107,48,117,32,97,110,100,32,48,116,104,101,114,115,46,32,86,105,115,105,116,32,117,115,32,97,116,32,102,112,108,101,116,122,32,86,105,108,108,48,103,101,46,32,83,104,48,117,116,32,108,48,117,100,58,32,76,97,117,110,99,104,32,84,104,101,32,82,48,107,101,116,33,0};
    char *nick=nickname;
    croppednickbase=&charwidthbuffer[0];

    lcdClear();
    setExtFont(GLOBAL(nickfont));
    nicky=1; 
    nickwidth=DoString(nickx,nicky,nick);
    if(nickwidth<50)nickoff=30;
    nickheight=getFontHeight();
    nicky=(RESY-getFontHeight())/2;
    // Adjust speed depending on rendered image size
    if(nickwidth>RESX && nickheight>14) { movx=2;}
    if(nickwidth>RESX && nickheight>40) { movx=3;}

    lcdClear();
    char stepmode=0;
    // ugly hack for wrong encoded multibyte chars in default Font
    if(utf8boundary==80){ nick=croppednickbase; setExtFont(NULL);
      nickwidth=DoString(nickx,nicky,nick);
      movx=1; delay=50;
    }
    while (1) {
        ++iter;
        lcdDisplay();
        lcdClear();
        // Old shift code. Can't handle longer Nicks...
        // if(iter%LCDSHIFT_EVERY_N==0) lcdShift(1,-2,1);
        // if(iter%LCDSHIFT_EVERY_N==0) { nickx=(nickx+1)%100-nickwidth; nicky=(nicky+1)%50;}
        if(iter%LCDSHIFTX_EVERY_N==0) { nickx-=movx; 
        if(nickx<=(-1*nickwidth-nickoff))nickx=0; }
#ifdef SIMULATOR
  fprintf(stderr,"nickx %d \n",nickx);
#endif
        DoString(nickx,nicky,nick);
        DoString(nickx+nickwidth+nickoff,nicky,nick);
        if(nickwidth<RESX) DoString(nickx+2*(nickwidth+nickoff),nicky,nick);
	char key=stepmode?getInputWait():getInputRaw();
	stepmode=0;
	switch(key) {
        // Buttons: Right change speed, Up hold scrolling
	case BTN_ENTER:
	  return;
	case BTN_RIGHT:
	  getInputWaitRelease();
          speedmode=(speedmode+1)%6;
          delay=15;
          // speeds: normal, slow, sloooow, double, tripple... 
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
	case BTN_UP:
	  stepmode=1;
	  getInputWaitRelease();
	  break;
	case BTN_LEFT:
          return;
	case BTN_DOWN:
	  return;
	}
        delayms_queue_plus(delay,0);
    }
    return;
}
