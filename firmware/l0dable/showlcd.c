#include <sysinit.h>

#include "basic/basic.h"
#include "lcd/lcd.h"
#include "lcd/print.h"
#include "filesystem/ff.h"

#include "usetable.h"

//shows *.lcd
void ram(void)
{
    char filename[13];
    char key;
    int i=500;

    // Image or Animation?
    lcdClear();
    lcdPrintln("Image | Anim?");
    lcdPrintln("Up=Image");
    lcdPrintln("Down=Anim");
    lcdRefresh();  
    key = getInputWait();
    delayms(230);

    // Show file
    if( key == BTN_DOWN ){
        // Choose animation speed
        while ( key != BTN_ENTER ){
            lcdClear();
            lcdPrintln("Choose speed:");
            lcdPrintln("");
            lcdPrint("   ");
            lcdPrint(IntToStr(i,5,0));
            lcdPrintln("ms");
            lcdRefresh();
            key = getInputWait();
            if ( key == BTN_UP ){
                if ( i < 60000 )
                    i+=10;
            } else if ( key == BTN_DOWN ){
                if ( i > 10 )
                    i-=10;
            }
            delayms(100);                
        }
        // Select file
        lcdClear();
        delayms(230);
        selectFile(filename,"LCD");
        // Load as animation
        lcdShowAnim(filename, i);
    } else if ( key == BTN_UP ){    
        // Select file
        lcdClear();
        delayms(230);
        selectFile(filename,"LCD");
        // Load as image
        lcdLoadImage(filename);
        lcdRefresh();
    } else {
        // Exit
        return;
    }
    getInputWait();
}

