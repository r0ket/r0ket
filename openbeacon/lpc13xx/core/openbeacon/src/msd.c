/***************************************************************
 *
 * OpenBeacon.org - HID ROM function code for LPC13xx
 *
 * Copyright 2010 Milosch Meriac <meriac@openbeacon.de>
 *
 ***************************************************************

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include <openbeacon.h>
#include "msd.h"

#ifdef USB_DISK_SUPPORT

#define     EN_TIMER32_1    (1<<10)
#define     EN_IOCON        (1<<16)
#define     EN_USBREG       (1<<14)

static ROM **rom = (ROM **) 0x1fff1ff8;

void
USB_IRQHandler (void)
{
  (*rom)->pUSBD->isr ();
}

/* USB String Descriptor (optional) */
const uint8_t USB_StringDescriptor[] = {
  /* Index 0x00: LANGID Codes */
  0x04,				/* bLength */
  USB_STRING_DESCRIPTOR_TYPE,	/* bDescriptorType */
  WBVAL (0x0409),		/* US English - wLANGID */
  /* Index 0x04: Manufacturer */
  0x1C,				/* bLength */
  USB_STRING_DESCRIPTOR_TYPE,	/* bDescriptorType */
  'B', 0, 'i', 0, 't', 0, 'm', 0, 'a', 0, 'n', 0, 'u', 0, 'f', 0,
  'a', 0, 'k', 0, 't', 0, 'u', 0, 'r', 0,
  /* Index 0x20: Product */
  0x28,				/* bLength */
  USB_STRING_DESCRIPTOR_TYPE,	/* bDescriptorType */
  'O', 0, 'p', 0, 'e', 0, 'n', 0, 'B', 0, 'e', 0, 'a', 0, 'c', 0,
  'o', 0, 'n', 0, ' ', 0, 'U', 0, 'S', 0, 'B', 0, ' ', 0, 'I', 0,
  'I', 0, ' ', 0, ' ', 0,
  /* Index 0x48: Serial Number */
  0x1A,				/* bLength */
  USB_STRING_DESCRIPTOR_TYPE,	/* bDescriptorType */
  '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0,
  '0', 0, '0', 0, '0', 0, '0', 0,
  /* Index 0x62: Interface 0, Alternate Setting 0 */
  0x0E,				/* bLength */
  USB_STRING_DESCRIPTOR_TYPE,	/* bDescriptorType */
  'M', 0, 'e', 0, 'm', 0, 'o', 0, 'r', 0, 'y', 0,
};

void
msd_init (void)
{
  volatile int i;

  /* 28 byte long SCSI inquiry string */
  static const uint8_t SCSI_Inquiry_String[28] =
    "Logfile OpenBeacon Tag  2.0a";

  /* Setup ROM initialization structure */
  static const MSC_DEVICE_INFO MscDevInfo = {
    .idVendor = USB_VENDOR_ID,
    .idProduct = USB_PROD_ID,
    .bcdDevice = USB_DEVICE,
    .StrDescPtr = (uint32_t) & USB_StringDescriptor[0],
    .MSCInquiryStr = (uint32_t) & SCSI_Inquiry_String,
    .BlockSize = DISK_BLOCK_SIZE,
    .BlockCount = DISK_SECTORS,
    .MemorySize = DISK_SIZE,
    .MSC_Read = msd_read,
    .MSC_Write = msd_write
  };

  /* Point DeviceInfo to MscDevInfo */
  static const USB_DEV_INFO DeviceInfo = {
    .DevType = USB_DEVICE_CLASS_STORAGE,
    .DevDetailPtr = (uint32_t) & MscDevInfo
  };

  /* Enable Timer32_1, IOCON, and USB blocks (for USB ROM driver) */
  LPC_SYSCON->SYSAHBCLKCTRL |= (EN_TIMER32_1 | EN_IOCON | EN_USBREG);

  /* Use pll and pin init function in rom */
  (*rom)->pUSBD->init_clk_pins ();
  /* fixing NXP stupidity - they break system clock */
  SystemCoreClockUpdate ();

  /* insert delay between init_clk_pins() and usb init */
  for (i = 0; i < 75; i++);

  /* USB Initialization ... */
  (*rom)->pUSBD->init (&DeviceInfo);
  /* Initialize Storage */
  init_msdstate ();
  /* ... and USB Connect */
  (*rom)->pUSBD->connect (1);
}

#endif /* USB_DISK_SUPPORT */
