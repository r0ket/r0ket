#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"

#include "lcd/print.h"

#include "usb/usbmsc.h"

#include "filesystem/ff.h"
#include "filesystem/select.h"

extern void * sram_top;

/**************************************************************************/

void execute_file (const char * fname){
    FRESULT res;
    FIL file;
    UINT readbytes;
    void (*dst)(void);

    /* XXX: why doesn't this work? sram_top contains garbage?
    dst=(void (*)(void)) (sram_top); 
    lcdPrint("T:"); lcdPrintIntHex(dst); lcdNl();
    */
    dst=(void (*)(void)) 0x10001800;

    res=f_open(&file, fname, FA_OPEN_EXISTING|FA_READ);
    //lcdPrint("open: ");
    //lcdPrintln(f_get_rc_string(res));
    //lcdRefresh();
    if(res){
        return;
    };

    res = f_read(&file, (char *)dst, RAMCODE, &readbytes);
    //lcdPrint("read: ");
    //lcdPrintln(f_get_rc_string(res));
    //lcdRefresh();
    if(res){
        return;
    };

    //lcdPrintInt(readbytes);
    //lcdPrintln(" bytes");
    //lcdRefresh();

    dst=(void (*)(void)) ((uint32_t)(dst) | 1); // Enable Thumb mode!
    dst();

};

/**************************************************************************/

void executeSelect(char *ext){
    char filename[15];
    filename[0]='0';
    filename[1]=':';
    filename[2]=0;

    if( selectFile(filename+2,ext) == 0)
        execute_file(filename);
};

