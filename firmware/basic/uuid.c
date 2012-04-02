#include "lpc134x.h"
#include "sysdefs.h"
#include "basic.h"
#include "xxtea.h"

#include "core/iap/iap.h"

uint32_t GetUUID32(void){
    static uint32_t uuid = 0;
    if( uuid == 0){
        IAP_return_t iap_return;
        iap_return = iapReadSerialNumber();
        if (iap_return.ReturnCode == 0){
            uint32_t block[4];
            uint32_t k[4] = {0,0,0,0};
            int i;
            for(i=0; i<4; i++)
                block[i] = iap_return.Result[i];
            xxtea_encode_words(block, 4, k);
            uuid = block[0];
        }
    }
    return uuid;
};

// What OpenBeacon used. Do we want this?
uint16_t GetUUID16(void){
    IAP_return_t iap_return;
    iap_return = iapReadSerialNumber();
    return crc16 ((uint8_t *) iap_return.Result, sizeof (iap_return.Result));
};
