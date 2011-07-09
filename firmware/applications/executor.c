#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/allfonts.h"

void backlightInit(void);

extern void * __sram_top;

void execute (const void *function, uint16_t length){
    void (*dst)(void);

    dst=(void (*)(void)) (__sram_top);

    DoString(0,8,"Enter RAM!");
    DoIntX(0,16,(uint32_t)function);
    DoIntX(0,24,(uint32_t)dst);
    lcdDisplay(0);

    for(int i=0;i<length;i++){
        ((char*)dst)[i]=((char*)function)[i];
    };

    dst=(void (*)(void)) ((uint32_t)(dst) | 1); // Enable Thumb mode!
    dst();
};

#include "filesystem/ff.h"
void put_rc_y (FRESULT rc, int y) {
	const TCHAR *p =
		_T("OK\0DISK_ERR\0INT_ERR\0NOT_READY\0NO_FILE\0NO_PATH\0INVALID_NAME\0")
		_T("DENIED\0EXIST\0INVALID_OBJECT\0WRITE_PROTECTED\0INVALID_DRIVE\0")
		_T("NOT_ENABLED\0NO_FILE_SYSTEM\0MKFS_ABORTED\0TIMEOUT\0LOCKED\0")
		_T("NOT_ENOUGH_CORE\0TOO_MANY_OPEN_FILES\0");
	FRESULT i;

	for (i = 0; i != rc && *p; i++) {
		while(*p++) ;
	}
    DoString(0,y,p);
}

void put_rc (FRESULT rc){
    put_rc_y(rc,0);
};

void execute_file (const char * fname){
    FRESULT res;
    FIL file;
    UINT readbytes;
    void (*dst)(void);

    dst=(void (*)(void)) 0x10001800;

    res=f_open(&file, fname, FA_OPEN_EXISTING|FA_READ);
    put_rc(res);
    if(res){
        return;
    };

    res = f_read(&file, (char *)dst, RAMCODE, &readbytes);
    put_rc_y(res,8);
    if(res){
        return;
    };

    int dx;
    dx=DoString(0,16,"read: ");
    DoInt(dx,16,readbytes);
    lcdDisplay(0);

    dst=(void (*)(void)) ((uint32_t)(dst) | 1); // Enable Thumb mode!
    dst();

    getInput();
};

/**************************************************************************/

void main_executor(void) {

    lcdSetPixel(0,0,0); //the spaceinvader module needs this
    backlightInit();

    //disable the JTAG on PIO3_3
    IOCON_PIO3_3 = 0x10;

    int yctr=8;
    int dx=0;

    font_direction = FONT_DIR_LTR; // LeftToRight is the default

    yctr=18;

    uint8_t trigger;

    trigger=20;

    uint32_t ctr=0;
    char key;
    while (1) {
        ctr++;

        lcdDisplay(0);
        delayms(10);

        key= getInput();
        if(key==BTN_UP){
            trigger +=1;
        }else if (key ==BTN_DOWN){
            trigger -=1;
        };

        font=&Font_7x8;
        dx=DoString(0,0,"Trig:");
        dx=DoInt(dx,0,trigger);
        DoString(dx,0," ");

        // Easy flashing
        if(key==BTN_LEFT){
            DoString(0,8,"Enter ISP!");
            lcdDisplay(0);
            ISPandReset(5);
        };

        // DF xecute
        if(key==BTN_RIGHT){
            DoString(0,0,"Enter RAM!");
            lcdDisplay(0);
            while(getInput()!=BTN_NONE);

            FATFS FatFs;          /* File system object for logical drive */
            put_rc(f_mount(0, &FatFs));

            execute_file("0:test.c0d");
            lcdDisplay(0);
            while(!getInput());
        };

        if(key==BTN_UP){
            usbMSCInit();
        };

        // Display nickname
        font = &Font_Ubuntu36pt;
        dx=DoString(0,0,"Sec");

        // Blink LED
        if(ctr++>trigger){
            ctr=0;
            if (gpioGetValue(RB_LED2) == CFG_LED_OFF){
                gpioSetValue (RB_LED2, CFG_LED_ON); 
            } else {
                gpioSetValue (RB_LED2, CFG_LED_OFF); 
            };
        };

        // Print Voltage
        font = &Font_7x8;
        dx=DoString(0,yctr+28,"Voltage:");
        DoInt(dx,yctr+28,GetVoltage());

    }
    return;
}

void tick_executor(void){
    static int foo=0;
    static int toggle=0;
	if(foo++>50){
        toggle=1-toggle;
		foo=0;
        gpioSetValue (RB_LED0, toggle); 
	};
    return;
};



