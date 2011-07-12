#include <sysinit.h>

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
    FATFS FatFs;          /* File system object for logical drive */
    FRESULT res;

    lcdPrintln("Enter RAM!");
    lcdRefresh();
    while(getInput()!=BTN_NONE);

    res=f_mount(0, &FatFs);
    lcdPrint("Mount:");
    lcdPrintln(f_get_rc_string(res));

    execute_file("0:test.c0d");
    lcdRefresh();
};

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

static menuentry menu[] = {
    &menu_msc,
    &menu_exe,
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


