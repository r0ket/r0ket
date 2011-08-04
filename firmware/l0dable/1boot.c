#include <sysinit.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/print.h"
#include "usetable.h"

static void screen_intro();
static void set_privacy(int level);
static void privacy0(); 
static void privacy1(); 
static void privacy2(); 
static bool screen_overview();
static const char levels[][12] = {"0-trackable","1-mesh only","2-RF OFF"};
bool privacy_set;
static const struct MENU submenu_privacy={ "Privacy?", {
	{ levels[0], &privacy0},
	{ levels[1], &privacy1},
	{ levels[2], &privacy2},
	{NULL,NULL}
}};



void ram(void){
	bool again = true;
	menuflags|=MENU_JUSTONCE;
	screen_intro();
	while (again) {
		privacy_set = false;
		while (!privacy_set) {	
			handleMenu(&submenu_privacy);
		}
    	input("Nickname:", GLOBAL(nickname), 32, 127, MAXNICK-1);
		getInputWaitRelease();
		again = screen_overview();
	}
	menuflags&= (~MENU_JUSTONCE);
	writeFile("nick.cfg",GLOBAL(nickname),strlen(GLOBAL(nickname)));
	saveConfig();
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
	GLOBAL(privacy) = level;
	privacy_set = true;
}

static void screen_intro() {
	lcdClear();
	lcdPrintln("Welcome to");
	lcdPrintln("r0ket");
	lcdRefresh();
	getInputWait();
	getInputWaitRelease();
}

static bool screen_overview() {
	char key = 0;
	while (key != BTN_ENTER) {
		lcdClear();
		lcdPrintln("Privacy:");
		lcdPrintln(levels[GLOBAL(privacy)]);
		lcdPrintln("");
		lcdPrintln("Nickname:");
		lcdPrintln(GLOBAL(nickname));
		lcdPrintln("");
		lcdPrintln("LEFT: cancel");
		lcdPrintln("ENTER: OK");
		lcdRefresh();
		key = getInputWait();
		if (key == BTN_LEFT) {
			//getInputWaitRelease();
			return true;
		}
	}

	return false;
}

