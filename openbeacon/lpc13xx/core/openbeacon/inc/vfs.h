/***************************************************************
 *
 * OpenBeacon.org - virtual FAT16 file system support
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

#ifndef __VFS_H__
#define __VFS_H__

typedef void (*TDiskHandler) (uint32_t offset, uint32_t length,
			      const void *src, uint8_t * dst);

typedef struct _TDiskFile
{
  uint32_t length;
  TDiskHandler handler;
  const void *data;
  const char *name;
  const struct _TDiskFile *next;
} TDiskFile;

typedef struct
{
  uint8_t BS_jmpBoot[3];
  char BS_OEMName[8];
  uint16_t BPB_BytsPerSec;
  uint8_t BPB_SecPerClus;
  uint16_t BPB_RsvdSecCnt;
  uint8_t BPB_NumFATs;
  uint16_t BPB_RootEntCnt;
  uint16_t BPB_TotSec16;
  uint8_t BPB_Media;
  uint16_t BPB_FATSz16;
  uint16_t BPB_SecPerTrk;
  uint16_t BPB_NumHeads;
  uint32_t BPB_HiddSec;
  uint32_t BPB_TotSec32;
  /* FAT12/FAT16 definition */
  uint8_t BS_DrvNum;
  uint8_t BS_Reserved1;
  uint8_t BS_BootSig;
  uint32_t BS_VolID;
  char BS_VolLab[11];
  char BS_FilSysType[8];
} PACKED TDiskBPB;

extern const TDiskBPB DiskBPB;
extern const uint32_t DiskSignature;

extern void vfs_init (const TDiskFile *file);
extern void vfs_status (void);

#endif/*__VFS_H__*/
