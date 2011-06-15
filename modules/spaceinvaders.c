#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/display.h"
#include "lcd/allfonts.h"

void ReinvokeISP(void);
void EnableWatchdog(uint32_t ms);
void delayms(uint32_t ms);

/**************************************************************************/
#define POS_PLAYER_Y 59
#define ENEMY_ROWS 3
#define ENEMY_COLUMNS 6
#define DISABLED 255

struct gamestate {
    int player;
    int shot_x, shot_y;
    int enemy_x[ENEMY_ROWS][ENEMY_COLUMNS];
    int enemy_row_y[ENEMY_ROWS];
    
} game = {RESX/2-4, DISABLED, 0};
char key;


void checkISP(void) {
    if(gpioGetValue(RB_BTN2)==0){
        gpioSetValue (RB_LED1, CFG_LED_ON); 
        delayms(200);
        gpioSetValue (RB_LED1, CFG_LED_OFF); 
        while(gpioGetValue(RB_BTN0)==0);
        EnableWatchdog(1000*5);
        ReinvokeISP();
    }
}

void init_enemy() {
    for (int row = 0; row<ENEMY_ROWS; row++) {
        game.enemy_row_y[row] = 10 + (40/ENEMY_ROWS)*row;
        for (int col = 0; col<ENEMY_COLUMNS; col++) {
            game.enemy_x[row][col] = 5+(86/ENEMY_COLUMNS)*col;
        }
    }
}

void move_shot() {
    //No shot, do nothing
    if(game.shot_x == DISABLED) {
        return;
    }
    
    //moving out of top, end shot
    if (game.shot_y <= 0) {
            game.shot_x = DISABLED;
            return;
    } 
    
    //check for collision with enemy, kill enemy if
    for (int row=0; row<ENEMY_ROWS; row++) {
        if (game.enemy_row_y[row] >= game.shot_y && game.enemy_row_y[row] < game.shot_y+8) {
            for(int col = 0; col<ENEMY_COLUMNS; col++) {
                if(game.shot_x >= game.enemy_x[row][col] && game.shot_x < game.enemy_x[row][col]+8) {
                    game.enemy_x[row][col]=DISABLED;
                    game.shot_x = DISABLED;
                }
            }
        }
    }
    
    game.shot_y -= 3;
}

void move_player() {
    if(gpioGetValue(RB_BTN0)==0 && game.player >= 0 ){
        game.player-=2;
    }
    
    if(gpioGetValue(RB_BTN1)==0 && game.player < RESX-8){
        game.player+=2;
    }
    
     if(gpioGetValue(RB_BTN4)==0 && game.shot_x == 255){
        game.shot_x = game.player+4;
        game.shot_y = POS_PLAYER_Y;
    }
}

void draw_player() {
    //draw_sprite(50, 20);
    draw_sprite(game.player, POS_PLAYER_Y);
}

void draw_enemy() {
    for (int row = 0; row<ENEMY_ROWS; row++) {
        for (int col = 0; col<ENEMY_COLUMNS; col++) {
            if (game.enemy_x[row][col] != DISABLED) {
                draw_sprite(game.enemy_x[row][col],game.enemy_row_y[row]);
            }
        }
    }
}
    
void draw_shot() {
    if (game.shot_x != 255) {
        for (int length=0; length<=5; length++) {
            lcdSetPixel(game.shot_x, game.shot_y+length, true);
        }
    }
}
    

void draw_sprite(int x, int y) {
    for (int dx=0; dx<8; dx++){
        for(int dy=0; dy<8; dy++){
            lcdSetPixel(x+dx,y+dy, true);
        }
    }
}

void module_spaceinvaders(void) {
    gpioSetValue (RB_LED1, CFG_LED_OFF); 
    backlightInit();
    
    init_enemy();
    
    while (1) {
        checkISP();
        lcdFill(0);
        
        move_shot();
        move_player();
        draw_player();
        draw_enemy();
        draw_shot();
        lcdDisplay(0);
        delayms(10);
    }
    return;
}