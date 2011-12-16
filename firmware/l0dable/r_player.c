#include <sysinit.h>

#include "basic/basic.h"
#include "basic/byteorder.h"
#include "lcd/lcd.h"
#include "lcd/print.h"
#include "funk/nrf24l01p.h"
#include <string.h>
#include "basic/random.h"
#include "basic/config.h"
#include "usetable.h"


//channel and mac used to transmit game announcements
#define ANNOUNCE_CHANNEL 81
#define ANNOUNCE_MAC     "REM0T"

struct NRF_CFG config;

struct packet{
    uint8_t len;
    uint8_t protocol;
    uint8_t command;
    uint32_t id;
    uint32_t ctr;
    
    //union with 19 bytes data
    union content{
        struct button{
            uint8_t button;
            uint8_t reserved[18];
        }__attribute__((packed)) button;
        struct text{
            uint8_t x;
            uint8_t y;
            uint8_t flags;
            uint8_t text[16];
        }__attribute__((packed)) text;
        struct nick{
            uint8_t flags;
            uint8_t text[18];
        }__attribute__((packed)) nick;
        struct nickrequest{
           uint8_t reserved[19];
        }__attribute__((packed)) nickrequest;
        struct ack{
           uint8_t flags;
           uint8_t reserved[18];
        }__attribute__((packed)) ack;
        struct announce{
           uint8_t gameMac[5];
           uint8_t gameChannel;
           //uint8_t playerMac[5]; playerMac = gameMac+1;
           uint32_t gameId;
           uint8_t gameFlags;
           uint8_t gameTitle[8];
        }__attribute__((packed)) announce;
        struct join{
           uint32_t gameId;
           uint8_t reserved[15];
        }__attribute__((packed)) join;
    }c;
    uint16_t crc;
}__attribute__((packed));

#define FLAGS_MASS_GAME 1
#define FLAGS_ACK_JOINOK    1
#define MASS_ID 1

/**************************************************************************/
/* l0dable for playing games which are announced by other r0kets with the l0dabel r_game */
/* Values of buf[3]:
 * B: packet sent by player, contain information which button is pressed
 * T: packet sent by game, contain text for display
 * N: packet sent by game, requesting nick
 * n: packet sent player, containing nick
 * A: packet sent by game, announcing game
 * J: packet sent by player, requesting to join game
 * a: ack, packet with $ctr was received
 */


uint32_t ctr;
uint32_t id;
uint32_t gameId;

void sendButton(uint8_t button);
void sendJoin(uint32_t game);
void processPacket(struct packet *p);
void processAnnounce(struct announce *a);

uint8_t selectGame();
void playGame();

struct announce games[7];
uint8_t gamecount;

void ram(void)
{
    int priv = GLOBAL(privacy);
    GLOBAL(privacy) = 3;
    config.nrmacs=1;
    config.maclen[0] = 32;
    config.channel = ANNOUNCE_CHANNEL;
    memcpy(config.mac0, ANNOUNCE_MAC, 5);
    nrf_config_set(&config);

    id = getRandom();
    ctr = 1;
 
    while( selectGame() ){
        playGame();
    }
    GLOBAL(privacy) = priv;
};

void playGame(void)
{
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
}

void showGames(uint8_t selected)
{
    int i;
    lcdClear();
    lcdPrintln("Games:");
    if( gamecount ){
        for(i=0;i<gamecount;i++){
            if( i==selected )
                lcdPrint("*");
            else
                lcdPrint(" ");
            char buf[9];
            memcpy(buf, games[i].gameTitle, 8);
            buf[8] = 0;
            lcdPrintln(buf);
        }
    }else{
        lcdPrint("*No Games");
    }
    lcdRefresh();
}

uint8_t joinGame()
{
    int i;
    struct packet p;
    p.len=sizeof(p); 
    p.protocol='G';
    p.command='J';
    p.id= id;
    p.ctr= ++ctr;
    p.c.join.gameId=gameId;
    lcdClear();
    lcdPrintln("Joining game");
    lcdRefresh();

    for(i=0; i<10; i++){
        nrf_snd_pkt_crc(sizeof(p),(uint8_t*)&p);

        int len = nrf_rcv_pkt_time(30,sizeof(p),(uint8_t*)&p);
        if( len==sizeof(p) ){
            if( (p.len==32) && (p.protocol=='G') && p.command=='a' ){   //check sanity, protocol
                if( p.id == id && p.ctr == ctr ){
                    if( p.c.ack.flags & FLAGS_ACK_JOINOK ){
                        lcdPrintln("Join OK");
                        lcdRefresh();
                        return 1;
                    }else{
                        lcdPrintln("Join rejected");
                        lcdRefresh();
                        getInputWait();
                        getInputWaitRelease();
                        return 0;
                    }
                }
            }
        }
        delayms(70);
    }
    lcdPrintln("timeout :(");
    lcdRefresh();
    getInputWait();
    getInputWaitRelease();

    return 0;
}

uint8_t selectGame()
{  
    int len, i, selected;
    struct packet p;
    int a = 0;
    config.channel = ANNOUNCE_CHANNEL;
    memcpy(config.mac0, ANNOUNCE_MAC, 5);
    nrf_config_set(&config);

    gamecount = 0;
    for(i=0;i<60;i++){
        len= nrf_rcv_pkt_time(30, sizeof(p), (uint8_t*)&p);
        if (len==sizeof(p)){
            if( a ) a = 0; else a = 1;
            gpioSetValue (RB_LED2, a);
            processPacket(&p);
        }
    }
    selected = 0;
    while(1){
        showGames(selected);
        int key=getInputWait();
        getInputWaitRelease();
        switch(key){
            case BTN_DOWN:
                if( selected < gamecount-1 ){
                    selected++;
                }
                break;
            case BTN_UP:
                if( selected > 0 ){
                    selected--;
                }
                break;
            case BTN_LEFT:
                return 0;
            case BTN_ENTER:
            case BTN_RIGHT:
                if( gamecount == 0 )
                    return 0;
                gameId = games[selected].gameId;
                memcpy(config.txmac, games[selected].gameMac, 5);
                memcpy(config.mac0, games[selected].gameMac, 5);
                config.mac0[4]++;
                config.channel = games[selected].gameChannel;
                nrf_config_set(&config);
                if( games[selected].gameFlags & FLAGS_MASS_GAME )
                    return 1;
                else
                    return joinGame();
        }
    }
}
   


void processPacket(struct packet *p)
{
   if ((p->len==32) && (p->protocol=='G') && (p->id == id || p->id == 0) ){   //check sanity, protocol, id
     if (p->command=='T'){
     //processText(&(p->c.text));
     } 
     else if (p->command=='N'){
     //processNickRequest(&(p->c.nickrequest));
     }
     else if (p->command=='A'){
        processAnnounce(&(p->c.announce));
     }
   }     
}

void processAnnounce(struct announce *a)
{
    if( gamecount < sizeof(games)/sizeof(games[0]) ){
        games[gamecount] = *a;
        gamecount++;
    }
}

//increment ctr and send button state, id and ctr
void sendButton(uint8_t button)
{
    struct packet p;
    p.len=sizeof(p); 
    p.protocol='G'; // Proto
    p.command='B';
    p.id= id;
    p.ctr= ++ctr;
    p.c.button.button=button;

    //lcdClear();
    //lcdPrint("Key:"); lcdPrintInt(buf[2]); lcdNl();

    nrf_snd_pkt_crc(sizeof(p),(uint8_t*)&p);
}

//send join request for game
void sendJoin(uint32_t game)
{
    struct packet p;
    p.len=sizeof(p);
    p.protocol='G';
    p.command='J';
    p.ctr= ++ctr;
    p.id=id;
    p.c.join.gameId=game;

    nrf_snd_pkt_crc(sizeof(p),(uint8_t*)&p);
}
    
