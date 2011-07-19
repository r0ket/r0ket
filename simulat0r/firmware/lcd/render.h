#include "../../../firmware/lcd/render.h"

#undef flip
#define flip(c) do {\
  c = ((c>>1)&0x55)|((c<<1)&0xAA); \
  c = ((c>>2)&0x33)|((c<<2)&0xCC); \
  c = (c>>4) | (c<<4); \
  }while(0)
