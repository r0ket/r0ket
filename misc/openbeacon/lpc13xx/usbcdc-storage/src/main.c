/*----------------------------------------------------------------------------
 *      Name:    vcomdemo.c
 *      Purpose: USB virtual COM port Demo
 *      Version: V1.02
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

#include "LPC13xx.h"
#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "cdc.h"
#include "cdcuser.h"
#include "uart.h"
#include "vcomdemo.h"
#include "mscuser.h"
#include "memory.h"
#include "stdio.h"
#include "string.h"
#include "debug_printf.h"

extern uint8_t Memory[MSC_MemorySize];	/* MSC Memory in RAM */

#define     EN_TIMER32_1    (1<<10)
#define     EN_IOCON        (1<<16)
#define     EN_USBREG       (1<<14)

/*----------------------------------------------------------------------------
 Initializes the VCOM port.
 Call this function before using VCOM_putchar or VCOM_getchar
 *---------------------------------------------------------------------------*/
void
VCOM_Init (void)
{

  CDC_Init ();
}


/*----------------------------------------------------------------------------
  Reads character from serial port buffer and writes to USB buffer
 *---------------------------------------------------------------------------*/
void
VCOM_Serial2Usb (void)
{
  int numBytesRead;

  if (UARTCount > 0)
    {
      if (CDC_DepInEmpty)
	{
	  numBytesRead = (UARTCount<USB_CDC_BUFSIZE)?UARTCount:USB_CDC_BUFSIZE;
	
	  CDC_DepInEmpty = 0;
	  USB_WriteEP (CDC_DEP_IN, (uint8_t*)&UARTBuffer, numBytesRead);
	  UARTCount=0;
	}
    }

}

/*----------------------------------------------------------------------------
  Reads character from USB buffer and writes to serial port buffer
 *---------------------------------------------------------------------------*/
void
VCOM_Usb2Serial (void)
{
  static char serBuf[32];
  int numBytesToRead, numBytesRead, numAvailByte;

  CDC_OutBufAvailChar (&numAvailByte);
  if (numAvailByte > 0)
    {
      numBytesToRead = numAvailByte > 32 ? 32 : numAvailByte;
      numBytesRead = CDC_RdOutBuf (&serBuf[0], &numBytesToRead);
      UARTSend ((uint8_t*)&serBuf, numBytesRead);
    }

}

/*----------------------------------------------------------------------------
  Reads character from USB buffer and writes to serial port buffer
 *---------------------------------------------------------------------------*/
void
VCOM_Usb2SerialTest (void)
{
  debug_printf("Hello World!\n");
}


/*----------------------------------------------------------------------------
  checks the serial state and initiates notification
 *---------------------------------------------------------------------------*/
void
VCOM_CheckSerialState (void)
{
  unsigned short temp;
  static unsigned short serialState;

  temp = CDC_GetSerialState ();
  if (serialState != temp)
    {
      serialState = temp;
      CDC_NotificationIn ();	// send SERIAL_STATE notification
    }
}

/*----------------------------------------------------------------------------
  Main Program
 *---------------------------------------------------------------------------*/
int
main (void)
{
  uint32_t n;

  for (n = 0; n < MSC_ImageSize; n++)
    {				/* Copy Initial Disk Image */
      Memory[n] = DiskImage[n];	/*   from Flash to RAM     */
    }

  /* Basic chip initialization is taken care of in SystemInit() called
   * from the startup code. SystemInit() and chip settings are defined
   * in the CMSIS system_<part family>.c file.
   */

  /* Enable Timer32_1, IOCON, and USB blocks */
  LPC_SYSCON->SYSAHBCLKCTRL |= (EN_TIMER32_1 | EN_IOCON | EN_USBREG);


  USBIOClkConfig ();

  VCOM_Init ();			// VCOM Initialization

  USB_Init ();			// USB Initialization
  USB_Connect (TRUE);		// USB Connect

  while (!USB_Configuration);	// wait until USB is configured

  /* NVIC is installed inside UARTInit file. */
  SystemCoreClockUpdate ();
  UARTInit (115200, 0);

  VCOM_Usb2SerialTest ();

  while (1)
    {				// Loop forever
      VCOM_Serial2Usb ();	// read serial port and initiate USB event
      VCOM_CheckSerialState ();
      VCOM_Usb2Serial ();
    }				// end while                                                                                           
}				// end main ()
