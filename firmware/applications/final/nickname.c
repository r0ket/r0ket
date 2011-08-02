#include <sysinit.h>
#include <string.h>
#include <time.h>

#include "basic/basic.h"
#include "basic/byteorder.h"
#include "basic/config.h"

#include "lcd/lcd.h"
#include "lcd/print.h"

#include "filesystem/ff.h"
#include "filesystem/select.h"

#include <string.h>

/**************************************************************************/

void fancyNickname(void) {
    int dx=0;
	int dy=0;
    static uint32_t ctr=0;
	ctr++;

	setExtFont(GLOBAL(nickfont));
	dx=DoString(0,0,GLOBAL(nickname));
    dx=(RESX-dx)/2;
    if(dx<0)
        dx=0;
    dy=(RESY-getFontHeight())/2;

	lcdClear();
	DoString(dx,dy,GLOBAL(nickname));
	lcdRefresh();

    while(getInputRaw()==BTN_NONE){
        work_queue();
    };
    return;
}

/**************************************************************************/

void init_nick(void){
	readFile("nick.cfg",GLOBAL(nickname),MAXNICK);
//	readFile("font.cfg",GLOBAL(nickfont),FILENAMELEN);
};

//# MENU nick editNick
void doNick(void){
	input("Nickname:", GLOBAL(nickname), 32, 127, MAXNICK-1);
	writeFile("nick.cfg",GLOBAL(nickname),strlen(GLOBAL(nickname)));
	getInputWait();
};

//# MENU nick changeFont
void doFont(void){
    getInputWaitRelease();
    if( selectFile(GLOBAL(nickfont),"F0N") != 0){
        lcdPrintln("No file selected.");
        return;
    };

    lcdClear();
    lcdPrintln(GLOBAL(nickfont));
    setExtFont(GLOBAL(nickfont));
    lcdPrintln("PUabc€");
    setIntFont(&Font_7x8);
    lcdPrintln("done.");
    lcdDisplay();
    while(!getInputRaw())delayms(10);
};
