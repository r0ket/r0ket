#ifndef CBITFIELD_H
#define CBITFIELD_H

#define BITSETCHUNKSIZE 32

#define one ((uint32_t)1)

struct bitset {
  uint16_t size;
  uint32_t bits[BITSET_SIZE/BITSETCHUNKSIZE+1];
};

static inline void bitset_set(struct bitset *bs,uint16_t index, uint8_t value) {
  uint16_t base=index/BITSETCHUNKSIZE;
  uint16_t offset=index%BITSETCHUNKSIZE;
  if(value) {
    bs->bits[base]|=(one<<offset);
  } else {
    bs->bits[base]&=~(one<<offset);
  }
}

static inline void bitset_xor(struct bitset *bs,uint16_t index, uint8_t value) {
  uint16_t base=index/BITSETCHUNKSIZE;
  uint16_t offset=index%BITSETCHUNKSIZE;
  if(value) {
    bs->bits[base]^=(one<<offset);
  }
}

static inline uint8_t bitset_get(struct bitset *bs,uint16_t index) {
  uint16_t base=index/BITSETCHUNKSIZE;
  uint16_t offset=index%BITSETCHUNKSIZE;
  return (bs->bits[base]&(one<<offset))==(one<<offset);;
}

static inline uint16_t bitset_offset2(uint8_t x, uint8_t y) {
  return ((uint16_t)x)+((uint16_t)y)*BITSET_X;
}

static inline void bitset_set2(struct bitset *bs, uint8_t x, uint8_t y, uint8_t value) {
  bitset_set(bs,bitset_offset2(x,y),value);
}

static inline void bitset_xor2(struct bitset *bs, uint8_t x, uint8_t y, uint8_t value) {
  bitset_xor(bs,bitset_offset2(x,y),value);
}

static inline uint8_t bitset_get2(struct bitset *bs,uint8_t x,uint8_t y) {
  return bitset_get(bs,bitset_offset2(x,y));
}

#endif
