#include <sysinit.h>

#include "filesystem/ff.h"

/**************************************************************************/

//# MENU config
void changer(void) {
    // now external to save space here
    execute_file("config.int");
}
