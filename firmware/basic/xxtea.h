#ifndef _XXTEA_H_
#define _XXTEA_H_

void xxtea_cbcmac(uint32_t mac[4], uint32_t *data, uint32_t len, uint32_t key[4]);
void xxtea_encode_words(uint32_t *v, int n, uint32_t const k[4]);
void xxtea_decode_words(uint32_t *v, int n, uint32_t const k[4]);

#endif
 
