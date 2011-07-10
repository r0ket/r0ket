#include <stdint.h>
#include "core/usbcdc/usb.h"
#include "core/usbcdc/usbcore.h"
#include "core/usbcdc/usbhw.h"
#include "core/usbcdc/cdcuser.h"
#include "core/usbcdc/cdc_buf.h"

volatile unsigned int lastTick;
int puts(const char * str)
{
  // There must be at least 1ms between USB frames (of up to 64 bytes)
  // This buffers all data and writes it out from the buffer one frame
  // and one millisecond at a time
    if (USB_Configuration) 
    {
      while(*str)
        cdcBufferWrite(*str++);
      // Check if we can flush the buffer now or if we need to wait
      unsigned int currentTick = systickGetTicks();
      if (currentTick != lastTick)
      {
        uint8_t frame[64];
        uint32_t bytesRead = 0;
        while (cdcBufferDataPending())
        {
          // Read up to 64 bytes as long as possible
          bytesRead = cdcBufferReadLen(frame, 64);
          USB_WriteEP (CDC_DEP_IN, frame, bytesRead);
          systickDelay(1);
        }
        lastTick = currentTick;
      }
    }
  return 0;
}

void main_cdc(void)
{
    //lastTick = systickGetTicks();   // Used to control output/printf timing
    CDC_Init();                     // Initialise VCOM
    USB_Init();                     // USB Initialization
    USB_Connect(TRUE);              // USB Connect
    // Wait until USB is configured or timeout occurs
    uint32_t usbTimeout = 0; 
    while ( usbTimeout < CFG_USBCDC_INITTIMEOUT / 10 )
    {
      if (USB_Configuration) break;
      delayms(10);             // Wait 10ms
      usbTimeout++;
    }
    uint8_t buf[2] = {0,0};
    int l;
    while(1){
        CDC_OutBufAvailChar(&l);
        if( l ){
            l = 1;
            CDC_RdOutBuf (buf, &l);
            puts(buf);
        }
        //puts("hello world\r\n");
        //delayms(1);
    }
}
