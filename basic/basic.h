#ifndef __BASIC_H_
#define __BASIC_H_

#include "core/gpio/gpio.h"
#include "core/adc/adc.h"

// LED
#define RB_LED0			0,11

#define RB_LED1			1,7

#define RB_LED2			1,6

#define RB_LED3			1,11


// Infrared
#define RB_IROUT		1,5

#define RB_IRIN			1,8
#define RB_IRIN_IO		IOCON_PIO1_8


// Buttons
#define RB_BTN0			0,1
#define RB_BTN0_IO		IOCON_PIO0_1

#define RB_BTN1			2,9
#define RB_BTN1_IO		IOCON_PIO2_9

#define RB_BTN2			2,6
#define RB_BTN2_IO		IOCON_PIO2_6

#define RB_BTN3			3,3
#define RB_BTN3_IO		IOCON_PIO3_3

#define RB_BTN4			1,4
#define RB_BTN4_IO		IOCON_PIO1_4


// LCD
#define RB_LCD_BL		1,10

#define RB_LCD_A0		2,0

#define RB_LCD_CS		2,1

#define RB_LCD_RST		2,2


// I2C
#define RB_I2C_SCL		0,4

#define RB_I2C_SCA		0,5


// SPI
#define RB_SPI_MISO		0,8

#define RB_SPI_MOSI		0,9

#define RB_SPI_SCK		0,10

#define RB_SPI_SS0		2,5

#define RB_SPI_SS1		2,4

#define RB_SPI_SS2		2,8

#define RB_SPI_SS3		3,2

#define RB_SPI_SS4		3,1

#define RB_SPI_SS5		2,11


// Power
#define RB_PWR_VOLT		1,0

#define RB_PWR_GOOD		1,9

#define RB_PWR_CHRG		2,3


// Hackerbus
#define RB_HB0			1,3
#define RB_HB0_IO		IOCON_PIO1_3

#define RB_HB1			1,2
#define RB_HB1_IO		IOCON_PIO1_2

#define RB_HB2			1,1
#define RB_HB2_IO		IOCON_PIO1_1

#define RB_HB3			0,2
#define RB_HB3_IO		IOCON_PIO0_2

#define RB_HB4			2,7
#define RB_HB4_IO		IOCON_PIO2_7

#define RB_HB5			0,6
#define RB_HB5_IO		IOCON_PIO0_6


// Misc
#define RB_BUSINT		3,0
#define RB_BUSINT_IO		IOCON_PIO3_0

#define RB_USB_VBUS		0,3

#define RB_EEPROM_ADDR 		0xA0

void rbInit(void);

#endif
