#include <sysinit.h>
#include <sysdefs.h>
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

void ReinvokeISP(void);
void EnableWatchdog(uint32_t ms);
void delayms(uint32_t ms);

/**************************************************************************/
struct mb {
    double rmin, rmax, imin, imax;
    bool dirty;
} mandel;

void mandelInit() {
    mandel.rmin = -2.2*0.9;
    mandel.rmax = 1.0*0.9;
    mandel.imin = -2.0*0.9;
    mandel.imax = 2.0*0.9;
    mandel.dirty = true;
}

void mandelMove() {
    const double factor = 0.1;
    double delta_r = (mandel.rmax - mandel.rmin)*factor;
    double delta_i = (mandel.imax - mandel.imin)*factor;
    
    
    if(gpioGetValue(RB_BTN0)==0) {
        mandel.imax -= delta_i;
        mandel.imin -= delta_i;
        mandel.dirty = true;
    } else if (gpioGetValue(RB_BTN1)==0) {
        mandel.imax += delta_i;
        mandel.imin += delta_i;
        mandel.dirty = true;
     } else if (gpioGetValue(RB_BTN2)==0) {
        mandel.rmax += delta_r;
        mandel.rmin += delta_r;
         mandel.dirty = true;
     } else if (gpioGetValue(RB_BTN3)==0) {
        mandel.rmax -= delta_r;
        mandel.rmin -= delta_r;
         mandel.dirty = true;
     } else if (gpioGetValue(RB_BTN4)==0) {
        mandel.imin = mandel.imin + (mandel.imax-mandel.imin)*(1-ZOOM_RATIO);
        mandel.imax = mandel.imax - (mandel.imax-mandel.imin)*(1-ZOOM_RATIO);
        mandel.rmin = mandel.rmin +(mandel.rmax-mandel.rmin)*(1-ZOOM_RATIO);
        mandel.rmax = mandel.rmax -(mandel.rmax-mandel.rmin)*(1-ZOOM_RATIO);
         mandel.dirty = true;
     }
}
    
void mandelCalc() {
    long r0,i0,rn, p,q;
    double rs,is;
    int iteration;
    char x, y;
    rs=(mandel.rmax-mandel.rmin)/68.0; 
    is=(mandel.imax-mandel.imin)/96.0;
    
    for (x=0; x<RESX; x++){
        for (y=0; y<RESY; y++) {           
            p=fixpt(mandel.rmin+y*rs);
            q=fixpt(mandel.imin+x*is);
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

void checkISP(void) {
    if(gpioGetValue(RB_BTN0)==0 && gpioGetValue(RB_BTN4)==0){
        gpioSetValue (RB_LED1, CFG_LED_ON); 
        delayms(200);
        gpioSetValue (RB_LED1, CFG_LED_OFF); 
        while(gpioGetValue(RB_BTN0)==0);
        EnableWatchdog(1000*5);
        ReinvokeISP();
    }
}

void cross(char x, char y) {
    lcdSetPixel(x,y,false);
    lcdSetPixel(x+1,y,true);
    lcdSetPixel(x-1,y,true);
    lcdSetPixel(x,y+1,true);
    lcdSetPixel(x,y-1,true);
    lcdSetPixel(x+2,y,false);
    lcdSetPixel(x-2,y,false);
    lcdSetPixel(x,y+2,false);
    lcdSetPixel(x,y-2,false);
    lcdDisplay(0);
}

void blink(){
    gpioSetValue (RB_LED1, CFG_LED_ON); 
    delayms(100);
    gpioSetValue (RB_LED1, CFG_LED_OFF); 
}

void module_mandelbrot(void) {
    gpioSetValue (RB_LED1, CFG_LED_OFF); 
    backlightInit();
    
    bool autozoom = false;
    double i_center=0, r_center=0;
    double zoom = 1;
    int x_center = 45;
    int y_center= 20;
    
    mandelInit();
    while (1) {
        checkISP();
         
        mandelMove();
        
        if (mandel.dirty) mandelCalc();
        lcdDisplay(0);
        
        //TODO fix this
        if (!autozoom) {
            continue;
        }
        
        bool selected_val = lcdGetPixel(x_center, y_center);
        for (int delta = 0; delta<100; delta++){
            double dist = 0;
            if (x_center + delta < RESX && lcdGetPixel(x_center+delta,y_center) != selected_val && dist <=2){
                x_center +=delta-1;
                
                break;
            }
            if (x_center - delta >= 0 && lcdGetPixel(x_center-delta,y_center) != selected_val && dist <=2){
                x_center -=delta+1;
                break;
            }
            if (y_center + delta < RESY && lcdGetPixel(x_center, y_center+delta) != selected_val && dist <=2){
                y_center +=delta-1;
                break;
            }
            if (y_center -delta >= 0 && lcdGetPixel(x_center, y_center-delta) != selected_val && dist <=2) {
                y_center -= delta+1;
                break;
            }
        }
        cross(x_center, y_center);
        
        
        i_center = x_center/96.0 -0.5;
        r_center = y_center/68.0 -0.5;
        
        double i_off = (mandel.imax-mandel.imin)*i_center;
        double r_off = (mandel.rmax-mandel.rmin)*r_center;

        mandel.imin += i_off*1.0;
        mandel.imax += i_off*1.0;
        mandel.rmin += r_off*1.0;
        mandel.rmax += r_off*1.0;
        
        mandel.imin = mandel.imin + (mandel.imax-mandel.imin)*(1-zoom);
        mandel.imax = mandel.imax - (mandel.imax-mandel.imin)*(1-zoom);
        mandel.rmin = mandel.rmin +(mandel.rmax-mandel.rmin)*(1-zoom);
        mandel.rmax = mandel.rmax -(mandel.rmax-mandel.rmin)*(1-zoom);
        
        zoom *= ZOOM_RATIO;
        //iteration_max = iteration_max*1.1;
        
        x_center = RESX/2;
        y_center= RESY/2;
        
        cross(x_center,y_center);
    }
    return;
}