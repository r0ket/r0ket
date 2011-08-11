#include <sysinit.h>
#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/lcd.h"
#include "lcd/print.h"
#include "usetable.h"

struct elem
{
    int x,y;
};

void reset(struct elem snake[],size_t *snake_size,int *dirc,
int*speed, int*points,int*point_s);
void o_rectangle (int x0, int y0, int width, int height);
struct elem rnd(void);

#define MAX_SNAKE_LEN (40)
#define SNAKE_DEM (3)
#define MIN_SPEED (25)
#define MAX_SPEED (3)
#define SIZE_X (RESX)
#define SIZE_Y (RESY)

void ram(void)
{
    int inpt,dirc,c,grows = 0,dx,dy,points,point_s=1;
    size_t n = 0, snake_size = 5, speed=MIN_SPEED;
    struct elem snake[MAX_SNAKE_LEN], food;
	char test[512]; /* scratch space */
  o_init (test, sizeof(test));

  reset(snake,&snake_size,&dirc,&speed,&points,&point_s);

  food = rnd();

    while (1)
    {
        head:
        if(!(++c % speed))
           {


inpt = getInputRaw();

dx=DoString(0,0,IntToStrX(points,2));
    dx=(SIZE_X-dx)/2;
    if(dx<0)
        dx=0;
    dy=(SIZE_Y-getFontHeight())/2;

         lcdFill(255);
      o_rectangle(1,0,SIZE_X-2,SIZE_Y-2);
      o_set_gray (0);
         o_fill ();

        //o_identity (); /* reset tranforms */

      o_set_gray (50);

      setExtFont("UBUNTU29");

    lcdSetPixel(1,1,1);
    DoString(dx,dy,IntToStrX(points,2));

      o_set_gray (255);


    for(n=0;n<snake_size;++n)
    {
        o_rectangle
(snake[n].x*SNAKE_DEM,snake[n].y*SNAKE_DEM,SNAKE_DEM,SNAKE_DEM); /*
fill background with black */
      o_fill ();     /* fill with 50% {
                       reset(snake,&snake_size);
                       goto head;
                   }gray */
    }
    o_rectangle
(food.x*SNAKE_DEM,food.y*SNAKE_DEM,SNAKE_DEM,SNAKE_DEM); /* fill
background with black */
      o_fill ();


      lcdDisplay();

        if (inpt == BTN_UP && dirc != 1)
            {
              dirc = 3;
            }
          else if (inpt == BTN_DOWN && dirc != 3)
            {
              dirc = 1;
            }
          else if (inpt == BTN_LEFT && dirc != 0)
            {
              dirc = 2;
            }
          else if (inpt == BTN_RIGHT && dirc !=2)
            {
              dirc = 0;
            }
//

               struct elem t = snake[snake_size-1];

               if(dirc == 0)
                ++t.x;
               else if(dirc == 1)
                ++t.y;
                else if(dirc == 2)
                --t.x;
               else if(dirc == 3)
                --t.y;

               if(t.x < 0 || t.y < 0 || t.y > SIZE_Y/SNAKE_DEM-1 ||
t.x > SIZE_X/SNAKE_DEM)
               {
                       reset(snake,&snake_size,&dirc,&speed,&points,&point_s);
                       goto head;
                   }

               for(n=0;n<snake_size-1;++n)
               {
                   if(snake[n].x == t.x && snake[n].y == t.y)
                   {
                       reset(snake,&snake_size,&dirc,&speed,&points,&point_s);
                       goto head;
                   }
                   else if(snake[n].x == food.x && snake[n].y == food.y)
                   {
                       grows = 1;
                       ++snake_size;
                       ++points;
                       if(speed > MAX_SPEED) --speed;
                        food = rnd();
                   }
               }

                if(!grows)
                {
               for(n=0;n<snake_size-1;++n)
               {
                   snake[n] = snake[n+1];
               }
                }
                else
                    grows = 0;

               snake[snake_size-1] = t;
           }
           else
               delayms(3);


           }



    }

    struct elem rnd(void)
    {
        struct elem res;
        res.x = getRandom() % (SIZE_X/SNAKE_DEM-1) +1;
                        res.y = getRandom() % (SIZE_Y/SNAKE_DEM-1) + 1;
        return  res;
    }

 void reset(struct elem snake[],size_t *snake_size,int *dirc,
int*speed, int*points,int* point_s)
{
    size_t n = 0;
    for(n=0;n<MAX_SNAKE_LEN;++n)
  { snake[n].x=5;snake[n].y=5; }
  *snake_size = 5;
  *dirc = 0;
  *speed = MIN_SPEED;
  *points=0;
  *point_s=1;
}


void o_rectangle (int x0, int y0, int width, int height)
{
  o_path_new ();
  o_move_to (x0, y0);
  o_line_to (x0 + width, y0);
  o_line_to (x0 + width, y0+height+1);
  o_line_to (x0, y0+height+1);
  o_close ();
}

