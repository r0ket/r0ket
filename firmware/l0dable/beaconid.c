#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"
#include "usetable.h"

/**************************************************************************/

void ram(void) {
    lcdClear();
    lcdPrintln("OpenBeaconId:");
    lcdPrintIntHex(GetUUID32());
    lcdRefresh();
    do{
    }while ((getInputRaw())==BTN_NONE);
}
