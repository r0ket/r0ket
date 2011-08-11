//  tedliz
//  ======
//	a kick ass tetris clone
//	code by twobit (c3d2 at pentagon ville)
////////////////////////////////////////////////////////////////////////////////

#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"
#include "basic/random.h"
#include "lcd/render.h"
#include "lcd/display.h"
#include "lcd/fonts.h"
#include "lcd/fonts/invaders.h"
#include "funk/mesh.h"
#include "usetable.h"

////////////////////////////////////////////////////////////////////////////////

enum {
	GRID_WIDTH	= 10,
	GRID_HEIGHT	= 20,
	TICK_LENGTH	= 30,
	KEY_REPEAT_DELEAY = 5,
	STARTING_SPEED = 12
};

// key repeat delay stuff
int key_rep[] = { 0, 0, 0, 0, 0 };

int ticks_per_drop = STARTING_SPEED;

int lines = 0;
int ticks = 0;

int rot;
int stone;
int next_rot;
int next_stone;
int x_pos;
int y_pos;

uint8_t grid[GRID_HEIGHT][GRID_WIDTH];

const uint8_t stones[7][16] = {
	{ 0, 0, 0, 0, 0,15,15, 0, 0,15,15, 0, 0, 0, 0, 0 },
	{ 0, 0,10, 0, 5, 5,15, 5, 0, 0,10, 0, 0, 0,10, 0 },
	{ 0, 0, 5, 0, 0, 0,15,15, 0,10,10, 5, 0, 0, 0, 0 },
	{ 0, 0, 0, 5, 0,10,15, 5, 0, 0,15,10, 0, 0, 0, 0 },
	{ 0, 4, 5, 8, 0,10,15,10, 0, 2, 5, 1, 0, 0, 0, 0 },
	{ 0, 8, 5, 1, 0,10,15,10, 0, 4, 5, 2, 0, 0, 0, 0 },
	{ 0, 0,11, 0, 0,13,15, 7, 0, 0,14, 0, 0, 0, 0, 0 }
};

void draw_grid();
void new_stone();
int collision(int top_also);

void ram() {

	int raw_key, key;
	int i, j;

	new_stone();
	new_stone();

	// epmty grid
	for(i = 0; i < GRID_HEIGHT; i++)
		for(j = 0; j < GRID_WIDTH; j++)
			grid[i][j] = 0;


	// prepare screen
	lcdClear();
	for(i = 0; i < 96; i++) {
		lcdSetPixel(i, 2, 1);
		lcdSetPixel(i, 65, 1);
	}
	for(i = 3; i < 65; i++) {
		lcdSetPixel(2, i, 1);
		lcdSetPixel(35, i, 1);
		lcdSetPixel(93, i, 1);
	}

	for(;;) {

		// handle input
		raw_key = getInputRaw();
		key = 0;
		for(i = 0; i < 5; i++) {
			if(raw_key & (1 << i)) {
				if(!key_rep[i] || key_rep[i] == KEY_REPEAT_DELEAY) key |= 1 << i;
				key_rep[i] += key_rep[i] < KEY_REPEAT_DELEAY;
			}
			else key_rep[i] = 0;
		}


		// rotate
		if(key & BTN_UP) {
			i = rot;
			rot = (rot << 1) % 15;
			if(collision(0)) rot = i;
		}

		// horizontal movement
		i = x_pos;
		x_pos += !!(key & BTN_RIGHT) - !!(key & BTN_LEFT);
		if(i != x_pos && collision(0)) x_pos = i;

		// vertical movement
		ticks++;
		if(key & BTN_DOWN || ticks >= ticks_per_drop) {
			ticks = 0;
			y_pos++;
			if(collision(0)) {
				y_pos--;

				// check for game over
				if(collision(1)) return;

				// copy stone to grid
				int x, y;
				for(y = 0; y < 4; y++) {
					for(x = 0; x < 4; x++) {
						if(stones[stone][(x << 2) + y] & rot)
							grid[y + y_pos][x + x_pos] = 1;
					}
				}

				// check for complete lines
				for(y = 0; y < GRID_HEIGHT; y++) {
					for(x = 0; x < GRID_WIDTH; x++)
						if(!grid[y][x]) break;

					if(x == GRID_WIDTH) {

						lines++;
						ticks_per_drop = STARTING_SPEED - lines / 10;
						if(ticks_per_drop < 1) ticks_per_drop = 1;

						for(i = y; i > 0; i--)
							for(x = 0; x < GRID_WIDTH; x++)
								grid[i][x] = grid[i - 1][x];
					}
				}

				// get a new stone
				new_stone();
			}
		}

		draw_grid();
        lcdDisplay();
		delayms(TICK_LENGTH);
	}
	return;
}

int collision(int top_also) {
	int x, y;
	for(y = 0; y < 4; y++) {
		for(x = 0; x < 4; x++) {
			if(stones[stone][(x << 2) + y] & rot && (
				x + x_pos < 0 || x + x_pos >= GRID_WIDTH ||
				(top_also && y + y_pos < 0) || y + y_pos >= GRID_HEIGHT ||
				grid[y + y_pos][x + x_pos]))
				return 1;	
		}
	}
	return 0;
}

void new_stone() {
	x_pos = 3;
	y_pos = -3;

	rot = next_rot;
	stone = next_stone;

	next_rot = 1 << getRandom() % 4;
	next_stone = getRandom() % 7;
}


void draw_block(int x, int y, int b) {
	lcdSetPixel(0 + x, 0 + y, b);
	lcdSetPixel(1 + x, 0 + y, b);
	lcdSetPixel(2 + x, 0 + y, b);
	lcdSetPixel(0 + x, 1 + y, b);
	lcdSetPixel(1 + x, 1 + y, b);
	lcdSetPixel(2 + x, 1 + y, b);
	lcdSetPixel(0 + x, 2 + y, b);
	lcdSetPixel(1 + x, 2 + y, b);
	lcdSetPixel(2 + x, 2 + y, b);
}

void draw_grid() {
	int b, y, x;

	for(y = 0; y < GRID_HEIGHT; y++) {
		for(x = 0; x < GRID_WIDTH; x++) {

			b = grid[y][x] ||
				(x >= x_pos && x < x_pos + 4 && y >= y_pos && y < y_pos + 4 &&
				stones[stone][((x - x_pos) << 2) + (y - y_pos)] & rot);

			draw_block(4 + x * 3, 4 + y * 3, b);
		}
	}

	// look adhead
	for(y = 0; y < 4; y++)
		for(x = 0; x < 4; x++)
			draw_block(40 + x * 3, 7 + y * 3, !!(stones[next_stone][(x << 2) + y] & next_rot));

}
