#ifndef _ECC_H_
#define _ECC_H_H
#include <stdint.h>

/******************************************************************************/

#define DEGREE 163                      /* the degree of the field polynomial */
#define MARGIN 3                                          /* don't touch this */
#define NUMWORDS ((DEGREE + MARGIN + 31) / 32)

   /* the following type will represent bit vectors of length (DEGREE+MARGIN) */
typedef uint32_t bitstr_t[NUMWORDS];
typedef bitstr_t elem_t;           /* this type will represent field elements */
typedef bitstr_t exp_t;

int bitstr_parse_export(char *exp, const char *s);

void ECIES_setup(void);

void ECIES_encyptkeygen(uint8_t *px, uint8_t *py,
                uint8_t k1[16], uint8_t k2[16], uint8_t *Rx_exp, uint8_t *Ry_exp);

int ECIES_decryptkeygen(uint8_t *rx, uint8_t *ry,
             uint8_t k1[16], uint8_t k2[16], const char *privkey);
#define ECIES_OVERHEAD (8 * NUMWORDS + 8)

                  /* ECIES encryption; the resulting cipher text message will be
                                            (len + ECIES_OVERHEAD) bytes long */
void ECIES_encryption(char *msg, const char *text, int len, 
		      const char *Px, const char *Py);
                                                          /* ECIES decryption */
int ECIES_decryption(char *text, const char *msg, int len, 
		     const char *privkey);

#define MACRO(A) do { A; } while(0)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
     /* some basic bit-manipulation routines that act on these vectors follow */
#define bitstr_getbit(A, idx) ((A[(idx) / 32] >> ((idx) % 32)) & 1)
#define bitstr_setbit(A, idx) MACRO( A[(idx) / 32] |= 1 << ((idx) % 32) )
#define bitstr_clrbit(A, idx) MACRO( A[(idx) / 32] &= ~(1 << ((idx) % 32)) )

#define bitstr_clear(A) MACRO( memset(A, 0, sizeof(bitstr_t)) )
#define bitstr_copy(A, B) MACRO( memcpy(A, B, sizeof(bitstr_t)) )
#define bitstr_swap(A, B) MACRO( bitstr_t h; \
  bitstr_copy(h, A); bitstr_copy(A, B); bitstr_copy(B, h) )
#define bitstr_is_equal(A, B) (! memcmp(A, B, sizeof(bitstr_t)))

#define field_set1(A) MACRO( A[0] = 1; memset(A + 1, 0, sizeof(elem_t) - 4) )


#define point_is_zero(x, y) (bitstr_is_clear(x) && bitstr_is_clear(y))
#define point_set_zero(x, y) MACRO( bitstr_clear(x); bitstr_clear(y) )
#define point_copy(x1, y1, x2, y2) MACRO( bitstr_copy(x1, x2); \
                                          bitstr_copy(y1, y2) )


#endif

