#include <sysinit.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/lcd.h"
#include "lcd/print.h"

#include "usetable.h"

void ram(void) {
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
