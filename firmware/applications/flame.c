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

#define FLAME_MODE_ON		0x00
#define FLAME_MODE_A		0x01
#define FLAME_MODE_B		0x02
#define FLAME_MODE_C		0x03

#define FLAME_MODE_MIN		0x00
#define FLAME_MODE_MAX		0x03

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

void tick_flame(void) { // every 10ms
}

void main_flame(void) {

    usbMSCInit();
    i2cInit(I2CMASTER); // Init I2C

//    flameSetI2C(FLAME_I2C_CR_LS0, FLAME_I2C_LS0_ON << FLAME_I2C_LS0_LED0); // set led to on
//    flameSetI2C(FLAME_I2C_CR_LS0, FLAME_I2C_LS0_OFF << FLAME_I2C_LS0_LED0); // set led to off

    flameSetI2C(FLAME_I2C_CR_PSC0, 0x66); // set prescaler
    flameSetI2C(FLAME_I2C_CR_PWM0, 0x33); // set pwm
    flameSetI2C(FLAME_I2C_CR_LS0, FLAME_I2C_LS0_PWM0 << FLAME_I2C_LS0_LED0); // set led to pwm

    char key;
    while (1) {
	key = getInput();
	if (key == BTN_LEFT) {
	    DoString(0,50,"ISP!");
	    lcdDisplay();
            ISPandReset();
        }
    }
    
    return;
}
