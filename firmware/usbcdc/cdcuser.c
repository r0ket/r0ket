/*----------/BulkBufOut------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    cdcuser.c
 *      Purpose: USB Communication Device Class User module 
 *      Version: V1.10
 *----------------------------------------------------------------------------
*      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP Semiconductors LPC microcontroller devices only. Nothing else 
 *      gives you the right to use this software.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include "projectconfig.h"
#include "basic/basic.h"

#include "usb.h"
#include "usbhw.h"
#include "usbcfg.h"
#include "usbcore.h"
#include "cdc.h"
#include "cdcuser.h"
#include "usbreg.h"


unsigned char BulkBufIn  [64];            // Buffer to store USB IN  packet
unsigned char BulkBufOut [64];            // Buffer to store USB OUT packet
unsigned char NotificationBuf [10];

CDC_LINE_CODING CDC_LineCoding  = {CFG_USBCDC_BAUDRATE, 0, 0, 8};
unsigned short  CDC_SerialState = 0x0000;
volatile unsigned char  CDC_DepInEmpty  = 1;                   // Data IN EP is empty

/*----------------------------------------------------------------------------
  We need a buffer for incoming data on USB port because USB receives
  much faster than  UART transmits
 *---------------------------------------------------------------------------*/
/* Buffer masks */
#define CDC_BUF_SIZE               (64)   // Output buffer in bytes (power 2)
                                                       // large enough for file transfer
#define CDC_BUF_MASK               (CDC_BUF_SIZE-1ul)

/* Buffer read / write macros */
#define CDC_BUF_RESET(cdcBuf)      (cdcBuf.rdIdx = cdcBuf.wrIdx = 0)
#define CDC_BUF_WR(cdcBuf, dataIn) (cdcBuf.data[CDC_BUF_MASK & cdcBuf.wrIdx++] = (dataIn))
#define CDC_BUF_RD(cdcBuf)         (cdcBuf.data[CDC_BUF_MASK & cdcBuf.rdIdx++])   
#define CDC_BUF_EMPTY(cdcBuf)      (cdcBuf.rdIdx == cdcBuf.wrIdx)
#define CDC_BUF_FULL(cdcBuf)       (cdcBuf.rdIdx == cdcBuf.wrIdx+1)
#define CDC_BUF_COUNT(cdcBuf)      (CDC_BUF_MASK & (cdcBuf.wrIdx - cdcBuf.rdIdx))


// CDC output buffer
typedef struct __CDC_BUF_T 
{
  unsigned char data[CDC_BUF_SIZE];
  volatile unsigned int wrIdx;
  volatile unsigned int rdIdx;
} CDC_BUF_T;

CDC_BUF_T  CDC_OutBuf;                                 // buffer for all CDC Out data
CDC_BUF_T  CDC_InBuf;                                 // buffer for all CDC Out data

/*----------------------------------------------------------------------------
  read data from CDC_OutBuf
 *---------------------------------------------------------------------------*/
int CDC_RdOutBuf (char *buffer, const int *length) 
{
  int bytesToRead, bytesRead;
  
  /* Read *length bytes, block if *bytes are not avaialable	*/
  bytesToRead = *length;
  bytesToRead = (bytesToRead < (*length)) ? bytesToRead : (*length);
  bytesRead = bytesToRead;


  // ... add code to check for underrun

  while (bytesToRead--) {
    *buffer++ = CDC_BUF_RD(CDC_OutBuf);
  }
  return (bytesRead);  
}

/*----------------------------------------------------------------------------
  write data to CDC_OutBuf
 *---------------------------------------------------------------------------*/
int CDC_WrOutBuf (const char *buffer, int *length) 
{
  int bytesToWrite, bytesWritten;

  // Write *length bytes
  bytesToWrite = *length;
  bytesWritten = bytesToWrite;


  // ... add code to check for overwrite

  while (bytesToWrite) {
      CDC_BUF_WR(CDC_OutBuf, *buffer++);           // Copy Data to buffer  
      bytesToWrite--;
  }     

  return (bytesWritten); 
}

/*----------------------------------------------------------------------------
  check if character(s) are available at CDC_OutBuf
 *---------------------------------------------------------------------------*/
int CDC_OutBufAvailChar (int *availChar) 
{
  *availChar = CDC_BUF_COUNT(CDC_OutBuf);

  return (0);
}
/* end Buffer handling */

/*----------------------------------------------------------------------------
  read data from CDC_InBuf
 *---------------------------------------------------------------------------*/
int CDC_RdInBuf (char *buffer, const int *length) 
{
  int bytesToRead, bytesRead;
  
  /* Read *length bytes, block if *bytes are not avaialable	*/
  bytesToRead = *length;
  //bytesToRead = (bytesToRead < (*length)) ? bytesToRead : (*length);
  bytesRead = bytesToRead;


  // ... add code to check for underrun

  while (bytesToRead--) {
    *buffer++ = CDC_BUF_RD(CDC_InBuf);
  }
  return (bytesRead);  
}

/*----------------------------------------------------------------------------
  write data to CDC_InBuf
 *---------------------------------------------------------------------------*/
int CDC_WrInBuf (const char *buffer, int *length) 
{
  int bytesToWrite, bytesWritten;

  // Write *length bytes
  bytesToWrite = *length;
  bytesWritten = bytesToWrite;
  
  // Just block if we can't write all at once
  // +1 to prevent an overflow of the ring buffer
  while( CDC_BUF_SIZE - CDC_BUF_COUNT(CDC_InBuf) < bytesToWrite+1 );

  //uint8_t flush = CDC_DepInEmpty;

  USB_DEVINTEN = 0;
  while (bytesToWrite--) {
      CDC_BUF_WR(CDC_InBuf, *buffer++);           // Copy Data to buffer  
  }
  //if( flush == 1 ){
  //if( CDC_DepInEmpty && CDC_BUF_COUNT(CDC_InBuf) ){
  if( CDC_DepInEmpty ){
    CDC_DepInEmpty = 0;
    CDC_BulkIn(); 
  }
  USB_DEVINTEN  = DEV_STAT_INT | (0xFF<<1) | (USB_SOF_EVENT   ? FRAME_INT : 0);

  return (bytesWritten); 
}

/*----------------------------------------------------------------------------
  check if character(s) are available at CDC_OutBuf
 *---------------------------------------------------------------------------*/
int CDC_InBufAvailChar (int *availChar) 
{
  *availChar = CDC_BUF_COUNT(CDC_InBuf);

  return (0);
}
/* end Buffer handling */



/*----------------------------------------------------------------------------
  CDC Initialisation
  Initializes the data structures and serial port
  Parameters:   None 
  Return Value: None
 *---------------------------------------------------------------------------*/
void CDC_Init (void) 
{
  CDC_DepInEmpty  = 1;
  CDC_SerialState = CDC_GetSerialState();

  CDC_BUF_RESET(CDC_OutBuf);
  CDC_BUF_RESET(CDC_InBuf);

}


/*----------------------------------------------------------------------------
  CDC SendEncapsulatedCommand Request Callback
  Called automatically on CDC SEND_ENCAPSULATED_COMMAND Request
  Parameters:   None                          (global SetupPacket and EP0Buf)
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_SendEncapsulatedCommand (void) 
{
  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC GetEncapsulatedResponse Request Callback
  Called automatically on CDC Get_ENCAPSULATED_RESPONSE Request
  Parameters:   None                          (global SetupPacket and EP0Buf)
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_GetEncapsulatedResponse (void) 
{
  /* ... add code to handle request */
  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC SetCommFeature Request Callback
  Called automatically on CDC Set_COMM_FATURE Request
  Parameters:   FeatureSelector
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_SetCommFeature (unsigned short wFeatureSelector) 
{
  /* ... add code to handle request */
  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC GetCommFeature Request Callback
  Called automatically on CDC Get_COMM_FATURE Request
  Parameters:   FeatureSelector
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_GetCommFeature (unsigned short wFeatureSelector) 
{
  /* ... add code to handle request */
  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC ClearCommFeature Request Callback
  Called automatically on CDC CLEAR_COMM_FATURE Request
  Parameters:   FeatureSelector
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_ClearCommFeature (unsigned short wFeatureSelector) 
{
  /* ... add code to handle request */
  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC SetLineCoding Request Callback
  Called automatically on CDC SET_LINE_CODING Request
  Parameters:   none                    (global SetupPacket and EP0Buf)
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_SetLineCoding (void) 
{
  CDC_LineCoding.dwDTERate   =   (EP0Buf[0] <<  0)
                               | (EP0Buf[1] <<  8)
                               | (EP0Buf[2] << 16)
                               | (EP0Buf[3] << 24); 
  CDC_LineCoding.bCharFormat =  EP0Buf[4];
  CDC_LineCoding.bParityType =  EP0Buf[5];
  CDC_LineCoding.bDataBits   =  EP0Buf[6];

  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC GetLineCoding Request Callback
  Called automatically on CDC GET_LINE_CODING Request
  Parameters:   None                         (global SetupPacket and EP0Buf)
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_GetLineCoding (void) 
{
  EP0Buf[0] = (CDC_LineCoding.dwDTERate >>  0) & 0xFF;
  EP0Buf[1] = (CDC_LineCoding.dwDTERate >>  8) & 0xFF;
  EP0Buf[2] = (CDC_LineCoding.dwDTERate >> 16) & 0xFF;
  EP0Buf[3] = (CDC_LineCoding.dwDTERate >> 24) & 0xFF;
  EP0Buf[4] =  CDC_LineCoding.bCharFormat;
  EP0Buf[5] =  CDC_LineCoding.bParityType;
  EP0Buf[6] =  CDC_LineCoding.bDataBits;

  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC SetControlLineState Request Callback
  Called automatically on CDC SET_CONTROL_LINE_STATE Request
  Parameters:   ControlSignalBitmap 
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_SetControlLineState (unsigned short wControlSignalBitmap) {

  /* ... add code to handle request */
  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC SendBreak Request Callback
  Called automatically on CDC Set_COMM_FATURE Request
  Parameters:   0xFFFF  start of Break 
                0x0000  stop  of Break
                0x####  Duration of Break
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_SendBreak (unsigned short wDurationOfBreak) {

  /* ... add code to handle request */
  return (TRUE);
}

/*----------------------------------------------------------------------------
  CDC_BulkIn call on DataIn Request
  Parameters:   none
  Return Value: none
 *---------------------------------------------------------------------------*/
void CDC_BulkIn(void) 
{
    int numBytesRead, numBytesAvail;
    CDC_InBufAvailChar(&numBytesAvail);
    numBytesRead = CDC_RdInBuf((char*)&BulkBufIn[0], &numBytesAvail);
    // send over USB
    if (numBytesRead > 0) {
        //gpioSetValue (RB_LED0, 1);
        USB_WriteEP (CDC_DEP_IN, &BulkBufIn[0], numBytesRead);
        //gpioSetValue (RB_LED0, 0); 
    } else {
        //USB_WriteEP (CDC_DEP_IN, "test\r\n", 6);
        CDC_DepInEmpty = 1;
        //gpioSetValue (RB_LED2, 1); 
    }
} 


/*----------------------------------------------------------------------------
  CDC_BulkOut call on DataOut Request
  Parameters:   none
  Return Value: none
 *---------------------------------------------------------------------------*/
void CDC_BulkOut(void) 
{
  int numBytesRead;

  // get data from USB into intermediate buffer
  numBytesRead = USB_ReadEP(CDC_DEP_OUT, &BulkBufOut[0]);

  // ... add code to check for overwrite

  // store data in a buffer to transmit it over serial interface
  CDC_WrOutBuf ((char *)&BulkBufOut[0], &numBytesRead);
}


/*----------------------------------------------------------------------------
  Get the SERIAL_STATE as defined in usbcdc11.pdf, 6.3.5, Table 69.
  Parameters:   none
  Return Value: SerialState as defined in usbcdc11.pdf
 *---------------------------------------------------------------------------*/
unsigned short CDC_GetSerialState (void) 
{
  CDC_SerialState = 0;

  return (CDC_SerialState);
}


/*----------------------------------------------------------------------------
  Send the SERIAL_STATE notification as defined in usbcdc11.pdf, 6.3.5.
 *---------------------------------------------------------------------------*/
void CDC_NotificationIn (void) 
{
  NotificationBuf[0] = 0xA1;                           // bmRequestType
  NotificationBuf[1] = CDC_NOTIFICATION_SERIAL_STATE;  // bNotification (SERIAL_STATE)
  NotificationBuf[2] = 0x00;                           // wValue
  NotificationBuf[3] = 0x00;
  NotificationBuf[4] = 0x00;                           // wIndex (Interface #, LSB first)
  NotificationBuf[5] = 0x00;
  NotificationBuf[6] = 0x02;                           // wLength (Data length = 2 bytes, LSB first)
  NotificationBuf[7] = 0x00; 
  NotificationBuf[8] = (CDC_SerialState >>  0) & 0xFF; // UART State Bitmap (16bits, LSB first)
  NotificationBuf[9] = (CDC_SerialState >>  8) & 0xFF;

  USB_WriteEP (CDC_CEP_IN, &NotificationBuf[0], 10);   // send notification
}
