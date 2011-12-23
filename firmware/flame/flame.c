#include "flame/flame.h"

#include "basic/basic.h"
#include "core/i2c/i2c.h"

uint8_t flamesClaimed = FLAME_TYPE_NONE;

uint8_t flameBrigthnessRGB = 0;
uint8_t flameBrigthnessM0n0 = 0;

uint8_t rgb[3];

uint32_t flameSetI2C(uint8_t addr, uint8_t cr, uint8_t value) {
    I2CMasterBuffer[0] = addr;
    I2CMasterBuffer[1] = cr;
    I2CMasterBuffer[2] = value;
    I2CWriteLength = 3;
    I2CReadLength = 0;
    return i2cEngine();
}

uint8_t flameDetect() {
    i2cInit(I2CMASTER); // Init I2C
    uint8_t found = FLAME_TYPE_NONE;
    if (flameSetI2C(FLAME_RGB_I2C_WRITE, FLAME_RGB_I2C_CR_MODE1, 0x00) == I2CSTATE_ACK) {
        found |= FLAME_TYPE_RGB;
    }
    if (flameSetI2C(FLAME_M0N0_I2C_WRITE, FLAME_M0N0_I2C_CR_LS0, FLAME_M0N0_I2C_LS0_OFF << FLAME_M0N0_I2C_LS0_LED0) == I2CSTATE_ACK) {
        found |= FLAME_TYPE_M0N0;
    }
    return found;
}

void flameInit(uint8_t type) {
    if (type & FLAME_TYPE_M0N0) {
        flameSetI2Cm0n0(FLAME_M0N0_I2C_CR_LS0, FLAME_M0N0_I2C_LS0_OFF << FLAME_M0N0_I2C_LS0_LED0); // set led0 off
        flameSetI2Cm0n0(FLAME_M0N0_I2C_CR_LS0, FLAME_M0N0_I2C_LS0_OFF << FLAME_M0N0_I2C_LS0_LED1); // set led1 off
        flameSetI2Cm0n0(FLAME_M0N0_I2C_CR_LS0, FLAME_M0N0_I2C_LS0_OFF << FLAME_M0N0_I2C_LS0_LED2); // set led2 off
        flameSetI2Cm0n0(FLAME_M0N0_I2C_CR_LS0, FLAME_M0N0_I2C_LS0_OFF << FLAME_M0N0_I2C_LS0_LED3); // set led3 off

        flameSetI2Cm0n0(FLAME_M0N0_I2C_CR_PSC0, 0x00); // set prescaler
        flameSetI2Cm0n0(FLAME_M0N0_I2C_CR_PWM0, 0x00); // set pwm
        flameSetI2Cm0n0(FLAME_M0N0_I2C_CR_LS0, FLAME_M0N0_I2C_LS0_PWM0 << FLAME_M0N0_I2C_LS0_LED0); // set led0 to pwm
    }
    if (type & FLAME_TYPE_RGB) {
        // enable individual PWN and group PWN for LED0, LED3 and LED2 and disable LED1
        flameSetI2Crgb(FLAME_RGB_I2C_CR_LEDOUT, FLAME_RGB_I2C_LED0_GRP | FLAME_RGB_I2C_LED1_OFF | FLAME_RGB_I2C_LED2_GRP | FLAME_RGB_I2C_LED3_GRP);
        flameSetI2Crgb(FLAME_RGB_I2C_CR_GRPPWM, 0x00); // overall dimming
    }
}

void _setFlamePWMrgb() {
    flameSetI2Crgb(FLAME_RGB_I2C_CR_GRPPWM, flameBrigthnessRGB);
}
void _setFlamePWMm0n0() {
    flameSetI2Cm0n0(FLAME_M0N0_I2C_CR_PWM0, flameBrigthnessM0n0);
}

void flameSetBrightness(uint8_t type, uint8_t bright) {
    if (type & FLAME_TYPE_M0N0) {
        flameBrigthnessM0n0 = bright;
        push_queue(&_setFlamePWMm0n0);
    }
    if (type & FLAME_TYPE_RGB) {
        flameBrigthnessRGB = bright;
        push_queue(&_setFlamePWMrgb);
    }
}

void _setFlameColor() {
    flameSetI2Crgb(FLAME_RGB_I2C_CR_PWM0, rgb[0]);
    flameSetI2Crgb(FLAME_RGB_I2C_CR_PWM3, rgb[1]);
    flameSetI2Crgb(FLAME_RGB_I2C_CR_PWM2, rgb[2]);
}

void flameSetColor(uint8_t type, uint8_t red, uint8_t green, uint8_t blue) {
    if (type & FLAME_TYPE_RGB) {
        rgb[0] = red;
        rgb[1] = green;
        rgb[2] = blue;
        push_queue(&_setFlameColor);
    }
}

void flameClaim(uint8_t type) {
    flamesClaimed |= type;
}

void flameFree(uint8_t type) {
    flamesClaimed &= ~type;
}

