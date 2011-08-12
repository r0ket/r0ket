#include <sysinit.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/lcd.h"
#include "lcd/print.h"

#include "usetable.h"

#define NUM_STARS 150

typedef struct {
	short x, y, z, speed;
} s_star;

static s_star stars[NUM_STARS];

void init_star(s_star *star, int z);

void ram(void)
{
	short centerx = RESX >> 1;
	short centery = RESY >> 1;
	short i;

	for (i = 0; i < NUM_STARS; i++) {
		init_star(stars + i, i + 1);
	}

	while(getInputRaw()==BTN_NONE){
		lcdClear();

		for (i = 0; i < NUM_STARS; i++) {
			stars[i].z -= stars[i].speed;

			if (stars[i].z <= 0)
				init_star(stars + i, i + 1);
			
			short tempx = ((stars[i].x * 30) / stars[i].z) + centerx;
			short tempy = ((stars[i].y * 30) / stars[i].z) + centery;

			if (tempx < 0 || tempx > RESX - 1 || tempy < 0 || tempy > RESY - 1) {
				init_star(stars + i, i + 1);
				continue;
			}

			lcdSetPixel(tempx, tempy, 1);
			if (stars[i].z < 50) {
				lcdSetPixel(tempx + 1, tempy, 1);
			}
			if (stars[i].z < 20) {
				lcdSetPixel(tempx, tempy + 1, 1);
				lcdSetPixel(tempx + 1, tempy + 1, 1);
			}
		}

		lcdRefresh();

        delayms_queue_plus(50,0);
	}	
}

void init_star(s_star *star, int z)
{
	star->x = (getRandom() % RESX) - (RESX >> 1);
	star->y = (getRandom() % RESY) - (RESY >> 1);
	star->z = z;
	star->speed = (getRandom() % 4) + 1;

	return;
}

