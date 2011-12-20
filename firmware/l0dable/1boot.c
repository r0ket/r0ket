#include <sysinit.h>

#include "basic/basic.h"
#include "basic/config.h"
#include "filesystem/ff.h"

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

static void yellow();
static void green();
static const char colors[][12] = {"0-yellow","1-green"};
bool color_set;
static const struct MENU submenu_color={ "r0ket color?", {
	{ colors[0], &yellow},
	{ colors[1], &green},
	{NULL,NULL}
}};


void ram(void){
	bool again = true;
    FIL file;
	menuflags|=(MENU_JUSTONCE|MENU_BIG);
	screen_intro();
	while (again) {
        color_set = false;

        if( f_open(&file, "yell0w", FA_OPEN_EXISTING|FA_READ) == 0 ){
            yellow();
            color_set = true;
        }

        if( f_open(&file, "green", FA_OPEN_EXISTING|FA_READ) == 0 ){
            yellow();
            color_set = true;
        }

		while (!color_set) {	
			handleMenu(&submenu_color);
		}

		privacy_set = false;
		while (!privacy_set) {	
			handleMenu(&submenu_privacy);
		}

    	input("Nickname?", GLOBAL(nickname), 32, 127, MAXNICK-1);
		getInputWaitRelease();
		again = screen_overview();
	}
	menuflags&= (~(MENU_JUSTONCE|MENU_BIG));
	writeFile("nick.cfg",GLOBAL(nickname),strlen(GLOBAL(nickname)));
	saveConfig();
};

static void green() {
	GLOBAL(daytrig) = 155;
    GLOBAL(daytrighyst) = 10;
    color_set = true;
}

static void yellow() {
	GLOBAL(daytrig) = 160;
    GLOBAL(daytrighyst) = 15;
    color_set = true;
}

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
	lcdPrintln("");
	lcdPrintln("    r0ket");
	lcdPrintln("");
	lcdPrintln("launch config");
	lcdPrintln("");
	lcdPrintln("press any key");
	lcdPrintln("to continue");
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

