#include <sysinit.h>
#include "basic/basic.h"
#include "lcd/lcd.h"

static void draw_rakett (int x, int y, int angle);

void main_rockets (void) {
    char test[512]; /* scratch space */
    o_init (test, sizeof(test));
    int frame_no;
    while (1) {
      o_identity (); /* reset transforms */
      o_rectangle (0,0,RESX, RESY);
      o_set_gray (500);
      o_fill ();     /* fill with 50% gray */

      draw_rakett (20, 20, 10*frame_no++);
      draw_rakett (50, 40, 450+14*frame_no++);
      lcdDisplay();
      delayms(1);
    }
}

signed char rakett[] = {
  ' ',
  'm',38,6,
  'c',38,6,36,13,36,15,
  'c',24,22,23,26,21,32,'c',19,41,23,61,23,61,'c',15,73,14,95,17,110,'l',26,109,'c',26,102,26,87,30,83,'c',30,83,30,88,30,95,'c',31,103,31,108,31,108,'l',36,108,'c',36,108,35,98,36,91,'c',37,83,38,80,38,80,'c',41,79,43,80,47,79,'c',56,85,56,89,58,99,'c',58,103,58,108,58,108,'l',68,108,'c',67,89,69,73,54,58,'c',54,58,56,41,53,31,'c',50,21,40,15,40,15,'l',38,6,'z','g',0,'f','g',100,'s',
  ' ',
  'm',33,20,'c',31,20,29,21,27,22,'c',25,24,23,27,22,29,'c',20,35,21,38,21,38,'c',26,38,29,36,34,33,'c',38,31,42,24,34,21,'c',34,21,33,20,33,20,'z','g',100,'f','\0'
};

static void draw_rakett (int x, int y, int angle)
{
  //o_save ();
  o_identity (); /* reset transform stack */
  o_translate(x * 1000, y * 1000);
  o_rotate (angle);
  o_scale (500, 500);
  o_translate (-37000, -60000);
  o_render (rakett);  /* render a rocket */
  //o_restore ();
}

