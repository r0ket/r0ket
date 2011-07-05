#include <basic/basic.h>
#include <nrf24l01p.h>
#include "core/ssp/ssp.h"

#define CHANNEL_BEACON 81
#define DEFAULT_SPEED R_RF_SETUP_DR_2M
#define MAC_BEACON "BEACO"

/*-----------------------------------------------------------------------*/
/* Transmit a byte via SPI  (Platform dependent)                         */
/*-----------------------------------------------------------------------*/
void xmit_spi(uint8_t dat) {
    sspSend(0, (uint8_t*) &dat, 1);
}

/*-----------------------------------------------------------------------*/
/* Receive a byte from MMC via SPI  (Platform dependent)                 */
/*-----------------------------------------------------------------------*/
uint8_t rcvr_spi (void) {
    uint8_t data = 0;

    sspReceive(0, &data, 1);

    return data;
}

#define rcvr_spi_m(dst) \
        do { \
                    sspReceive(0, (uint8_t*)(dst), 1); \
                } while(0)

#define CS_LOW()    gpioSetValue(RB_SPI_NRF_CS, 0)
#define CS_HIGH()   gpioSetValue(RB_SPI_NRF_CS, 1)
#define CE_LOW()    gpioSetValue(RB_NRF_CE, 0)
#define CE_HIGH()   gpioSetValue(RB_NRF_CE, 1)

void nrf_cmd(uint8_t cmd){
    xmit_spi(cmd);
};

uint8_t nrf_cmd_status(uint8_t cmd){
    xmit_spi(cmd);
    return rcvr_spi();
};

void nrf_write_reg(const uint8_t reg, const uint8_t val){
    xmit_spi(C_W_REGISTER | reg);
    xmit_spi(val);
};

/*
uint8_t nrf_read_reg(const uint8_t reg, uint8_t val){
    xmit_spi(C_R_REGISTER | reg);
    // do i need to read the status byte here?
    xmit_spi(val);
    return rcvr_spi();
};
*/

void nrf_write_reg_long(const uint8_t reg, int len, char* data){
    xmit_spi(C_W_REGISTER | reg);
    for(int i=0;i<len;i++){
        xmit_spi(data[i]);
    };
};

void nrf_cmd_read_long(const uint8_t cmd, int *len, char* data){
    xmit_spi(cmd);
    // do i need to read the status byte here?
    for(int i=0;i<*len;i++){
        xmit_spi(0);
        rcvr_spi_m(&data[i]);
    };
};

void nrf_init() {
    // Enable SPI correctly
    sspInit(0, sspClockPolarity_Low, sspClockPhase_RisingEdge);

    // Enable CS & CE pins
    gpioSetDir(RB_SPI_NRF_CS, gpioDirection_Output);
    gpioSetPullup(&RB_SPI_NRF_CS_IO, gpioPullupMode_Inactive);
    gpioSetDir(RB_NRF_CE, gpioDirection_Output);
    gpioSetPullup(&RB_NRF_CE_IO, gpioPullupMode_PullUp);

    // Setup for nrf24l01+
    // power up takes 1.5ms - 3.5ms (depending on crystal)
    CS_LOW();
    nrf_write_reg(R_CONFIG,
            R_CONFIG_PRIM_RX| // Receive mode
            R_CONFIG_PWR_UP|  // Power on
            R_CONFIG_CRCO     // 2-byte CRC
            );
    
    nrf_write_reg(R_EN_AA, 0); // Disable Enhanced ShockBurst;

    nrf_write_reg(R_RF_CH, CHANNEL_BEACON &127); // Select channel

    // enable receive pipes
    nrf_write_reg(R_EN_RXADDR,R_EN_RXADDR_ERX_P0 
//            |R_EN_RXADDR_ERX_P1
            );

    nrf_write_reg(R_RX_PW_P0,16);
    nrf_write_reg_long(R_RX_ADDR_P0,5,"\x1\x2\x3\x2\1");

//    nrf_write_reg(R_RX_PW_P1,16);
//    nrf_write_reg_long(R_RX_ADDR_P1,5,"R0KET");

    // OpenBeacon transmit address
    nrf_write_reg_long(R_TX_ADDR,5,MAC_BEACON);

    // Set speed / strength
    nrf_write_reg(R_RF_SETUP,DEFAULT_SPEED|R_RF_SETUP_RF_PWR_3);

    // XXX: or write R_CONFIG last?
    CS_HIGH();
};

int nrf_rcv_pkt_time(int maxtime, int maxsize, char * pkt){
    char buf;
    int len;

    CS_LOW();
    nrf_write_reg(R_CONFIG,
            R_CONFIG_PRIM_RX| // Receive mode
            R_CONFIG_PWR_UP|  // Power on
            R_CONFIG_CRCO     // 2-byte CRC
            );
    CE_HIGH();
    delayms(maxtime); // XXX: check interrupt?
    CE_LOW();
    len=1;
    nrf_cmd_read_long(C_R_RX_PL_WID,&len,&buf);
    len=buf;
    if(len>32 || len==0){
        return 0; // no packet
    };
    if(len>maxsize){
        return -1; // packet too large
    };
    nrf_cmd_read_long(C_R_RX_PAYLOAD,&len,pkt);
    CS_HIGH();
    return len;
};
