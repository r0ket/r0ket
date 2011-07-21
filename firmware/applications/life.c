#include <sysinit.h>

#include "basic/basic.h"

//#include "lcd/render.h"
#include "lcd/display.h"
//#include "lcd/allfonts.h"

#define BITSET_X (RESX+2)
#define BITSET_Y (RESY+2)
#define BITSET_SIZE (BITSET_X*BITSET_Y)

#include "cbitset.h"

typedef uint8_t uchar;

unsigned char rnd1();

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

struct bitset _buf1,*buf1=&_buf1;
struct bitset _buf2,*buf2=&_buf2;

struct bitset *life =&_buf1;
struct bitset *new =&_buf2;

void swap_areas() {
  struct bitset *tmp=life;
  life=new;
  new=tmp;
}

void fill_area(struct bitset *area, uchar x0, uchar y0, uchar x1, uchar y1,uchar value) {
  for(uchar x=x0; x<=x1; ++x) {
    for(uchar y=y0; y<=y1; ++y) {
      bitset_set2(area,x,y,value);
    }
  }
} 

bool find_area(struct bitset *area, uchar x0, uchar y0, uchar x1, uchar y1,uchar value) {
  for(uchar x=x0; x<=x1; ++x) {
    for(uchar y=y0; y<=y1; ++y) {
      if(bitset_get2(area,x,y)==value) return true;
    }
  }
  return false;
} 

uint32_t sum_area(struct bitset *area, uchar x0, uchar y0, uchar x1, uchar y1) {
  uint32_t sum=0; 
  for(uchar x=x0; x<=x1; ++x) {
    for(uchar y=y0; y<=y1; ++y) {
      sum+=bitset_get2(area,x,y);
    }
  }
  return sum;
} 

void draw_area() {
  for(uchar x=0; x<RESX; ++x) {
    for(uchar y=0; y<RESY; ++y) {
      lcdSetPixel(x,y,bitset_get2(life,x+1,y+1));
    }
  }
}

void calc_area() {
#ifdef SIMULATOR
  static unsigned long iter=0;
  fprintf(stderr,"Iteration %d \n",++iter);
#endif
  for(uchar x=1; x<=RESX; ++x) {
    for(uchar y=1; y<=RESY; ++y) {
      uchar sum=sum_area(life,x-1,y-1,x+1,y+1)-bitset_get2(life,x,y);
      bitset_set2(new,x,y,sum==3||(sum==2&&bitset_get2(life,x,y)));
    }
  }
  swap_areas();
}

int pattern=0;
#define PATTERNCOUNT 3

void reset_area() {
  fill_area(life,0,0,RESX+1,RESY+1,0);
  fill_area(new,0,0,RESX+1,RESY+1,0);

  switch(pattern) {
  case 0:
  bitset_set2(life,41,40,1);
  bitset_set2(life,42,40,1);
  bitset_set2(life,41,41,1);
  bitset_set2(life,40,41,1);
  bitset_set2(life,41,42,1);
  break;
  case 1:
    for(int i=0; i<RESX/2; ++i) bitset_set2(life,i,0,1);
    bitset_set2(life,40,40,1);
    bitset_set2(life,41,40,1);
    bitset_set2(life,41,41,1);
    break;
  case 2:
    bitset_set2(life,40,40,1);
    bitset_set2(life,41,40,1);
    bitset_set2(life,42,40,1);
    bitset_set2(life,42,41,1);
    bitset_set2(life,42,42,1);
    bitset_set2(life,40,41,1);
    bitset_set2(life,40,42,1);
    break;
  }
}

void random_area(struct bitset *area, uchar x0, uchar y0, uchar x1, uchar y1,uchar value) {
  for(uchar x=x0; x<=x1; ++x) {
    for(uchar y=y0; y<=y1; ++y) {
      bitset_set2(area,x,y,rnd1()<value);
    }
  }
}

#define LEDINTERVAL 1
uint8_t ledcycle=3;
void nextledcycle() {
  ledcycle=(ledcycle+1)%(8*LEDINTERVAL);
  uint8_t a=ledcycle/LEDINTERVAL;
  switch(a) {
  case 0: gpioSetValue (RB_LED0, CFG_LED_ON); break;
  case 4: gpioSetValue (RB_LED0, CFG_LED_OFF); break;
  case 1: gpioSetValue (RB_LED1, CFG_LED_ON); break;
  case 5: gpioSetValue (RB_LED1, CFG_LED_OFF); break;
  case 2: gpioSetValue (RB_LED2, CFG_LED_ON); break;
  case 6: gpioSetValue (RB_LED2, CFG_LED_OFF); break;
  case 3: gpioSetValue (RB_LED3, CFG_LED_ON); break;
  case 7: gpioSetValue (RB_LED3, CFG_LED_OFF); break;
  }
}

uchar stepmode=0;
uchar randdensity=0;

void main_life(void) {
    backlightInit();
    reset_area();
    gpioSetValue (RB_LED0, CFG_LED_ON);
    gpioSetValue (RB_LED1, CFG_LED_ON);
    gpioSetValue (RB_LED2, CFG_LED_ON);
    gpioSetValue (RB_LED3, CFG_LED_ON);
    while (1) {
      //        checkISP();
        lcdFill(0);
	uint32_t button=(stepmode?getInputWait():getInput());
	if(button!=BTN_ENTER) randdensity=0;
	switch(button) {
	case BTN_DOWN:
	  stepmode=1;
	  nextledcycle();
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
