/* simple XXTEA en/decrypt utility
 * 
 * BSD Licence
 *
 * btea function is from 
 * <https://secure.wikimedia.org/wikipedia/en/wiki/XXTEA#Reference_code>
 *
 * (c) by Sec <sec@42.org> 6/2011
 */

#include <stdint.h>
#include "xxtea.h"

#ifdef SAFE
uint32_t htonl(uint32_t v)
{
    uint32_t r=0;
    r |= (v>> 0)&0xFF; r<<=8;
    r |= (v>> 8)&0xFF; r<<=8;
    r |= (v>>16)&0xFF; r<<=8;
    r |= (v>>24)&0xFF;
    return r;
}
#else
uint32_t htonl(uint32_t v){
    __asm("rev %[value], %[value];" \
            : [value] "+r" (v) : );
    return v;
}
#endif


void htonlp(uint32_t *v, uint8_t n)
{
    while(n--){
        v[n] = htonl(v[n]);
    }
}

void xxtea_cbcmac(uint32_t mac[4], uint32_t *data,
                    uint32_t len, uint32_t const key[4])
{
    if( len & 0x03 )
        return;
    mac[0]=0;mac[1]=0;mac[2]=0;mac[3]=0;
    for(int i=0; i<len;){
        mac[0] ^= data[i++];
        mac[1] ^= data[i++];
        mac[2] ^= data[i++];
        mac[3] ^= data[i++];
        xxtea_encode_words(mac, 4, key);
    }
}

#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (k[(p&3)^e] ^ z)))

void xxtea_encode_words(uint32_t *v, int n, uint32_t const k[4])
{
    //if(k[0] == 0 && k[1] == 0 && k[2] == 0 && k[3] == 0) return;
    uint32_t y, z, sum;
    unsigned p, rounds, e;

    htonlp(v ,n);
    rounds = 6 + 52/n;
    sum = 0;
    z = v[n-1];
    do {
        sum += DELTA;
        e = (sum >> 2) & 3;
        for (p=0; p<n-1; p++) {
            y = v[p+1]; 
            z = v[p] += MX;
        }
        y = v[0];
        z = v[n-1] += MX;
    } while (--rounds);
    htonlp(v ,n);
}

void xxtea_decode_words(uint32_t *v, int n, uint32_t const k[4])
{
    //if(k[0] == 0 && k[1] == 0 && k[2] == 0 && k[3] == 0) return;
    uint32_t y, z, sum;
    unsigned p, rounds, e;
    htonlp(v ,n);

    rounds = 6 + 52/n;
    sum = rounds*DELTA;
    y = v[0];
    do {
        e = (sum >> 2) & 3;
        for (p=n-1; p>0; p--) {
            z = v[p-1];
            y = v[p] -= MX;
        }
        z = v[n-1];
        y = v[0] -= MX;
    } while ((sum -= DELTA) != 0);
    htonlp(v ,n);
}

