#include <sysinit.h>
#include <string.h>
#include "basic/basic.h"
#include "funk/mesh.h"
#include "funk/nrf24l01p.h"
#include "basic/byteorder.h"
#include "basic/random.h"

char meshgen=0; // Generation
MPKT meshbuffer[MESHBUFSIZE];

uint32_t const meshkey[4] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
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

inline void blink(char a, char b){
    gpioSetValue (a,b, 1-gpioGetValue(a,b));
};

void mesh_cleanup(void){
    time_t now=getSeconds();
    for(int i=1;i<MESHBUFSIZE;i++){
        if(meshbuffer[i].flags&MF_USED){
            if (MO_GEN(meshbuffer[i].pkt)<meshgen)
                meshbuffer[i].flags=MF_FREE;
            if (MO_TIME(meshbuffer[i].pkt)<now)
                meshbuffer[i].flags=MF_FREE;
            if (MO_TIME(meshbuffer[i].pkt)-now>SECS_DAY)
                meshbuffer[i].flags=MF_FREE;
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
        };

        if(MO_TYPE(buf)=='T'){
            time_t toff=MO_TIME(buf)-((getTimer()+(300/SYSTICKSPEED))/(1000/SYSTICKSPEED));
            if (toff>_timet) // Do not live in the past.
                _timet = toff;
            continue;
        };

        // Safety: Truncate ascii packets by 0-ing the CRC
        if (MO_TYPE(buf) >='A' && MO_TYPE(buf) <='Z'){
            buf[MESHPKTSIZE-2]=0;
        };

        // Store packet in a free slot
        int free=-1;
        for(int i=0;i<MESHBUFSIZE;i++){
            if ( ((meshbuffer[i].flags&MF_USED)==0) && free<0 )
                free=i;
            if ( (meshbuffer[i].flags&MF_USED) &&
                    (MO_TYPE(meshbuffer[i].pkt) == MO_TYPE(buf))){
                if ( MO_TIME(buf) >
                        MO_TIME(meshbuffer[i].pkt)){
                    free=i;
                    break;
                }else{
                    free=-2;
                    break;
                };
            };
        };

        if(free==-1){ // Buffer full. Ah well. Kill a random packet
            free=1; // XXX: GetRandom()?
        };

        if(free<0)
            continue;

        memcpy(meshbuffer[free].pkt,buf,MESHPKTSIZE);
        meshbuffer[free].flags=MF_USED;

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

