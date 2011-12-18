/*

    flame m0dul with RGB (rev c) - https://github.com/schneider42/flame
    LED driver is a PCA9633
*/

#include "basic/basic.h"
#include "core/i2c/i2c.h"
#include "basic/config.h"
#include "basic/xxtea.h"

#include "filesystem/util.h"

#define FLAME_I2C_WRITE         0xC6
#define FLAME_I2C_READ          0xC7

#define FLAME_I2C_CR_MODE1      0x00
#define FLAME_I2C_CR_MODE2      0x01
#define FLAME_I2C_CR_PWM0       0x02
#define FLAME_I2C_CR_PWM1       0x03
#define FLAME_I2C_CR_PWM2       0x04
#define FLAME_I2C_CR_PWM3       0x05
#define FLAME_I2C_CR_GRPPWM     0x06
#define FLAME_I2C_CR_GRPFREQ    0x07
#define FLAME_I2C_CR_LEDOUT     0x08

#define FLAME_I2C_LED0_OFF      0x00
#define FLAME_I2C_LED0_FULL     0x01
#define FLAME_I2C_LED0_INDY     0x02
#define FLAME_I2C_LED0_GRP      0x03
#define FLAME_I2C_LED1_OFF      0x00
#define FLAME_I2C_LED1_FULL     0x04
#define FLAME_I2C_LED1_INDY     0x08
#define FLAME_I2C_LED1_GRP      0x0c
#define FLAME_I2C_LED2_OFF      0x00
#define FLAME_I2C_LED2_FULL     0x10
#define FLAME_I2C_LED2_INDY     0x20
#define FLAME_I2C_LED2_GRP      0x30
#define FLAME_I2C_LED3_OFF      0x00
#define FLAME_I2C_LED3_FULL     0x40
#define FLAME_I2C_LED3_INDY     0x80
#define FLAME_I2C_LED3_GRP      0xC0

#define FLAME_OFF               0x00
#define FLAME_UP                0x01
#define FLAME_UP_WAIT           0x02
#define FLAME_DOWN              0x03
#define FLAME_DOWN_WAIT         0x04

/**************************************************************************/

#define flameBrightnessMax GLOBAL(flamemax)
#define flameBrightnessMin GLOBAL(flamemin)
#define flameSpeedUp       GLOBAL(flamespeed)
#define flameSpeedDown     GLOBAL(flamespeed)
#define flameWaitUp        GLOBAL(flamemaxw)
#define flameWaitDown      GLOBAL(flameminw)

#define nick               GLOBAL(nickname)

static uint32_t flameRGBSetI2C(uint8_t cr, uint8_t value) {
    I2CMasterBuffer[0] = FLAME_I2C_WRITE;
    I2CMasterBuffer[1] = cr;
    I2CMasterBuffer[2] = value;
    I2CWriteLength = 3;
    I2CReadLength = 0;
    return i2cEngine();
}

uint8_t flameRGBEnabled = 0;
uint8_t flameRGBMode = FLAME_OFF;
uint8_t flameRGBI2Cpwm = 0;
uint8_t flameRGBTicks = 0;

uint8_t rgbData[24];
uint8_t rgbDataSize = 0;
uint8_t rgbDataOffset = 0;

void setFlameRGBPWM() {
    flameRGBSetI2C(FLAME_I2C_CR_GRPPWM, flameRGBI2Cpwm); // set pwm
}

void setFlameRGBColor() {
    if (rgbDataSize > 2) {
        flameRGBSetI2C(FLAME_I2C_CR_PWM0, rgbData[rgbDataOffset]);
        flameRGBSetI2C(FLAME_I2C_CR_PWM3, rgbData[rgbDataOffset + 1]);
        flameRGBSetI2C(FLAME_I2C_CR_PWM2, rgbData[rgbDataOffset + 2]);
        if (rgbDataSize >= rgbDataOffset + 5) {
            rgbDataOffset += 3;
        } else {
            rgbDataOffset = 0;
        }
    } else {
        // generate a hash from the nickname
        uint32_t hash[4];
        uint32_t const key[4] = {0, 0, 0, 0};
        xxtea_cbcmac(hash, (uint32_t *)nick, 4, key);
        // set color, LED0 = red, LED1 = unused, LED2 = blue, LED3 = green
        int i;
        uint8_t regs[] = {FLAME_I2C_CR_PWM0, FLAME_I2C_CR_PWM1,
                          FLAME_I2C_CR_PWM2, FLAME_I2C_CR_PWM3 };
        for(i=0; i<4; i++)
            flameRGBSetI2C(regs[i], hash[i] & 0xFF);
    }
}

void tick_flame_rgb(void) { // every 10ms
    static char night=0;

    if (!flameRGBEnabled) {
        return;
    }

    if (night != isNight()) {
        night = isNight();
        if (!night) {
            flameRGBMode = FLAME_OFF;
            flameRGBI2Cpwm = 0;
            push_queue(&setFlameRGBPWM);
        };
    };

    flameRGBTicks++;

    if (flameRGBI2Cpwm > flameBrightnessMax) {
        flameRGBI2Cpwm = flameBrightnessMax;
    }
    if (flameRGBI2Cpwm < flameBrightnessMin) {
        flameRGBI2Cpwm = flameBrightnessMin;
    }

    if (flameRGBMode == FLAME_OFF) {
        if (isNight()) {
            flameRGBTicks = 0;
            flameRGBMode = FLAME_UP;
            push_queue(&setFlameRGBColor);
        }
    }

    if (flameRGBMode == FLAME_UP) {
        if (0xFF - flameRGBI2Cpwm >= flameSpeedUp ) {
            flameRGBI2Cpwm += flameSpeedUp;
        } else {
            flameRGBI2Cpwm = 0xFF;
        }
        push_queue(&setFlameRGBPWM);
        if (flameRGBI2Cpwm >= flameBrightnessMax) {
            flameRGBMode = FLAME_UP_WAIT;
            flameRGBTicks = 0;
        }
    }

    if (flameRGBMode == FLAME_UP_WAIT) {
        if (flameRGBTicks >= flameWaitUp) {
            flameRGBMode = FLAME_DOWN;
        }
    }

    if (flameRGBMode == FLAME_DOWN) {
        if (flameSpeedDown <= flameRGBI2Cpwm) {
            flameRGBI2Cpwm -= flameSpeedDown;
        } else {
            flameRGBI2Cpwm = 0x00;
        }
        push_queue(&setFlameRGBPWM);
        if (flameRGBI2Cpwm <= flameBrightnessMin) {
            flameRGBMode = FLAME_DOWN_WAIT;
            flameRGBTicks = 0;
        }
    }

    if (flameRGBMode == FLAME_DOWN_WAIT) {
        if (flameRGBTicks >= flameWaitDown) {
            flameRGBMode = FLAME_OFF;
        }
    }
}

void init_flame_rgb(void) {
    i2cInit(I2CMASTER); // Init I2C

    flameRGBEnabled = (flameRGBSetI2C(FLAME_I2C_CR_MODE1, 0x00) == I2CSTATE_ACK); // probe i2c and enable flame if present

    if (!flameRGBEnabled)
        return;

    // enable individual PWN and group PWN for LED0, LED3 and LED2 and disable LED1
    flameRGBSetI2C(FLAME_I2C_CR_LEDOUT, FLAME_I2C_LED0_GRP | FLAME_I2C_LED1_OFF | FLAME_I2C_LED2_GRP | FLAME_I2C_LED3_GRP);

    flameRGBSetI2C(FLAME_I2C_CR_GRPPWM, 0x00); // overall dimming

    rgbDataSize = readTextFile("FLAME.RGB", rgbData, 24);

    enableConfig(CFG_TYPE_FLAME,1);
}

