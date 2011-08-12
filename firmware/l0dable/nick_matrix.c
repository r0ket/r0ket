#include <sysinit.h>

#include "basic/basic.h"
#include "lcd/render.h"

#include "usetable.h"

#define SCREEN_WIDTH 12
#define SCREEN_HEIGHT 8
#define INVALID_POS 127

#define FONT_WIDTH 7
#define FONT_HEIGHT 8

#define MATRIX_LINES_LENGTH 15

#define MAX_LINE_LENGTH 6

void ram(void){
	lcdClear();

	struct matrix_line {
		int head_x, head_y;
		int cur_length;
		int length;
	} matrix_lines[MATRIX_LINES_LENGTH];

	for (int i = 0; i < MATRIX_LINES_LENGTH; i++) {
		matrix_lines[i].cur_length = -1;
	}

	while (1) {
		for(int i = 0; i<MATRIX_LINES_LENGTH; i++) {
			struct matrix_line *ml = matrix_lines + i;
			// new tail
			if (ml->cur_length == -1) {
				ml->head_x = getRandom() % SCREEN_WIDTH;
				ml->head_y = getRandom() % SCREEN_HEIGHT - 1;
				ml->length = getRandom() % MAX_LINE_LENGTH + 3;
				ml->cur_length = 0;
			}
			// new char
			if (ml->head_y < SCREEN_HEIGHT-1) {
				ml->head_y++;
				int chr = getRandom() % 95 + 33;
				DoChar(ml->head_x * FONT_WIDTH, ml->head_y * FONT_HEIGHT, chr);
				ml->cur_length++;
			}
			// remove chars (when length or bottom is reached)
			if (ml->cur_length > ml->length || ml->head_y >= SCREEN_HEIGHT-1) {
				DoChar(ml->head_x * FONT_WIDTH, (ml->head_y - ml->cur_length) * FONT_HEIGHT, ' ');
				ml->cur_length--;
			}
		}
		lcdDisplay();
		// Exit on enter+left
		int key = getInputRaw();
		if(key!= BTN_NONE)
			return;
        delayms_queue_plus(90,0);
	};
};
