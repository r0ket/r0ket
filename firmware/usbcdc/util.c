#include <sysinit.h>

#include "usbcdc/usb.h"
#include "usbcdc/usbcore.h"
#include "usbcdc/cdc_buf.h"
#include "usbcdc/usbhw.h"
#include "usbcdc/cdcuser.h"

#include "basic/basic.h"

volatile unsigned int lastTick;

// There must be at least 1ms between USB frames (of up to 64 bytes)
// This buffers all data and writes it out from the buffer one frame
// and one millisecond at a time
int puts(const char * str){
    if(!USB_Configuration)
        return -1;

    while(*str)
        cdcBufferWrite(*str++);

    //XXX: This assumes systick is 1ms, which it isn't for us.
    // this makes usbserial unnecessary slow. Ah well....

    // Check if we can flush the buffer now or if we need to wait
    unsigned int currentTick = systickGetTicks();
    if (currentTick != lastTick){
        uint8_t frame[64];
        uint32_t bytesRead = 0;
        while (cdcBufferDataPending()){
            // Read up to 64 bytes as long as possible
            bytesRead = cdcBufferReadLen(frame, 64);
            USB_WriteEP (CDC_DEP_IN, frame, bytesRead);
            systickDelay(1);
        }
        lastTick = currentTick;
    }
    return 0;
}

void usbCDCInit(){
    lastTick = systickGetTicks();   // Used to control output/printf timing
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

usbCDCOff(void){
    USB_Connect(FALSE);
}
