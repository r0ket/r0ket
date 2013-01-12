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
#define ANNOUNCE_CHANNEL 87
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
            uint8_t nick[18];
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
           uint16_t gameId;
           uint8_t gameFlags;
           uint8_t interval;
           uint8_t jitter;
           uint8_t gameTitle[8];
        }__attribute__((packed)) announce;
        struct join{
           uint16_t gameId;
           uint8_t reserved[17];
        }__attribute__((packed)) join;
    }c;
    uint16_t crc;
}__attribute__((packed));

#define FLAGS_MASS_GAME         1
#define FLAGS_SHORT_PACKET      2
#define FLAGS_LONG_RECV         4

#define FLAGS_ACK_JOINOK    1
#define MASS_ID 1

#define FLAGS_CLS               1

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

void setLeft();
void setRight();
struct packet a;

void setJeopardy();

void ram(void)
{
    int priv = GLOBAL(privacy);

    GLOBAL(privacy) = 3;
    config.nrmacs=1;
    config.maclen[0] = 32;
    config.channel = ANNOUNCE_CHANNEL;
    memcpy(config.mac0, ANNOUNCE_MAC, 5);
    memcpy(config.txmac, ANNOUNCE_MAC, 5);
    nrf_config_set(&config);

    nrf_set_strength(3);
//    int rnd;

   volatile uint16_t i;
    while( 1 ){
        delayms(100);
        i = getRandom()&0xfff; while(i--);
        setJeopardy();
        nrf_snd_pkt_crc(sizeof(a),(uint8_t*)&a);
    }
    GLOBAL(privacy) = priv;
}


void setJeopardy()
{
    a.len = 32;
    a.protocol = 'G';
    a.command = 'A';
    a.id = 0;
    a.ctr = 1;
    a.c.announce.gameMac[0] = 'B';
    a.c.announce.gameMac[1] = 'P';
    a.c.announce.gameMac[2] = 'O';
    a.c.announce.gameMac[3] = 'N';
    a.c.announce.gameMac[4] = 'G';

    a.c.announce.gameChannel = 91;
    a.c.announce.gameId = 23;
    a.c.announce.gameFlags = 3;

    a.c.announce.interval = 10;
    a.c.announce.jitter = 10;

    a.c.announce.gameTitle[0] = 'J';
    a.c.announce.gameTitle[1] = 'e';
    a.c.announce.gameTitle[2] = 'o';
    a.c.announce.gameTitle[3] = 'p';
    a.c.announce.gameTitle[4] = 'r';
    a.c.announce.gameTitle[5] = 'd';
    a.c.announce.gameTitle[6] = 'y';
    a.c.announce.gameTitle[7] = 0;
}

