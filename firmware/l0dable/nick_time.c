#include <sysinit.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/lcd.h"
#include "lcd/print.h"

#include "usetable.h"

void ram(void)
{
   struct tm* tm;
   char timestr[9]="00:00:00";
   int dx1=0;
   int dx2=0;
   int dy1=0;
   int dy2=0;
   setExtFont(GLOBAL(nickfont));

   dx1=DoString(0,0,GLOBAL(nickname));
   dx1=(RESX-dx1)/2;
   if(dx1<0)
   {
      dx1=0;
   }
   dy1=(RESY/2-getFontHeight())/2;
   dy2=RESY/2 + dy1;
   while(getInputRaw()==BTN_NONE)
   {
      tm = mygmtime(getSeconds());
    
      timestr[0] = '0' + tm->tm_hour / 10;
      timestr[1] = '0' + tm->tm_hour % 10;

      timestr[3] = '0' + tm->tm_min / 10;
      timestr[4] = '0' + tm->tm_min % 10;

      timestr[6] = '0' + tm->tm_sec / 10;
      timestr[7] = '0' + tm->tm_sec % 10;
      
      dx2=DoString(0,0,&timestr[0]);
      dx2=(RESX-dx2)/2;
      if(dx2<0)
      {
         dx2=0;
      }
      
      lcdClear();
      
      DoString(dx1,dy1,GLOBAL(nickname));
      DoString(dx2,dy2,&timestr[0]);
    
      lcdRefresh();
      delayms_queue_plus(10,0);
   }
   return;
}
