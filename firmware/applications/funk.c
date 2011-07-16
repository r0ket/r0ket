#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/lcd.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"

#include <string.h>

#include "funk/rftransfer.h"
/**************************************************************************/

#define BEACON_CHANNEL 81
#define BEACON_MAC     "\x1\x2\x3\x2\1"

uint32_t const testkey[4] = {
        0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
};

int enctoggle=0;

void f_init(void){
    nrf_init();

    struct NRF_CFG config = {
        .channel= BEACON_CHANNEL,
        .txmac= BEACON_MAC,
        .nrmacs=1,
        .mac0=  BEACON_MAC,
        .maclen ="\x10",
    };

    nrf_config_set(&config);
    lcdPrintln("Done.");
};

void f_status(void){
    int dx=0;
    int dy=8;
    uint8_t buf[4];

    buf[0]=C_R_REGISTER | R_CONFIG;
    buf[1]=0;
    buf[2]=0;
    buf[3]=0;
    dx=DoString(0,dy,"S:"); 
    dx=DoCharX(dx,dy,buf[0]);
    dx=DoCharX(dx,dy,buf[1]);
    dx=DoCharX(dx,dy,buf[2]);
    dx=DoCharX(dx,dy,buf[3]);
    dy+=8;
    nrf_cmd_rw_long(buf,2);
    dx=DoString(0,dy,"R:");
    dx=DoCharX(dx,dy,buf[0]);
    dx=DoCharX(dx,dy,buf[1]);
    dx=DoCharX(dx,dy,buf[2]);
    dx=DoCharX(dx,dy,buf[3]);
    dy+=8;

    int status=nrf_cmd_status(C_NOP);
    dx=DoString(0,dy,"St:"); DoCharX(dx,dy,status);dy+=8;
};

void f_recv(void){
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int len;

    len=nrf_rcv_pkt_time_encr(1000,sizeof(buf),buf,enctoggle?testkey:NULL);

    if(len==0){
        lcdPrintln("No pkt (Timeout)");
    };
    lcdPrint("Size:");lcdPrintInt(len);lcdNl();
    lcdPrint("1:");lcdPrintIntHex( *(int*)(buf+ 0) ); lcdNl();
    lcdPrint("2:");lcdPrintIntHex( *(int*)(buf+ 4) ); lcdNl();
    lcdPrint("3:");lcdPrintIntHex( *(int*)(buf+ 8) ); lcdNl();
    lcdPrint("4:");lcdPrintIntHex( *(int*)(buf+12) ); lcdNl();

    len=crc16(buf,14);
    lcdPrint("crc:");lcdPrintShortHex(len); lcdNl();

};

void f_cfg(void){
    struct NRF_CFG config;

    nrfconfig cfg=&config;

    nrf_config_get(cfg);

    lcdPrint("ch:");lcdPrintInt( cfg->channel ); lcdNl();
    lcdPrint("nr:");lcdPrintInt( cfg->nrmacs ); lcdNl();

    lcdPrint("0:");
    lcdPrintCharHex(cfg->mac0[0]);
    lcdPrintCharHex(cfg->mac0[1]);
    lcdPrintCharHex(cfg->mac0[2]);
    lcdPrintCharHex(cfg->mac0[3]);
    lcdPrintCharHex(cfg->mac0[4]);
    lcdNl();
    lcdPrint("1:");
    lcdPrintCharHex(cfg->mac1[0]);
    lcdPrintCharHex(cfg->mac1[1]);
    lcdPrintCharHex(cfg->mac1[2]);
    lcdPrintCharHex(cfg->mac1[3]);
    lcdPrintCharHex(cfg->mac1[4]);
    lcdNl();
    lcdPrint("2345:");
    lcdPrintCharHex(cfg->mac2345[0]);
    lcdPrintCharHex(cfg->mac2345[1]);
    lcdPrintCharHex(cfg->mac2345[2]);
    lcdPrintCharHex(cfg->mac2345[3]);
    lcdNl();
    lcdPrint("tx:");
    lcdPrintCharHex(cfg->txmac[0]);
    lcdPrintCharHex(cfg->txmac[1]);
    lcdPrintCharHex(cfg->txmac[2]);
    lcdPrintCharHex(cfg->txmac[3]);
    lcdPrintCharHex(cfg->txmac[4]);
    lcdNl();
    lcdPrint("len:");
    lcdPrintCharHex(cfg->maclen[0]);
    lcdPrintCharHex(cfg->maclen[1]);
    lcdPrintCharHex(cfg->maclen[2]);
    lcdPrintCharHex(cfg->maclen[3]);
    lcdPrintCharHex(cfg->maclen[4]);
    lcdNl();
};

void f_cfg_set(void){
    struct NRF_CFG config = {
        .channel= 13,
        .txmac= "R0KET",
        .nrmacs=1,
        .mac0=  "R0KET",
        .maclen ="\x10",
    };

    nrf_config_set(&config);
};

void f_enctog(void){
    enctoggle=1-enctoggle;
    if(enctoggle)
        lcdPrintln("Encrypt ON!");
    else
        lcdPrintln("encrypt off!");

};

void f_send(void){
    static char ctr=1;
    uint8_t buf[32];
    int status;

    buf[0]=0x10; // Length: 16 bytes
    buf[1]=0x17; // Proto - fixed at 0x17?
    buf[2]=0xff; // Flags (0xff)
    buf[3]=0xff; // Send intensity

    /*
    buf[4]=0x00; // ctr
    buf[5]=0x00; // ctr
    buf[6]=0x00; // ctr
    buf[7]=ctr++; // ctr
    */

    *(int*)(buf+4)=ctr++;

    buf[8]=0x0; // Object id
    buf[9]=0x0;
    buf[10]=0x05;
    buf[11]=0xec;

    buf[12]=0xff; // salt (0xffff always?)
    buf[13]=0xff;

    status=nrf_snd_pkt_crc_encr(16,buf,enctoggle?testkey:NULL);

    lcdPrint("Status:");
    lcdPrintCharHex(status);
    lcdNl();
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

void f_sendBlock(void)
{
    uint8_t data[] = "hallo welt, das hier ist ein langer string, der"
                   "per funk verschickt werden soll.";
    rftransfer_send(strlen((char *)data), data);
}

/**************************************************************************/

const struct MENU_DEF menu_ISP =    {"Invoke ISP",  &gotoISP};
const struct MENU_DEF menu_init =   {"F Init",   &f_init};
const struct MENU_DEF menu_status = {"F Status", &f_status};
const struct MENU_DEF menu_rcv =    {"F Recv",   &f_recv};
const struct MENU_DEF menu_snd =    {"F Send",   &f_send};
const struct MENU_DEF menu_cfg =    {"F CfgGet",   &f_cfg};
const struct MENU_DEF menu_cfg2 =    {"F CfgSet",   &f_cfg_set};
const struct MENU_DEF menu_enc =    {"Toggle Encr",   &f_enctog};
const struct MENU_DEF menu_sndblock={"F Send block", &f_sendBlock};
const struct MENU_DEF menu_mirror = {"Mirror",   &lcd_mirror};
const struct MENU_DEF menu_volt =   {"Akku",   &adc_check};
const struct MENU_DEF menu_nop =    {"---",   NULL};

static menuentry menu[] = {
    &menu_init,
    &menu_status,
    &menu_rcv,
    &menu_snd,
    &menu_enc,
    &menu_cfg2,
    &menu_cfg,
    &menu_nop,
    &menu_mirror,
    &menu_volt,
    &menu_ISP,
    NULL,
};

static const struct MENU mainmenu = {"Mainmenu", menu};

void main_funk(void) {

    backlightInit();
    font=&Font_7x8;

    while (1) {
        lcdFill(0); // clear display buffer
        lcdDisplay(0);
        handleMenu(&mainmenu);
        gotoISP();
    }
};

void tick_funk(void){
    static int foo=0;
    static int toggle=0;
	if(foo++>50){
        toggle=1-toggle;
		foo=0;
        gpioSetValue (RB_LED0, toggle); 
	};
    return;
};


