#ifndef _XXTEA_H_
#define _XXTEA_H_

void xxtea_encode(uint32_t *v, int n, uint32_t const k[4]);
void xxtea_decode(uint32_t *v, int n, uint32_t const k[4]);

#endif
 
