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
    int ctr;
    int pos = 0;
    res = f_opendir(&dir, "0:");
    if(res){
        //lcdPrint("OpenDir:"); lcdPrintln(f_get_rc_string(res)); lcdRefresh(); 
        return 0;
    };
    ctr=0;
    while(1){
        res = f_readdir(&dir, &Finfo);
        if ((res != FR_OK) || !Finfo.fname[0])
            break;

        int len=strlen(Finfo.fname);
        int extlen = strlen(ext);

        if( strcmp(Finfo.fname+len-extlen, ext) != 0)
            continue;
        if (Finfo.fattrib & AM_DIR)
            continue;

        if( ctr++ < skip )
            continue;
        strcpy(files[pos++],Finfo.fname);
        if( pos == count )
            break;
    }
    return pos;
}

int selectFile(char *filename, char *extension)
{
    int skip = 0;
    char key;
    int selected = 0;
    font=&Font_7x8;
    while(1){
        char files[7][FLEN];
        int count = getFiles(files, 7, skip, extension);
        
        redraw:
        if( count )
            lcdClear();
        lcdPrintln("Select file:");
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
        if( key==BTN_DOWN ){
            if( selected < count-1 ){
                selected++;
                goto redraw;
            }else{
                skip++;
            }
        }else if( key==BTN_UP ){
            if( selected > 0 ){
                selected--;
                goto redraw;
            }else{
                if( skip > 0 ){
                    skip--;
                }
            }
        }else if( key==BTN_LEFT ){
            return 1;
        }else if( key==BTN_RIGHT ){
            strcpy(filename, files[selected]);
            return 0;
        }
    }
}
