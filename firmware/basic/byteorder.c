#include <stdint.h>

void uint32touint8p(uint32_t v, uint8_t *p)
{
    *p++ = (v>>24)&0xFF;
    *p++ = (v>>16)&0xFF;
    *p++ = (v>> 8)&0xFF;
    *p++ = (v>> 0)&0xFF;
}
