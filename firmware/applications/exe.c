#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"

#include "lcd/lcd.h"
#include "lcd/allfonts.h"
#include "lcd/print.h"

#include "usb/usbmsc.h"

#include "filesystem/ff.h"

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
    lcdPrint("open: ");
    lcdPrintln(f_get_rc_string(res));
    lcdRefresh();
    if(res){
        return;
    };

    res = f_read(&file, (char *)dst, RAMCODE, &readbytes);
    lcdPrint("read: ");
    lcdPrintln(f_get_rc_string(res));
    lcdRefresh();
    if(res){
        return;
    };

    lcdPrintInt(readbytes);
    lcdPrintln(" bytes");
    lcdRefresh();

    dst=(void (*)(void)) ((uint32_t)(dst) | 1); // Enable Thumb mode!
    dst();

};

/**************************************************************************/

void execute_menu(void){

    lcdPrintln("Enter RAM!");
    lcdRefresh();
    while(getInput()!=BTN_NONE);

    execute_file("0:test.c0d");
    lcdRefresh();
};

#define MAXENTRIES 10
#define FLEN 13

//typedef char[13] FILENAME;

int getFiles(char files[][13], uint8_t count, uint16_t skip, char *ext)
{
    DIR dir;                /* Directory object */
    FILINFO Finfo;
    FRESULT res;
    int ctr;
    int pos = 0;
    res = f_opendir(&dir, "0:");
    if(res){
        lcdPrint("OpenDir:"); lcdPrintln(f_get_rc_string(res)); lcdRefresh(); 
        return 0;
    };
    for(ctr=0;pos<count;ctr++) {
        res = f_readdir(&dir, &Finfo);
        //lcdPrint("ReadDir:"); lcdPrintln(f_get_rc_string(res)); lcdRefresh(); 
        if ((res != FR_OK) || !Finfo.fname[0]) break;
        if( ctr < skip )
            continue;
        int len=strlen(Finfo.fname);
        int extlen = strlen(ext);

        if( strcmp(Finfo.fname+len-extlen, ext) != 0)
            continue;
        if (Finfo.fattrib & AM_DIR)
            continue;

        strcpy(files[pos++],Finfo.fname);
    }
    //lcdPrint("getFiles:"); lcdPrintInt(pos); lcdRefresh(); 
    return pos;
}

void select_menu(void)
{
    int skip = 0;
    char key;
    int selected = 0; 
    while(1){
        char files[7][13];
        int count = getFiles(files, 7, skip, "TXT");
        
        redraw:
        if( count )
            lcdClear();
        lcdPrintln("Select:");
        for(int i=0; i<count; i++){
            if( selected == i )
                lcdPrint(">");
            lcdPrintln(files[i]);
        }
        lcdRefresh();
        key=getInputWait();
        delayms(20);
        if( key==BTN_DOWN ){
            if( selected < 6 ){
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
            return;
        }else if( key==BTN_RIGHT ){
            return; //return file here
        }
    }
}

void msc_menu(void){
    DoString(0,8,"MSC Enabled.");
    lcdDisplay(0);
    usbMSCInit();
    while(!getInputRaw())delayms(10);
    DoString(0,16,"MSC Disabled.");
    usbMSCOff();
};

void gotoISP(void) {
    DoString(0,0,"Enter ISP!");
    lcdDisplay(0);
    ISPandReset(5);
}

void lcd_mirror(void) {
    lcdToggleFlag(LCD_MIRRORX);
};

void adc_check(void) {
    int dx=0;
    int dy=8;
    // Print Voltage
    dx=DoString(0,dy,"Voltage:");
    while ((getInputRaw())==BTN_NONE){
        DoInt(dx,dy,GetVoltage());
        lcdDisplay(0);
    };
    dy+=8;
    dx=DoString(0,dy,"Done.");
};

/**************************************************************************/

const struct MENU_DEF menu_ISP =    {"Invoke ISP",  &gotoISP};
const struct MENU_DEF menu_mirror = {"Mirror",   &lcd_mirror};
const struct MENU_DEF menu_volt =   {"Akku",   &adc_check};
const struct MENU_DEF menu_nop =    {"---",   NULL};
const struct MENU_DEF menu_msc =   {"MSC",   &msc_menu};
const struct MENU_DEF menu_exe =   {"Exec",   &execute_menu};
const struct MENU_DEF menu_exesel =   {"Exec2",   &select_menu};

static menuentry menu[] = {
    &menu_msc,
    &menu_exe,
    &menu_exesel,
    &menu_nop,
    &menu_mirror,
    &menu_volt,
    &menu_ISP,
    NULL,
};

static const struct MENU mainmenu = {"Mainmenu", menu};

void main_exe(void) {

    lcdSetPixel(0,0,0);

    backlightInit();
    font=&Font_7x8;

    FATFS FatFs;          /* File system object for logical drive */
    FRESULT res;
    res=f_mount(0, &FatFs);
    if(res!=FR_OK){
        lcdPrint("Mount:");
        lcdPrintln(f_get_rc_string(res));
        getInput();
    };

    while (1) {
        lcdFill(0); // clear display buffer
        lcdDisplay(0);
        handleMenu(&mainmenu);
        gotoISP();
    }
};

void tick_exe(void){
    static int foo=0;
    static int toggle=0;
	if(foo++>50){
        toggle=1-toggle;
		foo=0;
        gpioSetValue (RB_LED0, toggle); 
	};
    return;
};


