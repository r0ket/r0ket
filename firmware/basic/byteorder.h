#ifndef _BYTEORDER_H_
#define _BYTEORDER_H_

#include <stdint.h>

void uint32touint8p(uint32_t v, uint8_t *p);
uint32_t uint8ptouint32(uint8_t *p);

#endif
