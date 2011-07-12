#include <display.h>
#include <render.h>
#include <fonts.h>

int x=0;
int y=0;

void lcdPrint(const char *string){
  x=DoString(x,y,string);
};

void lcdNl(void){
  x=0;y+=font->u8Height;
};

void lcdPrintln(const char *string){
  lcdPrint(string);
  lcdNl();
};
  
void lcdPrintInt(const int num){
  x=DoInt(x,y,num);
};

void lcdPrintIntHex(const int num){
  x=DoIntX(x,y,num);
};

void lcdClear(){
  x=0;y=0;
  lcdFill(0);
};

void lcdRefresh(){
    lcdDisplay(0);
};
