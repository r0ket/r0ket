
#include "basic/basic.h"
#include "lcd/render.h"
#include "lcd/display.h"
#include "lcd/allfonts.h"

#define CHARWIDTH 12
#define CHARSPACE 0x20

struct in{
    char *line, *prompt;
    uint8_t pos, dcursor, maxlength, asciistart, asciiend;
	bool done;
} s_input;

void inputInit(char p[],char s[], uint8_t l, uint8_t as, uint8_t ae) {
	//TODO: Check length!
	s_input.prompt = p;
	s_input.line = s;
	s_input.maxlength = l;
	s_input.asciistart = as;
	s_input.asciiend = ae;
	s_input.pos = 0;
	s_input.dcursor = 0;
	s_input.done = false;
}


void inputMove() {
	char *cur = s_input.line+s_input.pos+s_input.dcursor;
	char key = getInput();
	 if (key == BTN_LEFT) {
		if (s_input.dcursor >0) {
			s_input.dcursor --;
		} else if (s_input.pos > 0) {
			s_input.pos --;
		}
    } else if (key == BTN_RIGHT) {
    	if (s_input.dcursor <RESX/CHARWIDTH-1 && s_input.pos + s_input.dcursor < s_input.maxlength) {
			if (*cur == 0) {
				*cur = CHARSPACE;
			}
			s_input.dcursor ++;
		} else if (s_input.pos + RESX/CHARWIDTH < s_input.maxlength) {
			s_input.pos++;
	 		if (*cur == 0) {
				*cur = CHARSPACE;
			}
		}			
	} else if (key == BTN_DOWN) {
	    if (*cur <= s_input.asciistart) {
			*cur = s_input.asciiend;
		} else if (*cur > s_input.asciiend) {
			*cur = s_input.asciiend;
		} else  {
			*cur = *cur - 1;
		}
	} else if (key == BTN_UP) {
		if (*cur >= s_input.asciiend) {
		   *cur = s_input.asciistart;
		} else if (*cur < s_input.asciistart) {
		   *cur = s_input.asciistart;
		} else {
			*cur = *cur + 1;
		}
	} else if (key == (BTN_ENTER)) {
		s_input.done = true;
	}
}

void inputDraw() {
	lcdFill(0);
	DoString(0,0,s_input.prompt);
	for (int dx = 0; dx<= RESX/CHARWIDTH && s_input.pos+dx<s_input.maxlength; dx++){
		char tmp[1];
		tmp[0] = s_input.line[s_input.pos+dx];
		DoString(dx*CHARWIDTH, 30,tmp);
	}
	DoString(s_input.dcursor * CHARWIDTH, 40, "-");
}

void inputClean() {
	for (int x=0;x<=s_input.maxlength;x++) {
		if (s_input.line[x] == 0) {
			x--;
			while (s_input.line[x] == CHARSPACE && x>=0) {
				s_input.line[x] = 0;
				x--;
			}
			return;
		}
	}
}

void input(char prompt[], char line[], uint8_t asciistart, uint8_t asciiend, uint8_t maxlength){
 	font=&Font_7x8;	
	inputInit(prompt, line, maxlength, asciistart, asciiend);
    lcdFill(0);
	while (!s_input.done) {
        lcdDisplay();
        inputMove();
        inputDraw();
    }
	inputClean();
	return;
}

