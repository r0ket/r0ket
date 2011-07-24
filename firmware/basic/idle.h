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
void idle_queue(uint32_t delayms);
int push_queue(void (*new)(void));
int magic(void *new);
#define incTimer(void) do{_timectr++;}while(0);


