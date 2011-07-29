#define MAXQENTRIES 8

typedef struct {
    void (*callback)(void);
} QENTRY;

typedef struct {
    int qstart;
    int qend;
    QENTRY queue[MAXQENTRIES];
} QUEUE;

extern QUEUE the_queue;
extern volatile uint32_t _timectr;

void work_queue(void);
void delayms_queue(uint32_t delayms);
void delayms_power(uint32_t delayms);
int push_queue(void (*qnew)(void));
int magic(void *qnew);

// Note: 
// Our time implementation will fail after 497 days of continous uptime.
// ( 2^32 / 1000 * SYSTICKSPEED ) seconds

#define incTimer(void) do{_timectr++;}while(0);
#define getTimer() (_timectr)

