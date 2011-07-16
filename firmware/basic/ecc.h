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

#endif

