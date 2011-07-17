#ifndef _XXTEA_H_
#define _XXTEA_H_

void xxtea_encode_words(uint32_t *v, int n, uint32_t const k[4]);
void xxtea_decode_words(uint32_t *v, int n, uint32_t const k[4]);

#endif
 
