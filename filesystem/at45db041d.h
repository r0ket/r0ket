#ifndef _AT45DB041D_H
#define _AT45DB041D_H 1

DSTATUS dataflash_initialize();
DSTATUS dataflash_status();
DRESULT dataflash_read(BYTE *buff, DWORD sector, BYTE count);
DRESULT dataflash_write(const BYTE *buff, DWORD sector, BYTE count);
DRESULT dataflash_ioctl(BYTE ctrl, void *buff);

#endif /* _AT45DB041D_H */
