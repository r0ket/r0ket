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

uint32_t const beaconkey[4] = {
        0xB4595344,0xD3E119B6,0xA814D0EC,0xEFF5A24E
};
uint32_t remotekey[4] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
};
uint32_t meshkey[4] = {
    0x0, 0x0, 0x0, 0x0
};


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

int process(char * input);

#define INPUTLEN 99
void dwim(void){
    char input[INPUTLEN+1];
    int inputptr=0;
    char dirty=0;

    usbCDCInit();
    delayms(500);
    getInputWaitRelease();
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
                if(input[inputptr+i] =='\r'){
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

    puts("D exit\r\n");
}

#define BUFLEN 32
#define NYB(x) ((x>'9')?(x|0x20)-'a'+10:x-'0')
uint8_t * hextobyte(char * input, int *len){
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

int process(char * input){
    if(input == NULL || input[0]==0)
        return -1;

    puts_plus("\r\n");
    puts_plus("D [");
    puts_plus(input);
    puts("]\r\n");

    if(input[0]=='i'){
        nrf_init();
        nrf_config_set(&config);
    }else if(input[0]=='c'){
        if(input[1]=='m'){
            config.channel=MESH_CHANNEL;
            memcpy(config.txmac,MESH_MAC,5);
            memcpy(config.mac0,MESH_MAC,5);
            config.maclen[0]=0x20;
            config.nrmacs=1;
            nrf_config_set(&config);
            memcpy(thekey,meshkey,sizeof(thekey));
        }else if(input[1]=='r'){
            config.channel=REMOTE_CHANNEL;
            memcpy(config.txmac,REMOTE_MAC,5);
            memcpy(config.mac0,REMOTE_MAC,5);
            config.maclen[0]=0x10;
            config.nrmacs=1;
            nrf_config_set(&config);
            memcpy(thekey,remotekey,sizeof(thekey));
        }else if(input[1]=='b'){
            config.channel=BEACON_CHANNEL;
            memcpy(config.txmac,BEACON_MAC,5);
            memcpy(config.mac0,BEACON_MAC,5);
            config.maclen[0]=0x10;
            config.nrmacs=1;
            nrf_config_set(&config);
            memcpy(thekey,beaconkey,sizeof(thekey));
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
        };
    }else if(input[0]=='C'){
        int len;
        uint8_t *hex=hextobyte(&input[2],&len);
        if(input[1]=='k'){
            thekey[0]=uint8ptouint32(hex);
            thekey[1]=uint8ptouint32(hex+4);
            thekey[2]=uint8ptouint32(hex+8);
            thekey[3]=uint8ptouint32(hex+12);
        }else if(input[1]=='m'){
            config.mac0[0]=uint8ptouint32(hex);
            config.mac0[1]=uint8ptouint32(hex+4);
            config.mac0[2]=uint8ptouint32(hex+8);
            config.mac0[3]=uint8ptouint32(hex+12);
            config.mac0[4]=uint8ptouint32(hex+16);
            nrf_config_set(&config);
        }else if(input[1]=='t'){
            config.txmac[0]=uint8ptouint32(hex);
            config.txmac[1]=uint8ptouint32(hex+4);
            config.txmac[2]=uint8ptouint32(hex+8);
            config.txmac[3]=uint8ptouint32(hex+12);
            config.txmac[4]=uint8ptouint32(hex+16);
            nrf_config_set(&config);
        }else if(input[1]=='c'){
            config.channel=*hex;
            nrf_config_set(&config);
        };
    }else if (input[0]=='s'){
        __attribute__ ((aligned (4))) uint8_t buf[32];
        int status=0;
        int len;
        if (input[1]==' '){
            uint8_t *hex=hextobyte(&input[2],&len);
            if(len<10) len=10;

            len+=2;
            puts_plus("S Len:");
            puts_plus(IntToStrX( len,2 ));
            puts_plus("\r\n");

            status=nrf_snd_pkt_crc_encr(len,hex,thekey);

            puts_plus("P ");
            puts_plus("[");puts_plus(IntToStrX(len,2));puts_plus("] ");
            for(int i=0;i<len;i++){
                puts_plus(IntToStrX( hex[i],2 ));
                puts_plus(" ");
            };
            puts_plus("\r\n");
        }else if (input[1]=='t'){
            static int ctr=1;
            int status;

            buf[0]=0x10; // Length: 16 bytes
            buf[1]='1'; // Proto
            buf[2]=0x00;
            buf[3]=0x00; // Unused

            uint32touint8p(ctr++,buf+4);

            uint32touint8p(0x5ec,buf+8);

            buf[12]=0xff; // salt (0xffff always?)
            buf[13]=0xff;
            status=nrf_snd_pkt_crc_encr(16,buf,thekey);
        }else{
        };
        puts_plus("S state=");
        puts_plus(IntToStrX( status,2 ));
        puts_plus("\r\n");
    }else if (input[0]=='r'){
        __attribute__ ((aligned (4))) uint8_t buf[32];
        int len;
        int pctr=5;
        int t=getTimer()+5000/SYSTICKSPEED;

        if(input[1]=='+'){
            if(input[2]!=0){
                uint8_t *hex=hextobyte(&input[2],&len);
                t=getTimer()+hex[0]*1000/SYSTICKSPEED;
            };
            pctr=-1;
        }
        if(input[1]=='-'){
            uint8_t *hex=hextobyte(&input[2],&len);
            pctr=hex[0];
        }

        puts_plus("D receive ...\r\n");
        nrf_rcv_pkt_start();
        do{
            len=nrf_rcv_pkt_poll_dec(sizeof(buf),buf,thekey);
            // Receive
            if(len==0||len==-1||len==-2){ //No pkt, Pkt2large, NoPktError
                delayms(10);
                continue;
            };
            puts_plus("R ");
            puts_plus("[");puts_plus(IntToStrX(len,2));puts_plus("] ");
            if(len==-3){
                puts_plus("[!crc] ");
                len=16;
            };
            for(int i=0;i<len;i++){
                puts_plus(IntToStrX( buf[i],2 ));
                puts_plus(" ");
            };
            if(pctr<0){
                int l=0;
                CDC_OutBufAvailChar (&l);
                if(l>0){
                    puts_plus("\r\nD Abort.\r\n");
                    break;
                };
            };
            puts("\r\n");
            if(pctr--==0)
                break;
        }while(t>getTimer());

        nrf_rcv_pkt_end();
    }else{
        puts_plus("D no action\r\n");
    };
    puts("D done.\r\n");
    return 0;
};
