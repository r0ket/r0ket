#include <sysinit.h>
#include "basic/basic.h"

#include "lcd/lcd.h"
#include "lcd/fonts/smallfonts.h"
#include "lcd/print.h"
#include "filesystem/ff.h"
#include "basic/random.h"

CONFIG globalconfig = { 0,310,50,0,0};

#define CONFFILE "r0ket.cfg"

/**************************************************************************/

int saveConfig(void){
    FIL file;            /* File object */
    UINT writebytes;
    int res;

	res=f_open(&file, CONFFILE, FA_OPEN_ALWAYS|FA_WRITE);
	lcdPrint("create:");
	lcdPrintln(f_get_rc_string(res));
	if(res){
		return 1;
	};

	res = f_write(&file, &globalconfig, sizeof(CONFIG), &writebytes);
	lcdPrint("write:");
	lcdPrintln(f_get_rc_string(res));
	if(res){
		return 1;
	};

	lcdPrint("wrote:");
	lcdPrintInt(writebytes);
	lcdPrintln("b");

	res=f_close(&file);
	lcdPrint("close:");
	lcdPrintln(f_get_rc_string(res));
	if(res){
		return 1;
	};
	return 0;
};

int readConfig(void){
    FIL file;            /* File object */
    UINT readbytes;
    int res;

    lcdFill(0); // clear display buffer

    res=f_open(&file, CONFFILE, FA_OPEN_EXISTING|FA_READ);
    lcdPrint("open:");
    lcdPrintln(f_get_rc_string(res));
    if(res){
        return 1;
    };

    res = f_read(&file, &globalconfig, sizeof(CONFIG), &readbytes);
    lcdPrint("read:");
    lcdPrintln(f_get_rc_string(res));
    if(res){
        return 1;
    };


    res=f_close(&file);
    lcdPrint("close:");
    lcdPrintln(f_get_rc_string(res));
    if(res){
        return 1;
    };
    return 0;
};

int applyConfig(){
	return 0;
};
