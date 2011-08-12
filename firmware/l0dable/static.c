#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"
#include "basic/random.h"

#include "lcd/render.h"
#include "lcd/display.h"

#include "lcd/fonts.h"
#include "lcd/fonts/invaders.h"

#include "funk/mesh.h"

#include "usetable.h"


void ram(void)
{
   int x, y;

   while( getInputRaw() == BTN_NONE ) {
       for( x = 0; x < RESX; x++ ) {
           for( y = 0; y < RESY_B; y++ ) {
               lcdBuffer[y*RESX+x]=getRandom()&0xff;
         }
      }
      lcdDisplay();
   }
   return;
}

