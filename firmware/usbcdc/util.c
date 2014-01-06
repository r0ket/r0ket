#include <sysinit.h>
#include <string.h>

#include "usbcdc/usb.h"
#include "usbcdc/usbcore.h"
#include "usbcdc/cdc_buf.h"
#include "usbcdc/usbhw.h"
#include "usbcdc/cdcuser.h"

int puts(const char * str){
    if(!USB_Configuration)
        return -1;
 
    int len = strlen(str);
    CDC_WrInBuf(str, &len);
    return 0;
}

int putchr(const char chr){
    if(!USB_Configuration)
        return -1;

    int len = 1;
    CDC_WrInBuf(&chr, &len);
    return 0;
}

void usbCDCInit(){
    CDC_Init();                     // Initialise VCOM
    USB_Init();                     // USB Initialization
    USB_Connect(TRUE);              // USB Connect

    /* You could wait until serial is connected. */
#if 0 // We don't
    uint32_t usbTimeout = 0; 

    while ( usbTimeout < CFG_USBCDC_INITTIMEOUT / 10 ) {
      if (USB_Configuration) break;
      delayms(10);             // Wait 10ms
      usbTimeout++;
    }
#endif
}

void usbCDCOff(void){
    USB_Connect(FALSE);
}
