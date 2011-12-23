#include <sysdefs.h>
#include <ff.h>
#include <string.h>
#include "at45db041d.h"
#include "lcd/print.h"
#include "usb/usbmsc.h"

FATFS FatFs;          /* File system object for logical drive */

const TCHAR *rcstrings =
    _T("OK\0DISK_ERR\0INT_ERR\0NOT_READY\0NO_FILE\0NO_PATH\0INVALID_NAME\0")
    _T("DENIED\0EXIST\0INVALID_OBJECT\0WRITE_PROTECTED\0INVALID_DRIVE\0")
    _T("NOT_ENABLED\0NO_FILE_SYSTEM\0MKFS_ABORTED\0TIMEOUT\0LOCKED\0")
    _T("NOT_ENOUGH_CORE\0TOO_MANY_OPEN_FILES\0");

const char* f_get_rc_string (FRESULT rc) {
	FRESULT i;
    const char *p=rcstrings;

	for (i = 0; i != rc && *p; i++) {
		while(*p++) ;
	}
    return p;
}

const uint8_t init1[] = {0xeb, 0xfe, 0x90, 0x4d, 0x53, 0x44, 0x4f, 0x53,
                   0x35, 0x2e, 0x30, 0x00, 0x02, 0x01, 0x01, 0x00,
                   0x01, 0x00, 0x02, 0x00, 0x04, 0xf0, 0x04, 0x00,
                   0x3f, 0x00, 0xff};

const uint8_t init2[] = {0x80, 0x00, 0x29, 0x37,
                   0x4b, 0x55, 0x33, 0x4e, 0x4f, 0x20, 0x4e, 0x41,
                   0x4d, 0x45, 0x20, 0x20, 0x20, 0x20, 0x46, 0x41,
                   0x54, 0x20, 0x20, 0x20, 0x20, 0x20};

inline void format_formatDF(void) {
	int i;
	BYTE buf[512];

    memset(buf, 0, 512);
    for(i=0; i<100; i++) dataflash_write(buf, i, 1);

    memcpy(buf, init1, sizeof(init1));
    memcpy(buf+0x24, init2, sizeof(init2)); 
    buf[510] = 0x55;
    buf[511] = 0xaa;
    dataflash_write(buf, 0, 1);

    for(i=0; i< 512; i++) buf[i] = 0x00;
    buf[0] = 0xf0;
    buf[1] = 0xff;
    buf[2] = 0xff;
    dataflash_write(buf, 1, 1);
}


void fsInit(){
    FRESULT i;
    DIR dir;                /* Directory object */
    f_mount(0, &FatFs);
    
    i = f_opendir(&dir, "0:");
 
    if( i == FR_NO_FILESYSTEM ){
        lcdPrintln("filesystm b0rk");
        lcdPrintln("Formating...");
        lcdRefresh(); 
        format_formatDF();
        lcdPrintln("Done.");
        lcdPrintln("Now copy files");
        lcdPrintln("to drive. See");
        lcdPrintln("");
        lcdPrintln("r0ket.de/init");
        lcdRefresh();
        usbMSCInit();
        while(1);
    }
};

void fsReInit(){
    f_mount(0, NULL);
    f_mount(0, &FatFs);
};

int readFile(char * filename, char * data, int len){
    FIL file;
    UINT readbytes;
    int res;

    res=f_open(&file, filename, FA_OPEN_EXISTING|FA_READ);
    if(res){
        return -1;
    };

    res = f_read(&file, data, len, &readbytes);
    if(res){
        return -1;
    };

    f_close(&file);

	return readbytes;
};

int readTextFile(char * filename, char * data, int len){
    UINT readbytes;

    readbytes=readFile(filename,data,len-1);
    if(len>=0)
        data[readbytes]=0;
    return readbytes;
};


int writeFile(char * filename, char * data, int len){
    FIL file;
    UINT writebytes;
    int res;

	res=f_open(&file, filename, FA_CREATE_ALWAYS|FA_WRITE);
    if(res){
        return -1;
    };

    res = f_write(&file, data, len, &writebytes);
    if(res){
        return -1;
    };
    f_close(&file);

	return writebytes;
};

