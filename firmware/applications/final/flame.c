/*

    original flame m0dul - https://github.com/kiu/flame

    flame m0dul with RGB (rev c) - https://github.com/schneider42/flame
*/

#include "basic/basic.h"
#include "basic/config.h"
#include "basic/xxtea.h"
#include "flame/flame.h"

#include "filesystem/util.h"

#define nick               GLOBAL(nickname)

static uint8_t flamesEnabled = 0;
static uint8_t flameMode = FLAME_STATE_OFF;
static uint8_t flameBrightness = 0;
static uint8_t flameTicks = 0;
static uint8_t flamesOwned = 0;

static uint8_t rgbData[24];
static uint8_t rgbDataSize = 0;
static uint8_t rgbDataOffset = 0;

static void changeColor() {
    if (rgbDataSize > 2) {
        flameSetColor(flamesOwned, rgbData[rgbDataOffset],
                rgbData[rgbDataOffset + 1], rgbData[rgbDataOffset + 2]);
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

        flameSetColor(flamesOwned, hash[0] & 0xFF, hash[1] & 0xFF, hash[2] & 0xFF);
    }
}

void tick_flame(void) { // every 10ms
    static char night=0;
    flamesOwned = flamesEnabled & ~flamesClaimed;

    if (!flamesOwned) {
        return;
    }

    if (night != isNight()) {
        night = isNight();
        if (!night) {
            flameMode = FLAME_STATE_OFF;
            flameBrightness = 0;
            flameSetBrightness(flamesOwned, flameBrightness);
        };
    };

    flameTicks++;

    if (flameBrightness > flameBrightnessMax) {
        flameBrightness = flameBrightnessMax;
    }
    if (flameBrightness < flameBrightnessMin) {
        flameBrightness = flameBrightnessMin;
    }

    if (flameMode == FLAME_STATE_OFF) {
        if (isNight()) {
            flameTicks = 0;
            flameMode = FLAME_STATE_UP;
            changeColor(flamesOwned, flameBrightness);
        }
    }

    if (flameMode == FLAME_STATE_UP) {
        if (0xFF - flameBrightness >= flameSpeedUp ) {
            flameBrightness += flameSpeedUp;
        } else {
            flameBrightness = 0xFF;
        }
        flameSetBrightness(flamesOwned, flameBrightness);
        if (flameBrightness >= flameBrightnessMax) {
            flameMode = FLAME_STATE_UP_WAIT;
            flameTicks = 0;
        }
    }

    if (flameMode == FLAME_STATE_UP_WAIT) {
        if (flameTicks >= flameWaitUp) {
            flameMode = FLAME_STATE_DOWN;
        }
    }

    if (flameMode == FLAME_STATE_DOWN) {
        if (flameSpeedDown <= flameBrightness) {
            flameBrightness -= flameSpeedDown;
        } else {
            flameBrightness = 0x00;
        }
        flameSetBrightness(flamesOwned, flameBrightness);
        if (flameBrightness <= flameBrightnessMin) {
            flameMode = FLAME_STATE_DOWN_WAIT;
            flameTicks = 0;
        }
    }

    if (flameMode == FLAME_STATE_DOWN_WAIT) {
        if (flameTicks >= flameWaitDown) {
            flameMode = FLAME_STATE_OFF;
        }
    }
}

void init_flame(void) {
    flamesEnabled = flameDetect();

    if (!flamesEnabled)
        return;

    flameInit(flamesEnabled);

    rgbDataSize = readTextFile("FLAME.RGB", (char *)rgbData, 24);

    enableConfig(CFG_TYPE_FLAME,1);
}

