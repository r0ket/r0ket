#ifndef _XXTEA_H_
#define _XXTEA_H_

uint32_t charp2uint32(uint8_t *data, uint8_t bytes);
void charp2uint32p(uint8_t* data, uint8_t  n, uint32_t *v);
void uint322charp(uint32_t data, uint8_t *buf, uint8_t bytes);
void uint32p2charp(uint8_t* data, uint8_t  n, uint32_t *v);
void xxtea_encode(uint8_t *data, int n, uint32_t const k[4]);
void xxtea_decode(uint8_t *v, int n, uint32_t const k[4]);
void xxtea_encode_words(uint32_t *v, int n, uint32_t const k[4]);
void xxtea_decode_words(uint32_t *v, int n, uint32_t const k[4]);

#endif
 
