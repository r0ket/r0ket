#include "lpc134x.h"
#include "sysdefs.h"
#include "basic.h"
#include "xxtea.h"

#include "core/iap/iap.h"

uint32_t uuid32=0;
uint16_t uuid16=0;

static uint32_t __GetUUID32(void){
    IAP_return_t iap_return;
    iap_return = iapReadSerialNumber();
    if (iap_return.ReturnCode == 0){
        uint32_t block[4];
        uint32_t k[4] = {0,0,0,0};
        int i;
        for(i=0; i<4; i++)
            block[i] = iap_return.Result[i];
        xxtea_encode_words(block, 4, k);
        return block[0];
    }
    return 0;
};

// What OpenBeacon used. Do we want this?
static uint16_t __GetUUID16(void){
    IAP_return_t iap_return;
    iap_return = iapReadSerialNumber();
    return crc16 ((uint8_t *) iap_return.Result, sizeof (iap_return.Result));
};

uint32_t initUUID(void){
    uuid32=__GetUUID32();
    uuid16=__GetUUID16();
};

uint32_t GetUUID32(void){
    if(uuid32==0){
        initUUID();
    };
    return uuid32;
};

uint16_t GetUUID16(void){
    if(uuid32==0){
        initUUID();
    };
    return uuid16;
};

