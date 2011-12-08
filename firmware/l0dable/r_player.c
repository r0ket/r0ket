#include <sysinit.h>

#include "basic/basic.h"
#include "basic/byteorder.h"

#include "lcd/lcd.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"

//includes useful for r_game:
//#include "usbcdc/usb.h"
//#include "usbcdc/usbcore.h"
//#include "usbcdc/usbhw.h"
//#include "usbcdc/cdcuser.h"
//#include "usbcdc/cdc_buf.h"
//#include "usbcdc/util.h"

#include <string.h>
#include "basic/random.h"

#include "usetable.h"

#define REMOTE_CHANNEL 81

//mac that the player receives
#define PLAYER_MAC     "\x1\x2\x3\x2\x1"

//mac that the game receives
#define GAME_MAC     "\x1\x2\x3\x2\x1"

//#if CFG_USBMSC
//#error "MSC is defined"
//#endif

//#if !CFG_USBCDC
//#error "CDC is not defined"
//#endif

struct NRF_CFG config = {        //for some reason this has to be global
        .channel= REMOTE_CHANNEL,
        .txmac= GAME_MAC,
        .nrmacs=1,
        .mac0=  PLAYER_MAC,
        .maclen ="\x10",
    };

struct packet{
    uint8_t len;
    uint8_t protocol;
    uint8_t command;
    
    //union with 11 bytes data
    union content{
        struct button{
            uint8_t button;
            uint32_t id;
            uint32_t cnt;
            uint8_t reserved[2];
        }button;

        struct text{
            uint8_t x;
            uint8_t y;
            uint8_t flags;
            uint8_t text[8];
        }text;
        struct nick{
            uint8_t flags;
            uint8_t text[10];
        }nick;
        struct nickrequest{
           uint8_t reserved[11];
        }nickrequest;
    }c;
    uint16_t crc;
};


/**************************************************************************/
/* l0dable for playing games which are announced by other r0kets with the l0dabel r_game */
/* Values of buf[3]:
 * B: packet sent by player, contain information which button is pressed
 * T: packet sent by game, contain text for display
 * N: packet sent by game, requesting nick
 * n: packet sent player, containing nick
 */

uint32_t ctr;
uint32_t id;

void sendButton(uint8_t button);
void sendJoin(uint8_t game);
void processPacket(struct packet *p);

void ram(void)
{
    nrf_config_set(&config);
    id = getRandom();
    ctr = 1;
    int len;
    struct packet p;

    while(1){
        uint8_t button = getInputRaw();
        sendButton(button);
        
        while(1){
            len = nrf_rcv_pkt_time(30,sizeof(p),(uint8_t*)&p);
            if(len==sizeof(p)){
            processPacket(&p);
            }else{
                break;
            }
        }
        delayms(20);
    };
};
//getInputRaw();

void processPacket(struct packet *p)
{
   if ((p->len==16) && (p->protocol=='G')){   //check sanity, protocol
     if (p->command=='T'){
     //processText(&(p->c.text));
     } 
     else if (p->command=='N'){
     //processNick(&(p->c.nickrequest));
     }
   }     
}


//increment ctr and send button state, id and ctr
void sendButton(uint8_t button)
{
    uint8_t buf[16];
    buf[0]=0x10; // Length: 16 bytes
    buf[1]='G'; // Proto
    buf[2]='B';
    buf[3]=button;

    ctr++;
    *(int*)(buf+4)=ctr;
    ctr+=4;
    *(int*)(buf+4)=id;

    //lcdClear();
    //lcdPrint("Key:"); lcdPrintInt(buf[2]); lcdNl();

    nrf_snd_pkt_crc(16,buf);
}

//send join request for game
void sendJoin(uint8_t game)
{
    uint8_t buf[16];
    buf[0]=0x10; // Length: 16 bytes
    buf[1]='G'; // Proto
    buf[2]='J';
    buf[3]=game;

    ctr++;
    *(int*)(buf+4)=ctr;
    ctr+=4;
    *(int*)(buf+4)=id;

    nrf_snd_pkt_crc(16,buf);
}
    

/*
void s_init(void){
    usbCDCInit();
    nrf_init();

    struct NRF_CFG config = {
        .channel= REMOTE_CHANNEL,
        .txmac= REMOTE_MAC,
        .nrmacs=1,
        .mac0=  REMOTE_MAC,
        .maclen ="\x10",
    };

    nrf_config_set(&config);
};
*/

/* void process(uint8_t * input){
    __attribute__ ((aligned (4))) uint8_t buf[32];
    puts("process: ");
    puts(input);
    puts("\r\n");
    if(input[0]=='M'){
        buf[0]=0x10; // Length: 16 bytes
        buf[1]='M'; // Proto
        buf[2]=0x01;
        buf[3]=0x01; // Unused

        uint32touint8p(0,buf+4);

        uint32touint8p(0x41424344,buf+8);

        buf[12]=0xff; // salt (0xffff always?)
        buf[13]=0xff;
        nrf_snd_pkt_crc_encr(16,buf,remotekey);
        nrf_rcv_pkt_start();
    };

};
*/

/*
#define INPUTLEN 99
void r_recv(void){
    __attribute__ ((aligned (4))) uint8_t buf[32];
    int len;

    uint8_t input[INPUTLEN+1];
    int inputptr=0;

    nrf_rcv_pkt_start();
    puts("D start");

    getInputWaitRelease();

    while(!getInputRaw()){
        delayms(100);

        // Input
        int l=INPUTLEN-inputptr;
        CDC_OutBufAvailChar (&l);

        if(l>0){
            CDC_RdOutBuf (input+inputptr, &l);
            input[inputptr+l+1]=0;
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
        len=nrf_rcv_pkt_poll_dec(sizeof(buf),buf,remotekey);

        // Receive
        if(len<=0){
            delayms(10);
            continue;
        };

        if(buf[1]=='C'){ // Cursor
            puts("C ");
            puts(IntToStrX( buf[2],2 ));
            puts(" ");
            puts(IntToStrX( uint8ptouint32(buf+4), 8 ));
            puts(" ");
            puts(IntToStrX( uint8ptouint32(buf+8), 8 ));
        }else{
            puts("U ");
//          puts("[");puts(IntToStrX(len,2));puts("] ");
            puts(IntToStrX( *(int*)(buf+  0),2 ));
            puts(" ");
            puts(IntToStrX( *(int*)(buf+  1),2 ));
            puts(" ");
            puts(IntToStrX( *(int*)(buf+  2),2 ));
            puts(" ");
            puts(IntToStrX( *(int*)(buf+  3),2 ));
            puts(" ");
            puts(IntToStrX( uint8ptouint32(buf+4),8 ));
            puts(".");
            puts(IntToStrX( uint8ptouint32(buf+8),8 ));
            puts(" ");
            puts(IntToStrX( uint8ptouint32(buf+10),4 ));
        };
        puts("\r\n");

    };

    nrf_rcv_pkt_end();
    puts("D exit");
}
};
*/
