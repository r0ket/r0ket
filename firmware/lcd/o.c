/*
 *   o - a project called circle - that draws stuff
 *
 * Copyright (c) 2011 Øyvind Kolås <pippin@gimp.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <string.h>
#include <display.h>

#ifdef SIMULATOR
#include <stdio.h>
#else
#endif

#define WIDTH  RESX
#define HEIGHT RESY

#include "o.h"

/* the following defines turn on and of capabilities, most capabilities can
 * be independently toggled, and the rest of the API should work with a smaller
 * code footprint as things are removed.
 */

#define O_ENABLE_FILL
//#define O_ENABLE_STROKE
#define O_ENABLE_USER_SHADER
#define O_ENABLE_BW   /* pick one ,. */
//#define O_ENABLE_GRAY
//#define O_ENABLE_GRAY_EXTRA
//#define O_ENABLE_RECTANGLE
//#define O_ENABLE_STACK
//#define O_ENABLE_CLIP
#define O_ENABLE_TRANSFORM
//#define O_ENABLE_TRANSFORM_FUNCS
//#define O_ENABLE_RENDER
//#define O_ENABLE_TEXT
//#define O_ENABLE_EXTERNAL_FONT  /* needs O_ENABLE_TEXT */
//#define O_ENABLE_FONT_DUMP      /* dumps font to font path upon first use */
//#define O_ENABLE_FILE           /* compile file accessing calls */

#ifdef O_ENABLE_EXTERNAL_FONT
#ifndef O_ENABLE_FILE
#define O_ENABLE_FILE
#endif
#endif

#ifdef O_ENABLE_EXTERNAL_CLIP
#ifndef O_ENABLE_FILL
#define O_ENABLE_FILL
#endif
#endif

#ifdef O_ENABLE_TEXT
#ifndef O_ENABLE_STROKE
#define O_ENABLE_STROKE
#endif
#endif


#define BEZIER_SEGMENTS    16
#define MAX_SPANS_PER_LINE 7                    /* maximum number of intersecting spans on one line */
#define SPP                10                   /* sup pixel precision divider, each pixel is 6 such units internally */
#define STACK_DEPTH        3                    /* needs O_ENABLE_STACK */
#define FONT_PATH          "/tmp/font.bin"      /* if O_ENABLE_EXTERNAL_FONT */
#define FIXED_ONE          1023  /* XXX: using 1023 instead of 1024 gives smaller
                                    code size.. evil.. */

typedef struct _Node    Node;
typedef struct _Path    Path;
typedef struct _Context Context;

#define CLAMP(val,min,max) ((val)<(min)?(min):(val)>(max)?(max):(val))

/* Default color generator shader */
static int shader_gray (int x, int y, void *data);

struct _Node
{
  short int x;
  short int y;
  unsigned char    type;    /* can maybe be removed and put in context?
                               or < 0 values indicate move, and -127 indicate curve_to? */
};


#ifdef O_ENABLE_CLIP
typedef struct _Clip Clip;
struct _Clip
{
  unsigned char row[HEIGHT][2]; /* 0 start 1 end */
};
#endif

struct _Context
{
#ifdef O_ENABLE_USER_SHADER
  ShaderFunction shader;
#endif
  void          *shader_data;
#ifdef O_ENABLE_TRANSFORM
  OMatrix        transform;
#endif
#ifdef O_ENABLE_STROKE
  char           line_width;
#endif
#ifdef O_ENABLE_TEXT
  char           font_size;
#endif
};

struct _Path
{
  signed short int x;
  signed short int y;
  int       count;
  int       max_size;
  Node     *nodes;
#ifdef O_ENABLE_STACK
  Context  *context;
  int       context_no;
#else
  Context   context;
#endif
#ifdef O_ENABLE_CLIP  /* would be better if it was part of the
                     * context stack, but it uses a lot of ram
                     */
  Clip      clip;
#endif
};
static Path *path = NULL;


/* the actual inner draw function used for doing the real painting */
static void o_render_span(int x0, int y,
                          int x1,
#ifdef O_ENABLE_USER_SHADER
                          ShaderFunction shader,
#endif
                          void          *shader_data)
{
#ifdef O_ENABLE_CLIP
  x0 = CLAMP (x0, path->clip.row[y][0], path->clip.row[y][1]);
  x1 = CLAMP (x1, path->clip.row[y][0], path->clip.row[y][1]);
#else
  x0 = CLAMP (x0, 0, WIDTH);
  x1 = CLAMP (x1, 0, WIDTH);
#endif
  if (y <0 || y>=HEIGHT)
    return;
  for(int x=x0; x<x1; x++)
    {
#ifdef O_ENABLE_USER_SHADER
      lcdSetPixel(x,y, shader(x,y,shader_data));
#else
      lcdSetPixel(x,y, shader_gray (x,y,shader_data));
#endif
    }
}

#ifdef O_ENABLE_STACK
static Context *context (void)
{
  return &path->context[path->context_no];
}
#else
#define context() (&path->context)
#endif

#ifdef O_ENABLE_TRANSFORM

/* copied from twin */
static void
o_matrix_multiply (OMatrix      *result,
                   const OMatrix *a,
                   const OMatrix *b)
{
  OMatrix    r;

  for (int row = 0; row < 3; row++)
    for (int col = 0; col < 2; col++)
      {
        int t = (row == 2) ?  t = b->m[2][col] : 0;

        for (int n = 0; n < 2; n++)
          t = t + ((a->m[row][n] * b->m[n][col])) / FIXED_ONE;
        r.m[row][col] = t;
      }
  *result = r;
}

static void
o_matrix_identity (OMatrix *m)
{
  m->m[0][0] = m->m[1][1] = FIXED_ONE;
  m->m[0][1] = m->m[1][0] = m->m[2][0] = m->m[2][1] = 0;
}

void o_transform  (OMatrix *matrix, int replace)
{
  OMatrix *m = &context()->transform;
  if (replace)
    context()->transform = *matrix;
  else
    o_matrix_multiply (m, matrix, m);
}

void
o_identity (void)
{
  o_matrix_identity (&context()->transform);
}

#endif

#ifdef O_ENABLE_TRANSFORM_FUNCS
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

void
o_translate (int tx, int ty)
{
  OMatrix t;
  OMatrix *m = &context()->transform;
  o_matrix_identity (&t);

  t.m[2][0] = tx * SPP * FIXED_ONE / 1000;
  t.m[2][1] = ty * SPP * FIXED_ONE / 1000;
  o_matrix_multiply (m, &t, m);
}

void
o_scale (int sx, int sy)
{
  OMatrix t;
  OMatrix *m = &context()->transform;
  o_matrix_identity (&t);
  t.m[0][0] = sx * FIXED_ONE / 1000;
  t.m[1][1] = sy * FIXED_ONE / 1000;
  o_matrix_multiply (m, &t, m);
}


void
o_rotate (int angle)
{
  OMatrix *m = &context()->transform;
  OMatrix t;
  int c = o_cos ( angle * 4 * 8192 / 3600 ) / 4;
  int s = o_sin ( angle * 4 * 8192 / 3600 ) / 4;

  t.m[0][0] = c;   t.m[0][1] = s;
  t.m[1][0] = -s;  t.m[1][1] = c;
  t.m[2][0] = 0;   t.m[2][1] = 0;
  o_matrix_multiply (m, &t, m);
}
#endif

#ifdef O_ENABLE_STROKE

static unsigned int o_sqrt (unsigned int n)
{
   unsigned int root = 0, bit, trial;
   bit = (n >= 0x10000) ? 1<<30 : 1<<14;
   do
   {
      trial = root+bit;
      if (n >= trial)
      {
         n -= trial;
         root = trial+bit;
      }
      root >>= 1;
      bit >>= 2;
   } while (bit);
   return root;
}

static int
point_dist (Node *a,
            Node *b)
{
  return o_sqrt ((a->x-b->x)*(a->x-b->x) +
                 (a->y-b->y)*(a->y-b->y));
}
#endif

/* linear interpolation between two points */
static void
lerp (Node *dest,
      Node *a,
      Node *b,
      int   t)
{
  dest->x = a->x + (((b->x-a->x) * t) / FIXED_ONE);
  dest->y = a->y + (((b->y-a->y) * t) / FIXED_ONE);
}

/* evaluate a point on a bezier-curve.
 */
static void
bezier (Node **curve,
        Node  *dest,
        int    t)
{
  Node ab,bc,cd,abbc,bccd;

  lerp (&ab, curve[0], curve[1], t);
  lerp (&bc, curve[1], curve[2], t);
  lerp (&cd, curve[2], curve[3], t);
  lerp (&abbc, &ab, &bc,t);
  lerp (&bccd, &bc, &cd,t);
  lerp (dest, &abbc, &bccd, t);
}

void o_current_point (int *x, int *y)
{
  if (x) *x = path->x / SPP;
  if (y) *y = path->y / SPP;
}

#ifdef O_ENABLE_STACK
void o_save (void)
{
  path->context_no ++;
#ifdef SIMULATOR
  if (path->context_no >= STACK_DEPTH)
    {
      fprintf (stderr, "stack depth bust\n");
    }
  /* copy existing state */
  path->context[path->context_no] = path->context[path->context_no-1];
#endif
}

void o_restore (void)
{
  path->context_no --;
#ifdef SIMULATOR
  if (path->context_no < 0)
    fprintf (stderr, "stack underflow\n");
#endif
}
#endif

/* types:
 *   's' : initialized state (last pen coordinates)
 *   'm' : move_to   'l' : line_to
 *   'c' : curve_to  '.' : curve_to_    'C' : curve_to__
 */

static void
o_add (unsigned char type,
       int      x,
       int      y)
{
  Node *iter = NULL;

#if 1
  if (type != 'A')
    {
      x *= SPP;
      y *= SPP;
    }
#endif

  /* transform inputed coords here */
  path->x = x;
  path->y = y;

  if (type=='A')
    {
      type = 'l';
    }
#ifdef O_ENABLE_TRANSFORM
  else
    {
       OMatrix *m = &context()->transform;
       x = ((path->x * m->m[0][0]) + (path->y * m->m[1][0]) + m->m[2][0]) / FIXED_ONE;
       y = ((path->y * m->m[1][1]) + (path->x * m->m[0][1]) + m->m[2][1]) / FIXED_ONE;
    }
#endif

#if 0
  x = CLAMP(x, -32000, 32000);
  y = CLAMP(y, -32000, 32000);
#endif

  if (path->count)
    iter = &path->nodes[0];
  if (path->count+1 >= path->max_size)
    return;

  iter = &path->nodes[path->count++];

  iter->type=type;
  iter->x = x;
  iter->y = y;
}

void o_init (void *data, int data_size);

#ifdef O_ENABLE_USER_SHADER
void o_set_shader (ShaderFunction shader, void *shader_data)
{
  context()->shader = shader;
  context()->shader_data = shader_data;
}
#endif

#ifdef O_ENABLE_STROKE
void
o_set_line_width (int line_width)
{
  context()->line_width = line_width;
}
#endif

#ifdef  O_ENABLE_BW
#define GRAY_PRECISION     8
static int shader_gray (int x, int y, void *data)
{
  return (int)(data); /* stored gray value as 8bit value */
}
#endif

#ifdef O_ENABLE_GRAY
#define GRAY_PRECISION     4
/* Default color generator shader */
static int shader_gray (int x, int y, void *data)
{
  int value = (int)(data); /* stored gray value as 8bit value */
  if (value >= 3) return 1;
  else if (value > 0)
    return (x%2==0) ? (y %2)? 1:0:
                      (y %2)? 0:1;
  return 0;
}
#endif

#ifdef O_ENABLE_GRAY_EXTRA
#define GRAY_PRECISION     8
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
#endif

#ifndef GRAY_PRECISION
#define GRAY_PRECISION 8
#endif

void o_set_gray (int value)
{
#ifdef O_ENABLE_USER_SHADER
#ifdef O_ENABLE_BW  /* hack, that permits setting a user shader for grayscale..
                        when only bw has been enabled, _and_ we have user shader */
  context()->shader_data = (void*)((int)(value * GRAY_PRECISION / 1000));
#else
  o_set_shader (shader_gray, (void*)((int)(value * GRAY_PRECISION / 1000)));
#endif
#else
  context()->shader_data = (void*)((int)(value * GRAY_PRECISION / 1000));
#endif
}

#ifdef O_ENABLE_CLIP
void o_reset_clip     (void)
{
  int i;
  for (i=0; i<HEIGHT; i++)
    {
      path->clip.row[i][0] = 0;
      path->clip.row[i][1] = WIDTH;
    }
}
#endif

void
o_init (void *dat, int data_size)
{
  char *data = ((char*)dat);
  path = (void*)data;

  data      += sizeof (Path);
  data_size -= sizeof (Path);
#ifdef O_ENABLE_STACK
  path->context = (void*)data;

  data      += sizeof (Context) * STACK_DEPTH;
  data_size -= sizeof (Context) * STACK_DEPTH;
#endif
  path->nodes = (void*)data;

  path->max_size = data_size / sizeof (Node);

#ifdef O_ENABLE_STACK
  path->context_no = 0;
#endif
#ifdef O_ENABLE_STROKE
  context()->line_width = 1;
#endif
#ifdef O_ENABLE_TEXT
  context()->font_size = 7;
#endif
  o_path_new ();
#ifdef O_ENABLE_TRANSFORM
  o_matrix_identity (&context()->transform);
#endif
#ifdef O_ENABLE_CLIP
  o_reset_clip ();
#endif


  o_set_gray (1000);
#ifdef O_ENABLE_BW  /* hack, that permits setting a user shader for grayscale..
                        when only bw has been enabled, _and_ we have user shader */
  context()->shader = shader_gray;
#endif
}

void
o_path_new (void)
{
  path->count = 0;
}

void o_move_to (int x, int y)
{
  o_add ('m', x, y);
}

void o_line_to (int x, int y)
{
  o_add ('l', x, y);
}

void o_curve_to (int x1, int y1,
                 int x2, int y2,
                 int x3, int y3)
{
  o_add ('c', x1, y1);
  o_add ('.', x2, y2);
  o_add ('C', x3, y3);
}

#ifdef O_ENABLE_STROKE

static void
o_line (int x0, int y0, int x1, int y1, int *need_to_travel, int *traveled_length)
{
  Node a, b;
  int spacing, local_pos, distance, offset, leftover;

  a.x = x0;
  a.y = y0;

  b.x = x1;
  b.y = y1;

  spacing = SPP - 1; /* *0.99 with divider!=1 */

  distance = point_dist (&a, &b);

  leftover = *need_to_travel - *traveled_length;
  offset = spacing - leftover;

  local_pos = offset;

  if (distance > 0)
    for (;local_pos <= distance; local_pos += spacing)
      {
        Node spot;
        int ratio = (local_pos * FIXED_ONE) / distance;
        int line_width;
        line_width = context()->line_width;

        /* scale line width by scaling factor */

        lerp (&spot, &a, &b, ratio);
        /* only 1px wide... */
        if (line_width <= 1)
          {
#ifdef O_ENABLE_USER_SHADER
            lcdSetPixel (spot.x / SPP, spot.y / SPP,
              context()->shader(spot.x / SPP, spot.y / SPP,
                               context()->shader_data));
#else
            lcdSetPixel (spot.x / SPP, spot.y / SPP,
              shader_gray(spot.x / SPP, spot.y / SPP,
                               context()->shader_data));
#endif
          }
        else
          {
            int line_radius = line_width /2;
            /* should use a more circular brush */
            int y;
            for (y = spot.y/SPP - line_radius;
                 y < spot.y/SPP + line_radius; y++)
              o_render_span (spot.x/SPP - line_radius,
                             y,
                             spot.x/SPP + line_radius,
#ifdef O_ENABLE_USER_SHADER
                             context()->shader,
#endif
                             context()->shader_data);
          }

        *traveled_length += spacing;
      }
  *need_to_travel += distance;
}


void
o_stroke (void)
{
  Node *iter = &path->nodes[0];
  int   traveled_length = 0;
  int   need_to_travel = 0;
  int   x = 0, y = 0;
  int   i = 0;

  i=0;
  while (i < path->count)
    {
      switch (iter->type)
        {
          case 'm':
            need_to_travel = 0;
            traveled_length = 0;
            x = iter->x;
            y = iter->y;
            break;
          case 'C':
            { /* create piecevize linear approximation of bezier curve */
               int   i;
               Node *pts[4];

               for (i=0;i<4;i++)
                   pts[i]=&iter[i-3];
               for (i=0; i< FIXED_ONE; i+= FIXED_ONE/ BEZIER_SEGMENTS)
                 {
                    Node iter2;
                    bezier (pts, &iter2, i);
                    o_line (x, y, iter2.x, iter2.y, &need_to_travel, &traveled_length);
                    x = iter2.x;
                    y = iter2.y;
                 }
            }
          case 'l':
            o_line (x, y, iter->x, iter->y, &need_to_travel, &traveled_length);
            x = iter->x;
            y = iter->y;
            break;
        }
      iter++;
      i++;
    }
}
#endif

#ifdef O_ENABLE_FILL

#define EMPTY_SCAN_SLOT -126

/* insertion sort integer into array */
static void insert_sorted (signed char *array, int value)
{
  for (int j=0; j<MAX_SPANS_PER_LINE; j++)
    {
      if (array[j] == EMPTY_SCAN_SLOT)
        {
          array[j] = value;
          return;
        }
      if (array[j] > value)
        {
          for (int k=MAX_SPANS_PER_LINE -1; k >= j+1; k--)
            array[k] = array[k-1];
          array[j] = value;
          return;
        }
    }
}

static void fill_line (int prev_x, int prev_y, int dest_x, int dest_y,
                       signed char scanlines[HEIGHT][MAX_SPANS_PER_LINE],
                       int *lastdir, int *lastline)
{
  int ydir;
  int dx, dy;
  dx = dest_x - prev_x;
  dy = dest_y - prev_y;

  ydir = (dy < 0) ? -1 : 1;

  /* do linear interpolation between vertexes */
  for (int y = prev_y; y != dest_y; y += ydir)
    if (y >= 0 && y < HEIGHT && *lastline != y)
      {
        int x = prev_x + (dx * (y-prev_y)) / dy;
        insert_sorted(&scanlines[y][0], x);
        if (ydir != *lastdir) // && *lastdir != -2)  //XXX: ugly optimizing hack
          insert_sorted(&scanlines[y][0], x);

        *lastdir = ydir;
        *lastline = y;
      }
}

void o_fill (void)
{
  if (path->count < 1)
    return;

  {
    signed char scanlines[HEIGHT][MAX_SPANS_PER_LINE];
    /* Stack allocated scanline intersection list  */
    Node *iter;
    int   prev_x,  prev_y;
    int   first_x, first_y;
    int   lastline = -1;
    int   lastdir  = -2;

    for (int i = 0; i<HEIGHT; i++)
      for (int j=0; j<MAX_SPANS_PER_LINE; j++)
        scanlines[i][j] = EMPTY_SCAN_SLOT;

    first_x = prev_x = path->nodes[0].x / SPP;
    first_y = prev_y = path->nodes[0].y / SPP;

    /* saturate scanline intersection list */
    iter = &path->nodes[1];
    for (int i=1; i<path->count; i++, iter++)
      {
        switch (iter->type)
          {
            int dest_x;
            int dest_y;
            case 'm':
              {
                first_x = prev_x = iter->x / SPP;
                first_y = prev_y = iter->y / SPP;
                break;
              }
            case 'l':
              {
                dest_x = iter->x / SPP;
                dest_y = iter->y / SPP;
fill_close:  /* label used for goto to close last segment */

                fill_line (prev_x, prev_y, dest_x, dest_y, scanlines, &lastdir, &lastline);

                prev_x = dest_x;
                prev_y = dest_y;
                /* if we're on the last knot, fake the first vertex being a next one */
                if (i+1 == path->count)
                  {
                    dest_x = first_x;
                    dest_y = first_y;
                    i++; /* to make the loop finally end */
                    goto fill_close;
                  }
                break;
              }
          case 'C':
            { /* create piecevize linear approximation of bezier curve */
               int   i;
               Node *pts[4];

               for (i=0;i<4;i++)
                   pts[i]=&iter[i-3];
               for (i=0; i< FIXED_ONE; i+= FIXED_ONE/ BEZIER_SEGMENTS)
                 {
                    Node iter2;
                    bezier (pts, &iter2, i);
                    fill_line (prev_x, prev_y, iter2.x/SPP, iter2.y/SPP, scanlines, &lastdir, &lastline);
                    prev_x = iter2.x / SPP;
                    prev_y = iter2.y / SPP;
                 }
               if (i+1 == path->count)
                 {
                   dest_x = first_x;
                   dest_y = first_y;
                   i++; /* to make the loop finally end */
                   goto fill_close;
                 }
               break;
            }
          }
      }

    /* Fill the spans */
    for (int i=0; i < HEIGHT; i++)
      {
        for (int j=0; j<MAX_SPANS_PER_LINE-2; j+=2)
          {
            if (scanlines[i][j] > EMPTY_SCAN_SLOT)
              {
                o_render_span (scanlines[i][j], i, scanlines[i][j+1],
#ifdef O_ENABLE_USER_SHADER
                      context()->shader,
#endif
                      context()->shader_data);
              }
          }
      }
  }
}

#endif

#ifdef O_ENABLE_CLIP
void o_clip           (void)
{
  /* massive code duplication with o_fill */
  if (path->count < 1)
    return;

  {
    Node *iter;
    int   prev_x,  prev_y;
    int   first_x, first_y;
    int   lastline = -1;

    first_x = prev_x = path->nodes[0].x / SPP;
    first_y = prev_y = path->nodes[0].y / SPP;

    /* saturate scanline intersection list */
    iter = &path->nodes[1];
    for (int i=1; i<path->count; i++, iter++)
      {
        int dest_x = iter->x / SPP;
        int dest_y = iter->y / SPP;
        int ydir;
        int dx, dy;
fill_close:  /* label used for goto to close last segment */
        dx = dest_x - prev_x;
        dy = dest_y - prev_y;

        ydir = (dy < 0) ? -1 : 1;

        /* do linear interpolation between vertexes */
        for (int y = prev_y; y != dest_y; y += ydir)
          if (y >= 0 && y < HEIGHT && lastline != y)
            {
              int x = prev_x + (dx * (y-prev_y)) / dy;
              if (path->clip.row[y][0] < x)
                path->clip.row[y][0] = x;
              if (path->clip.row[y][1] > x)
                path->clip.row[y][1] = x;
              lastline = y;
            }

        prev_x = dest_x;
        prev_y = dest_y;
        /* if we're on the last knot, fake the first vertex being a next one */
        if (i+1 == path->count)
          {
            dest_x = first_x;
            dest_y = first_y;
            i++; /* to make the loop finally end */
            goto fill_close;
          }
      }
  }
}
#endif


void o_close (void)
{
  Node *iter;
  int i = path->count;
  if (path->count == 0)
    return;
  iter = &path->nodes[path->count];
  while (i>0 && iter->type != 'm') /* find previous move to */
    {
      iter --;
      i--;
    }
  if (i>=0 && iter->type == 'm')
    o_add ('A', iter->x, iter->y);
}

#ifdef O_ENABLE_RECTANGLE
void o_orectangle (int x0, int y0, int width, int height)
{
  o_path_new ();
  o_move_to (x0, y0);
  o_line_to (x0 + width, y0);
  o_line_to (x0 + width, y0+height);
  o_line_to (x0, y0+height);
  o_close ();
}
#endif

static const signed char *_o_bounds_op (const signed char *g,
                                        int *xmin, int *ymin, int *xmax, int *ymax)
{
  int ccount = 0;
  switch (*g++) {
    case 'l': case 'm': ccount = 1; break;
    case 'c': ccount = 3; break;
    case 'g': case 'w': g++; break;
    case 'e': case '.': /* end */ return NULL;
    default:
    case 'z': case 'f': case 's': case ' ': break;
  }
  for (int c = 0; c < ccount; c++)
    {
      if (xmin && *xmin > g[c*2+0])
        *xmin = g[c*2+0];
      if (ymin && *ymin > g[c*2+1])
        *ymin = g[c*2+1];
      if (xmax && *xmax < g[c*2+0])
        *xmax = g[c*2+0];
      if (ymax && *ymax < g[c*2+1])
        *ymax = g[c*2+1];
    }
  g += ccount * 2;
  return g;
}

#ifdef O_ENABLE_RENDER
static const signed char * _o_process_op (const signed char *g)
{
  switch (*g++) {

    case ' ': o_path_new (); break;
    /* all of these path commands are directly in integer coordinates */
    case 'm': o_move_to  (g[0], g[1]); g += 2; break;
    case 'l': o_line_to  (g[0], g[1]); g += 2; break;
    case 'c': o_curve_to (g[0], g[1], g[2], g[3], g[4], g[5]); g += 6; break;
    case 'z': o_close (); break;

              /* directly in px XXX: should be 10 = 1.0 instead? */
    case 'w': o_set_line_width (g[0]); g ++; break;
              /* 0 = black, 50 = gray, 100 = white */
    case 'g': o_set_gray (g[0]*10); g ++; break;

#ifdef O_ENABLE_FILL
    case 'f': o_fill (); break;
#endif
#ifdef O_ENABLE_STROKE
    case 's': o_stroke (); break;
#endif

#ifdef O_ENABLE_FLUFF_CODE
    case 'r': o_rectangle (g[0], g[1], g[2], g[3]); g += 4; break;
#endif
#ifdef O_ENABLE_STACK
    case '+': o_save (); break;
    case '-': o_restore (); break;
#endif
#ifdef O_ENABLE_TRANSFORM
              /* 1 = 1 10 = 10 100 = 100 */
    case '!': o_identity (); break;
    case 'T': o_translate (g[0] * 100, g[1] * 100); g+=2; break;
              /* 1 = 0.01 10 = 0.1 50 = 0.5 100 = 10x */
    case 'S': o_scale (g[0] * 10, g[1] * 10); g+=2; break;
              /* -128 = -360  64 = 180 128 = 360 */
    case 'R': o_rotate ((g[0] * 3600)/127); g+=1; break;
#else
    case 'T': case 'S': g+=2;break;
    case 'R': g+=1;break;
#endif

#ifdef O_ENABLE_CLIP
    case 'x': o_clip (); break;
    case 'X': o_reset_clip (); break;
#endif
#ifdef O_ENABLE_TEXT_API
    case 't': o_text_path (g); while (*g) g++; break;
#endif
    default:
    case '\0':
    case '.':  /* end */
      return NULL;
    }
  return g;
}

void
o_render (const signed char *g)
{
  for (; g; g = _o_process_op (g));
}

void
o_bounds (const signed char *g,
          int *minx, int *miny,
          int *maxx, int *maxy)
{
  if (minx) *minx= 10000;
  if (miny) *miny= 10000;
  if (maxx) *maxx= -10000;
  if (maxy) *maxy= -10000;
  for (; g; g = _o_bounds_op (g, minx, miny, maxx, maxy));
}

#ifdef O_ENABLE_FILE

#ifdef SIMULATOR
static FILE *file;
#else
static FIL file; /* font file */
#endif

void o_render_file (const char *file_path, int         offset)
{
  unsigned int readbytes;
#ifdef SIMULATOR
  if (!(file = fopen (file_path, "r")))
#else
  unsigned int res;
  if ((res = f_open (&file, file_path, FA_OPEN_EXISTING|FA_READ)))
#endif
    {
      /* XX: failed to open */
      return;
    }
#ifdef SIMULATOR
  fseek(file, offset, SEEK_SET);
#else
  f_lseek(&file, offset);
#endif
  signed char buf[8];
  const signed char *b;
  int chunk = sizeof (buf);
  do {
#ifdef SIMULATOR
    readbytes = fread (buf + sizeof(buf) - chunk, 1, chunk, file);
#else
    res = f_read (&file, buf + sizeof(buf)-chunk, chunk, &readbytes);
#endif
    b = buf;
    b = _o_process_op (b);
    chunk = b-buf;
    if (b)
      memmove (buf, b, sizeof (buf) - chunk);
  } while (b && readbytes);
  if (b)
    for (b=buf;b;b = _o_process_op (b));
#ifdef SIMULATOR
  fclose (file);
#else
  f_close (&file);
#endif
}

int
o_file_offset (const char *file_path,
               int         no)
{
  short count;
  short offset;
#ifdef SIMULATOR
  if (!(file = fopen (file_path, "r")))
#else
  unsigned int readbytes;
  unsigned int res;
  if ((res = f_open (&file, file_path, FA_OPEN_EXISTING|FA_READ)))
#endif
      /* XX: failed to open */
      return -1;

#ifdef SIMULATOR
    fread (&count, sizeof(count), 1, file);
#else
    res = f_read (&file, &count, sizeof(count), &readbytes);
#endif
    if (no > count)
      {
#ifdef SIMULATOR
        fclose (file);
#else
        f_close (&file);
#endif
        return -1;
      }

#ifdef SIMULATOR
  fseek(file, 2 * (no+1), SEEK_SET);
#else
  f_lseek(&file, 2 * (no+1));
#endif

#ifdef SIMULATOR
    fread (&offset, sizeof(offset), 1, file);
#else
    res = f_read (&file, &offset, sizeof(offset), &readbytes);
#endif

#ifdef SIMULATOR
  fclose (file);
#else
  f_close (&file);
#endif
  return offset;
}

void
o_bounds_file (const char *file_path,
               int         offset,
               int *minx, int *miny,
               int *maxx, int *maxy)
{
  unsigned int readbytes;
  unsigned int res;
  if (minx) *minx=  10000;
  if (miny) *miny=  10000;
  if (maxx) *maxx= -10000;
  if (maxy) *maxy= -10000;
#ifdef SIMULATOR
  if (!(file = fopen (file_path, "r")))
#else
  if ((res = f_open (&file, file_path, FA_OPEN_EXISTING|FA_READ)))
#endif
    {
      /* XX: failed to open */
      return;
    }
#ifdef SIMULATOR
  fseek(file, offset, SEEK_SET);
#else
  f_lseek(&file, offset);
#endif
  signed char buf[8];
  const signed char *b;
  int chunk = sizeof (buf);
  do {
#ifdef SIMULATOR
    readbytes = fread (buf + sizeof(buf) - chunk, 1, chunk, file);
#else
    res = f_read (&file, buf + sizeof(buf)-chunk, chunk, &readbytes);
#endif
    b = buf;
    b = _o_bounds_op (b, minx, miny, maxx, maxy);
    chunk = b-buf;
    if (b)
      memmove (buf, b, sizeof (buf) - chunk);
  } while (b && readbytes);
  if (b)
    for (b=buf;b;b = _o_bounds_op (b, minx, miny, maxx, maxy));
#ifdef SIMULATOR
  fclose (file);
#else
  f_close (&file);
#endif
  /* */
}
#endif

#endif

/******************************************************************************/

#ifndef O_ENABLE_TEXT

void o_set_font_size (int font_size)
{/*adding the symbol to make things still compile without having fontsupport*/}

void o_show_text (const char *ascii,
                  int         wrap_width)
{/*adding the symbol to make things still compile without having fontsupport*/}

#else

void o_set_font_size (int font_size)
{
  context()->font_size = font_size;
}

#ifndef O_ENABLE_EXTERNAL_FONT
#include "o-glyphs.c" /* include the font data directly */

static const signed char *
glyph_data (char ascii)
{
  int no = 0;
  int pos = 0;
  ascii -= 31;
  if (ascii > 126)
    return _o_glyphs;

  while (no < ascii)
    {
      int val = _o_glyphs[pos];
      if (val == '.')
        no ++;
      pos++;
    }
  if (no == ascii)
    return _o_glyphs + pos;
  return _o_glyphs;
}
#endif

/* end of glue */

#define PARA_DET  3 /* 1.5 */
#define PARA_NOM  2
#define WORD_DET  4 /* 0.4 */
#define WORD_NOM 10
#define LINE_DET 12 /* 1.2 */
#define LINE_NOM 10

static int
block_width (int  font_size,
             char ascii)
{
  int maxx = 0;
#ifndef O_ENABLE_EXTERNAL_FONT
  const signed char *g = glyph_data (ascii);
  o_bounds (g, NULL, NULL, &maxx, NULL);
#else
  int offset = o_file_offset (FONT_PATH, ascii);
  o_bounds_file (FONT_PATH, offset, NULL, NULL, &maxx, NULL);
#endif
  return (maxx + 12) * font_size / 42;
}

static void
font_ascii_path (int  font_size,
                 int  x,
                 int  y,
                 char ascii)
{
  OMatrix orig_transform;

  int  origin_x, origin_y;
  int  next_x;
  int  baseline;

  o_current_point (&origin_x, &origin_y);
  next_x = origin_x + block_width (font_size, ascii);
  baseline = origin_y;
  orig_transform = context()->transform;
  o_translate (origin_x * 1000, origin_y * 1000);
  o_scale (font_size * 1000 / 42, font_size * 1000 / 42);
#ifndef O_ENABLE_EXTERNAL_FONT
  const signed char *g = glyph_data (ascii);
  o_render (g);
#else
  int offset = o_file_offset (FONT_PATH, ascii);
  o_render_file (FONT_PATH, offset);
#endif
  o_move_to (next_x, baseline);
  context()->transform = orig_transform;
}

static int
wordlength (int         font_size,
            const char *ascii)
{
  int sum = 0;
  while (*ascii &&
         *ascii != ' ' &&
         *ascii != '\n')
    {
      sum += block_width (font_size, *ascii);
      ascii++;
    }
  return sum;
}

static void
font_path (int         font_size,
           int         width,
           const char *ascii)
{
  int x = path->x / SPP;
  int y = path->y / SPP;

  while (*ascii)
    {
      if (*ascii == '\n')
        {
          o_move_to (x, y += (font_size * PARA_DET) / PARA_NOM);
        }
      else if (*ascii == ' ')
        {
           int pen_x;
           o_current_point (&pen_x, NULL);
           if (width &&
               pen_x + wordlength (font_size, ascii+1) > x + width &&
               wordlength (font_size, ascii+1) < width)
             {
               o_move_to (x, y += (font_size * LINE_DET) / LINE_NOM);
             }
           else
             {
               /* word spacing */
               o_move_to (pen_x + (font_size * WORD_DET) / WORD_NOM, y);
             }
        }
      else
        {
          font_ascii_path (font_size, x,y, *ascii);
        }
      ascii++;
    }
}

#ifdef SIMULATOR
#ifdef O_ENABLE_FONT_DUMP

#define wordout(w) \
   { short int temp = (w);\
     fwrite (&temp, sizeof (temp), 1, file); \
   }
#define byteout(c) \
   { signed char temp = (c);\
     fwrite (&temp, sizeof (temp), 1, file); \
   }

#define FUDGE 3
#define FIRST_CHAR 32   /* padding out of first few entires */

void o_write_font (void)
{
  FILE *file;
  static char done = 0;
  if (done)
    return;
  int count = 0;
  done = 1;
  fprintf (stderr, "@write the font\n");
  file = fopen (FONT_PATH, "w");

  for (int pos = 0; pos < sizeof (_o_glyphs); pos++)
    {
      int val = _o_glyphs[pos];
      if (val == '.')
        count ++;
    }

  fprintf (stderr, "%d\n", count + FIRST_CHAR);
  wordout(count+FIRST_CHAR);
  int no = 0;
  for (int pos = 0; pos < FIRST_CHAR; pos ++)
    {
      fprintf (stderr, "%i, ", 0);
      wordout (0 + (2 * (count + FIRST_CHAR) + FUDGE));
    }
  for (int pos = 0; pos < sizeof (_o_glyphs); pos++)
    {
      int val = _o_glyphs[pos];
      if (val == '.' && no < 129)
        {
          fprintf (stderr, "%i, ", pos + (2 * (count + FIRST_CHAR) + FUDGE));
          wordout (pos + (2 * (count + FIRST_CHAR)) + FUDGE );
          no ++;
        }
    }
  no = FIRST_CHAR - 1; //31;  XXX: might not work quite out..

  fprintf (stderr, "\n\n /* unknown box 0x%x -  %i*/\n",  0,
           0 + (2 * (count + FIRST_CHAR) + FUDGE ));

  for (int pos = 0; pos < sizeof (_o_glyphs); pos++)
    {
      int val = _o_glyphs[pos];
      if (((val >= 'a') && (val <= 'z')) || (val == ' ') || (val == '.'))
        fprintf (stderr, "\n'%c', ", _o_glyphs[pos]);
      else
        fprintf (stderr, "%i, ", _o_glyphs[pos]);
      byteout (_o_glyphs[pos]);
      if (val == '.')
        {
          no ++;
          fprintf (stderr, "\n\n /* '%c' 0x%x -  %i*/\n", no, no,
                            pos + (2 * (count + FIRST_CHAR)) + FUDGE);
        }
    }
  fprintf (stderr, "\n");
  fclose (file);
}
#endif
#endif

void o_show_text (const char *ascii, int wrap_width)
{
#ifdef SIMULATOR
#ifdef O_ENABLE_FONT_DUMP
  o_write_font ();
#endif
#endif
  font_path (context()->font_size, wrap_width, ascii);
}

#endif
