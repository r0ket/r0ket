#include <sysinit.h>

#include "basic/basic.h"
#include "lcd/print.h"

QUEUE the_queue;
volatile uint32_t _timectr=0;

/**************************************************************************/

void work_queue(void){
    void (*elem)(void);
	int start;

	if (the_queue.qstart == the_queue.qend){
#ifdef ARM
		__asm volatile ("WFI");
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
	int end=_timectr+ms/10;
	do {
		if (the_queue.qstart == the_queue.qend){
#ifdef ARM
			__asm volatile ("WFI");
#endif
		}else{
			work_queue();
		};
	} while (end >_timectr);
};

void delayms_power(uint32_t ms){
	do {
		ms-=10;
#ifdef ARM
		__asm volatile ("WFI");
#endif
	} while(ms>10);
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
