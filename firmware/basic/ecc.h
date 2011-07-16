#ifndef _ECC_H_
#define _ECC_H_H

void ECIES_encyptkeygen(const char *Px, const char *Py,
                uint8_t k1[16], uint8_t k2[16], uint8_t *Rx_exp, uint8_t *Ry_exp);

int ECIES_decryptkeygen(const char *Rx_imp, const char *Ry_imp,
             uint8_t k1[16], uint8_t k2[16], const char *privkey);
#define ECIES_OVERHEAD (8 * NUMWORDS + 8)

                  /* ECIES encryption; the resulting cipher text message will be
                                            (len + ECIES_OVERHEAD) bytes long */
void ECIES_encryption(char *msg, const char *text, int len, 
		      const char *Px, const char *Py);
{
                                                          /* ECIES decryption */
int ECIES_decryption(char *text, const char *msg, int len, 
		     const char *privkey);

#endif

