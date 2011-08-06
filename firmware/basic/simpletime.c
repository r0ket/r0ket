#include <sysinit.h>
#include "simpletime.h"
#include "basic/basic.h"

time_t _timet=0;

static int days_per_month (int year, int month)
{
  if (month == 1)
    {
      if (LEAPYEAR(year))
        return 29;
      return 28;
    }
  else if (month > 6)
    month--;
  if ((month % 2))
    return 30;
  else
    return 31;
}

struct tm * mygmtime(register const time_t time) {
        static struct tm br_time;
        register struct tm *timep = &br_time;
        register unsigned long dayclock, dayno;
        int year = EPOCH_YR;

        dayclock = (unsigned long)time % SECS_DAY;
        dayno = (unsigned long)time / SECS_DAY;

        timep->tm_sec = dayclock % 60;
        timep->tm_min = (dayclock % 3600) / 60;
        timep->tm_hour = dayclock / 3600;
        timep->tm_wday = (dayno + 4) % 7;  /* day 0 was a thursday */
        while (dayno >= YEARSIZE(year)) {
                dayno -= YEARSIZE(year);
                year++;
        }
        timep->tm_year = year - YEAR0;
        timep->tm_yday = dayno;
        timep->tm_mon = 0;
        while (dayno >= days_per_month (year, timep->tm_mon)) {
                dayno -= days_per_month (year, timep->tm_mon);
                timep->tm_mon++;
        }
        timep->tm_mday = dayno + 1;
        timep->tm_isdst = 0;

        return timep;
}

// Timezones suck. Currently we only do it all in localtime.
// I know it's broken. Sorry
time_t getSeconds(void){
    return _timet+(getTimer()*SYSTICKSPEED/1000);
};
