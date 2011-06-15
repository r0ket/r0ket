#include "projectconfig.h"
#include "diskio.h"
#include "mmc.h"
#include "at45db041d.h"

/* diskio interface */

DSTATUS disk_initialize(BYTE drv) {
    #if CFG_HAVE_SDCARD == 1
    switch (drv) {
        case 0:
    #endif
            return dataflash_initialize();
    #if CFG_HAVE_SDCARD == 1
        case 1:
            return mmc_initialize();
        default:
            return STA_NOINIT;
    }
    #endif
}

DSTATUS disk_status(BYTE drv) {
    #if CFG_HAVE_SDCARD == 1
    switch (drv) {
        case 0:
    #endif
            return dataflash_status();
    #if CFG_HAVE_SDCARD == 1
        case 1:
            return mmc_status();
        default:
            return STA_NOINIT;
    }
    #endif
}

DRESULT disk_read(BYTE drv, BYTE *buff, DWORD sector, BYTE count) {
    #if CFG_HAVE_SDCARD == 1
    switch (drv) {
        case 0:
    #endif
            return dataflash_read(buff, sector, count);
    #if CFG_HAVE_SDCARD == 1
        case 1:
            return mmc_read(buff, sector, count);
        default:
            return RES_PARERR;
    }
    #endif
}

#if _READONLY == 0
DRESULT disk_write(BYTE drv, const BYTE *buff, DWORD sector, BYTE count) {
    #if CFG_HAVE_SDCARD == 1
    switch (drv) {
        case 0:
    #endif
            return dataflash_write(buff, sector, count);
    #if CFG_HAVE_SDCARD == 1
        case 1:
            return mmc_write(buff, sector, count);
        default:
            return RES_PARERR;
    }
    #endif
}
#endif /* _READONLY == 0 */

#if _USE_IOCTL != 0
DRESULT disk_ioctl(BYTE drv, BYTE ctrl, void *buff) {
    #if CFG_HAVE_SDCARD == 1
    switch (drv) {
        case 0:
    #endif
            return dataflash_ioctl(ctrl, buff);
    #if CFG_HAVE_SDCARD == 1
        case 1:
            return mmc_ioctl(ctrl, buff);
        default:
            return RES_PARERR;
    }
    #endif
}
#endif /* _USE_IOCTL != 0 */
