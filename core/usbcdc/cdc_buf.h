/*----------------------------------------------------------------------------
 *      Name:    cdc_buf.h
 *      Purpose: usb cdc buffer handling
 *      Version: V1.00
 *---------------------------------------------------------------------------*/

#ifndef __CDC_BUF_H__ 
#define __CDC_BUF_H__

#include "projectconfig.h"

// Buffer used for circular fifo
typedef struct _cdc_buffer_t
{
  volatile uint8_t len;
  volatile uint8_t wr_ptr;
  volatile uint8_t rd_ptr;
  uint8_t buf[CFG_USBCDC_BUFFERSIZE];
} cdc_buffer_t;

cdc_buffer_t * cdcGetBuffer();
void           cdcBufferInit();
uint8_t        cdcBufferRead();
uint32_t       cdcBufferReadLen(uint8_t* buf, uint32_t len);
void           cdcBufferWrite(uint8_t data);
void           cdcBufferClearFIFO();
uint8_t        cdcBufferDataPending();

#endif
