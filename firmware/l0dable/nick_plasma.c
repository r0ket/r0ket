/* nick_plasma.c - provided by Grigori Goronzy <greg@geekmind.org> */
/*
    simple plasma with horribly slow code.
    makes a nice nick animation, though.

    gregg/gnp for camp 2011
*/
#include <sysinit.h>
#include "basic/basic.h"
#include "lcd/print.h"
#include "lcd/render.h"
#include "lcd/display.h"

#include "basic/config.h"

#include "usetable.h"

#define INT_MAX 2147483647

#define BLINK

static short sintab[] = {
    0 , 3 , 6 , 9 , 12 , 15 , 18 , 21 , 24 , 28 , 31 , 34 , 37 , 40 , 43 ,
    46 , 48 , 51 , 54 , 57 , 60 , 63 , 65 , 68 , 71 , 73 , 76 , 78 , 81 ,
    83 , 85 , 88 , 90 , 92 , 94 , 96 , 98 , 100 , 102 , 104 , 106 , 108 ,
    109 , 111 , 112 , 114 , 115 , 116 , 118 , 119 , 120 , 121 , 122 ,
    123 , 124 , 124 , 125 , 126 , 126 , 127 , 127 , 127 , 127 , 127 ,
    127 , 127 , 127 , 127 , 127 , 127 , 126 , 126 , 125 , 124 , 124 , 123
        , 122 , 121 , 120 , 119 , 118 , 117 , 115 , 114 , 112 , 111 , 109 ,
    108 , 106 , 104 , 102 , 100 , 99 , 97 , 94 , 92 , 90 , 88 , 86 , 83 ,
    81 , 78 , 76 , 73 , 71 , 68 , 65 , 63 , 60 , 57 , 54 , 52 , 49 , 46 ,
    43 , 40 , 37 , 34 , 31 , 28 , 25 , 22 , 18 , 15 , 12 , 9 , 6 , 3 ,
    0 , -2 , -6 , -9 , -12 , -15 , -18 , -21 , -24 , -27 , -30 , -33 ,
    -36 , -39 , -42 , -45 , -48 , -51 , -54 , -57 , -60 , -62 , -65 ,
    -68 , -70 , -73 , -76 , -78 , -81 , -83 , -85 , -88 , -90 , -92 ,
    -94 , -96 , -98 , -100 , -102 , -104 , -106 , -107 , -109 , -111 ,
    -112 , -114 , -115 , -116 , -118 , -119 , -120 , -121 , -122 , -123 ,
    -124 , -124 , -125 , -126 , -126 , -127 , -127 , -127 , -127 , -127 ,
    -127 , -127 , -127 , -127 , -127 , -127 , -126 , -126 , -125 , -124 ,
    -124 , -123 , -122 , -121 , -120 , -119 , -118 , -117 , -115 , -114 ,
    -113 , -111 , -109 , -108 , -106 , -104 , -103 , -101 , -99 , -97 ,
    -95 , -92 , -90 , -88 , -86 , -83 , -81 , -79 , -76 , -74 , -71 ,
    -68 , -66 , -63 , -60 , -57 , -55 , -52 , -49 , -46 , -43 , -40 ,
    -37 , -34 , -31 , -28 , -25 , -22 , -19 , -16 , -12 , -9 , -6 , -3 ,
};

// 4x4 ordered dithering
// smoother, but actually *too* smooth for our little display
static unsigned char dithertab4[] = {
    1, 9, 3, 11,
    13, 5, 15, 7,
    4, 12, 2, 10,
    16, 8, 14, 6
};

// 2x2 ordered dithering
static unsigned char dithertab2[] = {
    1, 3,
    4, 2
};

// integer square root
static unsigned isqrt(unsigned val)
{
    unsigned temp, g = 0, b = 0x8000, bshft = 15;
    do {
        if (val >= (temp = (((g << 1) + b)<<bshft--))) {
           g += b;
           val -= temp;
        }
    } while (b >>= 1);
    return g;
}


static void plasma(unsigned int t)
{
   int i, j;

   for (j = 0; j < RESY; j++)
       for (i = 0; i < RESX; i++) {
           int sin1 = 128 + sintab[(t+i*4) % 256];
           int cx = RESX/2 + sintab[(INT_MAX-t) % 256] / 4;
           int cy = RESY/2 + sintab[(INT_MAX-t+64) % 256] / 4;
           int dx = cx - i;
           int dy = cy - j;
           int sin2 = 128 + sintab[(isqrt(dx*dx + dy*dy)*8) % 256];
           int sin3 = (sin1 + sin2) / 2;
           unsigned char pix = ((sin3 + 32) / 64) >= dithertab2[(j%2)*2 + (i%2)];
           lcdSetPixel(i, j, pix);
       }
}

void ram(void)
{
    unsigned int t = 0;
    int px, py;

    // set up font and nick position
    setExtFont(GLOBAL(nickfont));
	px = DoString(0, 0, GLOBAL(nickname));
    px = (RESX-px) / 2;
    if (px < 0)
        px = 0;
    py = (RESY - getFontHeight()) / 2;

    // display plasma
    for (;;) {
        plasma(t++);
        DoString(px, py, GLOBAL(nickname));
        lcdRefresh();

#ifdef BLINK
        // blink some LEDs
        if ((t % 30) == 0) {
            gpioSetValue(RB_LED2, 1);
            gpioSetValue(RB_LED0, 1);
        }
        if ((t % 30) == 2) {
            gpioSetValue(RB_LED2, 0);
            gpioSetValue(RB_LED0, 0);
        }
#endif

        char key = getInputRaw();
        if (key == BTN_ENTER)
            return;
    }

}
