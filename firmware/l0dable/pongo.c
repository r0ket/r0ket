/**************************************************************************/
/*! 
    @file     pong.c
    @author   tverrbjelke & Simon 
    @date     ccc2011
    @version  0.1

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2010, microBuilder SARL
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**************************************************************************/



#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"
#include "basic/random.h"

#include "lcd/render.h"
#include "lcd/display.h"

#include "usetable.h"


/********************************************
 ** Config Stuff
*********************************************/

/* Ball */
#define PONG_BALL_SIZE ( 6 )


/********************************************
** Datatypes
*********************************************/


typedef struct {
	int8_t heigth;
	int8_t width;
	int8_t offset; // size between display border and paddle
	int8_t pos;
} paddle_t;


typedef struct   {
	paddle_t paddle;
	int8_t score;
	bool second;
} player_t;

typedef struct {
	int8_t x;
	int8_t y;
	int8_t size;
	int8_t angle;
	int8_t speed_x;
	int8_t speed_y;
	bool moving;
	int8_t player_has_ball;
} ball_t;



ball_t ball1;
player_t player1;
player_t player2;


void print_score();
void title_screen();
void move_player();
void draw_paddle(player_t ply);
void draw_ball();
void check_border();
void check_collision_player(player_t ply);
void move_bot();
void end_game();

void init();
/* void init_ball(ball_t ball); */

/********************************************
 ** Code Section
*********************************************/

void init();
void ram();

void game(void)
{
  init();

  while (1)
  {

    lcdFill(0);

    if(gpioGetValue(RB_BTN0) == 0)
    {
      ram();
      return;
    }
    print_score();
    move_player();
    draw_ball(); 
    check_collision_player(player1);
    check_collision_player(player2);
    move_bot();  
    draw_paddle(player2);
    lcdDisplay();
    delayms(100);
  }
}

void init()
{
  // init ball
  ball1.size = PONG_BALL_SIZE;
  ball1.x = 15;
  ball1.y = 15;
  ball1.speed_x = 5;
  ball1.speed_y = 2;
  ball1.moving = false;
  ball1.player_has_ball = 1;
  
  // init player1
  player1.paddle.pos = 10;
  player1.second = false;
  player1.paddle.heigth = 20;
  player1.paddle.width = 3;
  player1.paddle.offset = 10;
  player1.score = 0; 
  
  // init player2
  player2.paddle.pos = 10;
  player2.second = true;
  player2.paddle.heigth = 20;
  player2.paddle.width = 3;
  player2.second = true;
  player2.paddle.offset = RESX - player2.paddle.width - 10;
  player2.score = 0; 
}

void print_score(void) 
{
  DoInt(RESX/2-10, 10, player2.score);
  DoInt(RESX/2+10, 10, player1.score);
}

void move_player(void) 
{
  if(gpioGetValue(RB_BTN3) == 0 && player1.paddle.pos > 0 ) 
  {
    player1.paddle.pos-=3;
  }

  if(gpioGetValue(RB_BTN2) == 0 && player1.paddle.pos < (RESY - player1.paddle.heigth))
  {
    player1.paddle.pos+=3;
  }
  draw_paddle(player1);
}


void draw_paddle(player_t ply) 
{
  for(int8_t i = 0; i < ply.paddle.heigth; i++)
   {
    for(int8_t  j = 0; j < ply.paddle.width; j++)
    {
      lcdSetPixel(ply.paddle.offset - j, i+ply.paddle.pos, 1);
    }
  }
}

void draw_ball(void) 
{
  if(ball1.moving == true) 
  {
    ball1.x += ball1.speed_x;
    ball1.y += ball1.speed_y;
    check_border();
  } 
  else 
  {
    if(ball1.player_has_ball == 1)
    {
      ball1.y = player1.paddle.pos + player1.paddle.heigth/2 - ball1.size/2;
      ball1.x = player1.paddle.offset + 3;
      /*
	if(ball1.speed_x < 0) 
	{
	fprintf(stderr, "speed change");
	ball1.speed_x = -ball1.speed_x; 
	}
      */

      if(gpioGetValue(RB_BTN1) == 0) 
      {
	ball1.moving = true;
      }
    } 
    else
    {
      ball1.y = player2.paddle.pos + player2.paddle.heigth/2 - ball1.size/2; 
      ball1.x = player2.paddle.offset - 3; 
      if(ball1.speed_x > 0) { 
	ball1.speed_x = -ball1.speed_x; 
      }
      
      ball1.moving = true;
    }
    
  }
  
  for(int8_t i = 0; i < ball1.size; i++) 
  {
    for(int8_t j = 0; j < ball1.size; j++) 
    {
      lcdSetPixel(ball1.x + j, ball1.y + i, 1);
    }
  }
}

void check_collision_player(player_t ply) {

	if (ply.second == true && ball1.moving  == true) {
		if(ball1.x >= (ply.paddle.offset - ply.paddle.width - 5) ) {
			if(ball1.y > ply.paddle.pos && ball1.y < (ply.paddle.pos + ply.paddle.heigth) ) {
				ball1.speed_x = - ball1.speed_x;
			}
		}
	}

	if(ply.second == false && ball1.moving == true) {
		if(ball1.x < (ply.paddle.offset + ply.paddle.width) ) {
			if(ball1.y > ply.paddle.pos && ball1.y < (ply.paddle.pos + ply.paddle.heigth) ) {
				ball1.speed_x = - ball1.speed_x;
			}
		}
	}
}

void check_border() {
	if(ball1.speed_y < 0) {
		if (ball1.y < 0) {
			ball1.speed_y = - ball1.speed_y;
		}
	} else if (ball1.speed_y > 0) {
		if(ball1.y > (RESY - ball1.size) ) {
			ball1.speed_y = - ball1.speed_y;
		}
	}

	// ball is out
	if(ball1.speed_x < 0) {
		if(ball1.x < 0) {
			ball1.speed_x = - ball1.speed_x;
			player1.score++;
			ball1.moving=false;
			ball1.player_has_ball = 1;
		}
	} else {
		if(ball1.x + ball1.size > RESX) {
			ball1.speed_x = - ball1.speed_x;
			player2.score++;
			ball1.moving = false;
			ball1.player_has_ball = 2;
		}
	}
}


void move_bot() {
	int8_t pad_pos = player2.paddle.pos + (player2.paddle.heigth / 2);
	int8_t real_y_ball = ball1.y + (ball1.size / 2);
	int8_t m_speed = 4;
	if(ball1.speed_x < 0) {
#ifdef SIMULATOR
 	fprintf(stderr,"Ball collide\n");
#endif
		if (pad_pos < (RESY/2) -3) {
			player2.paddle.pos += m_speed;
		} else if(pad_pos > (RESY/2) +1 ) {
			player2.paddle.pos -= m_speed;
		}
	} else {
		if ( pad_pos != real_y_ball ) {
			if(pad_pos > real_y_ball) {
				player2.paddle.pos -= m_speed;
			} else if (pad_pos < real_y_ball) {
				player2.paddle.pos += m_speed;
			}
		}
	}
}


static const struct MENU mainmenu = {"PONG", {
    {"Play Game",   &game},
//   {"Multiplayer",  &multi},
    {"Exit",   &end_game},
    {NULL,NULL}
}};

void end_game(void) {
	return;
}

void ram(void) {
    handleMenu(&mainmenu);
}
