#include <stdint.h>
#include "funk/openbeacon.h"
#include "funk/nrf24l01p.h"
#include "basic/byteorder.h"
#include "sysdefs.h"
#include "filesystem/ff.h"
#include "basic/uuid.h"

#include "SECRETS"

#define SAVE_OPENBEACON 1
const uint8_t mac[5] = {1,2,3,2,1};

volatile uint32_t oid = 0;
volatile uint32_t seq = 0;
volatile uint8_t strength = 0;

#if SAVE_OPENBEACON
static void openbeaconSave(uint32_t s);
#endif

static struct NRF_CFG oldconfig;

#if SAVE_OPENBEACON
static void openbeaconShutdown(void)
{    
    openbeaconSave(seq);
}

static void openbeaconSaveBlock(void)
{

    openbeaconSave(seq + OPENBEACON_SAVE + 1);
}

static void openbeaconSave(uint32_t s)
{
    FIL file;
    BYTE buf[4];
    UINT readbytes;

    if( f_open(&file, "beacon.cfg", FA_OPEN_ALWAYS|FA_WRITE) )
        return;

    uint32touint8p(s, buf);

    if( f_write(&file, buf, 4, &readbytes) )
        return; 

    f_close(&file);
}

static void openbeaconRead()
{
    FIL file;
    BYTE buf[4];
    UINT readbytes;

    if( f_open(&file, "beacon.cfg", FA_OPEN_EXISTING|FA_READ) )
        return;

    if( f_read(&file, buf, 4, &readbytes) )
        return;
    seq = uint8ptouint32(buf);
    
    f_close(&file);
}
#endif

void openbeaconSetup(void)
{
    oid = GetUUID32();
    strength = 0;
#if SAVE_OPENBEACON
    openbeaconRead();
    openbeaconSaveBlock();
#endif
}

static uint8_t openbeaconSendPacket(uint32_t id, uint32_t seq,
        uint8_t flags, uint8_t strength)
{
    uint8_t buf[32];

    volatile uint8_t i;
    i = getRandom()&0xff;
    while(i--);

    buf[0]=0x10; // Length: 16 bytes
    buf[1]=0x17; // Proto - fixed at 0x17?
    buf[2]=flags;
    buf[3]=strength*85; // Send intensity

    uint32touint8p(seq, buf+4);
    uint32touint8p(id, buf+8);

    buf[12]=0xff; // salt (0xffff always?)
    buf[13]=0xff;

#if ENCRYPT_OPENBEACON
    return nrf_snd_pkt_crc_encr(16,buf,openbeaconkey);
#else
    return nrf_snd_pkt_crc_encr(16,buf,NULL);
#endif
}

void openbeaconSend(void)
{
    //uint8_t status;

    nrf_config_get(&oldconfig);

    nrf_set_channel(OPENBEACON_CHANNEL);
    nrf_set_strength(strength);
    nrf_set_tx_mac(sizeof(mac), mac);

    openbeaconSendPacket(oid, seq++, 0xFF, strength++);
    if( strength == 4 )
        strength = 0;
#if SAVE_OPENBEACON
    if( (seq & OPENBEACON_SAVE) == OPENBEACON_SAVE )
        openbeaconSaveBlock();
#endif
    nrf_config_set(&oldconfig);
    //return status;
}

