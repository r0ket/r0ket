#include <sysinit.h>

#include "basic/basic.h"
#include "basic/byteorder.h"

#include "lcd/lcd.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"
#include "usbcdc/usb.h"
#include "usbcdc/usbcore.h"
#include "usbcdc/usbhw.h"
#include "usbcdc/cdcuser.h"
#include "usbcdc/cdc_buf.h"
#include "usbcdc/util.h"

#include <string.h>

#define REMOTE_CHANNEL 91
#define REMOTE_MAC     "REM0T"
#define MESH_CHANNEL 85
#define MESH_MAC     "MESHB"
#define BEACON_CHANNEL 81
#define BEACON_MAC     "\x1\x2\x3\x2\1"

#include "SECRETS.release"

char funkencrypt=0;

#if CFG_USBMSC
#error "MSC is defined"
#endif

#if !CFG_USBCDC
#error "CDC is not defined"
#endif

/**************************************************************************/

uint32_t thekey[4] = { 0x0, 0x0, 0x0, 0x0 };

struct NRF_CFG config = {
    .channel= BEACON_CHANNEL,
    .txmac= BEACON_MAC,
    .nrmacs=1,
    .mac0=  BEACON_MAC,
    .maclen ="\x10",
};

char type=0;
char filter=0;
uint8_t filterdata[10];
char outc[2]= {0,0};

static int process(char * input);

#define INPUTLEN 99
//# MENU serial
void dwim(void){
    char input[INPUTLEN+1];
    int inputptr=0;
    char dirty=0;

    usbCDCInit();
    delayms(500);
    getInputWaitRelease();
    nrf_init();
    gpioSetValue (RB_LED1, 0); 
    puts("D start\r\n");

    while(!getInputRaw()){

        // Input
        int l=INPUTLEN-inputptr;
        CDC_OutBufAvailChar (&l);

        if(l>0){
            dirty=10;
            if(l>1)
                dirty=50;
            CDC_RdOutBuf (input+inputptr, &l);
            if(input[inputptr]==8){
                inputptr=0;
                puts("<oops>\r\n");
                continue;
            };
            input[inputptr+l]=0;
            puts_plus(&input[inputptr]);
            for(int i=0;i<l;i++){
                if(input[inputptr+i] =='\r' || input[inputptr+i] =='\n'){
                    input[inputptr+i]=0;
                    process(input);
                    if(i<l)
                        memmove(input,input+inputptr+i+1,l-i);
                    inputptr=-i-1;
                    break;
                };
            };
        }else{
            delayms(1);
            if(dirty>0)
                dirty--;
            if(dirty==1)
                puts("");
        };
        inputptr+=l;
    };
    gpioSetValue (RB_LED1, 0); 
    puts("D exit\r\n");
}

#define BUFLEN 32
#define NYB(x) ((x>'9')?(x|0x20)-'a'+10:x-'0')
static uint8_t * hextobyte(char * input, int *len){
    static uint8_t buf[BUFLEN];

    int p=0;
    int bp=0;
    char c;
    while(bp<BUFLEN){
        if(input[p]==0 || input[p+1]==0)
            break;
        if(input[p]==' '){
            p++;
            continue;
        };
        buf[bp]= c=NYB(input[p])*16+NYB(input[p+1]);
        bp++;p+=2;
    };
    *len=bp;
    return buf;
};

static uint32_t gethexval(char * input){
    int len;
    uint8_t *hex=hextobyte(input,&len);
    uint32_t v=hex[0];
    while(--len>0)
        v=v*256+(*++hex);
    return v;
};

static int process(char * input){
    if(input == NULL || input[0]==0)
        return -1;

    puts_plus("\r\n");
//    puts_plus("D ["); puts_plus(input); puts("]\r\n");

    if(input[0]=='i'){
        nrf_init();
        nrf_config_set(&config);
    }else if(input[0]=='p'){
        type=input[1];
        if(input[1]=='m'){
            config.channel=MESH_CHANNEL;
            memcpy(config.txmac,MESH_MAC,5);
            memcpy(config.mac0,MESH_MAC,5);
            config.maclen[0]=0x20;
            config.nrmacs=1;
            nrf_config_set(&config);
//            memcpy(thekey,meshkey,sizeof(thekey));
            funkencrypt=0;
        }else if(input[1]=='M'){
            config.channel=83;
            memcpy(config.txmac,MESH_MAC,5);
            memcpy(config.mac0,MESH_MAC,5);
            config.maclen[0]=0x20;
            config.nrmacs=1;
            nrf_config_set(&config);
//            static const uint32_t const pubmesh[4] = {
//                0x00000042, 0x000005ec, 0x00000023, 0x00000005
//            };
//            memcpy(thekey,pubmesh,sizeof(thekey));
            funkencrypt=0;
        }else if(input[1]=='r'){
            config.channel=REMOTE_CHANNEL;
            memcpy(config.txmac,REMOTE_MAC,5);
            memcpy(config.mac0,REMOTE_MAC,5);
            config.maclen[0]=0x10;
            config.nrmacs=1;
            nrf_config_set(&config);
//            memcpy(thekey,remotekey,sizeof(thekey));
            funkencrypt=0;
        }else if(input[1]=='b'){
            config.channel=BEACON_CHANNEL;
            memcpy(config.txmac,BEACON_MAC,5);
            memcpy(config.mac0,BEACON_MAC,5);
            config.maclen[0]=0x10;
            config.nrmacs=1;
            nrf_config_set(&config);
//            memcpy(thekey,openbeaconkey,sizeof(thekey));
            funkencrypt=0;
        }else if(input[1]=='B'){
            config.channel=BEACON_CHANNEL;
            memcpy(config.txmac,BEACON_MAC,5);
            memcpy(config.mac0,BEACON_MAC,5);
            config.maclen[0]=0x10;
            config.nrmacs=1;
            nrf_config_set(&config);
//            static const uint32_t pubbeaconkey[4] = {
//                        0xB4595344, 0xD3E119B6, 0xA814D0EC, 0xEFF5A24E 
//            };
//            memcpy(thekey,pubbeaconkey,sizeof(thekey));
            funkencrypt=0;
        };
    }else if(input[0]=='t'){
        type=input[1];
    }else if(input[0]=='c'){
        int len;
        uint8_t *hex=hextobyte(&input[2],&len);
        if(input[1]=='k'){
            thekey[0]=uint8ptouint32(hex);
            thekey[1]=uint8ptouint32(hex+4);
            thekey[2]=uint8ptouint32(hex+8);
            thekey[3]=uint8ptouint32(hex+12);
        }else if(input[1]=='m'){
            memcpy(config.mac0,hex,5);
            nrf_config_set(&config);
        }else if(input[1]=='t'){
            memcpy(config.txmac,hex,5);
            nrf_config_set(&config);
        }else if(input[1]=='c'){
            config.channel=hex[0];
            nrf_config_set(&config);
        }else if(input[1]=='l'){
            config.maclen[0]=hex[0];
            nrf_config_set(&config);
        }else if(input[1]=='e'){
            funkencrypt= hex[0];
        }else if(input[1]=='?'){
            nrf_config_get(&config);
            puts_plus("Ch: ");puts_plus(IntToStrX( config.channel,2 )); puts_plus("\r\n");

            puts_plus("TxMac: ");
            for(int i=0;i<5;i++)
                puts_plus(IntToStrX( config.txmac[i],2 ));
            puts_plus("\r\n");

            puts_plus("Len  : ");
            for(int i=0;i<5;i++)
                puts_plus(IntToStrX( config.maclen[i],2 ));
            puts_plus("\r\n");

            puts_plus("0mac : ");
            for(int i=0;i<5;i++)
                puts_plus(IntToStrX( config.mac0[i],2 ));
            puts_plus("\r\n");

            puts_plus("1mac : ");
            for(int i=0;i<5;i++)
                puts_plus(IntToStrX( config.mac1[i],2 ));
            puts_plus("\r\n");

            puts_plus("2345 : ");
            for(int i=0;i<4;i++)
                puts_plus(IntToStrX( config.mac2345[i],2 ));
            puts_plus("\r\n");

            puts_plus("key : ");
            for(int i=0;i<4;i++){
                puts_plus(IntToStrX( thekey[i],8 ));
                puts_plus(" ");
            };
            puts_plus("\r\n");

            puts_plus("encrypt : ");
                puts_plus(IntToStrX( funkencrypt,2 ));
            puts_plus("\r\n");
        };
    }else if (input[0]=='s'){
        __attribute__ ((aligned (4))) uint8_t buf[32];
        int status=0;
        int len;
        int idx=1;
        char ctr=1;
        char debug=0;
        if(input[idx]=='d'){
            debug=1;
            idx++;
        };
        if(input[idx]=='+'){
            ctr=10;
            idx++;
        };

        if (input[idx]==' '){
            uint8_t *hex=hextobyte(&input[idx],&len);
            if(len<10) len=10; // minmal packet length?
            len+=2; // Add crc!

            memcpy(buf,hex,len);
            status=nrf_snd_pkt_crc_encr(len,buf,funkencrypt?thekey:NULL);

            if(debug){
                puts_plus("P ");
                puts_plus("[");puts_plus(IntToStrX(len,2));puts_plus("] ");
                for(int i=0;i<len;i++){
                    puts_plus(IntToStrX( buf[i],2 ));
                    puts_plus(" ");
                };
            };
            puts_plus("S ");
            puts_plus("[");puts_plus(IntToStrX(len,2));puts_plus("] ");
            puts_plus("state=");
            puts_plus(IntToStrX( status,2 ));
            puts("\r\n");

            while(--ctr>0){
                delayms(23);
                memcpy(buf,hex,len);
                status=nrf_snd_pkt_crc_encr(len,buf,funkencrypt?thekey:NULL);
            };
        }else{
            ;
        };
        puts_plus("\r\n");
    }else if (input[0]=='f'){
        int len;
        filter=input[1];
        uint8_t *hex=hextobyte(input+2,&len);
        if(len>9)
            len=9;
        memcpy(filterdata,hex,len);
        puts_plus("F ");
        if(filter){
        puts_plus("[");
        outc[0]=filter;
        puts_plus(outc);
        puts_plus("]");
        for(int i=0;i<len;i++){
            puts_plus(IntToStrX(filterdata[i],2 ));
        };
        }else{
            puts_plus("off");
        };
        puts_plus("\r\n");
    }else if (input[0]=='r'){
        __attribute__ ((aligned (4))) uint8_t buf[32];
        int len;
        int pctr=5;
        int t=5;

        if(input[1]=='+'){
            if(input[2]!=0){
                t=gethexval(&input[2]);
            };
            pctr=-1;
        }
        if(input[1]=='-'){
            pctr=gethexval(&input[2]);
            t=-1;
        }

        puts_plus("D rcv: ");
        puts_plus(IntToStr(pctr,8,0));
        puts_plus("pkts, ");
        puts_plus(IntToStr(t,8,0));
        puts_plus("secs ");
        if(filter){
            puts_plus("FILTER=");
            outc[0]=filter;
            puts_plus(outc);
        };
        puts_plus(" ...\r\n");
        if(t>0)
            t=getTimer()+t*1000/SYSTICKSPEED;
        nrf_rcv_pkt_start(R_CONFIG_EN_CRC);
        do{
            len=nrf_rcv_pkt_poll_dec(sizeof(buf),buf,funkencrypt?thekey:NULL);
            // Receive
            if(len==0||len==-1||len==-2){ //No pkt, Pkt2large, NoPktError
                delayms(10);
                continue;
            };
            if (!filter){
                if(len==-3){
                    puts_plus("[!crc] ");
                    len=32;
                };
            };
            if(type=='m' || type=='M'){
                if(filter && filter!=buf[0])
                    continue;
                puts_plus("R ");
                puts_plus(IntToStrX( buf[0],2 ));
                puts_plus(" ");
                puts_plus(IntToStrX( buf[1],2 ));
                puts_plus(" ");
                puts_plus(IntToStrX( uint8ptouint32(buf+2),8 ));
                puts_plus(" ");
                puts_plus(IntToStrX( uint8ptouint32(buf+6),8 ));
                puts_plus(" ");
                puts_plus(IntToStrX( uint8ptouint32(buf+10),8 ));
                puts_plus(" ");
                puts_plus(IntToStrX( uint8ptouint32(buf+14),8 ));
                puts_plus(" ");
                puts_plus(IntToStrX( uint8ptouint32(buf+18),8 ));
                puts_plus(" ");
                puts_plus(IntToStrX( uint8ptouint32(buf+22),8 ));
                puts_plus(" ");
                puts_plus(IntToStrX( uint8ptouint32(buf+26),8 ));
                puts_plus(" ");
                puts_plus(IntToStrX( buf[30],2 ));
                puts_plus(IntToStrX( buf[31],2 ));
                puts_plus(" ");
            }else if(type=='B'){
                if(filter)
                    if(uint8ptouint32(buf+8)!=uint8ptouint32(filterdata))
                        continue;
                puts_plus("RF ");
                puts_plus(IntToStrX( buf[2],2 ));
                puts_plus(" ");
                puts_plus(IntToStrX( uint8ptouint32(buf+8),8 ));
            }else if(type=='b'){
                if(filter=='.' || filter=='u')
                    if(uint8ptouint32(buf+8)!=uint8ptouint32(filterdata))
                        continue;
                if(filter=='0' && buf[3] != 0x00)
                    continue;
                if(filter=='5' && buf[3] != 0x55)
                    continue;
                if(filter=='a' && buf[3] != 0xaa)
                    continue;
                if(filter=='A' && buf[3] != 0xaa)
                    continue;
                if(filter=='f' && buf[3] != 0xff)
                    continue;
                if(filter=='F' && buf[3] != 0xff)
                    continue;
                puts_plus("R ");
                puts_plus(IntToStrX( buf[0],2 ));
                puts_plus(" ");
                puts_plus(IntToStrX( buf[1],2 ));
                puts_plus(" ");
                puts_plus(IntToStrX( buf[2],2 ));
                puts_plus(" ");
                puts_plus(IntToStrX( buf[3],2 ));
                puts_plus(" ");
                puts_plus(IntToStrX( uint8ptouint32(buf+4),8 ));
                puts_plus(" ");
                puts_plus(IntToStrX( uint8ptouint32(buf+8),8 ));
                puts_plus(" ");
                puts_plus(IntToStrX( buf[12],2 ));
                puts_plus(IntToStrX( buf[13],2 ));
                puts_plus(" ");
                puts_plus(IntToStrX( buf[14],2 ));
                puts_plus(IntToStrX( buf[15],2 ));
            }else{
                puts_plus("R ");
                puts_plus("[");puts_plus(IntToStrX(len,2));puts_plus("] ");
                for(int i=0;i<len;i++){
                    puts_plus(IntToStrX( buf[i],2 ));
                    puts_plus(" ");
                };
            };
            if(1){
                int l=0;
                CDC_OutBufAvailChar (&l);
                if(l>0){
                    puts_plus("\r\nD Abort.\r\n");
                    break;
                };
            };
            puts("\r\n");
            if(--pctr==0)
                break;
        }while((t>0)?(t>getTimer()):1);

        nrf_rcv_pkt_end();
    }else{
        puts_plus("D no action\r\n");
    };
    puts("D done.\r\n");
    return 0;
};
