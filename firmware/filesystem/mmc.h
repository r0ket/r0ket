#ifndef _MMC_H
#define _MMC_H 1

DSTATUS mmc_initialize();
DSTATUS mmc_status();
DRESULT mmc_read(BYTE *buff, DWORD sector, BYTE count);
DRESULT mmc_write(const BYTE *buff, DWORD sector, BYTE count);
DRESULT mmc_ioctl(BYTE ctrl, void *buff);

#endif /* _MMC_H */
