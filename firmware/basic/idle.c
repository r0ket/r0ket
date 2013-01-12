#include <sysinit.h>

#include "basic/basic.h"
#include "lcd/print.h"

QUEUE the_queue;
#ifdef __arm__
volatile uint32_t _timectr=0;
#else
extern uint32_t simTimeCounter();
#define _timectr (simTimeCounter())
#endif

/**************************************************************************/


uint8_t work_queue_minimal(void){
	int start;

	if (the_queue.qstart == the_queue.qend){
		return 0;
	};

	start=the_queue.qstart;
	start=(start+1)%MAXQENTRIES;
    if(the_queue.queue[start].type == QT_NORMAL){
        void (*elem)(void);
        elem=the_queue.queue[start].u.callback;
        the_queue.qstart=start;
        elem();
        return 0;
    }else if(the_queue.queue[start].type == QT_PLUS){
        uint8_t (*elem)(uint8_t);
        uint8_t state=the_queue.queue[start].state;
        elem=the_queue.queue[start].u.callbackplus;
        state=elem(state);
        if(state==QS_END){
            the_queue.qstart=start;
            return 0;
        }else{
            the_queue.queue[start].state=state;
            return 1;
        };
    };
    /* NOTREACHED */
    return 0;
}

void work_queue(void){

	if (the_queue.qstart == the_queue.qend){
        WFI;
        return;
	};

    while(work_queue_minimal());
}


uint8_t delayms_queue_plus(uint32_t ms, uint8_t final){
    int ret=0;
    int end=_timectr+ms/SYSTICKSPEED;
    do {
        if (the_queue.qstart == the_queue.qend){
            WFI;
        }else{
            ret=work_queue_minimal();
        };
    } while (end >_timectr);
    if(ret && final){
        while(work_queue_minimal());
    };
    return ret;
}

void delayms_queue(uint32_t ms){
	int end=_timectr+ms/SYSTICKSPEED;
	do {
		if (the_queue.qstart == the_queue.qend){
            WFI;
		}else{
			work_queue();
		};
	} while (end >_timectr);
}

void delayms_power(uint32_t ms){
    ms/=SYSTICKSPEED;
    ms+=_timectr;
	do {
        WFI;
	} while (ms >_timectr);
}

int push_queue(void (*new)(void)){
	int end;

	end=the_queue.qend;
	end=(end+1)%MAXQENTRIES;

	if(end == the_queue.qstart) // Queue full
		return -1;

	the_queue.queue[end].u.callback=new;
	the_queue.queue[end].type=QT_NORMAL;
	the_queue.qend=end;

	return 0;
}

int push_queue_plus(uint8_t (*new)(uint8_t)){
	int end;

	end=the_queue.qend;
	end=(end+1)%MAXQENTRIES;

	if(end == the_queue.qstart) // Queue full
		return -1;

	the_queue.queue[end].u.callbackplus=new;
	the_queue.queue[end].type=QT_PLUS;
	the_queue.queue[end].state=QS_START;
	the_queue.qend=end;

	return 0;
}
