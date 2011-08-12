/*

    lilakit - example source for r0ket workshop

*/

#include "basic/basic.h"
#include "core/i2c/i2c.h"

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

void ReinvokeISP(void);

/**************************************************************************/

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

uint32_t lkSetI2C(uint8_t cr, uint8_t value) {
    I2CMasterBuffer[0] = LK_I2C_WRITE;
    I2CMasterBuffer[1] = cr;
    I2CMasterBuffer[2] = value;
    I2CWriteLength = 3;
    I2CReadLength = 0;
    return i2cEngine();
}

uint8_t lkGetI2C(uint8_t cr) {
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

void lkUpdatePWM() {
    lkSetI2C(LK_I2C_CR_LS0, lk_ls0);
    lkSetI2C(LK_I2C_CR_LS1, lk_ls1);
    lkSetI2C(LK_I2C_CR_LS2, lk_ls2);
    lkSetI2C(LK_I2C_CR_LS3, lk_ls3);
}
    
void tick_lilakit(void) { // every 10ms

    char key = getInput();
    if (key == BTN_ENTER) {
	DoString(0,50,"ISP!");
	lcdDisplay();
	ISPandReset();
    }

    if (lkEnabled == 0) {
	return;
    }

    lk_in0 = lkGetI2C(LK_I2C_CR_INPUT0);
    lk_in1 = lkGetI2C(LK_I2C_CR_INPUT1);

    DoString(0,20,"             ");
    DoString(0,30,"             ");
    DoString(0,40,"             ");
    lcdDisplay();

    DoInt(0,20,lk_in0);
    DoInt(0,30,lk_in1);
    DoInt(0,40,lk_button_mode);
    DoInt(0,50,lk_in0 & 0x02);
    lcdDisplay();

    lk_ticks++;

    if ((lk_in0 & 0x02) == 0 && lk_button_mode == 0) {
	lk_ticks = 0;
	lk_button_mode = 1;

	lk_ls1 |= LK_I2C_LS_PWM0 << 4;
	lk_ls1 |= LK_I2C_LS_PWM1 << 6;
	push_queue(&lkUpdatePWM);
    }

    if (lk_button_mode == 1 && lk_ticks > 0xFF) {
	lk_button_mode = 0;
	lk_ls1 |= LK_I2C_LS_ON << 4;
	lk_ls1 |= LK_I2C_LS_ON << 6;
	push_queue(&lkUpdatePWM);
    }

    if (lk_button_mode == 1) {
	if (lk_ticks % 100 == 0) {
	    lk_piezo_toggle = lk_piezo_toggle ^ 1;
//	    gpioSetValue(LK_PIEZO, lk_piezo_toggle);	
	}
    }
    
}


void init_lilakit(void) {

    i2cInit(I2CMASTER); // Init I2C

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
//    gpioSetDir(LK_PIEZO, gpioDirection_Output);
//    gpioSetValue(LK_PIEZO, 1);

    // Enable both LEDs
    lk_ls1 |= LK_I2C_LS_ON << 4;
    lk_ls1 |= LK_I2C_LS_ON << 6;
    lkSetI2C(LK_I2C_CR_LS1, lk_ls1);

    // Prepare blinking
    lkSetI2C(LK_I2C_CR_PSC0, 0x23);
    lkSetI2C(LK_I2C_CR_PWM0, 0x66);
    lkSetI2C(LK_I2C_CR_PSC1, 0x75);
    lkSetI2C(LK_I2C_CR_PWM1, 0x12);
}
