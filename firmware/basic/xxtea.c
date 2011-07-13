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

void xxtea_encode(uint32_t *v, int n, uint32_t const k[4]) {
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

void xxtea_decode(uint32_t *v, int n, uint32_t const k[4]) {
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

