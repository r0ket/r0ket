/*----------------------------------------------------------------------------
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

#include <openbeacon.h>
#ifdef  ENALBLE_USB_FULLFEATURED

#include "cdcusb.h"
#include "usbhw.h"
#include "usbcfg.h"
#include "usbcore.h"
#include "cdc.h"
#include "cdcuser.h"

static CDC_LINE_CODING CDC_LineCoding = { 115200, 0, 0, 8 };

static unsigned short CDC_SerialState = 0x0000;
static BOOL CDC_DepInEmpty;	// Data IN EP is empty

#ifdef  ENABLE_FREERTOS
static xQueueHandle g_QueueRxUSB = NULL;
static xQueueHandle g_QueueTxUSB = NULL;
#endif/*ENABLE_FREERTOS*/

/*----------------------------------------------------------------------------
  CDC Initialisation
  Initializes the data structures and serial port
  Parameters:   None 
  Return Value: None
 *---------------------------------------------------------------------------*/
void
CDC_Init (void)
{
  USBIOClkConfig();

  CDC_DepInEmpty = TRUE;
  CDC_SerialState = CDC_GetSerialState ();

#ifdef  ENABLE_FREERTOS
  /* Create the queues used to hold Rx and Tx characters. */
  g_QueueRxUSB = xQueueCreate (USB_CDC_BUFSIZE*2, sizeof (uint8_t));
  g_QueueTxUSB = xQueueCreate (USB_CDC_BUFSIZE*2, sizeof (uint8_t));
#endif/*ENABLE_FREERTOS*/
}


/*----------------------------------------------------------------------------
  CDC SendEncapsulatedCommand Request Callback
  Called automatically on CDC SEND_ENCAPSULATED_COMMAND Request
  Parameters:   None                          (global SetupPacket and EP0Buf)
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t
CDC_SendEncapsulatedCommand (void)
{
  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC GetEncapsulatedResponse Request Callback
  Called automatically on CDC Get_ENCAPSULATED_RESPONSE Request
  Parameters:   None                          (global SetupPacket and EP0Buf)
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t
CDC_GetEncapsulatedResponse (void)
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
uint32_t
CDC_SetCommFeature (unsigned short wFeatureSelector)
{
  (void) wFeatureSelector;
  /* ... add code to handle request */
  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC GetCommFeature Request Callback
  Called automatically on CDC Get_COMM_FATURE Request
  Parameters:   FeatureSelector
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t
CDC_GetCommFeature (unsigned short wFeatureSelector)
{
  (void) wFeatureSelector;
  /* ... add code to handle request */
  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC ClearCommFeature Request Callback
  Called automatically on CDC CLEAR_COMM_FATURE Request
  Parameters:   FeatureSelector
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t
CDC_ClearCommFeature (unsigned short wFeatureSelector)
{
  (void) wFeatureSelector;
  /* ... add code to handle request */
  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC SetLineCoding Request Callback
  Called automatically on CDC SET_LINE_CODING Request
  Parameters:   none                    (global SetupPacket and EP0Buf)
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t
CDC_SetLineCoding (void)
{
  CDC_LineCoding.dwDTERate = (EP0Buf[0] << 0)
    | (EP0Buf[1] << 8) | (EP0Buf[2] << 16) | (EP0Buf[3] << 24);
  CDC_LineCoding.bCharFormat = EP0Buf[4];
  CDC_LineCoding.bParityType = EP0Buf[5];
  CDC_LineCoding.bDataBits = EP0Buf[6];

  /* configure serial port hardware settings if needed */

  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC GetLineCoding Request Callback
  Called automatically on CDC GET_LINE_CODING Request
  Parameters:   None                         (global SetupPacket and EP0Buf)
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t
CDC_GetLineCoding (void)
{
  EP0Buf[0] = (CDC_LineCoding.dwDTERate >> 0) & 0xFF;
  EP0Buf[1] = (CDC_LineCoding.dwDTERate >> 8) & 0xFF;
  EP0Buf[2] = (CDC_LineCoding.dwDTERate >> 16) & 0xFF;
  EP0Buf[3] = (CDC_LineCoding.dwDTERate >> 24) & 0xFF;
  EP0Buf[4] = CDC_LineCoding.bCharFormat;
  EP0Buf[5] = CDC_LineCoding.bParityType;
  EP0Buf[6] = CDC_LineCoding.bDataBits;

  return (TRUE);
}


/*----------------------------------------------------------------------------
  CDC SetControlLineState Request Callback
  Called automatically on CDC SET_CONTROL_LINE_STATE Request
  Parameters:   ControlSignalBitmap 
  Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t
CDC_SetControlLineState (unsigned short wControlSignalBitmap)
{
  (void) wControlSignalBitmap;
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
uint32_t
CDC_SendBreak (unsigned short wDurationOfBreak)
{
  (void) wDurationOfBreak;
  /* ... add code to handle request */
  return (TRUE);
}


#ifdef  ENABLE_FREERTOS
/*----------------------------------------------------------------------------
  CDC_BulkIn_Handler call on DataIn Request
  Parameters:   from_isr - set true if called from ISR
  Return Value: none
 *---------------------------------------------------------------------------*/
static void
CDC_BulkIn_Handler(BOOL from_isr)
{
	uint8_t* p;
	uint32_t data, bs;
	int count;
	portBASE_TYPE xTaskWoken = pdFALSE;

	if(!from_isr)
		vPortEnterCritical ();

	count = uxQueueMessagesWaitingFromISR(g_QueueTxUSB);
	if(count>USB_CDC_BUFSIZE)
		count = USB_CDC_BUFSIZE;

	if(!count)
		CDC_DepInEmpty = 1;
	else
	{
		USB_WriteEP_Count (CDC_DEP_IN, count);
		while(count>0)
		{
			if(count>(int)sizeof(data))
			{
				bs = sizeof(data);
				count -= sizeof(data);
			}
			else
			{
				bs = count;
				data = count = 0;
			}
			p = (uint8_t*) &data;
			while(bs--)
				xQueueReceiveFromISR (g_QueueTxUSB,p++, &xTaskWoken);

			USB_WriteEP_Block (data);

		}
		USB_WriteEP_Terminate (CDC_DEP_IN);

		if(from_isr && xTaskWoken)
			taskYIELD ();
	}

	if(!from_isr)
		vPortExitCritical ();
}

/*----------------------------------------------------------------------------
  CDC_BulkIn call on DataIn Request
  Parameters:   none
  Return Value: none
 *---------------------------------------------------------------------------*/
void
CDC_BulkIn(void)
{
	CDC_BulkIn_Handler(TRUE);
}

void
CDC_Flush (void)
{
	if(CDC_DepInEmpty)
		CDC_BulkIn_Handler (FALSE);
}

BOOL
CDC_PutChar(uint8_t cByte)
{
	BOOL res;

	res = xQueueSend (g_QueueTxUSB, &cByte, 0) ? TRUE : FALSE;

	if(cByte == '\n')
		CDC_Flush ();

	return res;
}

/*----------------------------------------------------------------------------
  CDC_BulkOut call on DataOut Request
  Parameters:   none
  Return Value: none
 *---------------------------------------------------------------------------*/
void CDC_BulkOut(void)
{
	int count, bs;
	uint32_t data;
	uint8_t* p;
	portBASE_TYPE xTaskWoken = pdFALSE;

	count = USB_ReadEP_Count(CDC_DEP_OUT);

	while (count > 0)
	{
		data = USB_ReadEP_Block();
		bs = count > (int)sizeof(data) ? (int)sizeof(data) : count;
		count -= bs;
		p = (uint8_t*) &data;
		while (bs--)
			xQueueSendFromISR (g_QueueRxUSB,p++, &xTaskWoken);
	}
	USB_ReadEP_Terminate(CDC_DEP_OUT);

	if(xTaskWoken)
		taskYIELD ();
}

portLONG CDC_Recv (portCHAR *cByte, portLONG size, portTickType xTicksToWait)
{
	portLONG res;

	if (size <= 0 || !cByte || !g_QueueRxUSB)
		return 0;

	res = 0;
	while (size-- && xQueueReceive(g_QueueRxUSB, cByte++, xTicksToWait))
		res++;

	return res;
}
#endif/*ENABLE_FREERTOS*/


/*----------------------------------------------------------------------------
  Get the SERIAL_STATE as defined in usbcdc11.pdf, 6.3.5, Table 69.
  Parameters:   none
  Return Value: SerialState as defined in usbcdc11.pdf
 *---------------------------------------------------------------------------*/
unsigned short
CDC_GetSerialState (void)
{
  return CDC_SerialState;
}


/*----------------------------------------------------------------------------
  Send the SERIAL_STATE notification as defined in usbcdc11.pdf, 6.3.5.
 *---------------------------------------------------------------------------*/
void
CDC_NotificationIn (void)
{
  uint8_t NotificationBuf[10];

  NotificationBuf[0] = 0xA1;	// bmRequestType
  NotificationBuf[1] = CDC_NOTIFICATION_SERIAL_STATE;	// bNotification (SERIAL_STATE)
  NotificationBuf[2] = 0x00;	// wValue
  NotificationBuf[3] = 0x00;
  NotificationBuf[4] = 0x00;	// wIndex (Interface #, LSB first)
  NotificationBuf[5] = 0x00;
  NotificationBuf[6] = 0x02;	// wLength (Data length = 2 bytes, LSB first)
  NotificationBuf[7] = 0x00;
  NotificationBuf[8] = (CDC_SerialState >> 0) & 0xFF;	// UART State Bitmap (16bits, LSB first)
  NotificationBuf[9] = (CDC_SerialState >> 8) & 0xFF;

  USB_WriteEP (CDC_CEP_IN, &NotificationBuf[0], 10);	// send notification
}

#endif /*ENALBLE_USB_FULLFEATURED*/
