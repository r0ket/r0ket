#include <sysinit.h>
#include "basic/basic.h"
#include "lcd/lcd.h"
#include "usetable.h"

#define SPP 10

static void draw_rakett (int x, int y, int scale, int angle);

void ram (void) {
    char test[384]; /* scratch space */
    o_init (test, sizeof(test));
    int frame_no = 0;
    int frame_dir = 1;
    while (1) {
      o_rectangle (0,0,RESX, RESY);
      o_set_gray (1000);
      o_fill ();     /* fill with 50% gray */

      draw_rakett (40, 20, frame_no / 3,  150*frame_no);
      draw_rakett (20, 20, frame_no,  10*frame_no);
      draw_rakett (80, 30, 1000-frame_no,  450+14*frame_no);
      frame_no += frame_dir;
      lcdDisplay();
      delayms(2);

      if (frame_no > 1000 || frame_no < 0)
        frame_dir *= -1;
    }
}

static signed char rakett[] = {
  ' ',
  'm',38,6,
  'c',38,6,36,13,36,15,
  'c',24,22,23,26,21,32,'c',19,41,23,61,23,61,'c',15,73,14,95,17,110,'l',26,109,'c',26,102,26,87,30,83,'c',30,83,30,88,30,95,'c',31,103,31,108,31,108,'l',36,108,'c',36,108,35,98,36,91,'c',37,83,38,80,38,80,'c',41,79,43,80,47,79,'c',56,85,56,89,58,99,'c',58,103,58,108,58,108,'l',68,108,'c',67,89,69,73,54,58,'c',54,58,56,41,53,31,'c',50,21,40,15,40,15,'l',38,6,'z','g',0,'f',
  ' ',
  'm',33,20,'c',31,20,29,21,27,22,'c',25,24,23,27,22,29,'c',20,35,21,38,21,38,'c',26,38,29,36,34,33,'c',38,31,42,24,34,21,'c',34,21,33,20,33,20,'z','g', 50,'f','.'
};

#define GRAY_PRECISION     4
/* Default color generator shader */
static int shader_gray (int x, int y, void *data)
{
  int value = (int)(data); /* stored gray value as 8bit value */
  switch (value)           /* the dither patterns are can be seen */
    {                      /* at the end of the lines */
      case 0:  /* 0.0  */
        return                    0;
      case 1:  /* 0.16 */
        return (x%3==0) ? (y %2)? 0:0:
               (x%3==1) ? (y %2)? 0:0:
                          (y %2)? 0:1;
      case 2: /* 0.25 */
        return (x%2)    ? (y %2)? 1:0:
                          (y %2)? 0:0;
      case 3: /* 0.33 */
        return (x%3==0) ? (y %2)? 1:0:
               (x%3==1) ? (y %2)? 0:0:
                          (y %2)? 0:1;
      case 4: /* 0.50 */
        return (x%2==0) ? (y %2)? 1:0:
                          (y %2)? 0:1;
      case 5: /* 0.66 */
        return (x%3==0) ? (y %2)? 0:1:
               (x%3==1) ? (y %2)? 1:1:
                          (y %2)? 1:0;
      case 6: /* 0.75 */
        return (x%2)    ? (y %2)? 1:0:
                          (y %2)? 1:1;
      case 7: /* 0.85 */
        return (x%3==0) ? (y %2)? 1:1:
               (x%3==1) ? (y %2)? 1:0:
                          (y %2)? 1:1;
      case 8: /* 1.0 */
        return                    1;
      default: //    return ((char)(rnd1())) < value;
        /* XXX: use a faster "random" source
                for this fallback */
        break;
    }
  return 0;
}

static const signed char * o_process_op (const signed char *g,
                                         int scale,
                                         int x,
                                         int y)
{
  scale *= SPP;
  switch (*g++) {

    case ' ': o_path_new (); break;
    /* all of these path commands are directly in integer coordinates */
    case 'm':
      o_move_to  (g[0]*scale/1000+x, g[1]*scale/1000+y); g += 2;
break;
    case 'l': o_line_to  (g[0]*scale/1000+x, g[1]*scale/1000+y); g += 2; break;
    case 'c': o_curve_to (g[0]*scale/1000+x, g[1]*scale/1000+y, g[2]*scale/1000+x, g[3]*scale/1000+y, g[4]*scale/1000+x, g[5]*scale/1000+y); g += 6; break;
    case 'z': o_close (); break;

    case 'g': o_set_gray (g[0]*10); g ++; break;

    case 'f': o_fill (); break;
              /* 1 = 1 10 = 10 100 = 100 */
#if 0
    case '!': o_identity (); break;
    case 'T': o_translate (g[0] * 100, g[1] * 100); g+=2; break;
              /* 1 = 0.01 10 = 0.1 50 = 0.5 100 = 10x */
    case 'S': o_scale (g[0] * 10, g[1] * 10); g+=2; break;
              /* -128 = -360  64 = 180 128 = 360 */
    case 'R': o_rotate ((g[0] * 3600)/127); g+=1; break;
#endif

    default:
    case '\0':
    case '.':  /* end */
      return NULL;
    }
  return g;
}

static void
orender (const signed char *g, int scale, int x, int y)
{
  o_set_shader (shader_gray, NULL);
  for (; g; g = o_process_op (g, scale, x, y));
}


void o_rectangle (int x0, int y0, int width, int height)
{
  o_path_new ();
  x0*=SPP;
  y0*=SPP;
  width*=SPP;
  height*=SPP;
  o_move_to (x0, y0);
  o_line_to (x0 + width, y0);
  o_line_to (x0 + width, y0+height);
  o_line_to (x0, y0+height);
  o_close ();
}

void draw_rakett (int x, int y, int scale, int angle)
{
  //o_save ();
  //o_identity (); /* reset transform stack */
  //o_translate(x * 1000, y * 1000);
  //o_rotate (angle);
  //o_scale (scale, scale);
  //o_translate (-37000, -60000);
  orender (rakett, scale, x, y);  /* render a rocket */
  //o_restore ();
}
