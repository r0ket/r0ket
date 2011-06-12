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
#include <openbeacon.h>
#include "msd.h"
#include "vfs.h"

#ifdef USB_DISK_SUPPORT

#define VOLUME_START 1UL
#define VOLUME_SECTORS (DISK_SECTORS-VOLUME_START)
#define DISK_SECTORS_PER_CLUSTER 8UL
#define DISK_CLUSTER_SIZE (DISK_SECTORS_PER_CLUSTER*DISK_BLOCK_SIZE)
#define FAT16_SIZE_SECTORS ((uint32_t)((((VOLUME_SECTORS/DISK_SECTORS_PER_CLUSTER+2)*2)+DISK_BLOCK_SIZE-1)/DISK_BLOCK_SIZE))
#define RESERVED_SECTORS_COUNT 1UL
#define BPB_NUMFATS 2UL
#define ROOT_DIR_SECTORS 1UL
#define MAX_FILES_IN_ROOT ((ROOT_DIR_SECTORS*DISK_BLOCK_SIZE)/sizeof(TDiskDirectoryEntry))
#define FIRST_FAT_SECTOR (RESERVED_SECTORS_COUNT)
#define FIRST_ROOT_DIR_SECTOR (FIRST_FAT_SECTOR+(BPB_NUMFATS*FAT16_SIZE_SECTORS))
#define FIRST_DATA_SECTOR (FIRST_ROOT_DIR_SECTOR+ROOT_DIR_SECTORS)
#define DATA_SECTORS (DISK_SECTORS-FIRST_DATA_SECTOR)
#define FIRST_SECTOR_OF_CLUSTER(N) (((N-2UL)*DISK_SECTORS_PER_CLUSTER)+FIRST_DATA_SECTOR)
#define BPB_MEDIA_TYPE 0xF8
#define DISK_FAT_EOC (0xFF00|BPB_MEDIA_TYPE)

static const TDiskFile *root_directory = NULL;

typedef uint16_t TFatCluster;

typedef struct TDiskRecord
{
  uint32_t offset, length;
  TDiskHandler handler;
  const void *data;
} TDiskRecord;

typedef struct
{
  uint8_t head, sector, cylinder;
} PACKED TDiskPartitionTableEntryCHS;

typedef struct
{
  uint8_t bootable;
  TDiskPartitionTableEntryCHS start;
  uint8_t partition_type;
  TDiskPartitionTableEntryCHS end;
  uint32_t start_lba;
  uint32_t length;
} PACKED TDiskPartitionTableEntry;

#define ATTR_READ_ONLY  0x01
#define ATTR_HIDDEN     0x02
#define ATTR_SYSTEM     0x04
#define ATTR_VOLUME_ID  0x08
#define ATTR_DIRECTORY  0x10
#define ATTR_ARCHIVE    0x20
#define ATTR_LONG_NAME  (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)

typedef struct
{
  char DIR_Name[11];
  uint8_t DIR_Attr;
  uint8_t DIR_NTRes;
  uint8_t DIR_CrtTimeTenth;
  uint16_t DIR_CrtTime;
  uint16_t DIR_CrtDate;
  uint16_t DIR_LstAccDate;
  uint16_t DIR_FstClusHI;
  uint16_t DIR_WrtTime;
  uint16_t DIR_WrtDate;
  uint16_t DIR_FstClusLO;
  uint32_t DIR_FileSize;
} PACKED TDiskDirectoryEntry;

/* disk signature */
const uint32_t DiskSignature = 0x1B07CF6E;

/* BPB - BIOS Parameter Block: actual volume boot block */
const TDiskBPB DiskBPB = {
  .BS_jmpBoot = {0xEB, 0x00, 0x90},
  .BS_OEMName = "MSWIN4.1",
  .BPB_BytsPerSec = DISK_BLOCK_SIZE,
  .BPB_SecPerClus = DISK_SECTORS_PER_CLUSTER,
  .BPB_RsvdSecCnt = RESERVED_SECTORS_COUNT,
  .BPB_NumFATs = BPB_NUMFATS,
  .BPB_RootEntCnt = MAX_FILES_IN_ROOT,
  .BPB_Media = BPB_MEDIA_TYPE,
  .BPB_FATSz16 = FAT16_SIZE_SECTORS,
  .BPB_SecPerTrk = DISK_SECTORS_PER_TRACK,
  .BPB_NumHeads = DISK_HEADS,
  .BPB_HiddSec = VOLUME_START,
  .BPB_TotSec32 = VOLUME_SECTORS,
  /* FAT12/FAT16 definition */
  .BS_DrvNum = 0x80,
  .BS_BootSig = 0x29,
  .BS_VolID = 0xe9d9489f,
  .BS_VolLab = "OPENBEACON ",
  .BS_FilSysType = "FAT16   ",
};

static void
msd_read_data_area (uint32_t offset, uint32_t length, const void *src,
		    uint8_t * dst)
{
  uint32_t t;
  uint32_t cluster, cluster_start, cluster_end, cluster_count;
  uint32_t cluster_file_count;

  /* remember variable content over calls to enable caching */
  static const TDiskFile *file = NULL;
  static uint32_t cluster_file_start, cluster_file_end;

  /* touch unused parameter 'src' */
  (void) src;

  /* ignore zero size read requests */
  if (!length)
    return;

  /* get extent of read/write request */
  cluster_start = offset / DISK_CLUSTER_SIZE;
  cluster_count = (length + DISK_CLUSTER_SIZE - 1) / DISK_CLUSTER_SIZE;
  cluster_end = cluster_start + cluster_count - 1;

  /* check for cached file request */
  if (file && (cluster_end >= cluster_file_start)
      && (cluster_start <= cluster_file_end))
    {
      if (file->handler)
	file->handler (offset -
		       (cluster_file_start * DISK_CLUSTER_SIZE),
		       length, file->data, dst);
      else
	memset (dst, ' ', length);
    }

  /* if no file cached, try to find file */
  cluster = 0;
  file = root_directory;
  while (file)
    {
      if (file->length)
	{
	  cluster_file_start = cluster;
	  cluster_file_count =
	    (file->length + DISK_CLUSTER_SIZE - 1) / DISK_CLUSTER_SIZE;
	  cluster_file_end = cluster_file_start + cluster_file_count - 1;

	  if ((cluster_end >= cluster_file_start)
	      && (cluster_start <= cluster_file_end))
	    {
	      /* normalize offset to file start */
	      offset -= (cluster_file_start * DISK_CLUSTER_SIZE);

	      /* if handler exists - run */
	      if (file->handler)
		file->handler (offset, length, file->data, dst);
	      /* if no handler exists copy data */
	      else if ((file->data) && (offset < file->length))
		{
		  /* calculate remaining length */
		  if ((t = file->length - offset) > length)
		    t = length;

		  /* copy data to output buffer */
		  memcpy (dst, ((uint8_t *) file->data) + offset, t);

		  /* if data is smaller, sent remainder to zero */
		  if (t < length)
		    memset (&dst[t], 0, length - t);
		}
	      /* if no data exists, set to zero */
	      else
		memset (dst, 0, length);

	      /* request could be satisfied - bail out with cached file
	         handle after handling request */
	      return;
	    }

	  cluster += cluster_file_count;
	}
      file = file->next;
    }

  /* didn't find a matching request - reset cached file to NULL */
  file = NULL;
}

static void
msd_read_fat_area (uint32_t offset, uint32_t length, const void *src,
		   uint8_t * dst)
{
  const TDiskFile *file;
  uint32_t count, t, index, remaining;
  uint32_t cluster, cluster_start, cluster_end, cluster_count;
  uint32_t cluster_file_start, cluster_file_end, cluster_file_count;
  TFatCluster *fat;

  /* touch unused parameter 'src' */
  (void) src;

  /* ignore read requests for sizes < cluster table granularity */
  if (length < sizeof (TFatCluster))
    return;

  /* get extent of read/write request */
  cluster_start = offset / sizeof (TFatCluster);
  cluster_count = length / sizeof (TFatCluster);
  cluster_end = cluster_start + cluster_count - 1;

  /* pre-set FAT */
  fat = (TFatCluster *) dst;
  t = cluster_count;

  /* special treatment for reserved clusters */
  if (!offset)
    {
      /* indicate media type */
      *fat++ = DISK_FAT_EOC;
      /* indicate clean volume */
      *fat++ = (0x8000 | 0x4000);
      t -= 2;
    }

  /* mark all custers as bad by default to make
     sure there is no free space left on the disk
     for new files */
  while (t--)
    *fat++ = 0xFFF7;

  /* first cluster number on disk is 2 */
  cluster = 2;
  file = root_directory;
  while (file)
    {
      if (file->length)
	{
	  cluster_file_start = cluster;
	  cluster_file_count =
	    (file->length + DISK_CLUSTER_SIZE - 1) / DISK_CLUSTER_SIZE;
	  cluster_file_end = cluster_file_start + cluster_file_count - 1;

	  if ((cluster_end >= cluster_file_start)
	      && (cluster_start < cluster_file_end))
	    {
	      if (cluster_file_start < cluster_start)
		{
		  index = cluster_start - cluster_file_start;
		  remaining = cluster_file_count - index;
		  index += cluster;
		  count =
		    (cluster_count > remaining) ? remaining : cluster_count;
		  fat = (TFatCluster *) dst;
		}
	      else
		{
		  index = cluster;
		  remaining = cluster_file_count;
		  t = (cluster_file_start - cluster_start);
		  count = cluster_count - t;
		  fat = ((TFatCluster *) dst) + t;
		}

	      /* populate FAT entries with linked FAT list */
	      if (remaining)
		for (t = 1; t <= count; t++)
		  if (remaining-- > 1)
		    *fat++ = index + t;
		  else
		    {
		      /* set last entry to EOC (End Of Chain) */
		      *fat = DISK_FAT_EOC;
		      break;
		    }
	    }
	  cluster += cluster_file_count;
	}
      file = file->next;
    }
}

static void
msd_read_root_dir (uint32_t offset, uint32_t length, const void *src,
		   uint8_t * dst)
{
  uint32_t cluster, t;
  const TDiskFile *file;
  TDiskDirectoryEntry *entry;

  /* touch unused parameter 'src' */
  (void) src;

  /* initialize response with zero */
  memset (dst, 0, length);

  /* first cluster number on disk is 2 */
  cluster = 2;
  /* skip first entries */
  file = root_directory;
  t = offset / sizeof (TDiskDirectoryEntry);
  while (t--)
    {
      if (!file)
	return;
      cluster += (file->length + DISK_CLUSTER_SIZE - 1) / DISK_CLUSTER_SIZE;
      file = file->next;
    }

  entry = (TDiskDirectoryEntry *) dst;
  t = length / sizeof (TDiskDirectoryEntry);
  while (t--)
    {
      if (!file)
	return;

      /* turn last entry to volume ID */
      entry->DIR_Attr = file->next ? ATTR_READ_ONLY : ATTR_VOLUME_ID;

      /* populate directory entry */
      entry->DIR_FileSize = file->length;
      entry->DIR_FstClusLO = (file->length) ? cluster : 0;
      strncpy (entry->DIR_Name, file->name, sizeof (entry->DIR_Name));

      /* increment cluster pos */
      cluster += (file->length + DISK_CLUSTER_SIZE - 1) / DISK_CLUSTER_SIZE;

      /* go to next entry */
      entry++;
      file = file->next;
    }
}

static void
msd_read_memory (uint32_t offset, uint32_t length, const void *src,
		 uint8_t * dst)
{
  memcpy (dst, ((const uint8_t *) src) + offset, length);
}

void
msd_read (uint32_t offset, uint8_t * dst, uint32_t length)
{
  const TDiskRecord *rec;
  uint32_t t, read_end, rec_start, rec_end, pos, count, written;
  uint8_t *p;


  /* first partition table entry */
  static const TDiskPartitionTableEntry DiskPartitionTableEntry = {
    .bootable = 0x00,
    .start = {
	      .head = 0,
	      .sector = VOLUME_START + 1,
	      .cylinder = 0},
    .partition_type = 0x06,
    .end = {
	    .head = DISK_HEADS - 1,
	    .sector = DISK_SECTORS_PER_TRACK,
	    .cylinder = DISK_CYLINDERS - 1},
    .start_lba = 1,
    .length = VOLUME_SECTORS
  };

  /* MBR termination signature */
  static const uint16_t BootSignature = 0xAA55;


  /* data mapping of virtual drive */
  static const TDiskRecord DiskRecord[] = {
    /* data area */
    {
     .offset = (VOLUME_START + FIRST_DATA_SECTOR) * DISK_BLOCK_SIZE,
     .length = DATA_SECTORS * DISK_BLOCK_SIZE,
     .handler = msd_read_data_area,
     .data = NULL}
    ,
    /* FAT area - primary copy */
    {
     .offset = (VOLUME_START + FIRST_FAT_SECTOR) * DISK_BLOCK_SIZE,
     .length = FAT16_SIZE_SECTORS * DISK_BLOCK_SIZE,
     .handler = msd_read_fat_area,
     .data = NULL}
    ,
    /* FAT area - secondary copy */
    {
     .offset =
     (VOLUME_START + FIRST_FAT_SECTOR + FAT16_SIZE_SECTORS) * DISK_BLOCK_SIZE,
     .length = FAT16_SIZE_SECTORS * DISK_BLOCK_SIZE,
     .handler = msd_read_fat_area,
     .data = NULL}
    ,
    /* root dir  */
    {
     .offset = (VOLUME_START + FIRST_ROOT_DIR_SECTOR) * DISK_BLOCK_SIZE,
     .length = ROOT_DIR_SECTORS * DISK_BLOCK_SIZE,
     .handler = msd_read_root_dir,
     .data = NULL}
    ,
    /* disk signature */
    {
     .offset = 0x1B8,
     .length = sizeof (DiskSignature),
     .handler = msd_read_memory,
     .data = &DiskSignature}
    ,
    /* first partition table entry */
    {
     .offset = 0x1BE,
     .length = sizeof (DiskPartitionTableEntry),
     .handler = msd_read_memory,
     .data = &DiskPartitionTableEntry}
    ,
    /* MBR termination signature */
    {
     .offset = 0x1FE,
     .length = sizeof (BootSignature),
     .handler = msd_read_memory,
     .data = &BootSignature}
    ,
    /* BPB - BIOS Parameter Block: actual volume boot block */
    {
     .offset = (VOLUME_START * DISK_BLOCK_SIZE),
     .length = sizeof (DiskBPB),
     .handler = msd_read_memory,
     .data = &DiskBPB}
    ,
    /* BPB termination signature */
    {
     .offset = (VOLUME_START * DISK_BLOCK_SIZE) + 0x1FE,
     .length = sizeof (BootSignature),
     .handler = msd_read_memory,
     .data = &BootSignature}
    ,
  };

  /* ignore zero reads and reads outside of disk area */
  if (!length || (offset >= DISK_SIZE))
    return;

  /* iterate DiskRecords and fill request with content */
  rec = DiskRecord;
  t = sizeof (DiskRecord) / sizeof (DiskRecord[0]);
  read_end = offset + length;
  written = 0;

  while (t--)
    {
      rec_start = rec->offset;
      rec_end = rec_start + rec->length;

      if ((read_end >= rec_start) && (offset < rec_end))
	{
	  if (rec_start >= offset)
	    {
	      pos = 0;
	      p = &dst[rec_start - offset];
	      count = (rec_end <= read_end) ?
		rec->length : read_end - rec_start;
	    }
	  else
	    {
	      pos = offset - rec_start;
	      p = dst;
	      count = (read_end > rec_end) ? rec_end - offset : length;
	    }

	  /* set memory of partial read to zero before filling up */
	  if (!written && (count != length))
	    memset (dst, 0, length);

	  /* handle request */
	  rec->handler (pos, count, rec->data, p);
	  written += count;

	  /* all bytes handled -> quit */
	  if (written == length)
	    break;
	}
      rec++;
    }

  /* set memory to zero if not written yet  */
  if (!written)
    memset (dst, 0, length);
}

void
msd_write (uint32_t offset, uint8_t * src, uint32_t length)
{
  (void) offset;
  (void) src;
  (void) length;
}

void
vfs_status (void)
{
}

void
vfs_init (const TDiskFile * file)
{
  if (file)
    {
      root_directory = file;
      /* init USB mass storage */
      msd_init ();
    }
}

#endif /* USB_DISK_SUPPORT */
