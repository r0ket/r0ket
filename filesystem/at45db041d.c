#include "projectconfig.h"
#include "diskio.h"
#include "iobase.h"
#include "core/ssp/ssp.h"
#include "basic/basic.h"

/* Opcodes */
#define OP_POWERDOWN      (0xB9)
#define OP_RESUME         (0xAB)
#define OP_PAGEREAD       (0xD2)
#define OP_BUFFER1READ    (0xD1) /* Low Frequency (<=33MHz) */
#define OP_BUFFER2READ    (0xD3) /* Low Frequency (<=33MHz) */
#define OP_BUFFER1WRITE   (0x84)
#define OP_BUFFER2WRITE   (0x87)
#define OP_BUFFER1PROG    (0x83) /* with builtin erase */
#define OP_BUFFER2PROG    (0x86) /* with builtin erase */
#define OP_STATUSREAD     (0xD7)
#define OP_DEVICEID       (0x9F)
#define OP_PAGE2BUFFER1   (0x53)
#define OP_PAGE2BUFFER2   (0x55)
#define OP_BUFFER1PAGECMP (0x60)
#define OP_BUFFER2PAGECMP (0x61)
#define OP_AUTOREWRITE1   (0x58) /* Auto Page Rewrite throught Buffer 1 */
#define OP_AUTOREWRITE2   (0x59) /* Auto Page Rewrite throught Buffer 2 */

#define SB_READY          (1 << 7)
#define SB_COMP           (1 << 6)
#define SB_PROTECT        (1 << 1)
#define SB_PAGESIZE       (1 << 0)

#define CS_LOW()    gpioSetValue(RB_SPI_CS_DF, 0)
#define CS_HIGH()   gpioSetValue(RB_SPI_CS_DF, 1)

static volatile DSTATUS status = STA_NOINIT;

static void wait_for_ready() {
    BYTE reg_status = 0xFF;

    CS_LOW();
    xmit_spi(OP_STATUSREAD);
    do {
        rcvr_spi_m((uint8_t *) &reg_status);
    } while (!(reg_status & SB_READY));
    CS_HIGH();
}

static void dataflash_powerdown() {
    CS_LOW();
    xmit_spi(OP_POWERDOWN);
    CS_HIGH();
}

static void dataflash_resume() {
    CS_LOW();
    xmit_spi(OP_RESUME);
    CS_HIGH();
}

DSTATUS dataflash_initialize() {
    sspInit(0, sspClockPolarity_Low, sspClockPhase_RisingEdge);

    gpioSetDir(RB_SPI_CS_DF, gpioDirection_Output);

    dataflash_resume();
    status &= ~STA_NOINIT;
    return status;
}

DSTATUS dataflash_status() {
    return status;
}

DRESULT dataflash_read(BYTE *buff, DWORD sector, BYTE count) {
    if (!count) return RES_PARERR;
    if (status & STA_NOINIT) return RES_NOTRDY;

    /* convert sector numbers to page numbers */
    sector *= 2;
    count  *= 2;

    do {
        wait_for_ready();
        DWORD pageaddr = sector << 9; // lower 9 bits are byte address within the page
        BYTE remaining = 255;
        CS_LOW();
        xmit_spi(OP_PAGEREAD);
        xmit_spi((BYTE)(pageaddr >> 16));
        xmit_spi((BYTE)(pageaddr >> 8));
        xmit_spi((BYTE)pageaddr);
        xmit_spi(0x00); // follow up with 4 don't care bytes
        xmit_spi(0x00);
        xmit_spi(0x00);
        xmit_spi(0x00);
        do {
            rcvr_spi_m(buff++);
        } while (--remaining);
        sector++;
        CS_HIGH();
    } while (--count);

    return count ? RES_ERROR : RES_OK;
}

#if _READONLY == 0
DRESULT dataflash_write(const BYTE *buff, DWORD sector, BYTE count) {
    if (!count) return RES_PARERR;
    if (status & STA_NOINIT) return RES_NOTRDY;

    /* convert sector numbers to page numbers */
    sector *= 2;
    count  *= 2;

    do {
        wait_for_ready();
        DWORD pageaddr = sector << 9; // lower 9 bits are byte address within the page
        BYTE remaining = 255;

        // write bytes into the dataflash buffer
        CS_LOW();
        xmit_spi(OP_BUFFER1WRITE);
        xmit_spi(0x00);
        xmit_spi(0x00);
        xmit_spi(0x00);
        do {
            xmit_spi(*buff++);
        } while (--remaining);
        sector++;
        CS_HIGH();
        wait_for_ready();

        // compare buffer with target memory page
        CS_LOW();
        xmit_spi(OP_BUFFER1PAGECMP);
        xmit_spi((BYTE)(pageaddr >> 16));
        xmit_spi((BYTE)(pageaddr >> 8));
        xmit_spi((BYTE)pageaddr);
        CS_HIGH();
        wait_for_ready();
        CS_LOW();
        BYTE reg_status = 0xFF;
        xmit_spi(OP_STATUSREAD);
        rcvr_spi_m((uint8_t *) &reg_status);
        CS_HIGH();

        // trigger program only if data changed
        if (reg_status & SB_COMP) {
            CS_LOW();
            xmit_spi(OP_BUFFER1PROG);
            xmit_spi((BYTE)(pageaddr >> 16));
            xmit_spi((BYTE)(pageaddr >> 8));
            xmit_spi((BYTE)pageaddr);
            CS_HIGH();
        }
    } while (--count);

    return count ? RES_ERROR : RES_OK;
}
#endif /* _READONLY */

#if _USE_IOCTL != 0
DRESULT dataflash_ioctl(BYTE ctrl, void *buff) {
    DRESULT res;
    BYTE *ptr = buff;

    res = RES_ERROR;


    if (ctrl == CTRL_POWER) {
        switch (*ptr) {
            case 0: /* Sub control code == 0 (POWER_OFF) */
                dataflash_powerdown();
                res = RES_OK;
                break;
            case 1: /* Sub control code == 1 (POWER_ON) */
                dataflash_resume();
                res = RES_OK;
                break;
            case 2: /* Sub control code == 2 (POWER_GET) */
                // TODO: figure out a way to retrieve the powerstate
                *(ptr+1) = (BYTE)1;
                res = RES_OK;
                break;
            default :
                res = RES_PARERR;
            }
    } else {
        if (status & STA_NOINIT) return RES_NOTRDY;

        switch (ctrl) {
            case CTRL_SYNC:
                wait_for_ready();
                res = RES_OK;
                break;
            case GET_SECTOR_COUNT:
                // TODO: read from device ID register
                *(WORD*)buff = 2048;
                res = RES_OK;
                break;
            case GET_SECTOR_SIZE:
                *(WORD*)buff = 512;
                res = RES_OK;
                break;
            case GET_BLOCK_SIZE:
                *(WORD*)buff = 1;
                res = RES_OK;
                break;
            default:
                res = RES_PARERR;
        }
    }

    return res;
}
#endif /* _USE_IOCTL != 0 */

