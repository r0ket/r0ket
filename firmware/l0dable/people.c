#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"
#include "usetable.h"

/**************************************************************************/

void ram(void) {
    struct NRF_CFG config;
    uint8_t buf[16];

    config.nrmacs=1;
    config.maclen[0] = 16;
    config.channel = 81;
    memcpy(config.mac0, "\x01\x02\x03\x02\x01", 5);
    nrf_config_set(&config);
    lcdClear();
    lcdPrintln("People nearby:");
    //lcdPrint("nearby:");
    lcdRefresh();
    do{
        if( nrf_rcv_pkt_time(64,sizeof(buf),buf) == 16 ){
            buf[14] = 0;
            if( buf[1] == 0x23 || buf[1] == 0x24){
                lcdPrintln((char*)(buf+6));
                //lcdPrintln("foo");
            }
            lcdRefresh();
        }
    }while ((getInputRaw())==BTN_NONE);
}
