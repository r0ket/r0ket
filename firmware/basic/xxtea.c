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

#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (k[(p&3)^e] ^ z)))
#include "xxtea.h"

uint32_t charp2uint32(uint8_t *data, uint8_t bytes)
{
    uint32_t r = 0;
    if( bytes ){
        r |= *data++;
    }
    if( bytes > 1){
        r<<=8;
        r |= *data++;
    }
    if( bytes > 2){
        r<<=8;
        r |= *data++;
    }
    if( bytes > 3){
        r<<=8;
        r |= *data++;
    }
    return r;
}

void charp2uint32p(uint8_t* data, uint8_t  n, uint32_t *v)
{
    int i,j=0;
    int fullwords = n/4;
    for(i=0; i<fullwords*4; i+=4){
        v[j++] = charp2uint32(data+i,4);
    }
    
    uint8_t extrabytes = n - i;
    v[j] = charp2uint32(data+i,extrabytes);
}

void uint322charp(uint32_t data, uint8_t *buf, uint8_t bytes)
{
    if( bytes )
        buf[0] = data>>24;
    if( bytes > 1)
        buf[1] = (data>>16)&0xFF;
    if( bytes > 2)
        buf[2] = (data>>8)&0xFF;
    if( bytes > 3)
        buf[3] = (data>>0)&0xFF;
}

void uint32p2charp(uint8_t* data, uint8_t  n, uint32_t *v)
{
    int i;
    int fullwords = n/4;
    
    for(i=0; i<fullwords; i++){
        uint322charp(v[i],data+i*4,4);
    }
    
    uint8_t extrabytes = n - fullwords*4;
    uint322charp(v[i],data+i*4,extrabytes);
}

void xxtea_encode(uint8_t *data, int n, uint32_t const k[4])
{
    uint32_t v[8];      //maximum 32 bytes
    int words = (n+3)/4;
    if( words > 8 )
        return;
    charp2uint32p(data, n, v);
    xxtea_encode_words(v, words, k);
    uint32p2charp(data, n, v);
}

void xxtea_decode(uint8_t *data, int n, uint32_t const k[4])
{
    uint32_t v[8];      //maximum 32 bytes
    int words = (n+3)/4;
    if( words > 8 )
        return;
    charp2uint32p(data, n, v);
    xxtea_decode_words(v, words, k);
    uint32p2charp(data, n, v);
}
 
void xxtea_encode_words(uint32_t *v, int n, uint32_t const k[4])
{
    if(k[0] == 0 && k[1] == 0 && k[2] == 0 && k[3] == 0) return;
    uint32_t y, z, sum;
    unsigned p, rounds, e;
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
}

void xxtea_decode_words(uint32_t *v, int n, uint32_t const k[4])
{
    if(k[0] == 0 && k[1] == 0 && k[2] == 0 && k[3] == 0) return;
    uint32_t y, z, sum;
    unsigned p, rounds, e;

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
}

