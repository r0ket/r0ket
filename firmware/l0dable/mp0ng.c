
#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/display.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"

#include "usetable.h"

#define RESX 96
#define RESY 68

#define CHANNEL 82
#define ANNOUNCEMENT_MAC "MP0NG"

#define MAX_POINTS 10
#define BALL_SIZE 6
#define PADDLE_OFFSET 10
#define PADDLE_WIDTH 3
#define PADDLE_HEIGHT 20

#define OFFSET_L PADDLE_OFFSET
#define OFFSET_R RESX - PADDLE_WIDTH - PADDLE_OFFSET

// Modes the program can be in
#define MODE_SERVER 1
#define MODE_INIT 0
#define MODE_CLIENT 2

// Packet types
#define PKT_ANNOUNCE 'a'
#define PKT_JOIN 'j'
#define PKT_ACK 'A'
#define PKT_GAMESTATE 's'
#define PKT_MOVEMENT 'm'
#define PKT_EXIT 'e'

// Directions
#define DIR_STAY BTN_NONE
#define DIR_UP BTN_UP
#define DIR_RIGHT BTN_RIGHT
#define DIR_DOWN BTN_DOWN
#define DIR_LEFT BTN_LEFT

struct packet{
	uint16_t gameid;	// a (hopefully) unique number identifiyng the game
	uint8_t seq;	// sequential number of packet - to be increased for every packet sent
	uint8_t type;	// the type of the packet - defined above

	union content{
		struct announce{
			uint8_t gameid;		//again the game id - needet for saving open games as announcement structs
			uint8_t nick[16];	//nickname of the user offering the game
			uint8_t mac[5];		//the mac to use for transmitting the game packets
			uint8_t channel;	//the channel on which the game packets will be transferred
			uint8_t reserved[3];//array to pad the rest of the unuses bytes in the packet
		}__attribute__((packed)) announce;
		struct join{
			uint8_t nick[16];
			uint8_t reserved[10];
		}__attribute__((packed)) join;
		struct ack{
			uint8_t reserved[26];
		}__attribute__((packed)) ack;
		struct gamestate{
			uint8_t pad1;		//Position of the paddle for player 1
			uint8_t pad2;		//same for p2
			uint8_t ball_x;		//x positoin of the ball
			uint8_t ball_y;		//y position of the ball
			uint8_t score1;		//points for player 1
			uint8_t score2;		//same for p2
			uint8_t reserved[20];
		}__attribute__((packed)) state;
		struct movement{
			uint8_t dir;		//direction in which th player wants to move - defined above
			uint8_t reserved[25];
		}__attribute__((packed)) movement;
		struct exit{
			uint8_t reserved[26];
		}__attribute__((packed)) exit;
	}__attribute__((packed)) c;

	uint16_t crc;	//checksum of the packet, automatically generated and verfied by the library
}__attribute__((packed));

struct player_t{
	int8_t paddle_pos;
	int8_t score;
	bool second;
};

struct ball_t{
	int8_t x;
	int8_t y;
	int8_t speed_x;
	int8_t speed_y;
	//bool moving;
	int8_t player_has_ball;
};

struct ball_t ball1;
struct player_t player1;
struct player_t player2;

void print_score();
void move_player(struct player_t *player, uint8_t dir);
void draw_paddles(void);
void draw_ball();
void shoot_ball();
void just_draw_ball();
void check_border();
//void check_collision_player(struct player_t *player);
void check_collision_players();

void init();

uint16_t gameid;

uint8_t next_move;

uint8_t mode;

uint8_t started;

uint8_t receive;
uint8_t player_joined;

struct packet latest_packet;

void startServer(void);
void startClient(void);
void handle_packet(struct packet *p);
uint8_t check_end(void);

void main(void){

	started = 0;
	receive = 1;

	next_move = DIR_STAY;

	mode = MODE_INIT;

	player_joined = 0;

	lcdClear();

	lcdPrintln("Up: server");
	lcdPrintln("Down: client");
	lcdPrintln("Enter: exit");
	
	lcdRefresh();

    int8_t priv = GLOBAL(privacy);
    GLOBAL(privacy) = 3;

	uint8_t btn;

	uint8_t cycles_wait;
	cycles_wait = 0;
	struct packet announce;
	do{
		btn = getInputRaw();

		if(mode == MODE_INIT){
			if(btn == BTN_UP){
				startServer();
			} else {
				if(btn == BTN_DOWN){
					startClient();
				}
			}
		}

		if(!started){
			if(mode == MODE_SERVER && !player_joined){
				memset((void*)&announce, 0, sizeof(announce));
				announce.gameid = gameid;
				announce.type = PKT_ANNOUNCE;
				memcpy(announce.c.announce.nick, GLOBAL(nickname), 16);
				announce.seq = 0;
				nrf_snd_pkt_crc(sizeof(announce), (uint8_t *)&announce);

				struct packet p;
				if(nrf_rcv_pkt_time(1000, sizeof(p), (uint8_t *)&p) == sizeof(p)){
					handle_packet(&p);
				}

			} else if (mode == MODE_CLIENT){
				struct packet p;
				if(nrf_rcv_pkt_time(100, sizeof(p), (uint8_t *)&p) == sizeof(p)){
					handle_packet(&p);
				}
			}
		} else {
			if(receive){
				struct packet p;
				if(nrf_rcv_pkt_time(50, sizeof(p), (uint8_t *)&p) == sizeof(p)){
					cycles_wait = 0;
					handle_packet(&p);
				} else {
					cycles_wait++;
				}

				if(cycles_wait >= 3){
					nrf_snd_pkt_crc(sizeof(latest_packet), (uint8_t *)&latest_packet);
					cycles_wait = 0;
				}
			} else {
				delayms(50);
			}

			next_move = btn;
		}
		
	}while(btn != BTN_ENTER);

	
	struct packet p;
	p.type = PKT_EXIT;
	for(uint8_t i = 0; i < 3; i++){
		delayms(50);
		nrf_snd_pkt_crc(sizeof(p), (uint8_t *)&p);
	}

	GLOBAL(privacy) = priv;
}

void configNrf(){
    struct NRF_CFG config;
	config.nrmacs = 1;
	config.maclen[0] = 32;
	config.channel = CHANNEL;
    memcpy(config.mac0, ANNOUNCEMENT_MAC, 5);
    memcpy(config.txmac, ANNOUNCEMENT_MAC, 5);
	nrf_config_set(&config);
    nrf_set_strength(3);
}

void startServer(void){
	mode = MODE_SERVER;
	lcdClear();
	lcdPrintln("Server started");
	lcdRefresh();

	configNrf();
}

void startClient(void){
	mode = MODE_CLIENT;
	lcdClear();
	lcdPrintln("Client started");
	lcdRefresh();

	configNrf();

	receive = 1;
}

void handle_packet(struct packet *p){
	switch(p->type){
		case PKT_ANNOUNCE:
			if(mode == MODE_CLIENT){
				if(!started){
					gameid = p->gameid;

					struct packet join;
					memset((void*)&join, 0, sizeof(join));
					join.gameid = gameid;
					join.type = PKT_JOIN;
					memcpy(join.c.join.nick, GLOBAL(nickname), 16);
					join.seq = p->seq+1;

					nrf_snd_pkt_crc(sizeof(join), (uint8_t *)&join);

					player_joined = 1;
				}
			}
			break;
		case PKT_JOIN:
			if(mode == MODE_SERVER){
				if(!started){
					if(!player_joined){
						started = 1;
						lcdClear();
						lcdPrintln("Player joined");
						lcdPrintln((char *)&p->c.join.nick);
						lcdRefresh();

						struct packet ack;
						memset((void*)&ack, 0, sizeof(ack));
						ack.gameid = gameid;
						ack.type = PKT_ACK;
						//memcpy(ack.c.ack.nick, GLOBAL(nickname), 16);
						ack.seq = p->seq+1;
						nrf_snd_pkt_crc(sizeof(ack), (uint8_t *)&ack);

						player_joined = 1;

						delayms(1000);

						init();

						struct packet state;
						memset((void*)&state, 0, sizeof(state));
						state.gameid = gameid;
						state.type = PKT_GAMESTATE;
						state.c.state.score1 = player1.score;
						state.c.state.score2 = player2.score;
						state.c.state.pad1 = player1.paddle_pos;
						state.c.state.pad2 = player2.paddle_pos;

						state.c.state.ball_x = ball1.x;
						state.c.state.ball_y = ball1.y;
						state.seq = p->seq+2;
						nrf_snd_pkt_crc(sizeof(state), (uint8_t *)&state);
					}
				}
			}
			break;
		case PKT_ACK:
			if(mode == MODE_CLIENT){
				if(!started){
					started = 1;
					lcdClear();
					lcdPrintln("Joined!");
					//lcdPrintln((char *)&p->c.announce.nick);

					init();

					lcdRefresh();
				}
			}
			break;
		case PKT_GAMESTATE:
			if(mode == MODE_CLIENT){
				if(started){
					player1.score = p->c.state.score1;
					player2.score = p->c.state.score2;
					player1.paddle_pos = p->c.state.pad1;
					player2.paddle_pos = p->c.state.pad2;

					ball1.x = p->c.state.ball_x;
					ball1.y = p->c.state.ball_y;

					if(!check_end()){
						lcdClear();
						print_score();
						draw_paddles();
						just_draw_ball();
						lcdDisplay();

						struct packet move;
						memset((void*)&move, 0, sizeof(move));
						move.gameid = gameid;
						move.type = PKT_MOVEMENT;
						move.c.movement.dir = next_move;
						move.seq = p->seq+1;
						delayms(50);
						nrf_snd_pkt_crc(sizeof(move), (uint8_t *)&move);
						latest_packet = move;

						next_move = DIR_STAY;
						receive = 1;
					}
				}
			}
			break;
		case PKT_MOVEMENT:
			if(mode == MODE_SERVER){
				if(started){
					if(next_move != DIR_LEFT) // Only let valid keys through
						move_player(&player1, next_move);
					if(p->c.movement.dir != DIR_RIGHT) // Only let valid keys through
						move_player(&player2, p->c.movement.dir);

					check_collision_players();
					draw_ball(); 

					lcdClear();
					print_score();
					//check_collision_player(&player1);
					//check_collision_player(&player2);
					draw_paddles();
					just_draw_ball();
					lcdDisplay();

					struct packet state;
					memset((void*)&state, 0, sizeof(state));
					state.gameid = gameid;
					state.type = PKT_GAMESTATE;

					state.c.state.score1 = player1.score;
					state.c.state.score2 = player2.score;
					state.c.state.pad1 = player1.paddle_pos;
					state.c.state.pad2 = player2.paddle_pos;

					state.c.state.ball_x = ball1.x;
					state.c.state.ball_y = ball1.y;

					state.seq = p->seq+1;
					delayms(50);
					nrf_snd_pkt_crc(sizeof(state), (uint8_t *)&state);
					latest_packet = state;

					next_move = DIR_STAY;
					receive = 1;

					check_end();
				}
			}
			break;
		case PKT_EXIT:
			if(started){
				if(mode == MODE_CLIENT)
					player2.score = MAX_POINTS;
				else
					player1.score = MAX_POINTS;

				check_end();
			}
			break;
	}
				
}

void init()
{
  // init ball
  ball1.x = 15;
  ball1.y = 15;
  ball1.speed_x = 5;
  ball1.speed_y = 2;
  //ball1.moving = false;
  ball1.player_has_ball = 1;
  
  // init player1
  player1.paddle_pos = 10;
  player1.second = false;
  player1.score = 0; 
  
  // init player2
  player2.paddle_pos = 10;
  player2.second = true;
  player2.score = 0; 
}

void print_score(void) 
{
  lcdPrint("    ");
  lcdPrintInt(player1.score);
  lcdPrint("    ");
  lcdPrintInt(player2.score);
}

void move_player(struct player_t *player, uint8_t dir)
{
	if(dir == DIR_UP && player->paddle_pos > 0 ) 
	{
		player->paddle_pos-=3;
	}

	if(dir == DIR_DOWN && player->paddle_pos < (RESY - PADDLE_HEIGHT))
	{
		player->paddle_pos+=3;
	}

	if(dir == DIR_RIGHT && ball1.player_has_ball == 1){
		//ball1.moving = true;
		ball1.player_has_ball = 0;
	}

	if(dir == DIR_LEFT && ball1.player_has_ball == 2){
		//ball1.moving = true;
		ball1.player_has_ball = 0;
	}
}

void draw_paddles(void)
{
	for(int8_t i = 0; i < PADDLE_HEIGHT; i++)
	{
		for(int8_t  j = 0; j < PADDLE_WIDTH; j++)
		{
			lcdSetPixel(OFFSET_L - j, i+player1.paddle_pos, 1);
			lcdSetPixel(OFFSET_R - j, i+player2.paddle_pos, 1);
		}
	}
}

void draw_ball(void) 
{
	//if(ball1.moving == true) 
	if(ball1.player_has_ball == 0) 
	{
		ball1.x += ball1.speed_x;
		ball1.y += ball1.speed_y;
		check_border();
	} 
	else 
	{
		if(ball1.player_has_ball == 1)
		{
			ball1.y = player1.paddle_pos + PADDLE_HEIGHT/2 - BALL_SIZE/2;
			ball1.x = OFFSET_L + 3;
		} 
		else
		{
			ball1.y = player2.paddle_pos + PADDLE_HEIGHT/2 - BALL_SIZE/2; 
			ball1.x = OFFSET_R - 3 - BALL_SIZE; 
			if(ball1.speed_x > 0) { 
				ball1.speed_x = -ball1.speed_x; 
			}
		}
	}
}

void just_draw_ball(void){
  for(int8_t i = 0; i < BALL_SIZE; i++) 
  {
    for(int8_t j = 0; j < BALL_SIZE; j++) 
    {
      lcdSetPixel(ball1.x + j, ball1.y + i, 1);
    }
  }
}

void check_collision_players(void) {
	if (ball1.player_has_ball == 0) {
		//Player 2
		if(ball1.x >= (OFFSET_R - PADDLE_WIDTH - 5) ) {
			if(ball1.y + BALL_SIZE > player2.paddle_pos && ball1.y < (player2.paddle_pos + PADDLE_HEIGHT) ) {
				ball1.speed_x = - ball1.speed_x;
			} else {
				ball1.speed_x = - ball1.speed_x;
				player1.score++;
				//ball1.moving = false;
				ball1.player_has_ball = 2;
			}
		}

		//Player 1
		if(ball1.x < (OFFSET_L + PADDLE_WIDTH) ) {
			if(ball1.y + BALL_SIZE > player1.paddle_pos && ball1.y < (player1.paddle_pos + PADDLE_HEIGHT) ) {
				ball1.speed_x = - ball1.speed_x;
			} else {
				ball1.speed_x = - ball1.speed_x;
				player2.score++;
				//ball1.moving=false;
				ball1.player_has_ball = 1;
			}
		}
	}
}

uint8_t check_end(void){
	if(player1.score >= MAX_POINTS || player2.score >= MAX_POINTS){
		receive = 0;
		lcdClear();
		lcdNl();
		lcdPrintln("  GAME OVER");
		lcdNl();
		lcdPrint(" Player ");
		lcdPrintInt(2 - (player1.score > player2.score));
		lcdPrintln(" won");

		lcdRefresh();

		return 1;
	}

	return 0;
}

void check_border() {
	if(ball1.speed_y < 0) {
		if (ball1.y < 0) {
			ball1.speed_y = - ball1.speed_y;
		}
	} else if (ball1.speed_y > 0) {
		if(ball1.y > (RESY - BALL_SIZE) ) {
			ball1.speed_y = - ball1.speed_y;
		}
	}

}
