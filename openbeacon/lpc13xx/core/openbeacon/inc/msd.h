/***************************************************************
 *
 * OpenBeacon.org - MSD ROM function code for LPC13xx
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
#ifndef __MSD_H__
#define __MSD_H__

/***************************************************************
 * bytes per sector
 ***************************************************************/
#define DISK_BLOCK_SIZE 512UL

/***************************************************************
 * disk geometry - make it small enough
 * to allow FAT16 with 512 byte cluster size
 ***************************************************************/
#define DISK_CYLINDERS 31UL
#define DISK_HEADS 255UL
#define DISK_SECTORS_PER_TRACK 63UL

/***************************************************************
 * derieved values
 ***************************************************************/
#define DISK_SECTORS (DISK_CYLINDERS*DISK_HEADS*DISK_SECTORS_PER_TRACK)
#define DISK_SIZE (DISK_SECTORS*DISK_BLOCK_SIZE)

/***************************************************************
 * exported functions
 ***************************************************************/
extern void msd_init (void);
extern void msd_read (uint32_t offset, uint8_t *dst, uint32_t length);
extern void msd_write (uint32_t offset, uint8_t *src, uint32_t length);

#endif/*__MSD_H__*/
