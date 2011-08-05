/* 
  This program implements the ECIES public key encryption scheme based on the
  NIST B163 elliptic curve and the XTEA block cipher. The code was written
  as an accompaniment for an article published in phrack #63 and is released to
  the public domain.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>

#define MACRO(A) do { A; } while(0)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CHARS2INT(ptr) ntohl(*(uint32_t*)(ptr))
#define INT2CHARS(ptr, val) MACRO( *(uint32_t*)(ptr) = htonl(val) )

#define DEV_RANDOM "/dev/urandom"

#define FATAL(s) MACRO( perror(s); exit(255) )

/******************************************************************************/

#define DEGREE 163                      /* the degree of the field polynomial */
#define MARGIN 3                                          /* don't touch this */
#define NUMWORDS ((DEGREE + MARGIN + 31) / 32)

   /* the following type will represent bit vectors of length (DEGREE+MARGIN) */
typedef uint32_t bitstr_t[NUMWORDS];

     /* some basic bit-manipulation routines that act on these vectors follow */
#define bitstr_getbit(A, idx) ((A[(idx) / 32] >> ((idx) % 32)) & 1)
#define bitstr_setbit(A, idx) MACRO( A[(idx) / 32] |= 1 << ((idx) % 32) )
#define bitstr_clrbit(A, idx) MACRO( A[(idx) / 32] &= ~(1 << ((idx) % 32)) )

#define bitstr_clear(A) MACRO( memset(A, 0, sizeof(bitstr_t)) )
#define bitstr_copy(A, B) MACRO( memcpy(A, B, sizeof(bitstr_t)) )
#define bitstr_swap(A, B) MACRO( bitstr_t h; \
  bitstr_copy(h, A); bitstr_copy(A, B); bitstr_copy(B, h) )
#define bitstr_is_equal(A, B) (! memcmp(A, B, sizeof(bitstr_t)))

int bitstr_is_clear(const bitstr_t x)
{
  int i;
  for(i = 0; i < NUMWORDS && ! *x++; i++);
  return i == NUMWORDS;
}

                              /* return the number of the highest one-bit + 1 */
int bitstr_sizeinbits(const bitstr_t x)
{
  int i;
  uint32_t mask;
  for(x += NUMWORDS, i = 32 * NUMWORDS; i > 0 && ! *--x; i -= 32);
  if (i)
    for(mask = 1 << 31; ! (*x & mask); mask >>= 1, i--);
  return i;
}

                                              /* left-shift by 'count' digits */
void bitstr_lshift(bitstr_t A, const bitstr_t B, int count)
{
  int i, offs = 4 * (count / 32);
  memmove((void*)A + offs, B, sizeof(bitstr_t) - offs);
  memset(A, 0, offs);
  if (count %= 32) {
    for(i = NUMWORDS - 1; i > 0; i--)
      A[i] = (A[i] << count) | (A[i - 1] >> (32 - count));
    A[0] <<= count;
  }
}

                                            /* (raw) import from a byte array */
void bitstr_import(bitstr_t x, const char *s)
{
  int i;
  for(x += NUMWORDS, i = 0; i < NUMWORDS; i++, s += 4)
    *--x = CHARS2INT(s);
}

                                              /* (raw) export to a byte array */
void bitstr_export(char *s, const bitstr_t x)
{
  int i;
  for(x += NUMWORDS, i = 0; i < NUMWORDS; i++, s += 4)
    INT2CHARS(s, *--x);
}

                                   /* export as hex string (null-terminated!) */
void bitstr_to_hex(char *s, const bitstr_t x)
{
  int i;
  for(x += NUMWORDS, i = 0; i < NUMWORDS; i++, s += 8)
    sprintf(s, "%08x", *--x);
}

                                                  /* import from a hex string */
int bitstr_parse(bitstr_t x, const char *s)
{
  int len;
  if ((s[len = strspn(s, "0123456789abcdefABCDEF")]) ||
      (len > NUMWORDS * 8))
    return -1;
  bitstr_clear(x);
  x += len / 8;
  if (len % 8) {
    sscanf(s, "%08x", x);
    *x >>= 32 - 4 * (len % 8);
    s += len % 8;
    len &= ~7;
  }
  for(; *s; s += 8)
    sscanf(s, "%08x", --x);
  return len;
}

/******************************************************************************/

typedef bitstr_t elem_t;           /* this type will represent field elements */

elem_t poly;                                      /* the reduction polynomial */

#define field_set1(A) MACRO( A[0] = 1; memset(A + 1, 0, sizeof(elem_t) - 4) )

int field_is1(const elem_t x)
{
  int i;
  if (*x++ != 1) return 0;
  for(i = 1; i < NUMWORDS && ! *x++; i++);
  return i == NUMWORDS;
}

void field_add(elem_t z, const elem_t x, const elem_t y)    /* field addition */
{
  int i;
  for(i = 0; i < NUMWORDS; i++)
    *z++ = *x++ ^ *y++;
}

#define field_add1(A) MACRO( A[0] ^= 1 )

                                                      /* field multiplication */
void field_mult(elem_t z, const elem_t x, const elem_t y)
{
  elem_t b;
  int i, j;
  /* assert(z != y); */
  bitstr_copy(b, x);
  if (bitstr_getbit(y, 0))
    bitstr_copy(z, x);
  else
    bitstr_clear(z);
  for(i = 1; i < DEGREE; i++) {
    for(j = NUMWORDS - 1; j > 0; j--)
      b[j] = (b[j] << 1) | (b[j - 1] >> 31);
    b[0] <<= 1;
    if (bitstr_getbit(b, DEGREE))
      field_add(b, b, poly);
    if (bitstr_getbit(y, i))
      field_add(z, z, b);
  }
}

void field_invert(elem_t z, const elem_t x)                /* field inversion */
{
  elem_t u, v, g, h;
  int i;
  bitstr_copy(u, x);
  bitstr_copy(v, poly);
  bitstr_clear(g);
  field_set1(z);
  while (! field_is1(u)) {
    i = bitstr_sizeinbits(u) - bitstr_sizeinbits(v);
    if (i < 0) {
      bitstr_swap(u, v); bitstr_swap(g, z); i = -i;
    }
    bitstr_lshift(h, v, i);
    field_add(u, u, h);
    bitstr_lshift(h, g, i);
    field_add(z, z, h);
  }
}

/******************************************************************************/

/* The following routines do the ECC arithmetic. Elliptic curve points
   are represented by pairs (x,y) of elem_t. It is assumed that curve
   coefficient 'a' is equal to 1 (this is the case for all NIST binary
   curves). Coefficient 'b' is given in 'coeff_b'.  '(base_x, base_y)'
   is a point that generates a large prime order group.             */

elem_t coeff_b, base_x, base_y;

#define point_is_zero(x, y) (bitstr_is_clear(x) && bitstr_is_clear(y))
#define point_set_zero(x, y) MACRO( bitstr_clear(x); bitstr_clear(y) )
#define point_copy(x1, y1, x2, y2) MACRO( bitstr_copy(x1, x2); \
                                          bitstr_copy(y1, y2) )

                           /* check if y^2 + x*y = x^3 + *x^2 + coeff_b holds */
int is_point_on_curve(const elem_t x, const elem_t y)
{
  elem_t a, b;
  if (point_is_zero(x, y))
    return 1;
  field_mult(a, x, x);
  field_mult(b, a, x);
  field_add(a, a, b);
  field_add(a, a, coeff_b);
  field_mult(b, y, y);
  field_add(a, a, b);
  field_mult(b, x, y);
  return bitstr_is_equal(a, b);
}

void point_double(elem_t x, elem_t y)               /* double the point (x,y) */
{
  if (! bitstr_is_clear(x)) {
    elem_t a;
    field_invert(a, x);
    field_mult(a, a, y);
    field_add(a, a, x);
    field_mult(y, x, x);
    field_mult(x, a, a);
    field_add1(a);        
    field_add(x, x, a);
    field_mult(a, a, x);
    field_add(y, y, a);
  }
  else
    bitstr_clear(y);
}

                   /* add two points together (x1, y1) := (x1, y1) + (x2, y2) */
void point_add(elem_t x1, elem_t y1, const elem_t x2, const elem_t y2)
{
  if (! point_is_zero(x2, y2)) {
    if (point_is_zero(x1, y1))
      point_copy(x1, y1, x2, y2);
    else {
      if (bitstr_is_equal(x1, x2)) {
	if (bitstr_is_equal(y1, y2))
	  point_double(x1, y1);
	else 
	  point_set_zero(x1, y1);
      }
      else {
	elem_t a, b, c, d;
	field_add(a, y1, y2);
	field_add(b, x1, x2);
	field_invert(c, b);
	field_mult(c, c, a);
	field_mult(d, c, c);
	field_add(d, d, c);
	field_add(d, d, b);
	field_add1(d);
	field_add(x1, x1, d);
	field_mult(a, x1, c);
	field_add(a, a, d);
	field_add(y1, y1, a);
	bitstr_copy(x1, d);
      }
    }
  }
}

/******************************************************************************/

typedef bitstr_t exp_t;

exp_t base_order;

                         /* point multiplication via double-and-add algorithm */
void point_mult(elem_t x, elem_t y, const exp_t exp)
{
  elem_t X, Y;
  int i;
  point_set_zero(X, Y);
  for(i = bitstr_sizeinbits(exp) - 1; i >= 0; i--) {
    point_double(X, Y);
    if (bitstr_getbit(exp, i))
      point_add(X, Y, x, y);
  }
  point_copy(x, y, X, Y);
}

                               /* draw a random value 'exp' with 1 <= exp < n */
void get_random_exponent(exp_t exp)
{
  char buf[4 * NUMWORDS];
  int fh, r, s;
  do {
    if ((fh = open(DEV_RANDOM, O_RDONLY)) < 0)
      FATAL(DEV_RANDOM);
    for(r = 0; r < 4 * NUMWORDS; r += s)
      if ((s = read(fh, buf + r, 4 * NUMWORDS - r)) <= 0)
	FATAL(DEV_RANDOM);
    if (close(fh) < 0)
      FATAL(DEV_RANDOM);
    bitstr_import(exp, buf);
    for(r = bitstr_sizeinbits(base_order) - 1; r < NUMWORDS * 32; r++)
      bitstr_clrbit(exp, r);
  } while(bitstr_is_clear(exp));
}

/******************************************************************************/

void XTEA_init_key(uint32_t *k, const char *key)
{
  k[0] = CHARS2INT(key + 0); k[1] = CHARS2INT(key + 4);
  k[2] = CHARS2INT(key + 8); k[3] = CHARS2INT(key + 12);
}

                                                     /* the XTEA block cipher */
void XTEA_encipher_block(char *data, const uint32_t *k)
{
  uint32_t sum = 0, delta = 0x9e3779b9, y, z;
  int i;
  y = CHARS2INT(data); z = CHARS2INT(data + 4);
  for(i = 0; i < 32; i++) {
    y += ((z << 4 ^ z >> 5) + z) ^ (sum + k[sum & 3]);
    sum += delta;
    z += ((y << 4 ^ y >> 5) + y) ^ (sum + k[sum >> 11 & 3]);
  }
  INT2CHARS(data, y); INT2CHARS(data + 4, z);
}
                                                       /* encrypt in CTR mode */
void XTEA_ctr_crypt(char *data, int size, const char *key) 
{
  uint32_t k[4], ctr = 0;
  int len, i;
  char buf[8];
  XTEA_init_key(k, key);
  while(size) {
    INT2CHARS(buf, 0); INT2CHARS(buf + 4, ctr++);
    XTEA_encipher_block(buf, k);
    len = MIN(8, size);
    for(i = 0; i < len; i++)
      *data++ ^= buf[i];
    size -= len;
  }
}

                                                     /* calculate the CBC MAC */
void XTEA_cbcmac(char *mac, const char *data, int size, const char *key)
{
  uint32_t k[4];
  int len, i;
  XTEA_init_key(k, key);
  INT2CHARS(mac, 0);
  INT2CHARS(mac + 4, size);
  XTEA_encipher_block(mac, k);
  while(size) {
    len = MIN(8, size);
    for(i = 0; i < len; i++)
      mac[i] ^= *data++;
    XTEA_encipher_block(mac, k);
    size -= len;
  }
}

                                     /* modified(!) Davies-Meyer construction.*/
void XTEA_davies_meyer(char *out, const char *in, int ilen)
{
  uint32_t k[4];
  char buf[8];
  int i;
  memset(out, 0, 8);
  while(ilen--) {
    XTEA_init_key(k, in);
    memcpy(buf, out, 8);
    XTEA_encipher_block(buf, k);
    for(i = 0; i < 8; i++)
      out[i] ^= buf[i];
    in += 16;
  }
}

/******************************************************************************/

void ECIES_generate_key_pair(void)      /* generate a public/private key pair */
{
  char buf[8 * NUMWORDS + 1], *bufptr = buf + NUMWORDS * 8 - (DEGREE + 3) / 4;
  elem_t x, y;
  exp_t k;
  get_random_exponent(k);
  point_copy(x, y, base_x, base_y);
  point_mult(x, y, k);
  
  //printf("Here is your new public/private key pair:\n");
  bitstr_to_hex(buf, x);
  //printf("Public key: %s:", bufptr); 
  FILE* f = fopen("pubx.key", "w");
  if( f == NULL ){
    printf("error opening pubx.key\n");
    while(1);
  }
  fprintf(f,"%s",bufptr);
  fclose(f);

  bitstr_to_hex(buf, y);
  //printf("%s\n", bufptr);
  f = fopen("puby.key", "w");
  if( f == NULL ){
    printf("error opening puby.key\n");
    while(1);
  }
  fprintf(f,"%s",bufptr);
  fclose(f);


  bitstr_to_hex(buf, k);
  //printf("Private key: %s\n", bufptr);
  f = fopen("priv.key", "w");
  if( f == NULL ){
    printf("error opening priv.key\n");
    while(1);
  }
  fprintf(f,"%s",bufptr);
  fclose(f);

}

       /* check that a given elem_t-pair is a valid point on the curve != 'o' */
int ECIES_embedded_public_key_validation(const elem_t Px, const elem_t Py)
{
  return (bitstr_sizeinbits(Px) > DEGREE) || (bitstr_sizeinbits(Py) > DEGREE) ||
    point_is_zero(Px, Py) || ! is_point_on_curve(Px, Py) ? -1 : 1;
}

      /* same thing, but check also that (Px,Py) generates a group of order n */
int ECIES_public_key_validation(const char *Px, const char *Py)
{
  elem_t x, y;
  if ((bitstr_parse(x, Px) < 0) || (bitstr_parse(y, Py) < 0))
    return -1;
  if (ECIES_embedded_public_key_validation(x, y) < 0)
    return -1;
  point_mult(x, y, base_order);
  return point_is_zero(x, y) ? 1 : -1;
}

void ECIES_kdf(char *k1, char *k2, const elem_t Zx,     /* a non-standard KDF */
	       const elem_t Rx, const elem_t Ry)
{
  int bufsize = (3 * (4 * NUMWORDS) + 1 + 15) & ~15;
  char buf[bufsize];
  memset(buf, 0, bufsize);
  bitstr_export(buf, Zx);
  bitstr_export(buf + 4 * NUMWORDS, Rx);
  bitstr_export(buf + 8 * NUMWORDS, Ry);
  buf[12 * NUMWORDS] = 0; XTEA_davies_meyer(k1, buf, bufsize / 16);
  buf[12 * NUMWORDS] = 1; XTEA_davies_meyer(k1 + 8, buf, bufsize / 16);
  buf[12 * NUMWORDS] = 2; XTEA_davies_meyer(k2, buf, bufsize / 16);
  buf[12 * NUMWORDS] = 3; XTEA_davies_meyer(k2 + 8, buf, bufsize / 16);
}

#define ECIES_OVERHEAD (8 * NUMWORDS + 8)

                  /* ECIES encryption; the resulting cipher text message will be
                                            (len + ECIES_OVERHEAD) bytes long */
void ECIES_encryption(char *msg, const char *text, int len, 
		      const char *Px, const char *Py)
{
  elem_t Rx, Ry, Zx, Zy;
  char k1[16], k2[16];
  exp_t k;
  do {
    get_random_exponent(k);
    bitstr_parse(Zx, Px);
    bitstr_parse(Zy, Py);
    point_mult(Zx, Zy, k);
    point_double(Zx, Zy);                           /* cofactor h = 2 on B163 */
  } while(point_is_zero(Zx, Zy));
  point_copy(Rx, Ry, base_x, base_y);
  point_mult(Rx, Ry, k);
  ECIES_kdf(k1, k2, Zx, Rx, Ry);

  bitstr_export(msg, Rx);
  bitstr_export(msg + 4 * NUMWORDS, Ry);
  memcpy(msg + 8 * NUMWORDS, text, len);
  XTEA_ctr_crypt(msg + 8 * NUMWORDS, len, k1);
  XTEA_cbcmac(msg + 8 * NUMWORDS + len, msg + 8 * NUMWORDS, len, k2);
}

                                                          /* ECIES decryption */
int ECIES_decryption(char *text, const char *msg, int len, 
		     const char *privkey)
{
  elem_t Rx, Ry, Zx, Zy;
  char k1[16], k2[16], mac[8];
  exp_t d;
  bitstr_import(Rx, msg);
  bitstr_import(Ry, msg + 4 * NUMWORDS);
  if (ECIES_embedded_public_key_validation(Rx, Ry) < 0)
    return -1;
  bitstr_parse(d, privkey);
  point_copy(Zx, Zy, Rx, Ry);
  point_mult(Zx, Zy, d);
  point_double(Zx, Zy);                             /* cofactor h = 2 on B163 */
  if (point_is_zero(Zx, Zy))
    return -1;
  ECIES_kdf(k1, k2, Zx, Rx, Ry);
  
  XTEA_cbcmac(mac, msg + 8 * NUMWORDS, len, k2);
  if (memcmp(mac, msg + 8 * NUMWORDS + len, 8))
    return -1;
  memcpy(text, msg + 8 * NUMWORDS, len);
  XTEA_ctr_crypt(text, len, k1);
  return 1;
}

/******************************************************************************/

void encryption_decryption_demo(const char *text, const char *public_x,
				const char *public_y, const char *private)
{
  int len = strlen(text) + 1;
  char *encrypted = malloc(len + ECIES_OVERHEAD);
  char *decrypted = malloc(len);

  printf("plain text: %s\n", text);
  ECIES_encryption(encrypted, text, len, public_x, public_y);   /* encryption */

  if (ECIES_decryption(decrypted, encrypted, len, private) < 0) /* decryption */
    printf("decryption failed!\n");
  else
    printf("after encryption/decryption: %s\n", decrypted);
  
  free(encrypted);
  free(decrypted);
}

int main()
{                                                /* the coefficients for B163 */
  bitstr_parse(poly, "800000000000000000000000000000000000000c9");
  bitstr_parse(coeff_b, "20a601907b8c953ca1481eb10512f78744a3205fd");
  bitstr_parse(base_x, "3f0eba16286a2d57ea0991168d4994637e8343e36");
  bitstr_parse(base_y, "0d51fbc6c71a0094fa2cdd545b11c5c0c797324f1");
  bitstr_parse(base_order, "40000000000000000000292fe77e70c12a4234c33");

  ECIES_generate_key_pair();          /* generate a public/private key pair */

  //encryption_decryption_demo("This secret demo message will be ECIES encrypted",
//			     "1c56d302cf642a8e1ba4b48cc4fbe2845ee32dce7", 
//			     "45f46eb303edf2e62f74bd68368d979e265ee3c03",
//			     "0e10e787036941e6c78daf8a0e8e1dbfac68e26d2");
  return 0;
}

/* f86c92039c992d2d2bd2b85c8807ac2f7af57c5c */
