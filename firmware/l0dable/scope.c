/*
  Simple oscilliscope with auto y-scale and variable x-scale.
  Use joystick to select channel and x-scale and touch the hackbus ports
  to generate some signals to see on your new oscilliscope.
  Press the button to quit.

  Paul Gardner-Stephen paul@servalproject.org

  I hereby place this program into the public domain so far is as possible under law.
  13 August 2011.
*/
#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"
#include "lcd/render.h"
#include "lcd/display.h"
#include "funk/mesh.h"
#include "usetable.h"
#include "core/adc/adc.c"

void ram(void) {
  int alive=0;
  int ys[96];
      DoString(5,1,"Oscilliscope");
    lcdDisplay();
    
    int i;
    int ch=1;
    int kok=1;
    int xs=3,yscale=1;
    char scale[]="01234567";

    while (1) {
      lcdSetPixel(alive,9,0);
      alive++; alive&=63;
      lcdSetPixel(alive,9,1);

      int ticks=64>>yscale;
      for(i=10;i<60;i++) lcdSetPixel(7,i,0);
      for(i=0;i<ticks;i++)
	lcdSetPixel(7,10+50*i/ticks,1);

      ticks=64>>xs;
      for(i=10;i<90;i++) lcdSetPixel(i,59,0);
      for(i=0;i<ticks;i++)
	lcdSetPixel(10+80*i/ticks,59,1);

      int junk=xs;
      int x;
      int ysum;
      int gnr,lnr;

      char n[2];
      n[1]=0;
      n[0]='Y'; 
      DoString(0,32,n);
      n[0]=scale[yscale];
      DoString(0,40,n);

      n[0]='X'; 
      DoString(16,60,n);
      n[0]=xs/10+'0';
      if (n[0]=='0') n[0]=' ';
      DoString(24,60,n);
      n[0]=xs%10+'0';
      DoString(32,60,n);

      n[0]='C';
      DoString(64,60,n);
      n[0]='h';
      DoString(72,60,n);
      n[0]=ch+'0';
      DoString(80,60,n);

      int ybias=ysum/(96-8);
      ysum=0;
      if ((lnr<4)&&(yscale>1)) {
	// increase scale to fit data
	yscale--;
      } else if ((gnr>8)&&(yscale<64)) {
	// reduce scale to fit data
	yscale++;
      }
      gnr=0; lnr=0;
      for(x=8;x<96;x++) {
	int y;
	y=adcRead(ch);
	ysum+=y;
	y=y-ybias;
	y=y/yscale;
	if (y<-25||y>25) gnr++;
	else if (y<-12||y>12) lnr++;
	y+=25;
	if (y<0) y=0;
	if (y>50) y=50;
	if (y<0) y=0; if (y>31) y=31;
	if (y!=ys[x]) {
	  lcdSetPixel(x,35+(ys[x]-25),0);
	  lcdSetPixel(x,35+(y-25),1);
	  ys[x]=y;
	}

	// Crude means of delay with bits to deal with zealous optimisers
	int delay=xs*xs;
	for(i=0;i<delay;i++) lcdSetPixel(0,xs,0);

      }

      if (1) {
	int k=getInputRaw();
	switch(k) {
	case BTN_LEFT: if (kok) { xs--; xs&=63; kok=0; } break;
	case BTN_RIGHT: if (kok) { xs++; xs&=63; kok=0; } break;      
	case BTN_UP: if (kok) { ch++; ch&=7; kok=0; } break;
	case BTN_DOWN: if (kok) { ch--; ch&=7; kok=0; } break;
	case BTN_ENTER: return 0;
	default: kok=1;
	}
      }
      
      lcdDisplay();
    }
}
