#ifndef __BASIC_H_
#define __BASIC_H_

#include "core/gpio/gpio.h"
#include "core/adc/adc.h"

// LED
#define RB_LED0_PORT		0
#define RB_LED0_PIN		11

#define RB_LED1_PORT		1
#define RB_LED1_PIN		7

#define RB_LED2_PORT		1
#define RB_LED2_PIN		6

#define RB_LED3_PORT		1
#define RB_LED3_PIN		11


// Infrared
#define RB_IROUT_PORT		1
#define RB_IROUT_PIN		5

#define RB_IRIN_PORT		1
#define RB_IRIN_PIN		8
#define RB_IRIN_IO		IOCON_PIO1_8


// Buttons
#define RB_BTN0_PORT		0
#define RB_BTN0_PIN		1
#define RB_BTN0_IO		IOCON_PIO0_1

#define RB_BTN1_PORT		2
#define RB_BTN1_PIN		9
#define RB_BTN1_IO		IOCON_PIO2_9

#define RB_BTN2_PORT		2
#define RB_BTN2_PIN		6
#define RB_BTN2_IO		IOCON_PIO2_6

#define RB_BTN3_PORT		3
#define RB_BTN3_PIN		3
#define RB_BTN3_IO		IOCON_PIO3_3

#define RB_BTN4_PORT		1
#define RB_BTN4_PIN		4
#define RB_BTN4_IO		IOCON_PIO1_4


// LCD
#define RB_LCD_BL_PORT		1
#define RB_LCD_BL_PIN		10

#define RB_LCD_A0_PORT		2
#define RB_LCD_A0_PIN		0

#define RB_LCD_CS_PORT		2
#define RB_LCD_CS_PIN		1

#define RB_LCD_RST_PORT		2
#define RB_LCD_RST_PIN		2


// I2C
#define RB_I2C_SCL_PORT		0
#define RB_I2C_SCL_PIN		4

#define RB_I2C_SCA_PORT		0
#define RB_I2C_SCA_PIN		5


// SPI
#define RB_SPI_MISO_PORT	0
#define RB_SPI_MISO_PIN		8

#define RB_SPI_MOSI_PORT	0
#define RB_SPI_MOSI_PIN		9

#define RB_SPI_SCK_PORT		0
#define RB_SPI_SCK_PIN		10

#define RB_SPI_SS0_PORT		2
#define RB_SPI_SS0_PIN		5

#define RB_SPI_SS1_PORT		2
#define RB_SPI_SS1_PIN		4

#define RB_SPI_SS2_PORT		2
#define RB_SPI_SS2_PIN		8

#define RB_SPI_SS3_PORT		3
#define RB_SPI_SS3_PIN		2

#define RB_SPI_SS4_PORT		3
#define RB_SPI_SS4_PIN		1

#define RB_SPI_SS5_PORT		2
#define RB_SPI_SS5_PIN		11


// Power
#define RB_PWR_VOLT_PORT	1
#define RB_PWR_VOLT_PIN		0

#define RB_PWR_GOOD_PORT	1
#define RB_PWR_GOOD_PIN		9

#define RB_PWR_CHRG_PORT	2
#define RB_PWR_CHRG_PIN		3


// Hackerbus
#define RB_HB0_PORT		1
#define RB_HB0_PIN		3
#define RB_HB0_IO		IOCON_PIO1_3

#define RB_HB1_PORT		1
#define RB_HB1_PIN		2
#define RB_HB1_IO		IOCON_PIO1_2

#define RB_HB2_PORT		1
#define RB_HB2_PIN		1
#define RB_HB2_IO		IOCON_PIO1_1

#define RB_HB3_PORT		0
#define RB_HB3_PIN		2
#define RB_HB3_IO		IOCON_PIO0_2

#define RB_HB4_PORT		2
#define RB_HB4_PIN		7
#define RB_HB4_IO		IOCON_PIO2_7

#define RB_HB5_PORT		0
#define RB_HB5_PIN		6
#define RB_HB5_IO		IOCON_PIO0_6


// Misc
#define RB_BUSINT_PORT		3
#define RB_BUSINT_PIN		0
#define RB_BUSINT_IO		IOCON_PIO3_0

#define RB_USB_VBUS_PORT	0
#define RB_USB_VBUS_PIN		3

#define RB_EEPROM_ADDR 		0xA0

void rbInit(void);

#endif
