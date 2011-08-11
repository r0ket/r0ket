/*
 *  Pizzaware by Thammi from Pentaville
 */

#include <sysinit.h>
#include <stdio.h>

#include "basic/basic.h"
#include "basic/random.h"
#include "lcd/display.h"
#include "usetable.h"

void randomizeCoin(int* x, int* y);

void ram() {
	int key;

	int d = 0;
	int x = RESX / 2;
	int y = RESY / 2;

	int p = 0;
	int q, r;

	randomizeCoin(&q, &r);

	lcdClear();

	while (1) {
		// display
		lcdSetPixel(q, r, p % 3);
		lcdSetPixel(x, y, 1);
		lcdDisplay();

		// wait

		delayms(20);

		// input

		key=getInputRaw();

		switch(key) {
			case BTN_RIGHT:
				d = 3;
				break;
			case BTN_LEFT:
				d = 2;
				break;
			case BTN_UP:
				d = 0;
				break;
			case BTN_DOWN:
				d = 1;
				break;
			case BTN_ENTER:
				return;
		}

		// step

		if(d > 1) {
			x = (RESX + x + (d & 1) * 2 - 1) % RESX;
		} else {
			y = (RESY + y + d * 2 - 1) % RESY;
		}

		p += 1;

		// collision

		if(x == q && y == r) {
			p += 99;
			randomizeCoin(&q, &r);
		} else if(lcdGetPixel(x, y)) {
			lcdClear();
			lcdPrintln("You lost!");
			lcdNl();
			lcdPrintInt(p);
			lcdPrintln(" points");
			lcdDisplay();

			while(getInputRaw() != BTN_ENTER);

			return;
		}
	}
	return;
}

void randomizeCoin(int* x, int* y) {
	// WARNING: if you are really got at this the game freezes :D
	for(;;) {
		*x = getRandom() % RESX;
		*y = getRandom() % RESY;

		if(!lcdGetPixel(*x, *y)) {
			return;
		}
	}
}

