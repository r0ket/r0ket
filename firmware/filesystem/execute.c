#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"

#include "lcd/print.h"

#include "usb/usbmsc.h"

#include "filesystem/ff.h"
#include "filesystem/select.h"


const uint32_t signature_key[4] = {0,0,0,0};

extern void * sram_top;

/**************************************************************************/

void execute_file (const char * fname, uint8_t checksignature){
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
    
    if( checksignature ){
        uint32_t mac[4];
        uint32_t *data = (uint32_t*)dst;
        uint32_t len = readbytes/4;

        xxtea_cbcmac(mac, (uint32_t*)dst, len-4, signature_key);
        if( data[len-4] != mac[0] || data[len-3] != mac[1]
            || data[len-2] != mac[2] || data[len-1] != mac[3] ){
            return;
        }
    }
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
        execute_file(filename,0);
};

