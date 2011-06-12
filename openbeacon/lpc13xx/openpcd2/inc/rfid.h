/***************************************************************
 *
 * OpenBeacon.org - PN532 routines for LPC13xx based OpenPCD2
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
#ifndef __RFID_H__
#define __RFID_H__

extern void rfid_init(void);
extern int rfid_read(void *data, unsigned char size);
extern int rfid_write(const void *data, int len);

#endif/*__RFID_H__*/
