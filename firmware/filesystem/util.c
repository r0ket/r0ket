#include <ff.h>

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


void fsInit(){
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

    res = f_read(&file, data, len-1, &readbytes);
    if(res){
        return -1;
    };

    f_close(&file);

    data[readbytes]=0;
	return readbytes;
};

int writeFile(char * filename, char * data, int len){
    FIL file;
    UINT writebytes;
    int res;

	res=f_open(&file, filename, FA_OPEN_ALWAYS|FA_WRITE);
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

