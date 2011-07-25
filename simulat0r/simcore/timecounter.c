#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>

volatile uint32_t _timectr; // to satisfy linker

volatile uint32_t simTimeCounter() {
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return (tv.tv_sec*1000000+tv.tv_usec)/100;
}
