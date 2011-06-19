/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    USBUSER.H
 *      Purpose: USB Custom User Definitions
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
 *      Copyright (c) 2005-2009 Keil Software.
 *---------------------------------------------------------------------------*/

#ifndef __USBUSER_H__
#define __USBUSER_H__


/* USB Device Events Callback Functions */
extern void USB_Power_Event (uint32_t power);
extern void USB_Reset_Event (void);
extern void USB_Suspend_Event (void);
extern void USB_Resume_Event (void);
extern void USB_WakeUp_Event (void);
extern void USB_SOF_Event (void);
extern void USB_Error_Event (uint32_t error);

/* USB Endpoint Callback Events */
#define USB_EVT_SETUP       1	/* Setup Packet */
#define USB_EVT_OUT         2	/* OUT Packet */
#define USB_EVT_IN          3	/*  IN Packet */
#define USB_EVT_OUT_NAK     4	/* OUT Packet - Not Acknowledged */
#define USB_EVT_IN_NAK      5	/*  IN Packet - Not Acknowledged */
#define USB_EVT_OUT_STALL   6	/* OUT Packet - Stalled */
#define USB_EVT_IN_STALL    7	/*  IN Packet - Stalled */

/* USB Endpoint Events Callback Pointers */
extern void (*const USB_P_EP[USB_LOGIC_EP_NUM]) (uint32_t event);

/* USB Endpoint Events Callback Functions */
extern void USB_EndPoint0 (uint32_t event);
extern void USB_EndPoint1 (uint32_t event);
extern void USB_EndPoint2 (uint32_t event);
extern void USB_EndPoint3 (uint32_t event);
extern void USB_EndPoint4 (uint32_t event);

/* USB Core Events Callback Functions */
extern void USB_Configure_Event (void);
extern void USB_Interface_Event (void);
extern void USB_Feature_Event (void);


#endif /* __USBUSER_H__ */
