#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"

#include "lcd/print.h"

#include "usb/usbmsc.h"

#include "filesystem/ff.h"
#include "filesystem/select.h"

#include "basic/xxtea.h"

#include "SECRETS"

//extern void * sram_top;

/**************************************************************************/

uint8_t execute_file (const char * fname){
    FRESULT res;
    FIL file;
    UINT readbytes;
    void (*dst)(void);

    /* XXX: why doesn't this work? sram_top contains garbage?
    dst=(void (*)(void)) (sram_top); 
    lcdPrint("T:"); lcdPrintIntHex(dst); lcdNl();
    */
    dst=(void (*)(void)) (0x10002000 - RAMCODE);

    res=f_open(&file, fname, FA_OPEN_EXISTING|FA_READ);

    //lcdPrint("open: ");
    //lcdPrintln(f_get_rc_string(res));
    //lcdRefresh();
    if(res){
        return -1;
    };
    
    res = f_read(&file, (char *)dst, RAMCODE, &readbytes);
    //lcdPrint("read: ");
    //lcdPrintln(f_get_rc_string(res));
    //lcdRefresh();
    if(res){
        return -1;
    };
#ifdef ENCRYPT_L0DABLE
    uint32_t *data;
    uint32_t len;
    uint32_t mac[4];
    data = (uint32_t*)dst;
    len = readbytes/4;

    if( readbytes & 0xF ){
        lcdClear();
        lcdPrint("!size");
        lcdRefresh();
        getInputWait();
        getInputWaitRelease();
        return -1;
    }

    xxtea_cbcmac(mac, (uint32_t*)dst, len-4, l0dable_sign_key);
    if( data[len-4] != mac[0] || data[len-3] != mac[1]
        || data[len-2] != mac[2] || data[len-1] != mac[3] ){
        lcdClear();
        lcdPrint("!mac");lcdNl();
        //lcdPrintIntHex(mac[0]); lcdNl();
        //lcdPrintIntHex(mac[1]); lcdNl();
        //lcdPrintIntHex(mac[2]); lcdNl();
        //lcdPrintIntHex(mac[3]); lcdNl();
        lcdRefresh();
        getInputWait();
        getInputWaitRelease();
        return -1;
    }
    data = (uint32_t*)dst;
    len = readbytes/4;
    xxtea_decode_words(data, len-4, l0dable_crypt_key);
#endif

    dst=(void (*)(void)) ((uint32_t)(dst) | 1); // Enable Thumb mode!
    dst();
    return 0;

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

