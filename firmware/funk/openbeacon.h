#ifndef _OPENBEACON_H_
#define _OPENBEACON_H_
#include <stdint.h>
#include "funk/openbeacon.h"
#include "funk/nrf24l01p.h"
#include "basic/byteorder.h"

#define OPENBEACON_SAVE 0xFFFF

void openbeaconShutdown(void);
void openbeaconSaveBlock(void);
void openbeaconSave(uint32_t s);
void openbeaconRead();
void openbeaconSetup(uint32_t id);
uint8_t openbeaconSendPacket(uint32_t id, uint32_t ctr,
                uint8_t flags, uint8_t strength);
uint8_t openbeaconSend(void);

#endif
