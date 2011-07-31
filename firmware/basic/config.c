#include <sysinit.h>
#include "basic/basic.h"

#include "lcd/print.h"
#include "filesystem/ff.h"
#include "basic/random.h"

#define CFGVER 23

struct CDESC the_config[]= {
    {"version",          CFGVER, CFGVER, CFGVER},
    {"privacy",          3,     0, 2  },
    {"daytrig",          310/2, 0, 255},
    {"daytrighyst",      10,    0, 50 },
    {"dayinvert",        1,     0, 1  },
    {"lcdbacklight",     50,    0, 100},
    {"lcdmirror",        0,     0, 1  },
    {"lcdinvert",        0,     0, 1  },
    {"lcdcontrast",      0,     1, 6  },
    { NULL,              0,     0, 0  },
};

#define CONFFILE "r0ket.cfg"
#define CONF_ITER for(int i=0;the_config[i].name!=NULL;i++)

/**************************************************************************/

void applyConfig(){
    if(GLOBAL(lcdcontrast)>0)
        lcdSetContrast(GLOBAL(lcdcontrast));
	return 0;
};

int saveConfig(void){
    FIL file;            /* File object */
    UINT writebytes;
    UINT allwrite=0;
    int res;

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

