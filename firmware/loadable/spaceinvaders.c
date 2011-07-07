#include <sysinit.h>

#include "basic/basic.h"

#include "lcd/render.h"
#include "lcd/display.h"
#include "lcd/fonts/invaders.c"
//#include "lcd/allfonts.h"

void ReinvokeISP(void);
void EnableWatchdog(uint32_t ms);
void delayms(uint32_t ms);

/**************************************************************************/
#define POS_PLAYER_Y 60
#define ENEMY_ROWS 3
#define ENEMY_COLUMNS 6
#define DISABLED 255

struct gamestate {
    char player;
    char shot_x, shot_y;
    char alive;
    char move, direction, lastcol;
    bool killed;
    char enemy_x[ENEMY_ROWS][ENEMY_COLUMNS];
    char enemy_row_y[ENEMY_ROWS];
    
} game = {RESX/2-4, DISABLED, 0,ENEMY_ROWS*ENEMY_COLUMNS, 0, -1, ENEMY_COLUMNS-1, false};
char key;
        
void ram(void) {
    gpioSetValue (RB_LED1, CFG_LED_OFF); 
    init_enemy();
    
    while (1) {
        if(gpioGetValue(RB_BTN2)==0){
            delayms(200);
            while(gpioGetValue(RB_BTN0)==0);
            return;
        }

        lcdFill(0);
        check_end();
        move_shot();
        move_player();
        move_enemy();
        draw_player();
        draw_enemy();
        draw_shot();
        draw_status();
        lcdDisplay(0);
        delayms(10);
    }
    return;
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
        if (game.enemy_row_y[row]+6 >= game.shot_y && game.enemy_row_y[row]+6 < game.shot_y+7) {
            for(int col = 0; col<ENEMY_COLUMNS; col++) {
                if(game.shot_x >= game.enemy_x[row][col] && game.shot_x < game.enemy_x[row][col]+8) {
                    game.enemy_x[row][col]=DISABLED;
                    game.shot_x = DISABLED;
                    game.alive--;
                    return;
                }
            }
        }
    }
    
    game.shot_y -= 3;
}

void move_player() {
    if(gpioGetValue(RB_BTN0)==0 && game.player > 0 ){
        game.player-=1;
    }
    
    if(gpioGetValue(RB_BTN1)==0 && game.player < RESX-8){
        game.player+=1;
    }
    
     if(gpioGetValue(RB_BTN4)==0 && game.shot_x == 255){
        game.shot_x = game.player+4;
        game.shot_y = POS_PLAYER_Y;
    }
}

void move_enemy() {
    if(game.move > 0){
        game.move--;
        return;
    }
    
    for (int col = 0; col < ENEMY_COLUMNS; col++) {
        for (int row = 0; row < ENEMY_ROWS; row++) {
            char pos = game.enemy_x[row][(game.direction==1)?(ENEMY_COLUMNS-(col+1)):col];
            if (pos != DISABLED) {
                //Check collision with player
                if(game.enemy_row_y[row]+8 >= POS_PLAYER_Y && pos+8 >= game.player && pos < game.player+8){
                    game.killed = true;
                }
                
                if((pos <=0 && game.direction != 1) ||
                   (pos >=RESX-8-1 && game.direction == 1)){
                    game.direction = (game.direction==1)?-1:1;
                    for (char r = 0; r<ENEMY_ROWS; r++) {
                        game.enemy_row_y[r]+=4;
                    }
                    return;
                }
                game.enemy_x[row][(game.direction==1)?(ENEMY_COLUMNS-(col+1)):col] += game.direction;
            }
        }
    }
    
    game.move = game.alive-1;
}

void move_enemy2() {
    if(game.move == 0) {
        bool next = false;
        for (int col = game.lastcol; col < ENEMY_COLUMNS || col <= 0; col+=game.direction) {
            for (int row = 0; row < ENEMY_ROWS; row++) {
                char pos = game.enemy_x[row][col];
                //There is an enemy on this row
                if (pos != DISABLED) {
                    if((pos <=0 && game.direction== -1) ||
                       (pos >=RESX-8 && game.direction == 1)){
                        game.direction *= -1;
                        //TODOmove down
                        return;
                    }
                    game.enemy_x[row][col] += game.direction;
                    next = true;
                }
            }
            if (next){
                game.lastcol += game.direction;
                return;
            }
        }
        game.move = game.alive;
        return;
    }
    game.move--;
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

void draw_status() {
    for (int p = 0; p<game.alive; p++){
        lcdSetPixel(p+1,1,true);
    }
}
    
    

void draw_sprite(char x, char y) {
    font = &Font_Invaders;
    DoString(x,y-1,"C");
/*    for (int dx=0; dx<8; dx++){
        for(int dy=0; dy<8; dy++){
            lcdSetPixel(x+dx,y+dy, true);
        }
    } */
}

void check_end() {
    if (game.killed) {
        game.player = RESX/2+4;
        for(int row; row<ENEMY_ROWS; row++) {
            game.enemy_row_y[row] = 10 + (40/ENEMY_ROWS)*row;
        }
        game.killed = false;
    }
    if (game.alive == 0) {
    }
}

