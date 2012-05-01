/*
 * nick_work.c - workshop nick animation
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

#define LED_OFF LK_I2C_LS_OFF
#define LED_ON LK_I2C_LS_ON
#define LED_PWM0 LK_I2C_LS_PWM0
#define LED_PWM1 LK_I2C_LS_PWM1

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

static void lksetLED(uint8_t led, uint8_t state);
static uint32_t lkSetI2C(uint8_t cr, uint8_t value);
static uint8_t lkGetI2C(uint8_t cr);
static void lkUpdateI2C();
static void lkReadI2C();

static void init_lilakit(void);
static void tick_lilakit(void);
static void mainloop();

void ram(void) {
    init_lilakit();
	mainloop();
}

static void mainloop(void)
{

    lksetLED(4, LED_PWM1);
    //lksetLED(6, LED_PWM0);
    //lksetLED(7, LED_PWM0);
    lksetLED(13, LED_PWM0);
    lksetLED(14, LED_PWM1);

    while(getInputRaw()==BTN_NONE){
        tick_lilakit();
        //delayms_queue(10);    //TODO: This hangs the r0ket
                                // but should save energy.
        delayms(10);
    }
    return;
}

static void tick_lilakit(void)
{ // every 10ms
    lk_ticks++;

    if (lkEnabled == 0) {
	    return;
    }
    return; 
    if (lk_ticks % 10 == 0) {
	    lkReadI2C();
    
        if ((lk_in0 & 0x02) == 0 && lk_button_mode == 0) {

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
    uint8_t i;

    lkEnabled = (lkSetI2C(LK_I2C_CR_LS0, LK_I2C_LS_OFF << 0) == I2CSTATE_ACK); // probe i2c

    //show nickname
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

    if (lkEnabled == 0) {
	return;
    }

    // All LEDs off
    for(i=0; i<16; i++){
        lksetLED(i, LED_OFF);
    }

    // All PWMs off
    lkSetI2C(LK_I2C_CR_PSC0, 0x00);
    lkSetI2C(LK_I2C_CR_PWM0, 0x00);
    lkSetI2C(LK_I2C_CR_PSC1, 0x00);
    lkSetI2C(LK_I2C_CR_PWM1, 0x00);

    // Prepare SS
    //gpioSetDir(LK_PIEZO, gpioDirection_Output);
    //gpioSetValue(LK_PIEZO, 1);

    // Prepare blinking
    lkSetI2C(LK_I2C_CR_PSC0, 0x23);
    lkSetI2C(LK_I2C_CR_PWM0, 0x66);
    lkSetI2C(LK_I2C_CR_PSC1, 0x75);
    lkSetI2C(LK_I2C_CR_PWM1, 0x12);

    // Enable both LEDs
    //lk_ls1 |= LK_I2C_LS_PWM0 << 4;
    //lk_ls1 |= LK_I2C_LS_PWM1 << 6;
    

    lk_ticks = 0;
    lk_button_mode = 0x00;
    lk_ls0 = 0x00;
    lk_ls1 = 0x00;
    lk_ls2 = 0x00;
    lk_ls3 = 0x00;
    lk_in0 = 0x00;
    lk_in1 = 0x00;
    lk_ticks = 0x0000;

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

static void lksetLED(uint8_t led, uint8_t state)
{
    uint8_t reg;
    uint8_t val;
    uint8_t pos = (led % 4)*2;

    if( led < 4 ){
        reg = LK_I2C_CR_LS0;
        lk_ls0 &= ~(LK_I2C_LS_OFF << pos);
        val = lk_ls0 = lk_ls0 | (state << pos );
    }else if( led < 8 ){
        reg = LK_I2C_CR_LS1;
        lk_ls1 &= ~(LK_I2C_LS_OFF << pos);
        val = lk_ls1 = lk_ls1 | (state << pos );
    }else if( led < 12 ){
        reg = LK_I2C_CR_LS2;
        lk_ls2 &= ~(LK_I2C_LS_OFF << pos);
        val = lk_ls2 = lk_ls2 | (state << pos );
    }else{
        reg = LK_I2C_CR_LS3;
        lk_ls3 &= ~(LK_I2C_LS_OFF << pos);
        val = lk_ls3 = lk_ls3 | (state << pos );
    }

    lkSetI2C(reg, val);
}
