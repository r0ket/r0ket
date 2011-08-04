#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/display.h"

#include "basic/config.h"

#include "usetable.h"

#define BITSET_X (RESX+2)
#define BITSET_Y (RESY+2)
#define BITSET_SIZE (BITSET_X*BITSET_Y)

#define BITSETCHUNKSIZE 32

#define one ((uint32_t)1)

struct bitset {
  uint16_t size;
  uint32_t bits[BITSET_SIZE/BITSETCHUNKSIZE+1];
};


typedef uint8_t uchar;

int pattern=0;
#define PATTERNCOUNT 3

uchar stepmode=0;
uchar randdensity=0;
//uint8_t bl=0;

struct bitset _buf1,*buf1=&_buf1;
struct bitset _buf2,*buf2=&_buf2;

struct bitset *life =&_buf1;
struct bitset *new =&_buf2;


static void draw_area();
static void calc_area();
static void random_area(struct bitset *area, uchar x0, uchar y0, uchar x1, uchar y1,uchar value);
static void reset_area();
static void nextledcycle();

void ram(void) {
    getInputWaitRelease();
    reset_area();
    random_area(life,1,1,RESX,RESY,40);

    lcdClear();
    setExtFont(GLOBAL(nickfont));
    DoString(20,20,GLOBAL(nickname));

#if 0
    gpioSetValue (RB_LED0, CFG_LED_ON);
    gpioSetValue (RB_LED1, CFG_LED_ON);
    gpioSetValue (RB_LED2, CFG_LED_ON);
    gpioSetValue (RB_LED3, CFG_LED_ON);
#endif
    while (1) {
        draw_area(); // xor life pattern over display content
        lcdDisplay();
        draw_area(); // xor life pattern again to restore original display content
        lcdShift(1,-2,1);
        if(getInputRaw())
            return;
        delayms_queue_plus(10,0);
        calc_area();
    }
    return;
}

static inline void bitset_set(struct bitset *bs,uint16_t index, uint8_t value) {
  uint16_t base=index/BITSETCHUNKSIZE;
  uint16_t offset=index%BITSETCHUNKSIZE;
  if(value) {
    bs->bits[base]|=(one<<offset);
  } else {
    bs->bits[base]&=~(one<<offset);
  }
}

static inline void bitset_xor(struct bitset *bs,uint16_t index, uint8_t value) {
  uint16_t base=index/BITSETCHUNKSIZE;
  uint16_t offset=index%BITSETCHUNKSIZE;
  if(value) {
    bs->bits[base]^=(one<<offset);
  }
}

static inline uint8_t bitset_get(struct bitset *bs,uint16_t index) {
  uint16_t base=index/BITSETCHUNKSIZE;
  uint16_t offset=index%BITSETCHUNKSIZE;
  return (bs->bits[base]&(one<<offset))==(one<<offset);;
}

static inline uint16_t bitset_offset2(uint8_t x, uint8_t y) {
  return ((uint16_t)x)+((uint16_t)y)*BITSET_X;
}

static inline void bitset_set2(struct bitset *bs, uint8_t x, uint8_t y, uint8_t value) {
  bitset_set(bs,bitset_offset2(x,y),value);
}

static inline void bitset_xor2(struct bitset *bs, uint8_t x, uint8_t y, uint8_t value) {
  bitset_xor(bs,bitset_offset2(x,y),value);
}

static inline uint8_t bitset_get2(struct bitset *bs,uint8_t x,uint8_t y) {
  return bitset_get(bs,bitset_offset2(x,y));
}

static void draw_rect(char x0, char y0, char x1, char y1) {
  for(char x=x0; x<=x1; ++x) {
    lcdSetPixel(x,y0,true);
    lcdSetPixel(x,y1,true);
  }
  for(char y=y0+1; y<y1; ++y) {
    lcdSetPixel(x0,y,true);
    lcdSetPixel(x1,y,true);
  }
} 

static void fill_rect(char x0, char y0, char x1, char y1) {
  for(char x=x0; x<=x1; ++x) {
    for(char y=y0; y<=y1; ++y) {
      lcdSetPixel(x,y,true);
    }
  }
} 

#define STARTVALUE 10
static void swap_areas() {
  struct bitset *tmp=life;
  life=new;
  new=tmp;
}

static void fill_area(struct bitset *area, uchar x0, uchar y0, uchar x1, uchar y1,uchar value) {
  for(uchar x=x0; x<=x1; ++x) {
    for(uchar y=y0; y<=y1; ++y) {
      bitset_set2(area,x,y,value);
    }
  }
} 

static bool find_area(struct bitset *area, uchar x0, uchar y0, uchar x1, uchar y1,uchar value) {
  for(uchar x=x0; x<=x1; ++x) {
    for(uchar y=y0; y<=y1; ++y) {
      if(bitset_get2(area,x,y)==value) return true;
    }
  }
  return false;
} 

static uint32_t sum_area(struct bitset *area, uchar x0, uchar y0, uchar x1, uchar y1) {
  uint32_t sum=0; 
  for(uchar x=x0; x<=x1; ++x) {
    for(uchar y=y0; y<=y1; ++y) {
      sum+=bitset_get2(area,x,y);
    }
  }
  return sum;
} 

static void draw_area() {
  for(uchar x=0; x<RESX; ++x) {
    for(uchar y=0; y<RESY; ++y) {
      lcdSetPixel(x,y,lcdGetPixel(x,y)^bitset_get2(life,x+1,y+1));
    }
  }
}

static void calc_area() {
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

static void reset_area() {
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
#if 0
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
#endif
    }
}

static void random_area(struct bitset *area, uchar x0, uchar y0, uchar x1, uchar y1,uchar value) {
  for(uchar x=x0; x<=x1; ++x) {
    for(uchar y=y0; y<=y1; ++y) {
      bitset_set2(area,x,y,(getRandom()>>24)<value);
    }
  }
}

#define LEDINTERVAL 1
static void nextledcycle() {
    static uint8_t ledcycle=3;
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
