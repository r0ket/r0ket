#ifndef __SIMPLETIME_H_
#define __SIMPLETIME_H_

#include <time.h>

#define YEAR0           1900                    /* the first year */
#define EPOCH_YR        1970            /* EPOCH = Jan 1 1970 00:00:00 */
#define SECS_DAY        (24L * 60L * 60L)
#define LEAPYEAR(year)  (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year)  (LEAPYEAR(year) ? 366 : 365)

extern time_t _timet;
struct tm * mygmtime(register const time_t time);
time_t getSeconds(void);

#endif
