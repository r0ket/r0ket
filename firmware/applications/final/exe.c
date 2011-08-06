#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "lcd/lcd.h"
#include "lcd/allfonts.h"
#include "lcd/print.h"
#include "usb/usbmsc.h"
#include "filesystem/ff.h"
#include "filesystem/select.h"
#include "filesystem/execute.h"

/**************************************************************************/

static void execute_it (const char *type)
{
  while(getInput()!=BTN_NONE);
  executeSelect(type);
  lcdRefresh();
}

//# MENU execute
void execute_menu(void){
  execute_it ("C0D");
};

//# MENU |devel
void execute_devel(void){
  execute_it ("INT");
};
