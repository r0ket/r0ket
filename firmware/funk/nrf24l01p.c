#include <basic/basic.h>
#include <nrf24l01p.h>
#include "core/ssp/ssp.h"
#include "basic/xxtea.h"

#define CHANNEL_BEACON 81
#define DEFAULT_SPEED R_RF_SETUP_DR_2M
#define MAC_BEACON "\x1\x2\x3\x2\1"

/*-----------------------------------------------------------------------*/
/* Transmit a byte via SPI                                               */
/*-----------------------------------------------------------------------*/
inline void xmit_spi(uint8_t dat) {
    sspSend(0, (uint8_t*) &dat, 1);
}

inline void rcv_spi(uint8_t *dat) {
    sspReceive(0, dat, 1);
}

#define CS_LOW()    gpioSetValue(RB_SPI_NRF_CS, 0)
#define CS_HIGH()   gpioSetValue(RB_SPI_NRF_CS, 1)
#define CE_LOW()    gpioSetValue(RB_NRF_CE, 0)
#define CE_HIGH()   gpioSetValue(RB_NRF_CE, 1)

void nrf_cmd(uint8_t cmd){
    CS_LOW();
    xmit_spi(cmd);
    CS_HIGH();
};

uint8_t nrf_cmd_status(uint8_t cmd){
    CS_LOW();
    sspSendReceive(0, &cmd, 1);
    CS_HIGH();
    return cmd;
};

void nrf_cmd_rw_long(uint8_t* data, int len){
    CS_LOW();
    sspSendReceive(0,data,len);
    CS_HIGH();
};


void nrf_write_reg(const uint8_t reg, const uint8_t val){
    CS_LOW();
    xmit_spi(C_W_REGISTER | reg);
    xmit_spi(val);
    CS_HIGH();
};

uint8_t nrf_read_reg(const uint8_t reg){
    uint8_t val;
    CS_LOW();
    xmit_spi(C_R_REGISTER | reg);
    rcv_spi(&val);
    CS_HIGH();
    return val;
};

void nrf_read_long(const uint8_t cmd, int len, uint8_t* data){
    CS_LOW();
    xmit_spi(cmd);
    for(int i=0;i<len;i++)
        data[i] = 0x00;
    sspSendReceive(0,data,len);
    CS_HIGH();
};

void nrf_read_pkt_crc(int len, uint8_t* data, uint8_t* crc){
    CS_LOW();
    xmit_spi(C_R_RX_PAYLOAD);
    sspReceive(0,data,len);
    sspReceive(0,crc,2);
    CS_HIGH();
};

void nrf_write_long(const uint8_t cmd, int len, uint8_t* data){
    CS_LOW();
    xmit_spi(cmd);
    sspSend(0,data,len);
    CS_HIGH();
};

#define nrf_write_reg_long(reg, len, data) \
    nrf_write_long(C_W_REGISTER|(reg), len, data)

// High-Level:
int nrf_rcv_pkt_time_xxtea(int maxtime, int maxsize,
                                uint8_t * pkt, uint32_t const k[4])
{
    int n = nrf_rcv_pkt_time(maxtime, maxsize, pkt);
    if( n ){
        if( n < 2 )
            return -4;
        xxtea_decode(pkt, n, k);
        uint16_t crc=crc16(pkt,n-2);
        if( pkt[n-2] == ((crc>>8)&0xFF) && pkt[n-1] == (crc&0xFF) ){
            return n;
        }else{
            return -5;
        }
    }
    return n;
}


int nrf_rcv_pkt_time(int maxtime, int maxsize, uint8_t * pkt){
    uint8_t len;
    uint8_t status=0;
    uint8_t crc[2];
    uint16_t cmpcrc;

    nrf_write_reg(R_CONFIG,
            R_CONFIG_PRIM_RX| // Receive mode
            R_CONFIG_PWR_UP|  // Power on
            R_CONFIG_EN_CRC   // CRC on, single byte
            );

    nrf_cmd(C_FLUSH_RX);
    nrf_write_reg(R_STATUS,0);

    CE_HIGH();

    for(int i=0;i<maxsize;i++) pkt[i] = 0x00; // Sanity: clear packet buffer

#define LOOPY 10
    for (;maxtime >= LOOPY;maxtime-=LOOPY){
        delayms(LOOPY);
        //        status =nrf_cmd_status(C_NOP);
        CS_LOW(); status=C_NOP; sspSendReceive(0, &status, 1); CS_HIGH();
        if( (status & R_STATUS_RX_DR) == R_STATUS_RX_DR){
            if( (status & R_STATUS_RX_P_NO) == R_STATUS_RX_FIFO_EMPTY){
                nrf_cmd(C_FLUSH_RX);
                delayms(1);
                nrf_write_reg(R_STATUS,0);
                continue;
            }else{ // Get/Check packet...
                nrf_read_long(C_R_RX_PL_WID,1,&len);
                if(len>32 || len==0){
                    continue;
                    return -2; // no packet error
                };
                len-=2; // crc is not part of the length
                if(len>maxsize){
                    continue;
                    return -1; // packet too large
                };

                nrf_read_pkt_crc(len,pkt,crc);
                cmpcrc=crc16(pkt,len);

                if(cmpcrc != (crc[0] <<8 | crc[1])) {
                    continue;
                    return -3; // CRC failed
                };
                break;
            };
        };
    };

    CE_LOW();
    CS_HIGH();

    if(maxtime<LOOPY)
        return 0; // timeout

    return len;
};

char nrf_snd_pkt_crc(int size, uint8_t * pkt){

    if(size > MAX_PKT)
        size=MAX_PKT;

    nrf_write_reg(R_CONFIG,
            R_CONFIG_PWR_UP|  // Power on
            R_CONFIG_EN_CRC   // CRC on, single byte
            );
    
//    nrf_write_long(C_W_TX_PAYLOAD,size,pkt);
    uint16_t crc=crc16(pkt,size);
    CS_LOW();
    xmit_spi(C_W_TX_PAYLOAD);
    sspSend(0,pkt,size);
    xmit_spi((crc >>8) & 0xff);
    xmit_spi(crc & 0xff);
    CS_HIGH();

    CE_HIGH();
    delayms(1); // Send it.  (only needs >10ys, i think)
    CE_LOW();

    return nrf_cmd_status(C_NOP);
};

char nrf_snd_pkt_xxtea(int size, uint8_t * pkt, uint32_t const k[4])
{
    xxtea_encode(pkt, size, k);
    return nrf_snd_pkt_crc(size, pkt);
}

void nrf_set_rx_mac(int pipe, int rxlen, int maclen, uint8_t * mac){
#ifdef SAFE
    assert(maclen>=1 || maclen<=5);
    assert(rxlen>=1 || rxlen<=32);
    assert(pipe>=0 || pipe<=5);
    assert(mac!=NULL);
    if(pipe>1)
        assert(maclen==1);
#endif
    nrf_write_reg(R_RX_PW_P0+pipe,rxlen);

    nrf_write_reg_long(R_RX_ADDR_P0+pipe,maclen,mac);
    nrf_write_reg(R_EN_RXADDR, 
            nrf_read_reg(R_EN_RXADDR) | (1<<pipe)
            );
};

void nrf_set_tx_mac(int maclen, uint8_t * mac){
#ifdef SAFE
    assert(maclen>=1 || maclen<=5);
    assert(mac!=NULL);
#endif
    nrf_write_reg_long(R_TX_ADDR,maclen,mac);
};

void nrf_disable_pipe(int pipe){
#ifdef SAFE
    assert(pipe>=0 || pipe<=5);
#endif
    nrf_write_reg(R_EN_RXADDR, 
            nrf_read_reg(R_EN_RXADDR) & ~(1<<pipe)
            );
};

void nrf_set_channel(int channel){
#ifdef SAFE
    assert(channel &~R_RF_CH_BITS ==0);
#endif
    nrf_write_reg(R_RF_CH, channel);
};

void nrf_init() {
    // Enable SPI correctly
    sspInit(0, sspClockPolarity_Low, sspClockPhase_RisingEdge);

    // Enable CS & CE pins
    gpioSetDir(RB_SPI_NRF_CS, gpioDirection_Output);
    gpioSetPullup(&RB_SPI_NRF_CS_IO, gpioPullupMode_Inactive);
    gpioSetDir(RB_NRF_CE, gpioDirection_Output);
    gpioSetPullup(&RB_NRF_CE_IO, gpioPullupMode_PullUp);
    CE_LOW();

    // Setup for nrf24l01+
    // power up takes 1.5ms - 3.5ms (depending on crystal)
    CS_LOW();
    nrf_write_reg(R_CONFIG,
            R_CONFIG_PRIM_RX| // Receive mode
            R_CONFIG_PWR_UP|  // Power on
            R_CONFIG_EN_CRC   // CRC on, single byte
            );
    
    nrf_write_reg(R_EN_AA, 0); // Disable Enhanced ShockBurst;

    nrf_set_channel(CHANNEL_BEACON);

    // enable receive pipes
    nrf_write_reg(R_EN_RXADDR,R_EN_RXADDR_ERX_P0 
//            |R_EN_RXADDR_ERX_P1
            );

    nrf_write_reg(R_RX_PW_P0,16);
    nrf_write_reg_long(R_RX_ADDR_P0,5,(uint8_t*)MAC_BEACON);

//    nrf_write_reg(R_RX_PW_P1,16);
//    nrf_write_reg_long(R_RX_ADDR_P1,5,"R0KET");

    // OpenBeacon transmit address
    nrf_write_reg_long(R_TX_ADDR,5,(uint8_t*)MAC_BEACON);

    // Set speed / strength
    nrf_write_reg(R_RF_SETUP,DEFAULT_SPEED|R_RF_SETUP_RF_PWR_3);

    // XXX: or write R_CONFIG last?
};

