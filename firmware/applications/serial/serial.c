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

    usbCDCInit();
    delayms(500);
    getInputWaitRelease();
    puts("D start\r\n");

    while(!getInputRaw()){
        delayms(10);

        // Input
        int l=INPUTLEN-inputptr;
        CDC_OutBufAvailChar (&l);

        if(l>0){
            CDC_RdOutBuf (input+inputptr, &l);
            input[inputptr+l]=0;
            puts(&input[inputptr]);
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
        };
        inputptr+=l;
    };

    puts("D exit\r\n");
}

int process(char * input){
    if(input == NULL || input[0]==0)
        return;

    puts("\r\n");
    puts("D [");
    puts(input);
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
            puts("Ch: ");puts(IntToStrX( config.channel,2 )); puts("\r\n");

            puts("TxMac: ");
            for(int i=0;i<5;i++)
                puts(IntToStrX( config.txmac[i],2 ));
            puts("\r\n");

            puts("Len  : ");
            for(int i=0;i<5;i++)
                puts(IntToStrX( config.maclen[i],2 ));
            puts("\r\n");

            puts("0mac : ");
            for(int i=0;i<5;i++)
                puts(IntToStrX( config.mac0[i],2 ));
            puts("\r\n");

            puts("1mac : ");
            for(int i=0;i<5;i++)
                puts(IntToStrX( config.mac1[i],2 ));
            puts("\r\n");

            puts("2345 : ");
            for(int i=0;i<4;i++)
                puts(IntToStrX( config.mac2345[i],2 ));
            puts("\r\n");

            puts("key : ");
            for(int i=0;i<4;i++){
                puts(IntToStrX( thekey[i],8 ));
                puts(" ");
            };
            puts("\r\n");
        }else if(input[1]=='k'){
            thekey[0]=0;
            thekey[1]=0;
            thekey[2]=0;
            thekey[3]=0;
        };
    }else if (input[0]=='s'){
        __attribute__ ((aligned (4))) uint8_t buf[32];
        int status=0;
        if (input[1]==' '){
            int p=2;
            int bp=0;
            char c;
//            puts("\r\n");
            while(bp<32){
                if(input[p]==0 || input[p+1]==0)
                    break;
                if(input[p]==' '){
                    p++;
                    continue;
                };
                c=((input[p]>'9')?input[p]-'a'+10:input[p]-'0')*16+
                    ((input[p+1]>'9')?input[p+1]-'a'+10:input[p+1]-'0');
//                puts(IntToStrX( c,2 ));
//                puts(".");
                buf[bp]=c;
                bp++;p+=2;
            };
//            puts("\r\n");

            if(bp<10) bp=10;

            bp+=2;
            puts("S Len:");
            puts(IntToStrX( bp,2 ));
            puts("\r\n");

            status=nrf_snd_pkt_crc_encr(bp,buf,thekey);

            puts("P ");
            puts("[");puts(IntToStrX(bp,2));puts("] ");
            for(int i=0;i<bp;i++){
                puts(IntToStrX( buf[i],2 ));
                puts(" ");
            };
            puts("\r\n");
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
        puts("S state=");
        puts(IntToStrX( status,2 ));
        puts("\r\n");
    }else if (input[0]=='r'){
        __attribute__ ((aligned (4))) uint8_t buf[32];
        int len;
        int pctr=5;
        int t=getTimer()+5000/SYSTICKSPEED;

        if(input[1]=='0')
            pctr=-1;

        puts("D receive ...\r\n");
        nrf_rcv_pkt_start();
        do{
            len=nrf_rcv_pkt_poll_dec(sizeof(buf),buf,thekey);
            // Receive
            if(len==0||len==-1||len==-2){ //No pkt, Pkt2large, NoPktError
                delayms(10);
                continue;
            };
            puts("R ");
            puts("[");puts(IntToStrX(len,2));puts("] ");
            for(int i=0;i<len;i++){
                puts(IntToStrX( buf[i],2 ));
                puts(" ");
            };
            if(len==-3)
                puts(" [!crc]");
            puts("\r\n");
            delayms(1);
            if(pctr--==0)
                break;
        }while(t>getTimer());

        nrf_rcv_pkt_end();
    }else{
        puts("D no action\r\n");
    };
    puts("D done.\r\n");
    return 0;
};
