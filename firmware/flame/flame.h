#ifndef _FLAME_H
#define _FLAME_H 1

#include <sysdefs.h>
#include "basic/config.h"

/* available types of flames */
#define FLAME_TYPE_NONE 0
#define FLAME_TYPE_M0N0 1
#define FLAME_TYPE_RGB  2

/* I2C protocol for m0n0 flame (PCA9533) */
#define FLAME_M0N0_I2C_WRITE         0xC4
#define FLAME_M0N0_I2C_READ          0xC5

#define FLAME_M0N0_I2C_CR_INPUT      0x00
#define FLAME_M0N0_I2C_CR_PSC0       0x01
#define FLAME_M0N0_I2C_CR_PWM0       0x02
#define FLAME_M0N0_I2C_CR_PSC1       0x03
#define FLAME_M0N0_I2C_CR_PWM1       0x04
#define FLAME_M0N0_I2C_CR_LS0        0x05

#define FLAME_M0N0_I2C_LS0_OFF       0x00
#define FLAME_M0N0_I2C_LS0_ON        0x01
#define FLAME_M0N0_I2C_LS0_PWM0      0x02
#define FLAME_M0N0_I2C_LS0_PWM1      0x03

#define FLAME_M0N0_I2C_LS0_LED0      0x00
#define FLAME_M0N0_I2C_LS0_LED1      0x02
#define FLAME_M0N0_I2C_LS0_LED2      0x04
#define FLAME_M0N0_I2C_LS0_LED3      0x06

/* I2C protocol for rgb flame (PC9633) */
#define FLAME_RGB_I2C_WRITE         0xC6
#define FLAME_RGB_I2C_READ          0xC7

#define FLAME_RGB_I2C_CR_MODE1      0x00
#define FLAME_RGB_I2C_CR_MODE2      0x01
#define FLAME_RGB_I2C_CR_PWM0       0x02
#define FLAME_RGB_I2C_CR_PWM1       0x03
#define FLAME_RGB_I2C_CR_PWM2       0x04
#define FLAME_RGB_I2C_CR_PWM3       0x05
#define FLAME_RGB_I2C_CR_GRPPWM     0x06
#define FLAME_RGB_I2C_CR_GRPFREQ    0x07
#define FLAME_RGB_I2C_CR_LEDOUT     0x08

#define FLAME_RGB_I2C_LED0_OFF      0x00
#define FLAME_RGB_I2C_LED0_FULL     0x01
#define FLAME_RGB_I2C_LED0_INDY     0x02
#define FLAME_RGB_I2C_LED0_GRP      0x03
#define FLAME_RGB_I2C_LED1_OFF      0x00
#define FLAME_RGB_I2C_LED1_FULL     0x04
#define FLAME_RGB_I2C_LED1_INDY     0x08
#define FLAME_RGB_I2C_LED1_GRP      0x0c
#define FLAME_RGB_I2C_LED2_OFF      0x00
#define FLAME_RGB_I2C_LED2_FULL     0x10
#define FLAME_RGB_I2C_LED2_INDY     0x20
#define FLAME_RGB_I2C_LED2_GRP      0x30
#define FLAME_RGB_I2C_LED3_OFF      0x00
#define FLAME_RGB_I2C_LED3_FULL     0x40
#define FLAME_RGB_I2C_LED3_INDY     0x80
#define FLAME_RGB_I2C_LED3_GRP      0xC0

/* state machine constants for default blinking */

#define FLAME_STATE_OFF               0x00
#define FLAME_STATE_UP                0x01
#define FLAME_STATE_UP_WAIT           0x02
#define FLAME_STATE_DOWN              0x03
#define FLAME_STATE_DOWN_WAIT         0x04

/* config options */

#define flameBrightnessMax GLOBAL(flamemax)
#define flameBrightnessMin GLOBAL(flamemin)
#define flameSpeedUp       GLOBAL(flamespeed)
#define flameSpeedDown     GLOBAL(flamespeed)
#define flameWaitUp        GLOBAL(flamemaxw)
#define flameWaitDown      GLOBAL(flameminw)

/* flame "API" */
uint32_t flameSetI2C(uint8_t addr, uint8_t cr, uint8_t value);
uint8_t  flameDetect();
void     flameInit(uint8_t type);
void     flameSetBrightness(uint8_t type, uint8_t bright);
void     flameSetColor(uint8_t type, uint8_t red, uint8_t green, uint8_t blue);
void     flameClaim(uint8_t type);
void     flameFree(uint8_t type);

extern uint8_t flamesClaimed;

#define flameSetI2Cm0n0(a, b)   flameSetI2C(FLAME_M0N0_I2C_WRITE, a, b)
#define flameSetI2Crgb(a, b)    flameSetI2C(FLAME_RGB_I2C_WRITE, a, b)

#endif /* _FLAME_H */
