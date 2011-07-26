#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/print.h"

#include "filesystem/ff.h"

#include <string.h>

/**************************************************************************/

void readcfg(void) {
    readConfig();
};

void savecfg(void){
    saveConfig();
};

void applycfg(void){
    applyConfig();
};

void show(void){
    lcdClear();
    lcdPrint("time:"); lcdPrintInt(globalconfig.time); lcdNl();
    lcdPrint("btrig:"); lcdPrintInt(globalconfig.backlighttrigger); lcdNl();
    lcdPrint("bval:"); lcdPrintInt(globalconfig.backlightvalue); lcdNl();
    lcdPrint("lcd:"); lcdPrintInt(globalconfig.lcdstate); lcdNl();
    lcdPrint("priv:"); lcdPrintInt(globalconfig.privacy); lcdNl();
    lcdRefresh();
};

void lcdmirror(void){
    globalconfig.lcdstate^=2;
};
