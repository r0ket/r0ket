#include <sysinit.h>

#include "basic/basic.h"
#include "lcd/lcd.h"
#include "lcd/display.h"
#include "filesystem/ff.h"

int lcdLoadImage(char *file) {
    return readFile(file,(char *)lcdBuffer,RESX*RESY_B);
}

int lcdSaveImage(char *file) {
	return writeFile(file,(char *)lcdBuffer,RESX*RESY_B);
}

uint8_t lcdShowAnim(char *fname, uint32_t framems) {
    FIL file;            /* File object */
	int res;
    UINT readbytes;
	uint8_t state=0;

	res=f_open(&file, fname, FA_OPEN_EXISTING|FA_READ);
	if(res)
		return 1;

	getInputWaitRelease();
	while(!getInputRaw()){
        lcdFill(0x55);
		res = f_read(&file, (char *)lcdBuffer, RESX*RESY_B, &readbytes);
        if(res)
            return -1;
		if(readbytes<RESX*RESY_B){
			f_lseek(&file,0);
            continue;
        };
		lcdDisplay();
        if(framems<100){
            state=delayms_queue_plus(framems,0);
        }else{
            getInputWaitTimeout(framems);
        };
	}

	if(state)
		work_queue();

    return 0;
};


