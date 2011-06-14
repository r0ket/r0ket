#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/display.h"
#include "lcd/allfonts.h"

#define FIXSIZE 25
#define mul(a,b) ((((long long)a)*(b))>>FIXSIZE)
#define fixpt(a) ((long)(((a)*(1<<FIXSIZE))))
#define integer(a) (((a)+(1<<(FIXSIZE-1)))>>FIXSIZE)

void ReinvokeISP(void);
void EnableWatchdog(uint32_t ms);
void delayms(uint32_t ms);

/**************************************************************************/
void checkISP(void) {
    if(gpioGetValue(RB_BTN0)==0){
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

void module_mandelbrot(void) {
    gpioSetValue (RB_LED1, CFG_LED_OFF); 
    backlightInit();
    
    bool toggle = false;
    int counter = 0;
    
    long r0,i0,p,q,rn,tot;
    //double xmin=-2.5,ymin=-1.5,xmax=1.5,ymax=1.5,xs,ys;
    
    double rmin0=-2.2*0.9, imin0=-2.0*0.9, rmax0=1.0*0.9, imax0=2.0*0.9;
    double i_center=0, r_center=0;
    double rmin=rmin0,imin=imin0,rmax=rmax0,imax=imax0,rs,is;
    int iteration,r,i;
    double zoom = 1;
    int iteration_max = 300;
    int x_center = 45;
    int y_center= 40;
    while (1) {
        checkISP();
        lcdDisplay(0);
        delayms(100);
        
        rs=(rmax-rmin)/68.0; 
        is=(imax-imin)/96.0; 
        
        
        for (r=0;r<RESY;r++) {
            for (i=0;i<RESX;i++) {
                p=fixpt(rmin+r*rs);
                q=fixpt(imin+i*is);
                rn=0;
                r0=0;
                i0=0;
                iteration=0;
                while ((mul(rn,rn)+mul(i0,i0))<fixpt(4) && ++iteration<iteration_max)  {
                    rn=mul((r0+i0),(r0-i0)) +p;           
                    i0=mul(fixpt(2),mul(r0,i0)) +q;
                    r0=rn;
                }
                tot+=iteration;
                if (iteration==iteration_max) iteration=1;
                bool pixel = ( iteration>1);
                
                lcdSetPixel (i,r,pixel);
                //lcdSetPixel ((RESX-1)-i,r,pixel); 
                checkISP();
            }
        }
        lcdDisplay(0);
        
        //for (int x=0; x<RESX;x++){
        //    for(int y=0; y<RESY;y++){
        //        bool p = lcdGetPixel(x,y);
        //        lcdSetPixel(x,y,~p);
        //        lcdDisplay(0);
        //   }
        //}
        
        
        bool selected_val = lcdGetPixel(x_center, y_center);
        for (int delta = 0; delta<100; delta++){
            double test_i = imin + (imax-imin)*x_center/96.0;
            double test_r = rmin +(imax-rmin)*y_center/68.0;
            //double dist = test_i * test_i + test_r * test_r;
            //double dist = ((i_center - test_i)*(i_center - test_i) + (r_center - test_r)*(r_center - test_r))*1.0/zoom;
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
        
        double i_off = (imax-imin)*i_center;
        double r_off = (rmax-rmin)*r_center;

        imin += i_off*1.0;
        imax += i_off*1.0;
        rmin += r_off*1.0;
        rmax += r_off*1.0;
        
        imin = imin + (imax-imin)*(1-zoom);
        imax = imax - (imax-imin)*(1-zoom);
        rmin = rmin +(rmax-rmin)*(1-zoom);
        rmax = rmax -(rmax-rmin)*(1-zoom);
        //imin*=zoom;
        //imax*=zoom;
        //rmin*=zoom;
        //rmax*=zoom;
        
        zoom *= 0.995;
        iteration_max = iteration_max*1.02;
        
        x_center = RESX/2;
        y_center= RESY/2;
        
        cross(x_center,y_center);
        if (counter > 24) {
            if (toggle) {
                toggle = false;
            } else {
                toggle = true;
            }
            counter = 0;
        } else {
            counter ++;
        }
        
    }
    return;
}