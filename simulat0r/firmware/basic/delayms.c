#include <sysdefs.h>
#include "lpc134x.h"

/* Simulator improved version */

#define _POSIX_C_SOURCE 199309
#include <time.h>
 
void delayms(uint32_t ms)
{
  struct timespec t;
  t.tv_sec = ms / 1000;
  t.tv_nsec = (ms % 1000) * 1000000;

  nanosleep (&t, NULL);
}
