#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "usetable.h"

/**************************************************************************/

void ram(void) {
    lcdClear();
    lcdPrintln("r0ket");
    lcdPrintln("Release: ");
    lcdPrintln(IntToStr(getrelease(),4,F_HEX));
    lcdRefresh();
    while(!getInputRaw())work_queue();
}
