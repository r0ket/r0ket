#include <string.h>

#include "basic/basic.h"
#include "lcd/render.h"
#include "lcd/display.h"
#include "lcd/print.h"

#define CHARWIDTH 12
#define CHARSPACE 0x20

struct in{
    char *line, *prompt;
    uint8_t pos, dcursor, maxlength, asciistart, asciiend;
	bool done;
} s_input;

static void inputInit(char p[],char s[], uint8_t l, uint8_t as, uint8_t ae) {
	//TODO: Check length!
	s_input.prompt = p;
	s_input.line = s;
	s_input.maxlength = l;
	s_input.asciistart = as;
	s_input.asciiend = ae;
	s_input.pos = 0;
	s_input.dcursor = 0;
	s_input.done = false;
    s[l-1]=0;
    for(int i=strlen(s);i<(l-1);i++)
        s[i]=0;
}


static void inputMove() {
	char *cur = s_input.line+s_input.pos+s_input.dcursor;
    switch(getInputWaitRepeat()){
        case BTN_LEFT:
            if (s_input.dcursor >0) {
                s_input.dcursor --;
            } else if (s_input.pos > 0) {
                s_input.pos --;
            }
            break;
        case BTN_RIGHT:
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
            break;
        case BTN_UP:
            if (*cur <= s_input.asciistart) {
                *cur = s_input.asciiend;
            } else if (*cur > s_input.asciiend) {
                *cur = s_input.asciiend;
            } else  {
                *cur = *cur - 1;
            }
            break;
        case BTN_DOWN:
            if (*cur >= s_input.asciiend) {
                *cur = s_input.asciistart;
            } else if (*cur < s_input.asciistart) {
                *cur = s_input.asciistart;
            } else {
                *cur = *cur + 1;
            }
            break;
        case BTN_ENTER:
            s_input.done = true;
//            getInputWaitRelease();
            break;
    }
}

static void inputDraw() {
    char tmp[2]= {0,0};
	lcdClear();
	DoString(0,0,s_input.prompt);
	for (int dx = 0; dx<= RESX/CHARWIDTH && s_input.pos+dx<s_input.maxlength; dx++){
		tmp[0] = s_input.line[s_input.pos+dx];
		DoString(dx*CHARWIDTH, 30,tmp);
	}
	DoString(s_input.dcursor * CHARWIDTH, 40, "-");
}

static void inputClean() {
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
 	setSystemFont();
	inputInit(prompt, line, maxlength, asciistart, asciiend);
	while (!s_input.done) {
        inputDraw();
        lcdDisplay();
        inputMove();
    }
	inputClean();
	return;
}

