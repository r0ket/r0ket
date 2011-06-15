#include "diskio.h"
#include "core/ssp/ssp.h"

/*-----------------------------------------------------------------------*/
/* Transmit a byte via SPI  (Platform dependent)                  */
/*-----------------------------------------------------------------------*/
void xmit_spi(BYTE dat) {
    sspSend(0, (uint8_t*) &dat, 1);
}

/*-----------------------------------------------------------------------*/
/* Receive a byte from MMC via SPI  (Platform dependent)                 */
/*-----------------------------------------------------------------------*/
BYTE rcvr_spi (void) {
    BYTE data = 0;

    sspReceive(0, &data, 1);

    return data;
}

