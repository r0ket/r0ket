#include <sysinit.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/print.h"
#include "usetable.h"

static void set_privacy();
static void privacy0(); 
static void privacy1(); 
static void privacy2(); 

static const char levels[][12] = {"0-trackable","1-mesh only","2-RF OFF"};

static const struct MENU submenu_privacy={ "Privacy?", {
	{ levels[0], &privacy0},
	{ levels[1], &privacy1},
	{ levels[2], &privacy2},
	{NULL,NULL}
}};

void ram(void){
	// check privacy
	menuflags|=MENU_JUSTONCE;
    handleMenu(&submenu_privacy);
	menuflags&= (~MENU_JUSTONCE);
};

static void privacy0() {
	set_privacy(0);
}

static void privacy1() {
	set_privacy(1);
}

static void privacy2() {
	set_privacy(2);
}

static void set_privacy(int level) {
	lcdClear();
	lcdPrintln("Privacy:");
	lcdPrintln(levels[level]);
	lcdRefresh();
	getInput();
	GLOBAL(privacy) = level;
}

