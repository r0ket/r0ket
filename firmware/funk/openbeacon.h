#ifndef _OPENBEACON_H_
#define _OPENBEACON_H_
#include <stdint.h>
#include "funk/openbeacon.h"
#include "funk/nrf24l01p.h"
#include "basic/byteorder.h"

#define OPENBEACON_SAVE 0xFFFF
#define OPENBEACON_CHANNEL 81

void openbeaconSetup(void);
void openbeaconSend(void);

#endif
