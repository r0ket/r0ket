#ifndef __BASICIDLE_H_
#define __BASICIDLE_H_

#define MAXQENTRIES 8

#define QT_NORMAL 0
#define QT_PLUS   1
#define QS_START 0x0
#define QS_END   0x7f

typedef struct {
    union {
        void (*callback)(void);
        uint8_t (*callbackplus)(uint8_t);
    } u;
    unsigned type  :1;
    unsigned state :7;
} QENTRY;

typedef struct {
    int qstart;
    int qend;
    QENTRY queue[MAXQENTRIES];
} QUEUE;

extern QUEUE the_queue;
extern volatile uint32_t _timectr;

void work_queue(void);
uint8_t work_queue_minimal(void);
void delayms_queue(uint32_t);
uint8_t delayms_queue_plus(uint32_t, uint8_t);
void delayms_power(uint32_t);
int push_queue(void (*qnew)(void));
int push_queue_plus(uint8_t (*qnew)(uint8_t));

// Note: 
// Our time implementation will fail after 497 days of continous uptime.
// ( 2^32 / 1000 * SYSTICKSPEED ) seconds

#define incTimer(void) do{_timectr++;}while(0);
#define getTimer() (_timectr)

#endif
