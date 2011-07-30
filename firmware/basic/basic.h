#ifndef __BASIC_H_
#define __BASIC_H_

#include <time.h>
#include "core/gpio/gpio.h"
#include "core/adc/adc.h"

// LED
#define RB_LED0			0,11

#define RB_LED1			1,7

#define RB_LED2			1,6

#define RB_LED3			1,11


// Infrared
//#define RB_IROUT		1,5

//#define RB_IRIN			1,8
//#define RB_IRIN_IO		IOCON_PIO1_8


// Buttons
#define RB_BTN0			0,1
#define RB_BTN0_IO		IOCON_PIO0_1

#define RB_BTN1			2,9
#define RB_BTN1_IO		IOCON_PIO2_9

#define RB_BTN2			2,6
#define RB_BTN2_IO		IOCON_PIO2_6

#define RB_BTN3			3,3
#define RB_BTN3_IO		IOCON_PIO3_3

#define RB_BTN4			2,7
#define RB_BTN4_IO		IOCON_PIO2_7


// LCD
#define RB_LCD_BL		1,9

#define RB_LCD_CS		2,1

#define RB_LCD_RST		2,2


// I2C
#define RB_I2C_SCL		0,4

#define RB_I2C_SCA		0,5


// SPI
#define RB_SPI_MISO		0,8

#define RB_SPI_MOSI		0,9

#define RB_SPI_SCK		2,11

#define RB_SPI_CS_DF	2,0

#define RB_SPI_SS0		2,5

#define RB_SPI_SS1		2,4

#define RB_SPI_SS2		2,8

#define RB_SPI_SS3		3,2

#define RB_SPI_SS4		3,1

#define RB_SPI_SS5		2,10


// Power
#define RB_PWR_VOLT		1,0

#define RB_PWR_GOOD		1,8

#define RB_PWR_CHRG		2,3

#define RB_PWR_LCDBL    0,0
#define RB_PWR_LCDBL_IO IOCON_nRESET_PIO0_0
#define RB_PWR_LCDBL_IO_FUNC_MASK IOCON_nRESET_PIO0_0_FUNC_MASK
#define RB_PWR_LCDBL_IO_FUNC_GPIO IOCON_nRESET_PIO0_0_FUNC_GPIO

// Hackerbus
#define RB_HB0			1,3
#define RB_HB0_IO		IOCON_PIO1_3

#define RB_HB1			0,10
#define RB_HB1_IO		IOCON_PIO0_10

#define RB_HB2			1,1
#define RB_HB2_IO		IOCON_PIO1_1

#define RB_HB3			0,2
#define RB_HB3_IO		IOCON_PIO0_2

#define RB_HB4			1,4
#define RB_HB4_IO		IOCON_PIO1_4

#define RB_HB5			1,2
#define RB_HB5_IO		IOCON_PIO1_2

// Funk
#define RB_NRF_CE		1,5
#define RB_NRF_CE_IO		IOCON_PIO1_5
#define RB_SPI_NRF_CS		1,10
#define RB_SPI_NRF_CS_IO	IOCON_PIO1_10

// Misc
#define RB_BUSINT		3,0
#define RB_BUSINT_IO		IOCON_PIO3_0

#define RB_USB_VBUS		0,3

#define RB_EEPROM_ADDR 		0xA0

#define USB_CONNECT 0,6

void rbInit(void);

// reinvoke_isp.c
void ReinvokeISP(void);
void EnableWatchdog(uint32_t ms);
void ISPandReset(void);

// delayms.c
void delayms(uint32_t ms);

// voltage.c
void VoltageCheck(void);
uint32_t GetVoltage(void);

// night.c
void LightCheck(void);
uint32_t GetLight(void);
char isNight(void);

// keyin.c
#define BTN_NONE 0
#define BTN_UP   (1<<0)
#define BTN_DOWN (1<<1)
#define BTN_LEFT (1<<2)
#define BTN_RIGHT (1<<3)
#define BTN_ENTER (1<<4)
uint8_t getInput(void);
uint8_t getInputRaw(void);
uint8_t getInputWait(void);
void getInputWaitRelease(void);

// stringin.c
void input(char prompt[], char line[], uint8_t asciistart, uint8_t asciiend, uint8_t maxlength);
//uuid.c
#include "basic/uuid.h"

// for core/iap/iap.c (no official definition)
void iap_entry(uint32_t param_tab[], uint32_t result_tab[]);

// crc.c
uint16_t crc16(uint8_t * buf, int len);

// menu.c

struct MENU_DEF {
    char *text;
    void (*callback)(void);
};

typedef const struct MENU_DEF * menuentry;

struct MENU {
    char *title;
    menuentry *entries;
};


void handleMenu(const struct MENU *the_menu);

// idle.c

#include "basic/idle.h"

// config.c

struct config_t {
	time_t     time;
	uint16_t   backlighttrigger;
	char       backlightvalue; 
	char       lcdstate;
	char       privacy;
} __attribute__((__packed__));

typedef struct config_t CONFIG;

extern CONFIG globalconfig;

int readConfig(void);
int saveConfig(void);
int applyConfig(void);

#define SYSTICKSPEED 10

// itoa.c
#define F_ZEROS  (1<<0)
#define F_LONG   (1<<1)
#define F_SPLUS  (1<<2)
#define F_SSPACE (1<<3)
const char* IntToStrX(unsigned int num, unsigned int mxlen);
const char* IntToStr(int num, unsigned int mxlen, char flag);

// simpletime.c

#include "basic/simpletime.h"

#endif
