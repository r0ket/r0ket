#include <iostream>
#include <time.h>
#include <QApplication>
#include <QImage>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPainter>
#include <QPushButton>
#include <QStatusBar>
#include <QTextEdit>
#include <QThread>
#include <cstdio>
#include <pthread.h>

using namespace std;

#include "simulat0rthread.h"

extern "C" {
#include "basic/basic.h"
#include "lcd/backlight.h"

#define lcdGetPixel __hideaway_lcdGetPixel
#include "lcd/display.h"
#undef lcdGetPixel

#include "../simcore/simulator.h"
extern int lcd_layout;

#define ptrXor(a,b) ((uint8_t*)(((uintptr_t)a)^((uintptr_t)b))) 

uint8_t guiBuffer1[RESX*RESY_B];
uint8_t guiBuffer2[RESX*RESY_B];
uint8_t *guiBuffer=guiBuffer1;
  uint8_t *xorBuffer=ptrXor(guiBuffer1,guiBuffer2);

static  bool lcdGetPixel(char x, char y){
  char y_byte = (RESY-(y+1)) / 8;
  char y_off = (RESY-(y+1)) % 8;
  char byte = guiBuffer[y_byte*RESX+(RESX-(x+1))];
  return byte & (1 << y_off);
}

}

time_t starttime;
long framecount=0;

const int colorGreenLED=QColor(0,255,0).rgb();
const int colorRedLED=QColor(255,0,0).rgb();
const int colorOffLED=QColor(64,64,64).rgb();

const int colorPixelOn=QColor(255,192,0).rgb();
const int colorPixelOff=QColor(64,64,64).rgb();
const int colorInvertedPixelOn=QColor(128,128,128).rgb(); // inverted and on => dark
const int colorInvertedPixelOff=QColor(128,255,128).rgb(); // inverted and off => bright

class LCD : public QWidget {
public:
  static const int ledsize=10;
  static const int ledsep=1;
  static const int paddingx=10;
  static const int paddingy=10;
  static const int pixh=3;
  static const int pixw=3;
  static const int sep=1;
  static const int rasterx=pixh+sep;
  static const int rastery=pixw+sep;
  static const int dimx=RESX; //96;
  static const int dimy=RESY;

  void drawLED(QImage& pixmap,int led, int x, int y,int colorOn) {
    int color=simGetLED(led)?colorOn:colorOffLED;
    for(int minix=0; minix<ledsize; ++minix) {
      for(int miniy=0; miniy<ledsize; ++miniy) {
	pixmap.setPixel(x+minix,y+miniy,color);
      }
    }
  }

  void paintEvent(QPaintEvent *) {
    QImage pixmap(dimx*rasterx,dimy*rastery+2*ledsize+2*ledsep,QImage::Format_ARGB4444_Premultiplied);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(paddingx,paddingy);
    painter.scale(1,1);
    QPoint pts[dimx*dimy];

    // draw backlight
    const int backlight=backlightGetBrightness()*255/100;
    pixmap.setPixel(1,1,(QColor(backlight,backlight,backlight).rgb()));
    pixmap.fill(pixmap.pixel(1,1));

    // draw lcd array
    for (int x = 0; x < dimx; ++x) {
      for(int y=0; y<dimy; ++y) {
	int color;
	if(globalconfig.lcdstate & LCD_INVERTED) {
	  color=lcdGetPixel((globalconfig.lcdstate & LCD_MIRRORX)?(RESX-x-1):x,(globalconfig.lcdstate & LCD_MIRRORY)?(RESY-y-1):y)?colorInvertedPixelOn:colorInvertedPixelOff;
	} else {
	  color=lcdGetPixel((globalconfig.lcdstate & LCD_MIRRORX)?(RESX-x-1):x,(globalconfig.lcdstate & LCD_MIRRORY)?(RESY-y-1):y)?colorPixelOn:colorPixelOff;
	}
	for(int minix=0; minix<pixw; ++minix) {
	  for(int miniy=0; miniy<pixh; ++miniy) {
	    pixmap.setPixel(x*rasterx+minix,ledsize+ledsep+y*rastery+miniy,color);
	  }
	}
      }
    }

    // draw leds
    const int x1=dimx*rasterx-1-ledsize;
    const int y1=dimy*rastery-1+2*ledsep+ledsize;
    drawLED(pixmap,0,x1,y1,colorGreenLED);
    drawLED(pixmap,1,0,0,colorGreenLED);
    drawLED(pixmap,2,0,y1,colorGreenLED);
    drawLED(pixmap,3,x1,0,colorRedLED);

    // finally output pixmap
    painter.drawImage(0,0,pixmap);
  }

};

QWidget* hackptr;

class R0ketMainWindow;
R0ketMainWindow *r0ketWidget;

class R0ketMainWindow : public QMainWindow {
public:
  static const int keyUp=16777235;
  static const int keyDown=16777237;
  static const int keyEnter=16777227;
  static const int keyAltEnter=32;     // Space
  static const int keyLeft=16777234;
  static const int keyRight=16777236;
  LCD* te;
  int buttonState;

  R0ketMainWindow() {
    buttonState=BTN_UP|BTN_DOWN|BTN_ENTER|BTN_LEFT|BTN_RIGHT;
    te=new LCD(); //TextEdit();
    hackptr=te;
    r0ketWidget=this;
    setCentralWidget(te);
    statusBar()->showMessage("Initialized",5000);
    statusBar()->showMessage("Temp",2000);
    resize(2*LCD::paddingx+LCD::rasterx*RESX,2*LCD::paddingy+LCD::rastery*RESY+2*LCD::ledsize+2*LCD::ledsep+statusBar()->height());

    show();
    setWindowTitle("r0ket simulat0r");
  }

  int buttonPressed(int button) {
    return getAllBits(buttonState,button);
  }

  int getAllBits(int i, int bit) {    
    return (i&bit)==bit;
  }

  void setBit(int & i, int bit) {
    statusBar()->showMessage("Set bit ",2000);
    i=i|bit;
    //    setLowLevelButtonState(bit,1);
  }

  void clearBit(int & i, int bit) {
    statusBar()->showMessage("Clear bit ",2000);
    i=i&(~bit);
    //    setLowLevelButtonState(bit,0);
  }

  /*
  void setLowLevelButtonState(int button, int state) {  
    if (button==BTN_UP) gpioSetValue(RB_BTN3, state);
    if (button==BTN_DOWN) gpioSetValue(RB_BTN2, state);
    if (button==BTN_ENTER) gpioSetValue(RB_BTN4, state);
    if (button==BTN_LEFT) gpioSetValue(RB_BTN0, state);
    if (button==BTN_RIGHT) gpioSetValue(RB_BTN1, state);
  }
  */

  void keyPressEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) return;

    if(event->key()==keyUp) clearBit(buttonState,BTN_UP);
    if(event->key()==keyDown) clearBit(buttonState,BTN_DOWN);
    if(event->key()==keyEnter) clearBit(buttonState,BTN_ENTER);
    if(event->key()==keyAltEnter) clearBit(buttonState,BTN_ENTER);
    if(event->key()==keyLeft) clearBit(buttonState,BTN_LEFT);
    if(event->key()==keyRight) clearBit(buttonState,BTN_RIGHT);
  }
  void keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) return;

    if(event->key()==keyUp) setBit(buttonState,BTN_UP);
    if(event->key()==keyDown) setBit(buttonState,BTN_DOWN);
    if(event->key()==keyEnter) setBit(buttonState,BTN_ENTER);
    if(event->key()==keyAltEnter) setBit(buttonState,BTN_ENTER);
    if(event->key()==keyLeft) setBit(buttonState,BTN_LEFT);
    if(event->key()==keyRight) setBit(buttonState,BTN_RIGHT);
  }

};

extern "C" {
void simlcdDisplayUpdate() {
  uint8_t *src=lcdBuffer;
  uint8_t *dst=ptrXor(guiBuffer,xorBuffer);
  memcpy(dst,src,RESX*RESY_B);
  guiBuffer=dst;

  hackptr->update();
  usleep(10000);
}

int simButtonPressed(int button) {
  return r0ketWidget->buttonPressed(button);
}

void simSetLEDHook(int led){
  hackptr->update();
}
}

int main(int argc, char *argv[])
{
  cout<<"Starting r0ket simulat0r..."<<endl;
  starttime=time(NULL);
  QApplication app(argc, argv);
  R0ketMainWindow * mw=new R0ketMainWindow();
  Simulat0rThread *mt=new Simulat0rThread();
  mt->start();
  return app.exec();
}
