#ifndef _OPENBEACON_H_
#define _OPENBEACON_H_
#include <stdint.h>

void openbeaconSetup(uint32_t id, uint32_t ctr);
void openbeaconSendPacket(uint32_t id, uint32_t ctr,
                            uint8_t flags, uint8_t strength);
void openbeaconSend(void);

#endif
