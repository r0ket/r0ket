#ifndef _OPENBEACON_H_
#define _OPENBEACON_H_
#include <stdint.h>
#include "funk/openbeacon.h"
#include "funk/nrf24l01p.h"
#include "basic/byteorder.h"

#define OPENBEACON_SAVECOUNTER  (1024*8)
void openbeaconSave();
void openbeaconRead();
void openbeaconSetup(uint32_t id);
void openbeaconSendPacket(uint32_t id, uint32_t ctr,
                uint8_t flags, uint8_t strength);
void openbeaconSend(void);

#endif
