
void xmit_spi(BYTE dat);

BYTE rcvr_spi (void);

/* Alternative macro to receive data fast */

#define rcvr_spi_m(dst) \
    do { \
        sspReceive(0, (uint8_t*)(dst), 1); \
    } while(0)

