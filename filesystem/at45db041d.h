#ifndef _AT45DB041D_H
#define _AT45DB041D_H 1

#include "diskio.h"

DSTATUS dataflash_initialize();
DSTATUS dataflash_status();
DRESULT dataflash_read(BYTE *buff, DWORD sector, BYTE count);
DRESULT dataflash_random_read(BYTE *buff, DWORD offset, DWORD length);
DRESULT dataflash_write(const BYTE *buff, DWORD sector, BYTE count);
DRESULT dataflash_random_write(const BYTE *buff, DWORD offset, DWORD length);
DRESULT dataflash_ioctl(BYTE ctrl, void *buff);

#endif /* _AT45DB041D_H */
