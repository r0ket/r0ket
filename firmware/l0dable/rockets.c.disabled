#include <sysinit.h>
#include "basic/basic.h"
#include "lcd/lcd.h"
#include "usetable.h"

#define O_FIXED 1023
#define SPP 10

static void draw_rakett (int x, int y, int scale, int angle);
static int  my_gray (int x, int y, void *data);
static int  o_sin(int x);
static int  o_cos(int x);
static int frame_no = 0;

#define ANIM(start_val, end_val) \
       (((start_val) * (1000-(t))) / 1000 + ((end_val) * ((t))) /1000)

void ram (void)
{
  char test[512]; /* scratch space */
  o_init (test, sizeof(test));
  int frame_dir = 1;
  int inpt;
  o_set_shader (my_gray, NULL);
  int x;
  int y;
  int angle;
  int scale;
  int velocity = 0;
  frame_no=0;

  while ((inpt = getInputRaw()) != BTN_ENTER)
    {
      int t;

      o_identity (); /* reset tranforms */
      o_set_gray (0);
      o_rectangle (0,0,RESX, RESY); /* fill background with black */
      o_fill ();     /* fill with 50% gray */

      if (frame_no < 800)
        {
          o_set_gray (250);
          o_rectangle (0,RESY-7,RESX, 10); /* fill background with black */
          o_fill ();     /* fill with 50% gray */
        }

      if (frame_no < 100)
        {
          t = frame_no * 1000 / 100;
          x = ANIM(-60 * 10, 100 * 10);
          y = RESY/2 * 10;
          angle = ANIM(500, 300);
          scale = ANIM(1200, 700);
        }
      else if (frame_no < 300)
        {
          t = (frame_no - 100) * 1000 / 200;
          x = ANIM(100 * 10, 10 * RESX/2);
          y = ANIM((RESY/2) * 10, (RESY/2 - 10) * 10);
          angle = ANIM(300, 0);
          scale = ANIM(700, 300);
        }
      else if (frame_no < 600)
        {
          t = (frame_no - 300) * 1000 / 300;
          x = RESX/2 * 10;
          y = ANIM((RESY/2-10) * 10, (RESY-14) * 10);
          angle = 0;
          scale = 300;
        }
      else
        {
          /* flying time */

          if (inpt == BTN_UP)
            {
              velocity ++;
            }
          else if (inpt == BTN_DOWN)
            {
              velocity --;
            }
          else if (inpt == BTN_LEFT)
            {
              angle -= 5;
            }
          else if (inpt == BTN_RIGHT)
            {
              angle += 5;
            }

          if (velocity > 10)
            velocity = 10;
          if (velocity < -10)
            velocity = -10;

          {
            int c = o_cos ((angle - 900) * 4 * 8192 / 3600) / 4;
            int s = o_sin ((angle - 900) * 4 * 8192 / 3600) / 4;
            x += (c / 100) * velocity / 10;
            y += (s / 100) * velocity / 10;
          }

          y++;

          if (y > (RESY-14) * 10)
            y = (RESY-14) * 10;
      }

    draw_rakett (x, y, scale,  angle);
    frame_no += frame_dir;
    lcdDisplay();
  }
}

static int my_gray (int x, int y, void *data)
{
  int value = (int)(data);
  switch (value)
    {
      case 0:  /* 0.0  */
        return                    0;
      case 1:  /* 0.16 */
        return (x%3==0) ? (y %2)? 0:0:
               (x%3==1) ? (y %2)? 0:0:
                          (y %2)? 0:1;
      case 2: /* 0.25 */
        switch (frame_no % 4) {
          case 0:
            return (x%2)    ? (y %2)? 1:0:
                              (y %2)? 0:0;
          case 1:
            return (x%2)    ? (y %2)? 0:1:
                              (y %2)? 0:0;
          case 2:
            return (x%2)    ? (y %2)? 0:0:
                              (y %2)? 0:1;
          case 3:
            return (x%2)    ? (y %2)? 0:0:
                              (y %2)? 1:0;
        }
      case 3: /* 0.33 */
        return (x%3==0) ? (y %2)? 1:0:
               (x%3==1) ? (y %2)? 0:0:
                          (y %2)? 0:1;
      case 4: /* 0.50 */
        if (frame_no %2)
          return (x%2==0) ? (y %2)? 1:0:
                            (y %2)? 0:1;
        else
          return (x%2==0) ? (y %2)? 0:1:
                            (y %2)? 1:0;
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

/* This is a very simple vector drawing of heart of gold encoded in a
 * string (search and replace regexp-fu on an SVG made in inkscape was
 * used to create the strings)
 *
 * to reduce size, all coordinates are encoded as bytes, 'g'ray values
 * are in the range 0-100.
 */
static signed char rakett[] = {
  ' ',
  'm',38,6,
  'c',38,6,36,13,36,15,
  'c',24,22,23,26,21,32,'c',19,41,23,61,23,61,'c',15,73,14,95,17,110,'l',26,109,'c',26,102,26,87,30,83,'c',30,83,30,88,30,95,'c',31,103,31,108,31,108,'l',36,108,'c',36,108,35,98,36,91,'c',37,83,38,80,38,80,'c',41,79,43,80,47,79,'c',56,85,56,89,58,99,'c',58,103,58,108,58,108,'l',68,108,'c',67,89,69,73,54,58,'c',54,58,56,41,53,31,'c',50,21,40,15,40,15,'l',38,6,'z','g',100,'f','g',100,'s',
  ' ',
  'm',33,20,'c',31,20,29,21,27,22,'c',25,24,23,27,22,29,'c',20,35,21,38,21,38,'c',26,38,29,36,34,33,'c',38,31,42,24,34,21,'c',34,21,33,20,33,20,'z','g', 50,'f','.'
};

static const signed char * o_process_op (const signed char *g)
{
  switch (*g++) {

    case ' ': o_path_new (); break;
    /* all of these path commands are directly in integer coordinates */
    case 'm':
      o_move_to  (g[0], g[1]); g += 2;
break;
    case 'l': o_line_to  (g[0], g[1]); g += 2; break;
    case 'c': o_curve_to (g[0], g[1], g[2], g[3], g[4], g[5]); g += 6; break;
    case 'z': o_close (); break;

    case 'g': o_set_gray (g[0]*10); g ++; break;

    case 'f': o_fill (); break;
    case 's': break;
    //case 's': o_stroke (); break;
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
orender (const signed char *g)
{
  for (; g; g = o_process_op (g));
}

void o_rectangle (int x0, int y0, int width, int height)
{
  o_path_new ();
  o_move_to (x0, y0);
  o_line_to (x0 + width, y0);
  o_line_to (x0 + width, y0+height);
  o_line_to (x0, y0+height);
  o_close ();
}

static int o_sin(int x)
{
#define qN 13
#define qA 12
#define qP 15
#define qR (2*qN-qP)
#define qS (qN+qP+1-qA)

    x= x<<(30-qN);          // shift to full s32 range (Q13->Q30)

    if( (x^(x<<1)) < 0)     // test for quadrant 1 or 2
        x= (1<<31) - x;

    x= x>>(30-qN);

    return (x * ( (3<<qP) - (x*x>>qR) ) >> qS );
}

static inline int o_cos(int x)
{
  return o_sin(x + 8192);
}

static void draw_rakett (int x, int y, int scale, int angle)
{
  /* directly including the matrices used to build up the transform, the fudging factors
     depend on internal values of o to add up properly  */
  OMatrix mtranslate = {{{O_FIXED,0},
                         {0,O_FIXED},
                         {x * 100 * SPP * O_FIXED / 1000, y * 100 * SPP * O_FIXED / 1000}}};
  int c = o_cos (angle * 4 * 8192 / 3600) / 4;
  int s = o_sin (angle * 4 * 8192 / 3600) / 4;
  OMatrix mrotate   = {{{c,s},
                        {-s,c},
                        {0, 0}}};
  OMatrix mscale    = {{{scale * O_FIXED / 1000, 0},
                        {0,scale * O_FIXED / 1000},
                        {0,0}}};
  OMatrix mtranslate2 = {{{O_FIXED,0},
                          {0,O_FIXED},
                          {-37000 * SPP * O_FIXED / 1000, -60000 * SPP * O_FIXED / 1000}}};
  o_transform (&mtranslate,  1); /* passing 1 as second arg sets the transform to this */
  o_transform (&mrotate,     0); /* passing 0 adds this transformaiton */
  o_transform (&mscale,      0); /* again (it is multiplying the matrices internally) */
  o_transform (&mtranslate2, 0); /* the final translate (first actually) sets the local origin. */
  orender (rakett);  /* render the rocket data */
}
