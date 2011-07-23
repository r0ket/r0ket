#ifndef _RFTRANSFER_H
#define _RFTRANSFER_H
#include <stdint.h>

void rftransfer_send(uint16_t size, uint8_t *data);
int16_t rftransfer_receive(uint8_t *buffer, uint16_t maxlen, uint16_t timeout);


#endif
