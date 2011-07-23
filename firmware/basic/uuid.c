#include "lpc134x.h"
#include "sysdefs.h"
#include "basic.h"
#include "xxtea.h"

#include "core/iap/iap.h"

uint32_t GetUUID32(void){
    IAP_return_t iap_return;
    iap_return = iapReadSerialNumber();
    if (iap_return.ReturnCode == 0){
        uint32_t block[4];
        uint32_t k[4] = {1,2,3,4};
        block[0] = iap_return.Result[0];
        block[1] = iap_return.Result[1];
        block[2] = iap_return.Result[2];
        block[3] = iap_return.Result[3];
        xxtea_encode_words(block, 4, k);
        return block[0];
    }
    return 0;
};

// What OpenBeacon used. Do we want this?
uint16_t GetUUID16(void){
    IAP_return_t iap_return;
    iap_return = iapReadSerialNumber();
    return crc16 ((uint8_t *) iap_return.Result, sizeof (iap_return.Result));
};
