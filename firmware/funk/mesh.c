#include <sysdefs.h>
#include <sysinit.h>
#include <string.h>
#include "basic/basic.h"
#include "funk/mesh.h"
#include "funk/nrf24l01p.h"
#include "basic/byteorder.h"
#include "basic/random.h"

char meshgen=0; // Generation
char meshincctr=0;
MPKT meshbuffer[MESHBUFSIZE];

uint32_t const meshkey[4] = {
    0x00000042, 0x000005ec, 0x00000023, 0x00000005
};

struct NRF_CFG oldconfig;

void initMesh(void){
    for(int i=0;i<MESHBUFSIZE;i++){
        meshbuffer[i].flags=MF_FREE;
    };
    memset(meshbuffer[0].pkt,0,MESHPKTSIZE);
    meshbuffer[0].pkt[0]='T';
    MO_TIME_set(meshbuffer[0].pkt,getSeconds());
    meshbuffer[0].flags=MF_USED;
};

MPKT * meshGetMessage(uint8_t type){
    int free=-1;
    for(int i=0;i<MESHBUFSIZE;i++){
        if ( ((meshbuffer[i].flags&MF_USED)==0) && free<0 )
            free=i;
        if ( (meshbuffer[i].flags&MF_USED) &&
                (MO_TYPE(meshbuffer[i].pkt) == type)){
            free=i;
            break;
        };
    };
    if(free==-1){ // Buffer full. Ah well. Kill a random packet
        free=1; // XXX: GetRandom()?
        meshbuffer[free].flags=MF_FREE;
    };
    if(meshbuffer[free].flags==MF_FREE){
        memset(&meshbuffer[free],0,sizeof(MPKT));
        MO_TYPE_set(meshbuffer[free].pkt,type);
        MO_GEN_set(meshbuffer[free].pkt,meshgen);
        meshbuffer[free].flags=MF_USED;
    };
    return &meshbuffer[free];
};

void mesh_cleanup(void){
    time_t now=getSeconds();
    for(int i=1;i<MESHBUFSIZE;i++){
        if(meshbuffer[i].flags&MF_USED){
            if (MO_GEN(meshbuffer[i].pkt)<meshgen)
                meshbuffer[i].flags=MF_FREE;
            if (MO_TYPE(meshbuffer[i].pkt)>='a' &&
                    MO_TYPE(meshbuffer[i].pkt)<='z'){
                ;
            }else{
                if (MO_TIME(meshbuffer[i].pkt)<now)
                    meshbuffer[i].flags=MF_FREE;
                if (MO_TIME(meshbuffer[i].pkt)-now>SECS_DAY)
                    meshbuffer[i].flags=MF_FREE;
            };
        };
    };
};

void mesh_recvloop(void){
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int len;
    int recvend=M_RECVTIM/SYSTICKSPEED+getTimer();
    int pktctr=0;

    nrf_config_get(&oldconfig);

    nrf_set_channel(MESH_CHANNEL);
    nrf_set_rx_mac(0,MESHPKTSIZE,strlen(MESH_MAC),(uint8_t*)MESH_MAC);

    mesh_cleanup();

    nrf_rcv_pkt_start();
    do{
        len=nrf_rcv_pkt_poll_dec(sizeof(buf),buf,meshkey);

        // Receive
        if(len<=0){
            delayms_power(10);
            continue;
        };
        pktctr++;

        if(MO_GEN(buf)>meshgen){
            if(meshgen)
                meshgen++;
            else
                meshgen=MO_GEN(buf);
            _timet=0;
            meshincctr=0;
        };

        if(MO_TYPE(buf)=='T'){
            time_t toff=MO_TIME(buf)-((getTimer()+(600/SYSTICKSPEED))/(1000/SYSTICKSPEED));
            if (toff>_timet){ // Do not live in the past.
                _timet = toff;
                meshincctr++;
            };
            continue;
        };

        // Safety: Truncate ascii packets by 0-ing the CRC
        buf[MESHPKTSIZE-2]=0;

        // Store packet in a same/free slot
        MPKT* mpkt=meshGetMessage(MO_TYPE(buf));

        // Skip locked packet
        if(mpkt->flags&MF_LOCK)
            continue;

        // only accept newer/better packets
        if(mpkt->flags==MF_USED)
            if(MO_TIME(buf)<MO_TIME(mpkt->pkt))
                continue;

        memcpy(mpkt->pkt,buf,MESHPKTSIZE);
        mpkt->flags=MF_USED;

    }while(getTimer()<recvend || pktctr>MESHBUFSIZE);

    nrf_rcv_pkt_end();
    nrf_config_set(&oldconfig);
}

void mesh_sendloop(void){
    int ctr=0;
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int status;

    nrf_config_get(&oldconfig);
    nrf_set_channel(MESH_CHANNEL);
    nrf_set_tx_mac(strlen(MESH_MAC),(uint8_t*)MESH_MAC);

    // Update [T]ime packet
    MO_TIME_set(meshbuffer[0].pkt,getSeconds());
    MO_GEN_set(meshbuffer[0].pkt,meshgen);

    for (int i=0;i<MESHBUFSIZE;i++){
        if(!meshbuffer[i].flags&MF_USED)
            continue;
        if(meshbuffer[i].flags&MF_LOCK)
            continue;
        ctr++;
        memcpy(buf,meshbuffer[i].pkt,MESHPKTSIZE);
        status=nrf_snd_pkt_crc_encr(MESHPKTSIZE,buf,meshkey);
        //Check status? But what would we do...
    };

    nrf_config_set(&oldconfig);
};

void mesh_systick(void){
    static int rcvctr=0;
    static int sendctr=0;

    if(rcvctr--<0){
        push_queue(&mesh_recvloop);
        rcvctr=M_RECVINT/SYSTICKSPEED/2;
        rcvctr+=getRandom()%(rcvctr*2);
    };

    if(sendctr--<0){
        push_queue(&mesh_sendloop);
        sendctr=M_SENDINT/SYSTICKSPEED/2;
        sendctr+=getRandom()%(sendctr*2);
    };
};
