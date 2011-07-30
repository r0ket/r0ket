#include <sysinit.h>

#include "basic/basic.h"
#include "lcd/render.h"
#include "lcd/display.h"
#include "lcd/allfonts.h"

#define CHARWIDTH 12
#define CHARSPACE 0x20

void main_inputtest(void) {
    backlightInit();
	
	char nick[20];
	memset(nick,0,20);
	strcpy(nick,"iggy");
   
	while(1) {
	   input("Nick?", nick, 0x20, 0x7f, 20);
	   
	   lcdFill(0);
	   font=&Font_Orbitron14pt;
	   DoString(20,20,nick);
	   lcdDisplay();
	   
	   while(getInput() != BTN_ENTER){}
	}
}

