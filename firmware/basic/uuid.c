#include "lpc134x.h"
#include "sysdefs.h"

#include "core/iap/iap.h"

uint32_t GetUUID32(void){
    IAP_return_t iap_return;
    iap_return = iapReadSerialNumber();

    return iap_return.Result[1];
};

// What OpenBeacon used. Do we want this?
uint16_t GetUUID16(void){
    IAP_return_t iap_return;
    iap_return = iapReadSerialNumber();
    return crc16 ((uint8_t *) iap_return.Result, sizeof (iap_return.Result));
};
