/*
 *  o - a project called circle - that draws stuff
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

#ifndef _O_H
#define _O_H

/* initialize o with a given work data buffer, and the size of the data buffer
   in bytes. This data buffer should be allocated on the stack (char buf[2048];
   o_init (buf, sizeof (buf));
 */
void o_init (void *data, int data_size);

/* macro defines for relative commands */
#define o_rel_move_to(x, y) do { int ox, oy;\
   o_current_point (&ox, &oy);\
   o_move_to  (ox + (x), oy + (y)); } while(0);
#define o_rel_line_to(x, y) do { int ox, oy;\
   o_current_point (&ox, &oy);\
   o_line_to  (ox + (x), oy + (y)); } while(0);
#define o_rel_curve_to(x1, y1, x2,y2,x3,y3) do { int ox, oy;\
   o_current_point (&ox, &oy);\
   o_curve_to  (ox + (x1), oy + (y1),\
                ox + (x2), oy + (y2),\
                ox + (x3), oy + (y3));} } while(0);


/* start drawing a new path */
/* clear current path */
void o_path_new       (void);
/* move to */
void o_move_to        (int x,  int y);
/* line to */
void o_line_to        (int x,  int y);
/* bezier curve */
void o_curve_to       (int x1, int y1, int x2, int y2, int x3, int y3);
/* close path with a straight line */
void o_close          (void);

/* only available if compiled with stack*/
void o_save           (void);
/* saved graphics drawing and transformation state*/
void o_restore        (void);

/* retrieve the current pen coordinates */
void o_current_point   (int *x, int *y);


void o_rectangle      (int x0, int y0, int width, int height);

/* the shader is the rendering function invoked for each fragment/pixel touched
 * when stroking and filling, setting the shader only possible when specially
 * compiled with such support.
 */
typedef int (*ShaderFunction)(int x, int y, void *shader_data);
void o_set_shader     (ShaderFunction shader, void *shader_data);

/* set a solid grayscale color, uses an internal shader for generating
 * dither
 */
/* 0 = black 500 = gray 1000 = white */
void o_set_gray       (int value);

/* fill currnet path (does not clear the path) */
void o_fill           (void);

/* set the stroking line width */
void o_set_line_width (int line_width);

/* stroke path with current shader and line width  */
void o_stroke         (void);


/* Interface for binary "EPS"/SVG like renderer, can be used for storing sprites/
 * icons inline with code or separately on disk
 */

void o_render         (const signed char *vector_data);

/* query the offset in a file containing vector sprite number no */
int  o_file_offset    (const char *file_path, int no);

/* renders the sprite starting at the given offset in a file */
void o_render_file    (const char *file_path, int offset);

/*** clipping ***/
void o_clip           (void);
void o_reset_clip     (void);

/* query the rough bounds of a program in mem/file,
   o_path_extents would be a better API for this ... */
void o_bounds         (const signed char *vector_data, int *minx, int *miny, int *maxx, int *maxy);
void o_bounds_file    (const char *file_path, int offset, int *minx, int *miny, int *maxx, int *maxy);


/*** transformation manipulation ***/

typedef struct _OMatrix OMatrix;
struct _OMatrix
{
  int m[3][2];
};

void o_transform      (OMatrix *matrix, int replace);

void o_identity       (void);
/* 1000 = 1.0 */
void o_translate      (int x, int y);
/* 1000 = 1.0 */
void o_scale          (int sx, int sy);
/* 1000 = 1.0, expressed in radians */
void o_rotate         (int angle);

/*** font rendering ***/

void o_set_font_size (int size);

/* appends a path of the given ascii text.
 *
 * x,y   : positioning of baseline for first characther
 * width : the right margin. 0 means no wrapping
 * ascii : a text string that may contain newlines.
 *
 */
void o_show_text (const char *ascii, int  wrap_width);

#endif
