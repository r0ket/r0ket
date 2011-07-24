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

void flameSetI2C(uint8_t cr, uint8_t value) {
    I2CMasterBuffer[0] = FLAME_I2C_WRITE;
    I2CMasterBuffer[1] = cr;
    I2CMasterBuffer[2] = value;
    I2CWriteLength = 3;
    I2CReadLength = 0;
    i2cEngine();
}


uint8_t flameMode = FLAME_OFF;
uint8_t flameI2Csend = 0;
uint8_t flameI2Cpwm = 0;
uint16_t flameTicks = 0;

void tick_flame(void) { // every 10ms
    flameTicks++;

    if (flameMode == FLAME_OFF) {
	if (isNight()) {
	    flameTicks = 0;
	    flameMode = FLAME_UP;
	}
    }

    if (flameMode == FLAME_UP) {
	flameI2Cpwm++;
	flameI2Csend = 1;
	if (flameI2Cpwm == 0xFF) {
	    flameMode = FLAME_UP_WAIT;
	    flameTicks = 0;
	}
    }

    if (flameMode == FLAME_UP_WAIT) {
	if (flameTicks > 0xFF) {
	    flameMode = FLAME_DOWN;
	}
    }

    if (flameMode == FLAME_DOWN) {
	flameI2Cpwm--;
	flameI2Csend = 1;
	if (flameI2Cpwm == 0x00) {
	    flameMode = FLAME_DOWN_WAIT;
	    flameTicks = 0;
	}
    }

    if (flameMode == FLAME_DOWN_WAIT) {
	if (flameTicks > 0x8F) {
	    flameMode = FLAME_OFF;
	}
    }
}

void main_flame(void) {

    i2cInit(I2CMASTER); // Init I2C

    flameSetI2C(FLAME_I2C_CR_PSC0, 0x00); // set prescaler
    flameSetI2C(FLAME_I2C_CR_PWM0, 0x00); // set pwm
    flameSetI2C(FLAME_I2C_CR_LS0, FLAME_I2C_LS0_PWM0 << FLAME_I2C_LS0_LED0); // set led0 to pwm
    
    while (1) {
        delayms(20);
        
	if (flameI2Csend == 1) {
	    flameI2Csend = 0;
	    flameSetI2C(FLAME_I2C_CR_PWM0, flameI2Cpwm); // set pwm
	}

	char key = getInput();
	if (key == BTN_ENTER) {
	    DoString(0,50,"ISP!");
	    lcdDisplay();
            ISPandReset();
        }

    }
    
    return;
}
