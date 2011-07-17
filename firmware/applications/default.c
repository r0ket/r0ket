#include <sysinit.h>
#include "basic/basic.h"

#include "lcd/lcd.h"
#include "lcd/print.h"
#include "filesystem/ff.h"
#include "usb/usbmsc.h"

/**************************************************************************/

FATFS FatFs[_VOLUMES];          /* File system object for logical drive */

#define CONFIGLEN 2
int lcdInitConfig(){
    FIL file[2];            /* File objects */
    BYTE buf[CONFIGLEN];
    UINT readbytes;
    int res;

    lcdFill(0); // clear display buffer
    res=f_mount(0, &FatFs[0]);
    lcdPrint("mount:");
    lcdPrintln(f_get_rc_string(res));
    if(res){
        return 1;
    };

    res=f_open(&file[0], "r0ket.cfg", FA_OPEN_EXISTING|FA_READ);
    lcdPrint("open:");
    lcdPrintln(f_get_rc_string(res));
    if(res){
        lcdPrintln("r0ket.cfg missing!");
        return 1;
    };

    for(int i=0;i<CONFIGLEN;i++)
        buf[i]=0;

    res = f_read(&file[0], buf, 2, &readbytes);
    lcdPrint("read:");
    lcdPrintln(f_get_rc_string(res));
    if(res){
        return 1;
    };

    lcdPrint("r: ");
    lcdPrintCharHex(buf[0]);
    lcdPrintCharHex(buf[1]);
    lcdNl();

    if(buf[0] == '1')
        lcdToggleFlag(LCD_INVERTED);
    if(buf[1] == '1')
        lcdToggleFlag(LCD_MIRRORX);

    res=f_close(&file[0]);
    lcdPrint("close:");
    lcdPrintln(f_get_rc_string(res));
    if(res){
        return 1;
    };
    return 0;
};


void main_default(void) {
    systickInit(10);

    if(getInputRaw()==BTN_ENTER){
        ISPandReset(7);
    };

    font=&Font_7x8;
    if(lcdInitConfig()){
        lcdDisplay(0);
        getInputWait();
    }else{
        lcdDisplay(0);
        delayms(300);
    };

    return;
};

// every 10 ms
void tick_default(void) {
    static int ctr;
    ctr++;
    if(ctr>100){
        VoltageCheck();
        ctr=0;
    };
    if(ctr%5==0){

        if(GetVoltage()<3600
#ifdef SAFE
                || GetVoltage() > 10000 // pin not connected
#endif
                ){
            IOCON_PIO1_11 = 0x0;
            gpioSetDir(RB_LED3, gpioDirection_Output);
            if( (ctr/5)%10 == 1 )
                gpioSetValue (RB_LED3, 1);
            else
                gpioSetValue (RB_LED3, 0);
        };
    };

    return;
};
