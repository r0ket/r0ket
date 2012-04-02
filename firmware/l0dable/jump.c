/**
 * r0ket JUMP!
 *
 * Author: webholics
 */

#include <sysinit.h>
#include <string.h>
#include "basic/basic.h"
#include "basic/config.h"
#include "basic/random.h"
#include "lcd/render.h"
#include "lcd/display.h"
#include "funk/mesh.h"
#include "usetable.h"

#define PLAYER_SPRITE_WIDTH 9
#define PLAYER_SPRITE_HEIGHT 10
static const bool PLAYER_SPRITE_DOWN[] = {
	0,0,0,1,1,1,0,0,0,
	1,0,0,1,1,1,0,0,1,
	1,1,0,0,1,0,0,1,1,
	0,1,1,1,1,1,1,1,0,
	0,0,1,1,1,1,1,0,0,
	0,0,0,1,1,1,0,0,0,
	0,0,0,1,1,1,0,0,0,
	0,0,1,1,0,1,1,0,0,
	0,0,1,1,0,1,1,0,0,
	0,0,1,1,0,1,1,0,0
};
static const bool PLAYER_SPRITE_UP[] = {
	0,0,0,1,1,1,0,0,0,
	0,0,0,1,1,1,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,0,
	1,1,0,1,1,1,0,1,1,
	0,0,0,1,1,1,0,0,0,
	0,0,1,1,0,1,1,0,0,
	0,0,1,1,0,1,1,0,0,
	0,0,1,1,0,1,1,0,0
};

#define GRAVITY 1
#define JUMP_FORCE -9
#define MAX_VEL_Y 5
#define MAX_VEL_X 2

#define NUM_PLATFORMS 30
#define PLATFORM_MARGIN_Y 14
#define PLATFORM_HEIGHT 3
#define PLATFORM_WIDTH 20
#define SPEEDUP_EVERY_FPS 1000
#define REDUCE_PLATFORM_EVERY_FPS 500

#define POS_PLAYER_Y RESY-PLATFORM_HEIGHT 
#define POS_PLAYER_X (RESX+PLAYER_SPRITE_WIDTH)/2

struct gamestate {
	bool running;

	char player_x;
	int player_y;
	int player_y_vel;
	int player_x_vel;
	bool player_ground;
	char scroll_speed;
	uint32_t scroll_pos;
	
	char platform_width;
	int platform_index;
	int platforms_y[NUM_PLATFORMS];
	char platforms_x1[NUM_PLATFORMS];
	char platforms_x2[NUM_PLATFORMS];
} game; 

static void splash_scene();
static void init_game();
static void draw_player();
static void move_player(long frame_count);
static void update_platforms(long frame_count);
static void draw_platforms();
static void draw_hud();
static bool gameover_scene();
static void blink_led();
static bool highscore_set(uint32_t score, char nick[]);
static uint32_t highscore_get(char nick[]);

void ram(void) {

	splash_scene();

	long frame_count = 0;
	init_game();

	while(1) {
		frame_count++;

		lcdFill(0);
		update_platforms(frame_count);
		move_player(frame_count);
		draw_platforms();
		draw_player();
		draw_hud();
		blink_led();
		lcdDisplay();

		if(!game.running) {
			if(!gameover_scene()){
                delayms_queue_plus(10,1);
				return;
            }
			init_game();
		}
        delayms_queue_plus(24,0);
	}
}

static void splash_scene() {
	uint32_t highscore;
	char highnick[20];

	char key = 0;
	while(key == 0) {
		getInputWaitRelease();

		int dy = getFontHeight();
		int s1x = DoString(0, 0, "r0ket");
		s1x = (RESX-s1x)/2;
		int s2x = DoString(0, 0, "JUMP!");
		s2x = (RESX-s2x)/2;

		lcdFill(0);
		
		DoString(s1x, 3*dy, "r0ket");
		DoString(s2x, 4*dy, "JUMP!");
		DoString(0, 7*dy, "by webholics");

		highscore = highscore_get(highnick);

		int s3x = DoInt(0, 0, highscore);
		DoChar(s3x, 0, 'm');
		DoString (0, dy, highnick);

		lcdDisplay();

		key = getInputWaitTimeout(1000);
	}
}

static void init_game() {
	game.running = true;

	game.player_x = POS_PLAYER_X;
	game.player_y = POS_PLAYER_Y;
	game.player_y_vel = 0;
	game.player_x_vel = 0;
	game.player_ground = true;
	game.scroll_speed = 8;
	game.platform_width = 20;
	game.scroll_pos = 0;

	game.platform_index = 0;
	game.platforms_y[0] = RESY - PLATFORM_HEIGHT;
	game.platforms_x1[0] = 0;
	game.platforms_x2[0] = RESX;
	for(int i = 1; i < NUM_PLATFORMS; i++) {
		game.platforms_y[i] = RESY+1;
	}
}

static void update_platforms(long frame_count) {
	// create new platforms
	while(1) {
		int y = game.platforms_y[game.platform_index];
		if(y <= 0) {
			break;
		}
		y -= PLATFORM_MARGIN_Y;
		int x = getRandom() % (RESX-game.platform_width);
		game.platform_index = (game.platform_index+1)%NUM_PLATFORMS;
		game.platforms_y[game.platform_index] = y;
		game.platforms_x1[game.platform_index] = x;
		game.platforms_x2[game.platform_index] = x + game.platform_width-1;
	}

	if(game.scroll_speed > 1 && frame_count % SPEEDUP_EVERY_FPS == 0) {
		game.scroll_speed--;
	}
	if(game.platform_width > 5 && frame_count % REDUCE_PLATFORM_EVERY_FPS == 0) {
		game.platform_width--;
	}

	// move platforms
	int scroll_speed = game.scroll_speed;
	if(game.player_y-PLAYER_SPRITE_HEIGHT < 0) {
		scroll_speed += (game.player_y-PLAYER_SPRITE_HEIGHT) / 5 - 1;
		if(scroll_speed <= 0)
			scroll_speed = 1;
	}
	if(frame_count % scroll_speed == 0) {
		game.scroll_pos++;
		for(int i = 0; i < NUM_PLATFORMS; i++) {
			game.platforms_y[i]++;
		}
	}
}

static void draw_platforms() {
	for(int i = 0; i < NUM_PLATFORMS; i++) {
		if(game.platforms_y[i] <= RESY) {
			for(int x = game.platforms_x1[i]; x <= game.platforms_x2[i]; x++) {
				for(int y = game.platforms_y[i]; y < game.platforms_y[i]+PLATFORM_HEIGHT; y++) {
					if(y >= 0 && y <= RESY) {
						lcdSetPixel(x, y, 1);
					}
				}
			}
		}
	}
}

static void draw_player() {
	const bool* sprite;
	if(game.player_y_vel > 0) {
		sprite = PLAYER_SPRITE_DOWN;
	}
	else {
		sprite = PLAYER_SPRITE_UP;
	}
	for(int x = 0; x < PLAYER_SPRITE_WIDTH; x++) {
		for(int y = 0; y < PLAYER_SPRITE_HEIGHT; y++) {
			int py = game.player_y + y - PLAYER_SPRITE_HEIGHT;
			if(sprite[x + y*PLAYER_SPRITE_WIDTH] && py >= 0 && py < RESY) {
				lcdSetPixel(
					(game.player_x + x)%RESX, 
					py,
					1);
			}
		}
	}

	if(game.player_y < 0) {
		int player_x_center = game.player_x + PLAYER_SPRITE_WIDTH/2;
		for(int y = 0; y < 2; y++) {
			for(int x = player_x_center-2; x <= player_x_center+2; x++) {
				if(x >= 0 && x < RESX && y >= 0 && y < RESY) {
					lcdSetPixel(x, y, 1);
				}
			}	
		}
	}
}

static void move_player(long frame_count) {
	// move x
	if(gpioGetValue(RB_BTN0) == 0) {
		game.player_x_vel--;
	}	    
	else if(gpioGetValue(RB_BTN1) == 0) {
		game.player_x_vel++;
	}
	else {
		game.player_x_vel = 0;
	}
	if(game.player_x_vel > MAX_VEL_X) {
		game.player_x_vel = MAX_VEL_X;
	}
	else if(game.player_x_vel < -1*MAX_VEL_X) {
		game.player_x_vel = -1*MAX_VEL_X;
	}
	game.player_x += game.player_x_vel + RESX;
	game.player_x %= RESX;

	// move y (jump)
	// half the speed
	if(frame_count%2 == 0) {

		int old_y = game.player_y;

		if(game.player_ground) {
			game.player_y_vel = JUMP_FORCE;
			game.player_ground = false;
		}

		game.player_y_vel += GRAVITY;
		game.player_y_vel = game.player_y_vel > MAX_VEL_Y ? MAX_VEL_Y : game.player_y_vel;
		int new_y = old_y + game.player_y_vel;

		// collision detection
		int player_x_center = game.player_x + PLAYER_SPRITE_WIDTH/2;
		for(int i = 0; i < NUM_PLATFORMS; i++) {
			if(game.player_y_vel > 0
					&& old_y < game.platforms_y[i] 
					&& new_y >= game.platforms_y[i]
					&& game.platforms_x1[i] <= player_x_center+2
					&& game.platforms_x2[i] >= player_x_center-2) {

				game.player_y = game.platforms_y[i]-1;
				game.player_y_vel = 0;
				game.player_ground = true;

				break;
			}
		}

		game.player_y = new_y;

		if(game.player_y > RESY + PLAYER_SPRITE_HEIGHT) {
			game.running = false;
		}
	}
}

static void draw_hud() {
	int x = DoInt(0, 0, game.scroll_pos / 15);
	DoChar(x, 0, 'm');
}

static void blink_led() {
	// this is r0ket booooost!
	if(game.player_y < 0) {
		gpioSetValue(RB_LED0, 1);
		gpioSetValue(RB_LED1, 1);
		gpioSetValue(RB_LED2, 1);
		gpioSetValue(RB_LED3, 1);
		return;
	}

	gpioSetValue(RB_LED0, game.player_ground);
	gpioSetValue(RB_LED1, game.player_ground);
	gpioSetValue(RB_LED2, game.player_ground);
	gpioSetValue(RB_LED3, game.player_ground);
}

static bool gameover_scene() {
	int dy = getFontHeight();
	int s1x = DoString(0, 0, "GAME OVER!");
	s1x = (RESX-s1x)/2;
	int s2x = DoString(0, 0, "HIGHTSCORE!");
	s2x = (RESX-s2x)/2;

	char key = 0;
	while(key != BTN_UP && key != BTN_DOWN) {
		lcdClear();

		if(highscore_set(game.scroll_pos / 15, GLOBAL(nickname)))
			DoString (s2x, dy, "HIGHSCORE!");
		else
			DoString(s1x, dy, "GAME OVER!");

		int x = DoInt(0, 3*dy, game.scroll_pos / 15);
		DoChar(x, 3*dy, 'm');

		DoString(0, 5*dy, "UP to play");
		DoString(0, 6*dy, "DOWN to quit");

		lcdDisplay();

		key = getInputWaitTimeout(5000);
	}

	return !(key==BTN_DOWN);
}

// thank you space invaders ;)

static bool highscore_set(uint32_t score, char nick[]) {
    MPKT * mpkt= meshGetMessage('j');
    if(MO_TIME(mpkt->pkt)>score)
        return false;

    MO_TIME_set(mpkt->pkt,score);
    strcpy((char*)MO_BODY(mpkt->pkt),nick);
    if(GLOBAL(privacy)==0){
        uint32touint8p(GetUUID32(),mpkt->pkt+26);
        mpkt->pkt[25]=0;
    };
	return true;
}

static uint32_t highscore_get(char nick[]){
    MPKT * mpkt= meshGetMessage('j');

    strcpy(nick,(char*)MO_BODY(mpkt->pkt));

	return MO_TIME(mpkt->pkt);
}
