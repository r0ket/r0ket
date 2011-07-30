#ifndef __MESH_H_
#define __MESH_H_

#define MESHBUFSIZE 10
#define MESHPKTSIZE 32

#define M_SENDINT 500
#define M_RECVINT 1000
#define M_RECVTIM 100

#define MESH_CHANNEL 85
#define MESH_MAC     "MESHB"

#define MO_TYPE(x)       (x[0])
#define MO_TYPE_set(x,y) (x[0]=y)
#define MO_GEN(x)        (x[1])
#define MO_GEN_set(x,y)  (x[1]=y)
#define MO_TIME(x)       (uint8ptouint32(x+2))
#define MO_TIME_set(x,y) (uint32touint8p(y,x+2))

typedef struct {
    uint8_t pkt[32];
    char flags;
} MPKT;

#define MF_FREE (0)
#define MF_USED (1<<0)

extern char meshgen; // Generation
extern MPKT meshbuffer[MESHBUFSIZE];

void initMesh(void);
void mesh_cleanup(void);
void mesh_recvloop(void);
void mesh_sendloop(void);
void mesh_systick(void);

#endif
