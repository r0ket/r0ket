/* Include this header first
   Otherwise it´s static inline functions would become invalid "static static" declarations */
#include "lpc134x.h"
/* now get rid of inline functions in the source file - luckily they´re not already static */
#define inline static
#include "../../../firmware/funk/nrf24l01p.c"
