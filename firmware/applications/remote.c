#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/lcd.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"
#include "usb/usbmsc.h"

#include <string.h>

/**************************************************************************/

uint32_t const testkey[4] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
};

void f_init(void){
    nrf_init();

    struct NRF_CFG config = {
        .channel= 81,
        .txmac= "REMOT",
        .nrmacs=1,
        .mac0=  "REMOT",
        .maclen ="\x10",
    };

    nrf_config_set(&config);

    lcdPrintln("Done.");
};

void f_recv(void){
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int len;

    while(1){
        len=nrf_rcv_pkt_time_encr(1000,sizeof(buf),buf,testkey);

        if(len==0){
            lcdPrintln("No pkt (Timeout)");
            return;
        };

        lcdClear();
        lcdPrint("Size:");lcdPrintInt(len);lcdNl();

        lcdPrintCharHex(buf[0]);
        lcdPrint(" ");
        lcdPrintCharHex(buf[1]);
        lcdPrint(" ");
        lcdPrintCharHex(buf[2]);
        lcdPrint(" ");
        lcdPrintCharHex(buf[3]);
        lcdNl();

        lcdPrint("ct:");lcdPrintIntHex( *(int*)(buf+ 4) ); lcdNl();
        lcdPrint("id:");lcdPrintIntHex( *(int*)(buf+ 8) ); lcdNl();
        lcdPrint("xx:");lcdPrintIntHex( *(int*)(buf+12) ); lcdNl();
        lcdDisplay(0);
    };
};


void f_send(void){
    int ctr=1;
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int len;
    int status;

    while(1){

        buf[0]=0x10; // Length: 16 bytes
        buf[1]='C'; // Proto
        buf[2]=getInputRaw();
        buf[3]=0x00; // Unused

        ctr++;
        *(int*)(buf+4)=ctr;

        /*
        buf[4]=0x00; // ctr
        buf[5]=0x00; // ctr
        buf[6]=0x00; // ctr
        buf[7]=ctr++; // ctr
        */

        buf[8]=0x0; // Object id
        buf[9]=0x0;
        buf[10]=0x05;
        buf[11]=0xec;

        buf[12]=0xff; // salt (0xffff always?)
        buf[13]=0xff;

        status=nrf_snd_pkt_crc_encr(16,buf,testkey);
        lcdClear();
        lcdPrint("Key:"); lcdPrintInt(buf[2]); lcdNl();
        lcdPrint("F-St:"); lcdPrintInt(status); 
        if(buf[2]==BTN_ENTER)
            break;
        lcdDisplay(0);
        len=nrf_rcv_pkt_time_encr(10,sizeof(buf),buf,testkey);
        if(len>0){
            lcdPrint("Got!");
        };
    };


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

void msc_menu(void){
    DoString(0,8,"MSC Enabled.");
    lcdDisplay(0);
    usbMSCInit();
    while(!getInputRaw())delayms(10);
    DoString(0,16,"MSC Disabled.");
    usbMSCOff();
};

/**************************************************************************/

const struct MENU_DEF menu_ISP =    {"Invoke ISP",  &gotoISP};
const struct MENU_DEF menu_init =   {"F Init",   &f_init};
const struct MENU_DEF menu_rcv =    {"F Recv",   &f_recv};
const struct MENU_DEF menu_snd =    {"F Send",   &f_send};
//const struct MENU_DEF menu_cfg =    {"F Cfg",   &f_cfg};
const struct MENU_DEF menu_mirror = {"Mirror",   &lcd_mirror};
const struct MENU_DEF menu_volt =   {"Akku",   &adc_check};
const struct MENU_DEF menu_msc =   {"MSC",   &msc_menu};
const struct MENU_DEF menu_nop =    {"---",   NULL};

static menuentry menu[] = {
    &menu_init,
    &menu_rcv,
    &menu_snd,
//    &menu_cfg,
    &menu_nop,
    &menu_mirror,
    &menu_volt,
    &menu_msc,
    &menu_nop,
    &menu_ISP,
    NULL,
};

static const struct MENU mainmenu = {"Mainmenu", menu};

void main_remote(void) {

    font=&Font_7x8;

    while (1) {
        lcdFill(0); // clear display buffer
        lcdDisplay(0);
        handleMenu(&mainmenu);
        gotoISP();
    }
};

void tick_remote(void){
    static int foo=0;
    static int toggle=0;
	if(foo++>80){
        toggle=1-toggle;
		foo=0;
        gpioSetValue (RB_LED0, toggle); 
	};
    return;
};


