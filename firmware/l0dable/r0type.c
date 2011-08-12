/**
 * r0type
 *
 * Author: @ranzwertig
 * 
 */

#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"
#include "lcd/render.h"
#include "lcd/display.h"
#include "funk/mesh.h"
#include "usetable.h"

#define SHIP_HEIGHT  9
#define SHIP_WIDTH 12
#define SHIP_ARRAY_SIZE 108

#define LIFE_WIDTH 11
#define LIFE_HEIGHT 9
#define LIFE_ARRAY_SIZE 99

#define ASTEROID_1_HEIGHT 6
#define ASTEROID_1_WIDTH 6
#define ASTEROID_1_ARRAY_SIZE 36

#define ASTEROID_3_HEIGHT 8
#define ASTEROID_3_WIDTH 8
#define ASTEROID_3_ARRAY_SIZE 64

#define ASTEROID_2_HEIGHT 7
#define ASTEROID_2_WIDTH 7
#define ASTEROID_2_ARRAY_SIZE 49

#define MAX_SHOTS  20
#define ENDED_SHOT 255
#define MAX_ASTEROIDS 10

#define SHOT_DELAY 12 
#define ASTEROID_FREQ 25
#define SCROLL_SPEED 20
#define ASTEROID_MIN_SPEED 6
#define ASTEROID_MAX_SPEED 5
#define SHIP_SAVE_TICKS 240
#define INCREASE_GAME_SPEED 50

static const uint8_t SHIP[] = {
    0,1,1,0,0,0,0,0,0,0,0,0,
    1,1,1,1,0,0,1,1,1,0,0,0,
    0,1,1,1,1,1,1,1,1,1,0,0,
    0,0,1,1,1,1,1,1,1,1,1,0,
    0,0,1,1,1,1,1,1,1,1,1,1,
    0,0,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,0,0,
    1,1,1,1,0,0,1,1,1,0,0,0,
    0,1,1,0,0,0,0,0,0,0,0,0
};

static const uint8_t ASTEROID_1[] = {
    0,1,1,0,0,0,
    0,1,1,1,0,1,
    1,1,1,1,1,1,
    0,1,1,1,1,0,
    0,1,1,1,1,0,
    0,0,1,0,0,0
};

static const uint8_t ASTEROID_3[] = {
    0,1,1,0,0,0,0,0,
    0,1,1,1,0,1,1,1,
    1,1,1,1,1,1,1,1,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,0,0,
    1,1,1,1,1,1,1,0,
    1,1,0,1,1,1,1,1,
    0,0,0,1,1,1,0,0
};

static const uint8_t ASTEROID_2[] = {
    0,0,0,0,1,1,0,
    0,1,1,1,1,1,1,
    1,1,1,1,1,1,1,
    0,1,1,1,1,0,0,
    0,0,1,1,0,0,0,
    0,0,1,1,1,0,0,
    0,0,0,1,0,0,0
};

struct gamestate {
    char player;
    uint8_t ship[SHIP_ARRAY_SIZE]; 
    uint8_t ship_x;
    uint8_t ship_y;
    uint8_t shots_x[MAX_SHOTS];
    uint8_t shots_y[MAX_SHOTS];
    uint8_t shot_delay;
    uint8_t asteroids_x[MAX_ASTEROIDS];
    uint8_t asteroids_y[MAX_ASTEROIDS];
    uint8_t asteroids_t[MAX_ASTEROIDS];
    uint8_t asteroids_s[MAX_ASTEROIDS];
    uint8_t asteroids_h[MAX_ASTEROIDS];
    uint32_t ticks;
    uint32_t score;
    uint8_t ships;
    uint32_t ship_safe;
    uint8_t ship_blinker;
    uint8_t speed;
    uint32_t highscore;
} game; 
char key;

static bool highscore_set(uint32_t score, char nick[]);
static uint32_t highscore_get(char nick[]);
static void init_game();
static void conrtols();
static bool screen_intro();
static void draw_game();
  static void draw_ship();
  static void draw_score();
  static void draw_shots();
  static void draw_shot();
  static void draw_environ();
  static void draw_asteroid();
  static void draw_splash();
static void shoot();
static void make_asteroids();
static void detect_collitions();

void ram(void){
	while(1) {
    if (!screen_intro())
     return;
    init_game();
    while(game.ships > 0){
      game.ticks++;
      lcdFill(0);
      conrtols();
      draw_game();
      lcdDisplay();
      delayms(6);
      if(game.shot_delay > 0){
        game.shot_delay--;
      }
    }
    draw_splash();
	}
};

static bool screen_intro(void) {
	char key=0;
	bool step = false;
	while(key==0) {
		getInputWaitRelease();
		lcdFill(0);
		int ct = 0;
    for (int i = 0; i < SHIP_HEIGHT; i++){
      for(int d = 0; d < SHIP_WIDTH; d++){
        lcdSetPixel((40+d)%RESX,(10+i)%RESY,SHIP[ct]);
        ct++;
      }
    }
		DoString (13,25,"R0KET TYPE");
		uint32_t highscore;
	  char highnick[20];
	  highscore = highscore_get(highnick);
		DoInt(13, 40, highscore);
		DoString (13, 50, highnick);
		lcdDisplay();
		key=getInputWaitTimeout(1000);
	}
	//getInputWaitRelease();
	return !(key==BTN_LEFT);
}

static void draw_splash(void){
  char key=0;
	bool step = false;
	while(key==0) {
    lcdFill(0);
    DoString (16,15, "GAME OVER");
    if (highscore_set(game.score, GLOBAL(nickname)))
			DoString (16,25,"HIGHSCORE!");
		DoString (24,40, "GAME BY");
		DoString (10,50, "@RANZWERTIG");
    lcdDisplay();
		key=getInputWaitTimeout(1000);
  }
  return !(key==BTN_LEFT);
}

static bool highscore_set(uint32_t score, char nick[]) {
    MPKT * mpkt= meshGetMessage('r');
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
    MPKT * mpkt= meshGetMessage('r');

    strcpy(nick,(char*)MO_BODY(mpkt->pkt));

	return MO_TIME(mpkt->pkt);
}

static void init_game(void) {
 
  game.ticks = 0;
  game.ships = 3;
  game.score = 0;
  game.speed = 0;
  game.ship_x = 5;
  game.ship_y = RESY/2;
  game.ship_blinker = 0;
  game.ship_safe = 0;

  for(int i = 0; i<MAX_SHOTS;i++){
    game.shots_x[0] = 255;
    game.shots_y[0] = 255;
  }
  
  for(int i = 0; i<MAX_ASTEROIDS;i++){
    game.asteroids_x[i] = 255;
    game.asteroids_y[i] = 255;
    game.asteroids_t[i] = 0;
    game.asteroids_s[i] = 15;
    game.asteroids_h[i] = 0;
  }
  
  game.shot_delay = 0;
}

static void draw_game(void){
  draw_ship();
  draw_shots();
  draw_score();
  make_asteroids();
  detect_collitions();
  draw_environ();
}

static void draw_ship(void) {
  if(game.ship_safe > 0){
    game.ship_safe--;
  }
  if(game.ship_safe > 0 && game.ship_blinker == 0 && game.ship_safe%40 == 0){
    game.ship_blinker = 20;
  }
  if(game.ship_blinker > 0){
    game.ship_blinker--;
  }
  int ct = 0;
  for (int i = 0; i < SHIP_HEIGHT; i++){
    for(int d = 0; d < SHIP_WIDTH; d++){
      if(game.ship_blinker > 0){
      } else {
        lcdSetPixel((game.ship_x+d)%RESX,(game.ship_y+i)%RESY,SHIP[ct]);
      }
      ct++;
    }
  }
}

static void draw_environ(void){
  if(game.ticks%INCREASE_GAME_SPEED+1 == 0){
    game.speed++;
  }
  for(int i = 0; i < MAX_ASTEROIDS;i++){
    if(game.asteroids_t[i] > 0){
      draw_asteroid(i);
      int speed = game.ticks%game.asteroids_s[i];
      if(speed == 0) {
        game.asteroids_x[i]--;
      }
      if(game.asteroids_x[i] == 0){
        game.asteroids_t[i] = 0;
      }
    }
  }
}

static void make_asteroids(void){
  int freq =  ASTEROID_FREQ-game.speed;
  if(freq < 1){
    freq = 1;
  }
  if(game.ticks%freq == 0){
    uint8_t rand_x = RESX+getRandom()%RESX;
    uint8_t rand_y = getRandom()%RESY;
    int as = 0;
    for(int i = 0; i < MAX_ASTEROIDS;i++){
      if(game.asteroids_t[i] == 0){
        as = i;
      }
    }
    game.asteroids_x[as] = rand_x;
    game.asteroids_y[as] = rand_y;
    game.asteroids_t[as] = getRandom()%3+1;
    int speed = (getRandom()%ASTEROID_MIN_SPEED+ASTEROID_MAX_SPEED)-game.speed;
    if(speed < 0){
      speed = 0;
    }
    game.asteroids_s[as] = speed;
    
  }
}

static void draw_asteroid(int as){
  if(game.asteroids_t[as] > 0){
    int x = game.asteroids_x[as];
    int y = game.asteroids_y[as];
  
    if(game.asteroids_t[as] == 1){
      int ct = 0;
      for (int i = 0; i < ASTEROID_1_HEIGHT; i++){
        for(int d = 0; d < ASTEROID_1_WIDTH; d++){
          if(ASTEROID_1[ct] == 1){
            lcdSetPixel(x+d,y+i,1 && game.asteroids_h[as] == 0);
          }
          ct++;
       }
      }
    } else if(game.asteroids_t[as] == 2){
      int ct = 0;
      for (int i = 0; i < ASTEROID_2_HEIGHT; i++){
        for(int d = 0; d < ASTEROID_2_WIDTH; d++){
          if(ASTEROID_2[ct] == 1 && game.asteroids_h[as] == 0){
            lcdSetPixel(x+d,y+i,1);
          }
          if(ASTEROID_2[ct] == 1 && game.asteroids_h[as] == 1 && getRandom()%2 == 0){
            lcdSetPixel(x+d,y+i,1);
          }
          ct++;
       }
      }
    } else if(game.asteroids_t[as] == 3){
      int ct = 0;
      for (int i = 0; i < ASTEROID_3_HEIGHT; i++){
        for(int d = 0; d < ASTEROID_3_WIDTH; d++){
          if(ASTEROID_3[ct] == 1 && game.asteroids_h[as] == 0){
            lcdSetPixel(x+d,y+i,1);
          }
          if(ASTEROID_3[ct] == 1 && game.asteroids_h[as] == 1 && getRandom()%2 == 0){
            lcdSetPixel(x+d,y+i,1);
          }
          if(ASTEROID_3[ct] == 1 && game.asteroids_h[as] == 2 && getRandom()%4 == 0){
            lcdSetPixel(x+d,y+i,1);
          }
          ct++;
       }
      }
    }
  }
}

static void draw_score(void){
  DoInt(2,2,game.score);
  DoInt(RESX-8,2,game.ships);
  DoString (RESX-20,2,"H");
} 

static void detect_collitions(void){
  for(int i = 0;i<MAX_ASTEROIDS;i++){
    if(game.asteroids_t[i] > 0){
      int as_x = game.asteroids_x[i];
      int as_y = game.asteroids_y[i];
      int as_h = 0;
      int as_w = 0;
      if(game.asteroids_t[i] == 1){
        as_h = ASTEROID_1_HEIGHT;
        as_w = ASTEROID_1_WIDTH;
      } else if(game.asteroids_t[i] == 2){
        as_h = ASTEROID_2_HEIGHT;
        as_w = ASTEROID_2_WIDTH;
      } else if(game.asteroids_t[i] == 3){
        as_h = ASTEROID_3_HEIGHT;
        as_w = ASTEROID_3_WIDTH;
      }
      for(int z = 0; z<MAX_SHOTS;z++){
        if(game.shots_x[z] != 255){
          if(game.shots_y[z] >= game.asteroids_y[i] && game.shots_y[z] <= game.asteroids_y[i]+as_h){
            if(game.shots_x[z] > game.asteroids_x[i]){
              game.asteroids_h[i]++;
              game.shots_x[z] = 255;
              game.shots_y[z] = 255;
              if(game.asteroids_t[i] == game.asteroids_h[i]){
                game.score += game.asteroids_t[i];
                game.asteroids_x[i] = 255;
                game.asteroids_y[i] = 255;
                game.asteroids_t[i] = 0;
                game.asteroids_s[i] = 0;
                game.asteroids_h[i] = 0;
              }
            }  
          }
        }
      }
      int xl = game.ship_x;
      int yo = game.ship_y;
      int xr = game.asteroids_x[i] + as_w;
      int yu = game.asteroids_y[i] + as_h;
      if(game.asteroids_x[i] < game.ship_x){
        xl = game.asteroids_x[i];
      }
      if(game.asteroids_y[i] < game.ship_y){
        yo = game.asteroids_y[i];
      }
      if(game.ship_x + SHIP_WIDTH > game.asteroids_x[i] + as_w){
        xr = game.ship_x + SHIP_WIDTH;
      }
      if(game.ship_y + SHIP_HEIGHT > game.asteroids_y[i] + as_h){
        yu = game.ship_y + SHIP_HEIGHT;
      }
      if(SHIP_WIDTH + as_w > xr - xl && SHIP_HEIGHT + as_h > yu - yo){
        if(game.ship_safe == 0){
          game.ships--;
          game.asteroids_x[i] = 255;
          game.asteroids_y[i] = 255;
          game.asteroids_t[i] = 0;
          game.asteroids_s[i] = 0;
          game.ship_safe = SHIP_SAVE_TICKS;
        }
      }
    }
  }
}

static void draw_shots() {
  for(int i = 0; i<MAX_SHOTS;i++){
    if(game.shots_x[i] < 255){
      draw_shot(i);
    }
  }
}
static void draw_shot(int shot) {
  if(game.shots_x[shot] != 255){
    for (int length=0; length<=4; length++) {
      lcdSetPixel(game.shots_x[shot]+length, game.shots_y[shot], true);
    }
    game.shots_x[shot] += 1;
  }
}
static void shoot(void){
  if(game.shot_delay == 0){
    int shot = 0;
    for(int i = 0; i<MAX_SHOTS;i++){
      if(game.shots_x[i] == 255){
        shot = i;
        break;
      }
    }
    game.shots_x[shot] = game.ship_x%RESX + SHIP_WIDTH;
    game.shots_y[shot] = game.ship_y%RESY + SHIP_HEIGHT/2;
    game.shot_delay = SHOT_DELAY;
  }
}
static void conrtols(void){
  if (gpioGetValue(RB_BTN0)==0) {
    if (game.ship_x > 0){
      game.ship_x -= 1;
    }
	} else if (gpioGetValue(RB_BTN1)==0) {
	  if (game.ship_x < RESX-SHIP_WIDTH){ 
      game.ship_x += 1;
    }
	} else if (gpioGetValue(RB_BTN2)==0) {
	  if (game.ship_y < RESY-SHIP_HEIGHT){ 
      game.ship_y += 1;
    }
	} else if (gpioGetValue(RB_BTN3)==0) {
	  if (game.ship_y > 0){
      game.ship_y -= 1;
    }
	} else if (gpioGetValue(RB_BTN4)==0) {
    shoot();
	}
}


