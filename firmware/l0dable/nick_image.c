#include <sysinit.h>

#include "basic/basic.h"
#include "lcd/lcd.h"
#include "filesystem/ff.h"

#include "usetable.h"

void ram(void) {
    lcdLoadImage("nick.lcd");
    lcdRefresh();
    getInputWait();
}
