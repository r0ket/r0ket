#include "basic.h"
#include "core/gpio/gpio.h"
#include "lcd/backlight.c"
#include "lcd/render.h"
#include "lcd/fonts/smallfonts.h"
#include "ecc.h"

void rbInit() {
    // TODO FIXME special port disable ? LEDs BTNs ?

    // prepare power
    // TODO FIXME more power init needed ? chrg + volt input ?
    // enable external vcc
    gpioSetDir(RB_PWR_GOOD, gpioDirection_Output);
    gpioSetValue (RB_PWR_GOOD, 0); 

    // Disable USB Connect (we don't want USB by default)
    gpioSetDir(USB_CONNECT, gpioDirection_Output);
    gpioSetValue(USB_CONNECT, 1);

    // prepare buttons
    gpioSetDir(RB_BTN0, gpioDirection_Input);
    gpioSetPullup (&RB_BTN0_IO, gpioPullupMode_PullUp);

    gpioSetDir(RB_BTN1, gpioDirection_Input);
    gpioSetPullup (&RB_BTN1_IO, gpioPullupMode_PullUp);

    gpioSetDir(RB_BTN2, gpioDirection_Input);
    gpioSetPullup (&RB_BTN2_IO, gpioPullupMode_PullUp);

    gpioSetDir(RB_BTN3, gpioDirection_Input);
    gpioSetPullup (&RB_BTN3_IO, gpioPullupMode_PullUp);

    gpioSetDir(RB_BTN4, gpioDirection_Input);
    gpioSetPullup (&RB_BTN4_IO, gpioPullupMode_PullUp);

    // prepate chrg_stat
    gpioSetDir(RB_PWR_CHRG, gpioDirection_Input);
    gpioSetPullup (&RB_PWR_CHRG_IO, gpioPullupMode_PullUp);

    // prepare LEDs
    IOCON_JTAG_TDI_PIO0_11 &= ~IOCON_JTAG_TDI_PIO0_11_FUNC_MASK;
    IOCON_JTAG_TDI_PIO0_11 |= IOCON_JTAG_TDI_PIO0_11_FUNC_GPIO; 
 
    gpioSetDir(RB_LED0, gpioDirection_Output);
    gpioSetValue (RB_LED0, 0); 

    gpioSetDir(RB_LED1, gpioDirection_Output);
    gpioSetValue (RB_LED1, 0); 

    gpioSetDir(RB_LED2, gpioDirection_Output);
    gpioSetValue (RB_LED2, 0); 

    gpioSetDir(RB_LED3, gpioDirection_Output);
    gpioSetValue (RB_LED3, 0); 

    // Set LED3 to ?
    IOCON_PIO1_11 = 0x41;

    // prepare lcd
    // TODO FIXME more init needed ?
    gpioSetDir(RB_LCD_BL, gpioDirection_Output);
    gpioSetValue (RB_LCD_BL, 0); 

    // Set P0.0 to GPIO
    RB_PWR_LCDBL_IO&= ~RB_PWR_LCDBL_IO_FUNC_MASK;
    RB_PWR_LCDBL_IO|= RB_PWR_LCDBL_IO_FUNC_GPIO; 
    gpioSetDir(RB_PWR_LCDBL, gpioDirection_Input);
    gpioSetPullup(&RB_PWR_LCDBL_IO, gpioPullupMode_Inactive);

    // prepare I2C
    #ifdef __I2C_h
	i2cInit(I2CMASTER); // TODO FIXME via define ?
    #endif


    // prepare SPI/SS
    // TODO FIXME init miso/mosi/sck somehow ?
    gpioSetDir(RB_SPI_SS0, gpioDirection_Output);
    gpioSetValue (RB_SPI_SS0, 1); 

    gpioSetDir(RB_SPI_SS1, gpioDirection_Output);
    gpioSetValue (RB_SPI_SS1, 1); 

    gpioSetDir(RB_SPI_SS2, gpioDirection_Output);
    gpioSetValue (RB_SPI_SS2, 1); 

    gpioSetDir(RB_SPI_SS3, gpioDirection_Output);
    gpioSetValue (RB_SPI_SS3, 1); 

    gpioSetDir(RB_SPI_SS4, gpioDirection_Output);
    gpioSetValue (RB_SPI_SS4, 1); 

    gpioSetDir(RB_SPI_SS5, gpioDirection_Output);
    gpioSetValue (RB_SPI_SS5, 1); 


    // prepare hackerbus
    gpioSetDir(RB_HB0, gpioDirection_Output);
    gpioSetValue (RB_HB0, 1); 

    gpioSetDir(RB_HB1, gpioDirection_Output);
    gpioSetValue (RB_HB1, 1); 

    gpioSetDir(RB_HB2, gpioDirection_Output);
    gpioSetValue (RB_HB2, 1); 

    gpioSetDir(RB_HB3, gpioDirection_Output);
    gpioSetValue (RB_HB3, 1); 

    gpioSetDir(RB_HB4, gpioDirection_Output);
    gpioSetValue (RB_HB4, 1); 

    gpioSetDir(RB_HB5, gpioDirection_Output);
    gpioSetValue (RB_HB5, 1); 


    // prepare BUSINT interrupt
    gpioSetDir(RB_BUSINT, gpioDirection_Input);
    gpioSetPullup (&RB_BUSINT_IO, gpioPullupMode_PullUp);
    gpioSetInterrupt(RB_BUSINT, gpioInterruptSense_Edge, gpioInterruptEdge_Single, gpioInterruptEvent_ActiveLow);
    gpioIntEnable(RB_BUSINT);

    //nrf_init();
    backlightInit();
    font=&Font_7x8;
    ECIES_setup();
}

#define WEAK_ALIAS(f) __attribute__ ((weak, alias (#f)));
void interrupt_undefined(void) {
}

void businterrupt(void)       WEAK_ALIAS(interrupt_undefined);


void PIOINT3_IRQHandler(void) {    
    if (gpioIntStatus(RB_BUSINT)) {
        gpioIntClear(RB_BUSINT);
        businterrupt();
    }
}

