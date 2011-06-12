/***************************************************************
 *
 * OpenBeacon.org - config file
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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define LED_PORT 1	/* Port for led                      */
#define LED_BIT 9	/* Bit on port for led               */
#define LED_ON 1	/* Level to set port to turn on led  */
#define LED_OFF 0	/* Level to set port to turn off led */

/* USB device settings */
#define ENALBLE_USB_FULLFEATURED
#define USB_VENDOR_ID	0x2366
#define USB_PROD_ID	0x0003
#define USB_DEVICE	0x0100

/* FreeRTOS configuration */
#define ENABLE_FREERTOS
#define TASK_SERIAL_STACK_SIZE 64
#define TASK_SERIAL_PRIORITY (tskIDLE_PRIORITY + 2)

#endif/*__CONFIG_H__*/
