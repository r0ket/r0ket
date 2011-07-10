/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbcore.h
 * Purpose: USB Core Definitions
 * Version: V1.20
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

#ifndef __USBCORE_H__
#define __USBCORE_H__

#include "usbcfg.h"

/* USB Endpoint Data Structure */
typedef struct _USB_EP_DATA {
  uint8_t  *pData;
  uint16_t Count;
} USB_EP_DATA;

/* USB Core Global Variables */
extern uint16_t USB_DeviceStatus;
extern uint8_t  USB_DeviceAddress;
volatile extern uint8_t  USB_Configuration;
extern uint32_t USB_EndPointMask;
extern uint32_t USB_EndPointHalt;
extern uint32_t USB_EndPointStall;
extern uint8_t  USB_AltSetting[USB_IF_NUM];


/* USB Endpoint 0 Buffer */
extern uint8_t  EP0Buf[USB_MAX_PACKET0];

/* USB Endpoint 0 Data Info */
extern USB_EP_DATA EP0Data;

/* USB Setup Packet */
extern USB_SETUP_PACKET SetupPacket;

/* USB Core Functions */
extern void USB_ResetCore (void);

/* Newer C compilers make it really difficult to add
 * an integer to a pointer */
static inline void UsbAddPtr(void **vpptr, uint32_t n);

/*
 * Add a number of bytes to a pointer's address
 * Harder than you might think. Some compilers say:
 * Expected an lvalue  -- Assignment expects its first operand to be
 * an lvalue.  Please note that a cast removes the lvaluedness of an
 * expression.
 *
 * vpptr = void pointer to pointer
 * n = number of bytes to add to pointer
 * Call looks like: AddPtr((void **)&myPointer, 8);
 */
static inline void UsbAddPtr(void **vpptr, uint32_t n)
{
  /* Declare a pointer to a pointer to a byte. Only a byte pointer
   * can be incremented by a number of bytes. Other pointers will
   * increment by a multiple of what they point to.
   */
  uint8_t **bpptr;

  /* Convert our void pointer to a pointer to a byte pointer to a pointer */
  bpptr = (uint8_t **)vpptr;

  /* Add 'n' bytes to our pointer value */
  (*bpptr) += n;
}







#endif  /* __USBCORE_H__ */
