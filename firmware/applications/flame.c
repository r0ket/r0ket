/*

    flame m0dul - https://github.com/kiu/flame

*/

#include "basic/basic.h"
#include "core/i2c/i2c.h"

#define FLAME_I2C_WRITE		0xC4
#define FLAME_I2C_READ		0xC5

#define FLAME_I2C_CR_INPUT	0x00
#define FLAME_I2C_CR_PSC0	0x01
#define FLAME_I2C_CR_PWM0	0x02
#define FLAME_I2C_CR_PSC1	0x03
#define FLAME_I2C_CR_PWM1	0x04
#define FLAME_I2C_CR_LS0	0x05

#define FLAME_I2C_LS0_OFF	0x00
#define FLAME_I2C_LS0_ON	0x01
#define FLAME_I2C_LS0_PWM0	0x02
#define FLAME_I2C_LS0_PWM1	0x03

#define FLAME_I2C_LS0_LED0	0x00
#define FLAME_I2C_LS0_LED1	0x02
#define FLAME_I2C_LS0_LED2	0x04
#define FLAME_I2C_LS0_LED3	0x06

#define FLAME_OFF		0x00
#define FLAME_UP		0x01
#define FLAME_UP_WAIT		0x02
#define FLAME_DOWN		0x03
#define FLAME_DOWN_WAIT		0x04

void ReinvokeISP(void);

/**************************************************************************/

//TODO SEC move to config
uint8_t flameBrightnessMax = 0xff;
uint8_t flameBrightnessMin = 0x00;
uint8_t flameSpeedUp = 0x01;
uint8_t flameSpeedDown = 0x01;
uint8_t flameWaitUp = 0xff;
uint8_t flameWaitDown = 0x8f;
//TODO SEC move to config


uint8_t flameEnabled = 0;
uint8_t flameMode = FLAME_OFF;
uint8_t flameI2Cpwm = 0;
uint8_t flameTicks = 0;

uint32_t flameSetI2C(uint8_t cr, uint8_t value) {
    I2CMasterBuffer[0] = FLAME_I2C_WRITE;
    I2CMasterBuffer[1] = cr;
    I2CMasterBuffer[2] = value;
    I2CWriteLength = 3;
    I2CReadLength = 0;
    return i2cEngine();
}

void setFlamePWM() {
    flameSetI2C(FLAME_I2C_CR_PWM0, flameI2Cpwm); // set pwm
}


void tick_flame(void) { // every 10ms
    flameTicks++;

    if (flameI2Cpwm > flameBrightnessMax) {
	flameI2Cpwm = flameBrightnessMax;
    }
    if (flameI2Cpwm < flameBrightnessMin) {
	flameI2Cpwm = flameBrightnessMin;
    }

    if (flameMode == FLAME_OFF) {
	if (isNight() && flameEnabled) {
	    flameTicks = 0;
	    flameMode = FLAME_UP;
	}
    }

    if (flameMode == FLAME_UP) {
	if (0xFF - flameI2Cpwm >= flameSpeedUp) {
	    flameI2Cpwm += flameSpeedUp;
	} else {	
	    flameI2Cpwm = 0xFF;
	}
	push_queue(&setFlamePWM);
	if (flameI2Cpwm >= flameBrightnessMax) {
	    flameMode = FLAME_UP_WAIT;
	    flameTicks = 0;
	}
    }

    if (flameMode == FLAME_UP_WAIT) {
	if (flameTicks >= flameWaitUp) {
	    flameMode = FLAME_DOWN;
	}
    }

    if (flameMode == FLAME_DOWN) {
	if (flameSpeedDown <= flameI2Cpwm) {
	    flameI2Cpwm -= flameSpeedDown;
	} else {
	    flameI2Cpwm = 0x00;
	}
	push_queue(&setFlamePWM);
	if (flameI2Cpwm <= flameBrightnessMin) {
	    flameMode = FLAME_DOWN_WAIT;
	    flameTicks = 0;
	}
    }

    if (flameMode == FLAME_DOWN_WAIT) {
	if (flameTicks >= flameWaitDown) {
	    flameMode = FLAME_OFF;
	}
    }
}

void main_flame(void) {

    i2cInit(I2CMASTER); // Init I2C

    flameEnabled = (flameSetI2C(FLAME_I2C_CR_LS0, FLAME_I2C_LS0_OFF << FLAME_I2C_LS0_LED0) == I2CSTATE_ACK); // probe i2c

    if (flameEnabled) {
	flameSetI2C(FLAME_I2C_CR_LS0, FLAME_I2C_LS0_OFF << FLAME_I2C_LS0_LED0); // set led0 off
	flameSetI2C(FLAME_I2C_CR_LS0, FLAME_I2C_LS0_OFF << FLAME_I2C_LS0_LED1); // set led1 off
	flameSetI2C(FLAME_I2C_CR_LS0, FLAME_I2C_LS0_OFF << FLAME_I2C_LS0_LED2); // set led2 off
	flameSetI2C(FLAME_I2C_CR_LS0, FLAME_I2C_LS0_OFF << FLAME_I2C_LS0_LED3); // set led3 off

	flameSetI2C(FLAME_I2C_CR_PSC0, 0x00); // set prescaler
	flameSetI2C(FLAME_I2C_CR_PWM0, 0x00); // set pwm
	flameSetI2C(FLAME_I2C_CR_LS0, FLAME_I2C_LS0_PWM0 << FLAME_I2C_LS0_LED0); // set led0 to pwm
    }
    
    while (1) {
	char key = getInput();
	if (key == BTN_ENTER) {
	    DoString(0,50,"ISP!");
	    lcdDisplay();
            ISPandReset();
        }

	work_queue();
    }
    
    return;
}
