#include <stdint.h>
#include "funk/openbeacon.h"
#include "funk/nrf24l01p.h"
#include "basic/byteorder.h"
#include "sysdefs.h"

const uint32_t key[4] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
const uint8_t enctoggle = 0;
uint32_t oid = 0;
uint32_t ctr = 0;
uint8_t strength = 0;

void openbeaconSave()
{
}

void openbeaconRead()
{
}


void openbeaconSetup(uint32_t id)
{
    oid = id;
    strength = 0;
    openbeaconRead();
}

void openbeaconSendPacket(uint32_t id, uint32_t ctr, uint8_t flags, uint8_t strength)
{
    uint8_t buf[32];
    int status;

    buf[0]=0x10; // Length: 16 bytes
    buf[1]=0x17; // Proto - fixed at 0x17?
    buf[2]=flags;
    buf[3]=strength*85; // Send intensity

    uint32touint8p(ctr, buf+4);
    uint32touint8p(id, buf+8);

    buf[12]=0xff; // salt (0xffff always?)
    buf[13]=0xff;

    status=nrf_snd_pkt_crc_encr(16,buf,enctoggle?key:NULL);
}

void openbeaconSend(void)
{
    //uint8_t tmp = nrfgetstrength();
    //nrfsetstrength(strength);
    openbeaconSendPacket(oid, ctr++, 0xFF, strength++);
    if( strength == 4 )
        strength = 0;
    if( ctr % OPENBEACON_SAVECOUNTER  == 0 )
        openbeaconSave();
    //maybe this produces timing problems?
    //nrfsetstrength(tmp);
}
