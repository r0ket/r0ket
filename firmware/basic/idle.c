#include <sysinit.h>

#include "basic/basic.h"
#include "lcd/print.h"

QUEUE the_queue;
#ifdef __arm__
volatile uint32_t _timectr=0;
#else
#error "foo"
extern uint32_t simTimeCounter();
#define _timectr (simTimeCounter())
#endif

/**************************************************************************/

void work_queue(void){
    void (*elem)(void);
	int start;

	if (the_queue.qstart == the_queue.qend){
#ifdef __arm__
		__asm volatile ("WFI");
#else
            delayms(SYSTICKSPEED);
#endif
		return;
	};

	start=the_queue.qstart;
	start=(start+1)%MAXQENTRIES;
	elem=the_queue.queue[start].callback;
	the_queue.qstart=start;

	elem();
};

void delayms_queue(uint32_t ms){
	int end=_timectr+ms/SYSTICKSPEED;
	do {
		if (the_queue.qstart == the_queue.qend){
#ifdef __arm__
			__asm volatile ("WFI");
#else
            delayms(SYSTICKSPEED);
#endif
		}else{
			work_queue();
		};
	} while (end >_timectr);
};

void delayms_power(uint32_t ms){
    ms/=SYSTICKSPEED;
    ms+=_timectr;
	do {
#ifdef __arm__
			__asm volatile ("WFI");
#else
            delayms(SYSTICKSPEED);
#endif
	} while (ms >_timectr);
};

int push_queue(void (*new)(void)){
	int end;

	end=the_queue.qend;
	end=(end+1)%MAXQENTRIES;

	if(end == the_queue.qstart) // Queue full
		return -1;

	the_queue.queue[end].callback=new;
	the_queue.qend=end;

	return 0;
};
