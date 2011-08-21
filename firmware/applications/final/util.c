#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"
#include "lcd/image.h"

#include "usb/usbmsc.h"

#include "filesystem/ff.h"
#include "filesystem/select.h"

#include "core/iap/iap.h"

/**************************************************************************/

//# MENU usb_storage
void msc_menu(void){
    lcdClear();
    lcdPrintln("MSC Enabled.");
    lcdRefresh();
    usbMSCInit();
    while(!getInputRaw())delayms(10);
    DoString(0,16,"MSC Disabled.");
    usbMSCOff();
    fsReInit();
};

//# MENU lcdread
void lcdrtest(void){
    lcdClear();
    lcdPrint("ID1:"); lcdPrintInt(lcdRead(128+64+16+8  +2  )); lcdNl();
    lcdPrint("ID2:"); lcdPrintInt(lcdRead(128+64+16+8  +2+1)); lcdNl();
    lcdPrint("ID3:"); lcdPrintInt(lcdRead(128+64+16+8+4    )); lcdNl();
    lcdPrint("ID4:"); lcdPrintInt(lcdRead(128+64+16+8+4  +1)); lcdNl();
    lcdPrint("Tmp:"); lcdPrintInt(lcdRead(128+64+16+8+4+2  )); lcdNl(); 
    lcdPrint("VM:"); lcdPrintInt(lcdRead(128+64+16+8+4+2+1)); lcdNl(); 
//    lcd_select(); mylcdWrite(0,128+32+8+4+1); lcd_deselect();
    delayms(10);
    lcdInit();
    lcdRefresh();
    while(!getInputRaw())delayms(10);
};

void blink_led0(void){
    gpioSetValue (RB_LED0, 1-gpioGetValue(RB_LED0));
};

void tick_alive(void){
    static int foo=0;

    if(GLOBAL(alivechk)==0)
        return;

	if(foo++>500/SYSTICKSPEED){
		foo=0;
        if(GLOBAL(alivechk)==2)
            push_queue(blink_led0);
        else
            blink_led0();
	};
    return;
};


// //# MENU img img
void t_img(void){
    char fname[FILENAMELEN];
    selectFile(fname, "LCD");
    lcdLoadImage(fname);
    lcdRefresh();
    getInputWait();
};

// //# MENU img anim
void t_ani(void){
    char fname[FILENAMELEN];
    selectFile(fname, "LCD");
    lcdShowAnim(fname,1000);
};
