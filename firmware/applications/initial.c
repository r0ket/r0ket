#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/print.h"
#include "filesystem/ff.h"
#include "usb/usbmsc.h"

#include "funk/nrf24l01p.h"

FATFS FatFs;
/**************************************************************************/

#define BEACON_CHANNEL 81
#define BEACON_MAC     "\x1\x2\x3\x2\1"

uint32_t const testkey[4] = {
    0xB4595344,0xD3E119B6,0xA814D0EC,0xEFF5A24E
};

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
};

void f_recv(void){
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int len;
    static int foo = 0;
    len=nrf_rcv_pkt_time_encr(100,sizeof(buf),buf,testkey);

    if(len==0){
        return;
    };
    
    if( foo )
        foo = 0;
    else
        foo = 1;
 
    gpioSetValue (RB_LED0, foo); 
    gpioSetValue (RB_LED1, foo); 
    gpioSetValue (RB_LED2, foo); 
    gpioSetValue (RB_LED3, foo); 
};


void init(void)
{
    systickInit(SYSTICKSPEED);

    gpioSetValue (RB_LED0, 0); 
    gpioSetValue (RB_LED1, 0); 
    gpioSetValue (RB_LED2, 0); 
    gpioSetValue (RB_LED3, 0); 
    IOCON_PIO1_11 = 0x0;
    gpioSetDir(RB_LED3, gpioDirection_Output);
    f_init();
}

void mount(void)
{
    int res;
    lcdPrint("Mount:");
    res=f_mount(0, &FatFs);
    lcdPrintln(f_get_rc_string(res));
    lcdRefresh();
}

void format(void)
{
    int res;
    lcdPrintln("Format DF:");
    res=f_mount(0, &FatFs);
    res=f_mkfs(0,1,0);
    lcdPrintln(f_get_rc_string(res));
    lcdRefresh();
}

int check(void)
{
    FIL file;
    int res = 1;
    res=f_open(&file, "flashed.cfg", FA_OPEN_EXISTING|FA_READ);
    lcdPrint("open:");
    lcdPrint(f_get_rc_string(res));
    lcdPrintln("   ");
    lcdRefresh();
    return res;
}

void msc(int timeout)
{
    lcdPrintln("MSC: Wait4data...");
    lcdRefresh();
    delayms_power(300);
    usbMSCInit();

    while(1){
        lcdSetCrsr(0,3*8);
        mount();
        if(!check())
            break;
        if(getInputRaw())
            break;
        delayms(100);
        f_recv();
        lcdRefresh();
    }
    while(timeout--){
        delayms(100);
    }
    lcdPrintln("MSC:Off.");
    usbMSCOff();
    lcdRefresh();
}

void isp(void)
{
    lcdPrintln("Enter ISP!");
    lcdRefresh();
    ISPandReset();
}
	
void main_initial(void) {
    init();
    format();
    msc(5);
    delayms(200);
    lcdPrintln("ISP:ON");
    lcdPrintln("turn off");
    lcdPrintln("when done.");
    lcdRefresh();
    ReinvokeISP();
}

void tick_initial(void){
    static int foo=0;
    static int toggle=0;
    incTimer();
    if(foo++>80){
        toggle=1-toggle;
        foo=0;
        gpioSetValue (RB_LED0, toggle); 
    };
};
