#include <string.h>
#include "sysdefs.h"
#include "lcd/lcd.h"
#include "lcd/fonts/smallfonts.h"
#include "lcd/print.h"
#include "filesystem/ff.h"
#include "basic/basic.h"

#define FLEN 13

int getFiles(char files[][FLEN], uint8_t count, uint16_t skip, char *ext)
{
    DIR dir;                /* Directory object */
    FILINFO Finfo;
    FRESULT res;
    int pos = 0;
    int extlen = strlen(ext);
    res = f_opendir(&dir, "0:");
    if(res){
        //lcdPrint("OpenDir:"); lcdPrintln(f_get_rc_string(res)); lcdRefresh(); 
        return 0;
    };
    while(f_readdir(&dir, &Finfo) == FR_OK && Finfo.fname[0]){
        int len=strlen(Finfo.fname);

        if(len<extlen)
            continue;

        if( strcmp(Finfo.fname+len-extlen, ext) != 0)
            continue;

        if (Finfo.fattrib & AM_DIR)
            continue;

        if( skip>0 ){
            skip--;
            continue;
        };

        strcpy(files[pos++],Finfo.fname);
        if( pos == count )
            break;
    }
    return pos;
}

#define PERPAGE 7
int selectFile(char *filename, char *extension)
{
    int skip = 0;
    char key;
    int selected = 0;
    font=&Font_7x8;
    while(1){
        char files[PERPAGE][FLEN];
        int count = getFiles(files, PERPAGE, skip, extension);
        if(!count){
            lcdPrintln("No Files?");
            lcdRefresh();
            getInputWait();
            getInputWaitRelease();
            return -1;
        };

        if(count<PERPAGE && selected==count){
            skip--;
            continue;
        };
        
        redraw:
        lcdClear();
        lcdPrint("Select file:");
        lcdSetCrsrX(40);
        lcdPrint(IntToStr(skip,1,0));
        lcdPrint("/");
        lcdPrint(IntToStr(selected,1,0));
        lcdNl();
        for(int i=0; i<count; i++){
            if( selected == i )
                lcdPrint("*");
            else
                lcdPrint(" ");
            lcdPrintln(files[i]);
        }
        lcdRefresh();
        key=getInputWait();
        getInputWaitRelease();
        switch(key){
            case BTN_DOWN:
                if( selected < count-1 ){
                    selected++;
                    goto redraw;
                }else{
                    skip++;
                }
                break;
            case BTN_UP:
                if( selected > 0 ){
                    selected--;
                    goto redraw;
                }else{
                    if( skip > 0 ){
                        skip--;
                    }
                }
                break;
            case BTN_LEFT:
                return -1;
            case BTN_ENTER:
            case BTN_RIGHT:
                strcpy(filename, files[selected]);
                return 0;
        }
    }
}
