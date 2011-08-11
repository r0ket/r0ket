#include <sysinit.h>
#include "basic/basic.h"

#include "lcd/display.h"
#include "lcd/print.h"
#include "filesystem/ff.h"
#include "basic/random.h"
#include "basic/config.h"

#define CFGVER 3

struct CDESC the_config[]= {
    {"version",          CFGVER, CFGVER, CFGVER, 0, 0},
    //                   dflt  min max
    {"privacy",          3,     0, 2  , 0, 0},
    {"daytrig",          310/2, 0, 255, 0, 0},
    {"daytrighyst",      10,    0, 50 , 0, 0},
    {"dayinvert",        1,     0, 1  , 0, 0},
    {"lcdbacklight",     30,    0, 100, 0, 0},
    {"lcdmirror",        0,     0, 1  , 0, 0},
    {"lcdinvert",        0,     0, 1  , 0, 0},
    {"lcdcontrast",      14,    0, 31 , 0, 0},
    {"alivechk",         0,     0, 2  , 1, CFG_TYPE_DEVEL},
    {"develmode",        0,     0, 1  , 1, CFG_TYPE_DEVEL},
    {"flamemax",         255,   0, 255, 1, CFG_TYPE_FLAME},
    {"flamemin",         0,     0, 255, 1, CFG_TYPE_FLAME},
    {"flamespeed",       1,     1, 100, 1, CFG_TYPE_FLAME},
    {"flamemaxw",        255,   1, 255, 1, CFG_TYPE_FLAME},
    {"flameminw",        0x8f,  1, 255, 1, CFG_TYPE_FLAME},
    {"l0nick",           0,     0, 1  , 0, 0},
    {"chargeled",        0,     0, 1  , 0, CFG_TYPE_GONE},
    {"positionleds",     0,     0, 1  , 0, 0},
    { NULL,              0,     0, 0  , 0, 0},
};

char nickname[MAXNICK]="anonymous";
char nickfont[FILENAMELEN];
char nickl0[FILENAMELEN];

#define CONFFILE "r0ket.cfg"
#define CONF_ITER for(int i=0;the_config[i].name!=NULL;i++)

/**************************************************************************/

void applyConfig(){
    if(GLOBAL(lcdcontrast)>0)
        lcdSetContrast(GLOBAL(lcdcontrast));
    if(GLOBAL(develmode))
        enableConfig(CFG_TYPE_DEVEL,1);
    if(isNight())
        backlightSetBrightness(GLOBAL(lcdbacklight));
};

int saveConfig(void){
    FIL file;            /* File object */
    UINT writebytes;
    UINT allwrite=0;
    int res;

    lcdClear();

	res=f_open(&file, CONFFILE, FA_OPEN_ALWAYS|FA_WRITE);
	lcdPrint("create:");
	lcdPrintln(f_get_rc_string(res));
	if(res){
		return 1;
	};

    CONF_ITER{
        res = f_write(&file, &the_config[i].value, sizeof(uint8_t), &writebytes);
        allwrite+=writebytes;
        if(res){
            lcdPrint("write:");
            lcdPrintln(f_get_rc_string(res));
            return 1;
        };
    };
	lcdPrint("write:");
	lcdPrintln(f_get_rc_string(res));
	lcdPrint(" (");
	lcdPrintInt(allwrite);
	lcdPrintln("b)");

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
    UINT allread;
    int res;

    res=f_open(&file, CONFFILE, FA_OPEN_EXISTING|FA_READ);
    if(res){
        return 1;
    };

    CONF_ITER{
        res = f_read(&file, &the_config[i].value, sizeof(uint8_t), &readbytes);
        allread+=readbytes;
        if(GLOBAL(version) != CFGVER){
            GLOBAL(version) =CFGVER;
            return 1;
        };
        if(res || GLOBAL(version) != CFGVER)
            return 1;
    };


    res=f_close(&file);
    if(res){
        return 1;
    };

    applyConfig();
    return 0;
};

void enableConfig(char type,char enable){
    CONF_ITER{
        if(the_config[i].type == type){
            the_config[i].disabled=!enable;
        }
    }
}
