#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/display.h"
#include "lcd/allfonts.h"

#define FIXSIZE 25
#define mul(a,b) ((((long long)a)*(b))>>FIXSIZE)
#define fixpt(a) ((long)(((a)*(1<<FIXSIZE))))
#define integer(a) (((a)+(1<<(FIXSIZE-1)))>>FIXSIZE)

#define ZOOM_RATIO 0.90
#define ITERATION_MAX 150

void blink(){
    gpioSetValue (RB_LED1, CFG_LED_OFF); 
    delayms(100);
    gpioSetValue (RB_LED1, CFG_LED_ON); 
    delayms(100);
    gpioSetValue (RB_LED1, CFG_LED_OFF); 
}

struct mb {
    long rmin, rmax, imin, imax;
    bool dirty;
} mandel;

void mandelInit() {
    //mandel.rmin = -2.2*0.9;
    //mandel.rmax = 1.0*0.9;
    //mandel.imin = -2.0*0.9;
    //mandel.imax = 2.0*0.9;
    mandel.rmin = fixpt(-2);
    mandel.rmax = fixpt(1);
    mandel.imin = fixpt(-2);
    mandel.imax = fixpt(2);
    
    mandel.dirty = true;
}

void mandelMove() {
    long delta_r = (mandel.rmax - mandel.rmin)/10;
    long delta_i = (mandel.imax - mandel.imin)/10;
    char key = getInputRaw();
    
    if(key == BTN_LEFT) {
        mandel.imax -= delta_i;
        mandel.imin -= delta_i;
        mandel.dirty = true;
    } else if (key == BTN_RIGHT) {
        mandel.imax += delta_i;
        mandel.imin += delta_i;
        mandel.dirty = true;
     } else if (key == BTN_DOWN) {
        mandel.rmax += delta_r;
        mandel.rmin += delta_r;
         mandel.dirty = true;
     } else if (key == BTN_UP) {
        mandel.rmax -= delta_r;
        mandel.rmin -= delta_r;
         mandel.dirty = true;
     } else if (key == BTN_ENTER) {
        mandel.imin = mandel.imin + (mandel.imax-mandel.imin)/10;
        mandel.imax = mandel.imax - (mandel.imax-mandel.imin)/10;
        mandel.rmin = mandel.rmin +(mandel.rmax-mandel.rmin)/10;
        mandel.rmax = mandel.rmax -(mandel.rmax-mandel.rmin)/10;
         mandel.dirty = true;
     }
     
}
    
void mandelCalc() {
    long r0,i0,rn, p,q;
    long rs,is;
    int iteration;
    char x, y;
    rs=(mandel.rmax-mandel.rmin)/RESY; 
    is=(mandel.imax-mandel.imin)/RESX;
    
    for (x=0; x<RESX; x++){
        for (y=0; y<RESY; y++) { 
            p=mandel.rmin+y*rs;
            q=mandel.imin+x*is;
            rn=0;
            r0=0;
            i0=0;
            iteration=0;
            while ((mul(rn,rn)+mul(i0,i0))<fixpt(4) && ++iteration<ITERATION_MAX)  {
                rn=mul((r0+i0),(r0-i0)) +p;           
                i0=mul(fixpt(2),mul(r0,i0)) +q;
                r0=rn;
            }
            if (iteration==ITERATION_MAX) iteration=1;
            bool pixel = ( iteration>1);
            lcdSetPixel(x, y, pixel);
        }
    }
    mandel.dirty = false;
}

void main_mandelbrot2(void) {
    backlightInit();
    IOCON_PIO3_3 = 0x10;
    font=&Font_7x8;
    mandelInit();
    while (1) {
        lcdDisplay(0);
        mandelMove();
        if (mandel.dirty) {
            mandelCalc();
        }
        
        if(gpioGetValue(RB_BTN0)==0 && gpioGetValue(RB_BTN4)==0){
        DoString(0,0,"Enter ISP!");
        lcdDisplay(0);
        ISPandReset(5);
    }
    
    }
    return;
}

void tick_mandelbrot2(void){
    static int foo=0;
    static int toggle=0;
	if(foo++>50){
        toggle=1-toggle;
		foo=0;
        gpioSetValue (RB_LED0, toggle); 
	};
    
    return;
};
