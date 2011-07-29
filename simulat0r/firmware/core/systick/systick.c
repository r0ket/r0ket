#define systickInit _hideaway_systickInit
#include "../../firmware/core/systick/systick.c"
#undef systickInit

void systickInit (uint32_t delayMs)
{
  fprintf(stderr,"systickConfig %d: unimplemented\n",delayMs);
  //  systickConfig ((CFG_CPU_CCLK / 1000) * delayMs);
}
