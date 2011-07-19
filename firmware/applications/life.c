#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/display.h"
#include "lcd/allfonts.h"

void ReinvokeISP(void);
void EnableWatchdog(uint32_t ms);
void delayms(uint32_t ms);

/**************************************************************************/
#define POS_PLAYER_Y 60
#define ENEMY_ROWS 3
#define ENEMY_COLUMNS 6
#define DISABLED 255


unsigned char rnd1();

struct gamestate {
    char player;
    char shot_x, shot_y;
    char alive;
    char move, direction, lastcol;
    bool killed;
    char enemy_x[ENEMY_ROWS][ENEMY_COLUMNS];
    char enemy_row_y[ENEMY_ROWS];
    
} game = {RESX/2-4, DISABLED, 0,ENEMY_ROWS*ENEMY_COLUMNS, 0, -1, ENEMY_COLUMNS-1, false};
char key;


void checkISP(void) {
    if(gpioGetValue(RB_BTN2)==0){
        gpioSetValue (RB_LED1, CFG_LED_ON); 
        delayms(200);
        gpioSetValue (RB_LED1, CFG_LED_OFF); 
        while(gpioGetValue(RB_BTN0)==0);
        EnableWatchdog(1000*5);
        ReinvokeISP();
    }
}

        
void draw_rect(char x0, char y0, char x1, char y1) {
  for(char x=x0; x<=x1; ++x) {
    lcdSetPixel(x,y0,true);
    lcdSetPixel(x,y1,true);
  }
  for(char y=y0+1; y<y1; ++y) {
    lcdSetPixel(x0,y,true);
    lcdSetPixel(x1,y,true);
  }
} 

void fill_rect(char x0, char y0, char x1, char y1) {
  for(char x=x0; x<=x1; ++x) {
    for(char y=y0; y<=y1; ++y) {
      lcdSetPixel(x,y,true);
    }
  }
} 

#define STARTVALUE 10
typedef unsigned char uchar;
typedef uchar row_t[RESY+2];
uchar buf1[RESX+2][RESY+2];
uchar buf2[RESX+2][RESY+2];
row_t *life =buf1;
row_t *new =buf2;

void swap_areas() {
  row_t *tmp=life;
  life=new;
  new=tmp;
}

void fill_area(uchar area[RESX+2][RESY+2], uchar x0, uchar y0, uchar x1, uchar y1,uchar value) {
  for(uchar x=x0; x<=x1; ++x) {
    for(uchar y=y0; y<=y1; ++y) {
      area[x][y]=value;
    }
  }
} 

bool find_area(uchar area[RESX+2][RESY+2], uchar x0, uchar y0, uchar x1, uchar y1,uchar value) {
  for(uchar x=x0; x<=x1; ++x) {
    for(uchar y=y0; y<=y1; ++y) {
      if(area[x][y]==value) return true;
    }
  }
  return false;
} 

uint32_t sum_area(uchar area[RESX+2][RESY+2], uchar x0, uchar y0, uchar x1, uchar y1) {
  uint32_t sum=0; 
  for(uchar x=x0; x<=x1; ++x) {
    for(uchar y=y0; y<=y1; ++y) {
      sum+=area[x][y];
    }
  }
  return sum;
} 

void draw_area() {
  for(uchar x=1; x<=RESX; ++x) {
    for(uchar y=1; y<=RESY; ++y) {
      lcdSetPixel(x,y,life[x+1][y+1]&1);
    }
  }
}

void calc_waldbrand() {
  fprintf(stderr,"calc area...\n");
  //  swap_areas();
  //   return;
  for(uchar x=1; x<=RESX; ++x) {
    for(uchar y=1; y<=RESY; ++y) {
      if(life[x][y]==0) {
	new[x][y]=rnd1()<20?0:(find_area(life,x-1,y-1,x+1,y+1,STARTVALUE)?STARTVALUE:0);
      } else {
	new[x][y]=life[x][y]-1;
      }
      //      new[x][y]=sum_area(life,x-1,y-1,x+1,y+1)%2;
    }
  }
  swap_areas();
}

void calc_area() {
  static unsigned long iter=0;
  fprintf(stderr,"Iteration %d \n",++iter);
  for(uchar x=1; x<=RESX; ++x) {
    for(uchar y=1; y<=RESY; ++y) {
      uchar sum=sum_area(life,x-1,y-1,x+1,y+1)-life[x][y];
      new[x][y]=sum==3||sum==2&&life[x][y];
    }
  }
  swap_areas();
}

int pattern=0;
#define PATTERNCOUNT 3

void reset_area() {
  fill_area(life,0,0,RESX+1,RESY+1,0);

  switch(pattern) {
  case 0:
  for(uchar x=STARTVALUE; x>0; --x) {
    for(uchar y=1; y<RESY/2; ++y) {
      life[RESX/2+x][y]=x;
    }
  }
  life[7][7]=1;
  life[17][7]=1;
  life[7][17]=1;
  new[55][15]=1;
  new[45][25]=1;
  new[35][35]=1;
  new[25][45]=1;
  new[15][55]=1;
  break;
  case 1:
    for(int i=0; i<RESX/3; ++i) life[i][0]=1;
    break;
  case 2:
    life[RESX/2][RESY/2]=1;
    break;
  }
}

void random_area(uchar area[RESX+2][RESY+2], uchar x0, uchar y0, uchar x1, uchar y1,uchar value) {
  for(uchar x=x0; x<=x1; ++x) {
    for(uchar y=y0; y<=y1; ++y) {
      area[x][y]=rnd1()<value;
    }
  }
}

uchar stepmode=0;
uchar randdensity=0;

void main_life(void) {
    gpioSetValue (RB_LED1, CFG_LED_OFF); 
    backlightInit();
    reset_area();
    while (1) {
      //        checkISP();
        lcdFill(0);
	uint32_t button=(stepmode?getInputWait():getInput());
	if(button!=BTN_ENTER) randdensity=0;
	switch(button) {
	case BTN_DOWN:
	  stepmode=1;
	  break;
	case BTN_RIGHT:
	  stepmode=0;
	  break;
	case BTN_LEFT:
	  reset_area();
	  break;
	case BTN_ENTER:
	  randdensity+=8;
	  random_area(life,1,1,RESX,RESY,randdensity);
	  stepmode=1;
	  break;
	case BTN_UP:
	  pattern=(pattern+1)%PATTERNCOUNT;
	  reset_area();
	  stepmode=1;
	  break;
	}
        draw_area();
        lcdDisplay();
        delayms(10);
	calc_area();
    }
    return;
}



// random code from ecc

static int xrandm=100000000;
static int xrandm1=10000;
static int xrandb1=51723621;

int xmult(int p,int q)
{
  int p1,p0,q1,q0;

  p1=p/xrandm1; p0=p%xrandm1;
  q1=q/xrandm1; q0=q%xrandm1;
  return (((p0*q1+p1*q0)%xrandm1)*xrandm1+p0*q0)%xrandm;
}

unsigned char rnd1()
{
  static int a=123456789;
  a = (xmult(a,xrandb1)+1)%xrandm;
  return a & 0xff;
}
