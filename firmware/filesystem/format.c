#include <sysinit.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "usb/usbmsc.h"

#include "lcd/print.h"

#include "funk/nrf24l01p.h"
#include "funk/mesh.h"
#include "filesystem/at45db041d.h"

uint8_t init1[] = {0xeb, 0xfe, 0x90, 0x4d, 0x53, 0x44, 0x4f, 0x53,
                   0x35, 0x2e, 0x30, 0x00, 0x02, 0x01, 0x01, 0x00,
                   0x01, 0x00, 0x02, 0x00, 0x04, 0xf0, 0x04, 0x00,
                   0x3f, 0x00, 0xff};

uint8_t init2[] = {0x80, 0x00, 0x29, 0x37,
                   0x4b, 0x55, 0x33, 0x4e, 0x4f, 0x20, 0x4e, 0x41,
                   0x4d, 0x45, 0x20, 0x20, 0x20, 0x20, 0x46, 0x41,
                   0x54, 0x20, 0x20, 0x20, 0x20, 0x20};



void format_formatDF(void)
{
	int i;
	char buf[512];

    //dataflash_initialize();
    //delayms(100);
    
    for(i=0; i< 512; i++) buf[i] = 0x00;

    //for(i=0; i<1024; i++)
    //    dataflash_write(buf, i, 1);
    memcpy(buf, init1, sizeof(init1));
    memcpy(buf+0x24, init2, sizeof(init2));
    
    buf[510] = 0x55;
    buf[511] = 0xaa;
    //memcpy(buf+510, "\x55\xaa", 2);
    
    dataflash_write(buf, 0, 1);

    for(i=0; i< 512; i++) buf[i] = 0x00;

    buf[0] = 0xf0;
    buf[1] = 0xff;
    buf[2] = 0xff;
    dataflash_write(buf, 1, 1);

}

