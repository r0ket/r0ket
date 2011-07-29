#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/lcd.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"

#include <string.h>

#include "funk/rftransfer.h"
#include "funk/openbeacon.h"

#include "core/iap/iap.h"

/**************************************************************************/

void f_uuid(void) {
    IAP_return_t iap_return;
    iap_return = iapReadSerialNumber();
    lcdPrintIntHex(iap_return.Result[0]); lcdNl();
    lcdPrintIntHex(iap_return.Result[1]); lcdNl();
    lcdPrintIntHex(iap_return.Result[2]); lcdNl();
    lcdPrintIntHex(iap_return.Result[3]); lcdNl();
}
