/*
 * geigerct.c
 *
 *
 *  Created on: 11.08.2011
 *      Author: Turbo J <turboj@web.de>
 *
 *  Implements simple Geiger Counter
 *  Counts rising edges on P3_0 = BUSINT
 *  so you can directly connect the Geiger board
 *  from http://mightyohm.com/blog/products/geiger-counter/
 *
 */

#include <sysinit.h>
#include <string.h>
#include <stdint.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "core/i2c/i2c.h"
#include "core/timer32/timer32.h"
#include "usetable.h"

//I2C address: (C0-CF)
#define LK_I2C_WRITE		0xCE 
#define LK_I2C_READ		0xCF

#define LK_I2C_CR_INPUT0	0x00
#define LK_I2C_CR_INPUT1	0x01
#define LK_I2C_CR_PSC0		0x02
#define LK_I2C_CR_PWM0		0x03
#define LK_I2C_CR_PSC1		0x04
#define LK_I2C_CR_PWM1		0x05
#define LK_I2C_CR_LS0		0x06
#define LK_I2C_CR_LS1		0x07
#define LK_I2C_CR_LS2		0x08
#define LK_I2C_CR_LS3		0x09

#define LK_I2C_LS_OFF		0x00
#define LK_I2C_LS_ON		0x01
#define LK_I2C_LS_PWM0		0x02
#define LK_I2C_LS_PWM1		0x03

#define LK_PIEZO		RB_SPI_SS3


uint8_t lkEnabled = 0;
uint8_t lk_button_mode = 0x00;

uint8_t lk_ls0 = 0x00;
uint8_t lk_ls1 = 0x00;
uint8_t lk_ls2 = 0x00;
uint8_t lk_ls3 = 0x00;

uint8_t lk_in0 = 0x00;
uint8_t lk_in1 = 0x00;
uint16_t lk_ticks = 0x0000;
uint8_t lk_piezo_toggle = 0x00;

struct note {
    int freq;
    int duration;
};
struct note  melody[] = {
        {1000, 1000},
        {2000,  300},
        {3000,  800},
        { 500, 1000},
        { 300, 1000}
};
#define MELODY_LEN (sizeof(melody)/sizeof(struct note))

int melody_index;
int melody_timeout;

static void init_lilakit(void);
static void tick_lilakit(void);
static void mainloop();
void handler(void);

void ram(void) {
    timer32Callback0 = handler;
    
    /* Enable the clock for CT32B0 */
    SCB_SYSAHBCLKCTRL |= (SCB_SYSAHBCLKCTRL_CT32B0);
    TMR_TMR32B0MR0  = (72E6/5E3)/2;
    TMR_TMR32B0MCR = (TMR_TMR32B0MCR_MR0_INT_ENABLED | TMR_TMR32B0MCR_MR0_RESET_ENABLED);
    melody_index = MELODY_LEN;
    
    init_lilakit();
	mainloop();

    NVIC_DisableIRQ(TIMER_32_0_IRQn);
    TMR_TMR32B0TCR = TMR_TMR32B0TCR_COUNTERENABLE_DISABLED;
}

void handler(void)
{
    static int time=0;
    if (time==0){time=1;} else {time=0;}
    gpioSetValue (LK_PIEZO, time);
}

static void mainloop(void)
{
    int dx=0;
	int dy=0;
    
	setExtFont(GLOBAL(nickfont));
	dx=DoString(0,0,GLOBAL(nickname));
    dx=(RESX-dx)/2;
    if(dx<0)
        dx=0;
    dy=(RESY-getFontHeight())/2;

	lcdClear();
    lcdSetPixel(1,1,1);
	DoString(dx,dy,GLOBAL(nickname));
    lcdRefresh();

    lk_ticks = 0;
    lk_button_mode = 0x00;
    lk_ls0 = 0x00;
    lk_ls1 = 0x00;
    lk_ls2 = 0x00;
    lk_ls3 = 0x00;
    lk_in0 = 0x00;
    lk_in1 = 0x00;
    lk_ticks = 0x0000;
    lk_piezo_toggle = 0x00;

    while(getInputRaw()==BTN_NONE){
        tick_lilakit();
        melody_play();
        //delayms_queue(10);    //XXX: this hangs the badge.
        delayms(10);
    }
    return;
}

void melody_play(void)
{
    int ton, timer;
    if( melody_index < MELODY_LEN){
        if(melody_timeout == 0){
            ton = melody[melody_index].freq;
            melody_timeout = melody[melody_index].duration / 10;
            timer = (72000000UL/ton/2);
            TMR_TMR32B0MR0 = timer;
            if (TMR_TMR32B0TC > timer){
                //schneller fix wenn ton zurueckgesetzt wird, aber timer weiterlaeuft
                TMR_TMR32B0TC=0;
            }
            if( melody_index == 0 ){
                NVIC_EnableIRQ(TIMER_32_0_IRQn);
                TMR_TMR32B0TCR = TMR_TMR32B0TCR_COUNTERENABLE_ENABLED;
            }
            melody_index++;
        }
        melody_timeout--;
    }else{
        NVIC_DisableIRQ(TIMER_32_0_IRQn);
        TMR_TMR32B0TCR = TMR_TMR32B0TCR_COUNTERENABLE_DISABLED;
    }
}

static uint32_t lkSetI2C(uint8_t cr, uint8_t value) {
    I2CMasterBuffer[0] = LK_I2C_WRITE;
    I2CMasterBuffer[1] = cr;
    I2CMasterBuffer[2] = value;
    I2CWriteLength = 3;
    I2CReadLength = 0;
    return i2cEngine();
}

static uint8_t lkGetI2C(uint8_t cr) {
    I2CMasterBuffer[0] = LK_I2C_WRITE;
    I2CMasterBuffer[1] = cr;
    I2CWriteLength = 2;
    I2CReadLength = 0;
    i2cEngine();

    I2CMasterBuffer[0] = LK_I2C_READ;
    I2CMasterBuffer[1] = cr;
    I2CWriteLength = 2;
    I2CReadLength = 1;
    i2cEngine();
    return I2CSlaveBuffer[0];
}

static void lkUpdateI2C() {
    lkSetI2C(LK_I2C_CR_LS0, lk_ls0);
    lkSetI2C(LK_I2C_CR_LS1, lk_ls1);
    lkSetI2C(LK_I2C_CR_LS2, lk_ls2);
    lkSetI2C(LK_I2C_CR_LS3, lk_ls3);
}

static void lkReadI2C() {
    lk_in0 = lkGetI2C(LK_I2C_CR_INPUT0);
    lk_in1 = lkGetI2C(LK_I2C_CR_INPUT1);
}

static void tick_lilakit(void)
{ // every 10ms
    lk_ticks++;

    if (lkEnabled == 0) {
	    return;
    }
 
    if (lk_ticks % 10 == 0) {
	    lkReadI2C();
    
        if ((lk_in0 & 0x02) == 0 && lk_button_mode == 0) {
            melody_index = 0;
            melody_timeout = 0;

            lk_ticks = 0;
            lk_button_mode = 1;
        
            lk_ls1 = 0;
            lk_ls1 |= LK_I2C_LS_ON << 4;
            lk_ls1 |= LK_I2C_LS_ON << 6;
            lkUpdateI2C();
        }

        if (lk_button_mode == 1 && lk_ticks > 0xFF) {
            lk_button_mode = 0;
            lk_ls1 = 0;
            lk_ls1 |= LK_I2C_LS_PWM0 << 4;
            lk_ls1 |= LK_I2C_LS_PWM1 << 6;
            lkUpdateI2C();
        }
    }

}

static void init_lilakit(void)
{

    lkEnabled = (lkSetI2C(LK_I2C_CR_LS0, LK_I2C_LS_OFF << 0) == I2CSTATE_ACK); // probe i2c
    if (lkEnabled == 0) {
	return;
    }

    // All LEDs off
    lkSetI2C(LK_I2C_CR_LS0, LK_I2C_LS_OFF << 0);
    lkSetI2C(LK_I2C_CR_LS0, LK_I2C_LS_OFF << 2);
    lkSetI2C(LK_I2C_CR_LS0, LK_I2C_LS_OFF << 4);
    lkSetI2C(LK_I2C_CR_LS0, LK_I2C_LS_OFF << 6);
    lkSetI2C(LK_I2C_CR_LS1, LK_I2C_LS_OFF << 0);
    lkSetI2C(LK_I2C_CR_LS1, LK_I2C_LS_OFF << 2);
    lkSetI2C(LK_I2C_CR_LS1, LK_I2C_LS_OFF << 4);
    lkSetI2C(LK_I2C_CR_LS1, LK_I2C_LS_OFF << 6);
    lkSetI2C(LK_I2C_CR_LS2, LK_I2C_LS_OFF << 0);
    lkSetI2C(LK_I2C_CR_LS2, LK_I2C_LS_OFF << 2);
    lkSetI2C(LK_I2C_CR_LS2, LK_I2C_LS_OFF << 4);
    lkSetI2C(LK_I2C_CR_LS2, LK_I2C_LS_OFF << 6);
    lkSetI2C(LK_I2C_CR_LS3, LK_I2C_LS_OFF << 0);
    lkSetI2C(LK_I2C_CR_LS3, LK_I2C_LS_OFF << 2);
    lkSetI2C(LK_I2C_CR_LS3, LK_I2C_LS_OFF << 4);
    lkSetI2C(LK_I2C_CR_LS3, LK_I2C_LS_OFF << 6);

    // All PWMs off
    lkSetI2C(LK_I2C_CR_PSC0, 0x00);
    lkSetI2C(LK_I2C_CR_PWM0, 0x00);
    lkSetI2C(LK_I2C_CR_PSC1, 0x00);
    lkSetI2C(LK_I2C_CR_PWM1, 0x00);

    // Prepare SS
    gpioSetDir(LK_PIEZO, gpioDirection_Output);
    gpioSetValue(LK_PIEZO, 1);

    // Prepare blinking
    lkSetI2C(LK_I2C_CR_PSC0, 0x23);
    lkSetI2C(LK_I2C_CR_PWM0, 0x66);
    lkSetI2C(LK_I2C_CR_PSC1, 0x75);
    lkSetI2C(LK_I2C_CR_PWM1, 0x12);

    // Enable both LEDs
    lk_ls1 |= LK_I2C_LS_PWM0 << 4;
    lk_ls1 |= LK_I2C_LS_PWM1 << 6;
    lkSetI2C(LK_I2C_CR_LS1, lk_ls1);
}

