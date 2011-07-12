#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/lcd.h"
#include "lcd/allfonts.h"

#include "usb/usbmsc.h"

#include "filesystem/ff.h"

/**************************************************************************/

void put_rc_y (FRESULT rc, int y) {
    DoString(0,y,f_get_rc_string(rc));
}

void put_rc (FRESULT rc){
    put_rc_y(rc,0);
};


extern void * sram_top;
void execute_file (const char * fname){
    FRESULT res;
    FIL file;
    UINT readbytes;
    void (*dst)(void);

    dst=(void (*)(void)) (sram_top);

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

void execute_menu(void){
//    int dx=0;
    int dy=0;

    DoString(0,dy,"Enter RAM!");dy+=8;
    lcdDisplay(0);
    while(getInput()!=BTN_NONE);

    FATFS FatFs;          /* File system object for logical drive */
    put_rc(f_mount(0, &FatFs));

    execute_file("0:test.c0d");
    lcdDisplay(0);
    while(!getInput());
};

void msc_menu(void){
    DoString(0,8,"MSC Enabled.");
    lcdDisplay(0);
    usbMSCInit();
    getInputWait();
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


