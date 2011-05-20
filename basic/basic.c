#include "basic.h"
#include "core/gpio/gpio.h"

void rbInit() {

    // TODO FIXME special port disable ? LEDs BTNs ?

    // prepare power
    // TODO FIXME more power init needed ? chrg + volt input ?
    // enable external vcc
    gpioSetDir(RB_PWR_GOOD_PORT, RB_PWR_GOOD_PIN, gpioDirection_Output);
    gpioSetValue (RB_PWR_GOOD_PORT, RB_PWR_GOOD_PIN, 0); 


    // prepare buttons
    gpioSetDir(RB_BTN0_PORT, RB_BTN0_PIN, gpioDirection_Input);
    gpioSetPullup (&RB_BTN0_IO, gpioPullupMode_PullUp);

    gpioSetDir(RB_BTN1_PORT, RB_BTN1_PIN, gpioDirection_Input);
    gpioSetPullup (&RB_BTN1_IO, gpioPullupMode_PullUp);

    gpioSetDir(RB_BTN2_PORT, RB_BTN2_PIN, gpioDirection_Input);
    gpioSetPullup (&RB_BTN2_IO, gpioPullupMode_PullUp);

    gpioSetDir(RB_BTN3_PORT, RB_BTN3_PIN, gpioDirection_Input);
    gpioSetPullup (&RB_BTN3_IO, gpioPullupMode_PullUp);

    gpioSetDir(RB_BTN4_PORT, RB_BTN4_PIN, gpioDirection_Input);
    gpioSetPullup (&RB_BTN4_IO, gpioPullupMode_PullUp);


    // prepare LEDs
    gpioSetDir(RB_LED0_PORT, RB_LED0_PIN, gpioDirection_Output);
    gpioSetValue (RB_LED0_PORT, RB_LED0_PIN, 1); 

    gpioSetDir(RB_LED1_PORT, RB_BTN1_PIN, gpioDirection_Output);
    gpioSetValue (RB_LED1_PORT, RB_LED1_PIN, 1); 

    gpioSetDir(RB_LED2_PORT, RB_BTN2_PIN, gpioDirection_Output);
    gpioSetValue (RB_LED2_PORT, RB_LED2_PIN, 1); 

    gpioSetDir(RB_LED3_PORT, RB_LED3_PIN, gpioDirection_Output);
    gpioSetValue (RB_LED3_PORT, RB_LED3_PIN, 1); 


    // prepare IR
    gpioSetDir(RB_IROUT_PORT, RB_IROUT_PIN, gpioDirection_Output);
    gpioSetValue (RB_IROUT_PORT, RB_IROUT_PIN, 1); 

    gpioSetDir(RB_IRIN_PORT, RB_IRIN_PIN, gpioDirection_Input);
    gpioSetPullup (&RB_IRIN_IO, gpioPullupMode_PullUp);


    // prepare lcd
    // TODO FIXME more init needed ?
    gpioSetDir(RB_LCD_BL_PORT, RB_LCD_BL_PIN, gpioDirection_Output);
    gpioSetValue (RB_LCD_BL_PORT, RB_LCD_BL_PIN, 1); 


    // prepare I2C
    #ifdef __I2C_h
	i2cInit(I2CMASTER); // TODO FIXME via define ?
    #endif


    // prepare SPI/SS
    // TODO FIXME init miso/mosi/sck somehow ?
    gpioSetDir(RB_SPI_SS0_PORT, RB_SPI_SS0_PIN, gpioDirection_Output);
    gpioSetValue (RB_SPI_SS0_PORT, RB_SPI_SS0_PIN, 1); 

    gpioSetDir(RB_SPI_SS1_PORT, RB_SPI_SS1_PIN, gpioDirection_Output);
    gpioSetValue (RB_SPI_SS1_PORT, RB_SPI_SS1_PIN, 1); 

    gpioSetDir(RB_SPI_SS2_PORT, RB_SPI_SS2_PIN, gpioDirection_Output);
    gpioSetValue (RB_SPI_SS2_PORT, RB_SPI_SS2_PIN, 1); 

    gpioSetDir(RB_SPI_SS3_PORT, RB_SPI_SS3_PIN, gpioDirection_Output);
    gpioSetValue (RB_SPI_SS3_PORT, RB_SPI_SS3_PIN, 1); 

    gpioSetDir(RB_SPI_SS4_PORT, RB_SPI_SS4_PIN, gpioDirection_Output);
    gpioSetValue (RB_SPI_SS4_PORT, RB_SPI_SS4_PIN, 1); 

    gpioSetDir(RB_SPI_SS5_PORT, RB_SPI_SS5_PIN, gpioDirection_Output);
    gpioSetValue (RB_SPI_SS5_PORT, RB_SPI_SS5_PIN, 1); 


    // prepare hackerbus
    gpioSetDir(RB_HB0_PORT, RB_HB0_PIN, gpioDirection_Output);
    gpioSetValue (RB_HB0_PORT, RB_HB0_PIN, 1); 

    gpioSetDir(RB_HB1_PORT, RB_HB1_PIN, gpioDirection_Output);
    gpioSetValue (RB_HB1_PORT, RB_HB1_PIN, 1); 

    gpioSetDir(RB_HB2_PORT, RB_HB2_PIN, gpioDirection_Output);
    gpioSetValue (RB_HB2_PORT, RB_HB2_PIN, 1); 

    gpioSetDir(RB_HB3_PORT, RB_HB3_PIN, gpioDirection_Output);
    gpioSetValue (RB_HB3_PORT, RB_HB3_PIN, 1); 

    gpioSetDir(RB_HB4_PORT, RB_HB4_PIN, gpioDirection_Output);
    gpioSetValue (RB_HB4_PORT, RB_HB4_PIN, 1); 

    gpioSetDir(RB_HB5_PORT, RB_HB5_PIN, gpioDirection_Output);
    gpioSetValue (RB_HB5_PORT, RB_HB5_PIN, 1); 


    // prepare BUSINT interrupt
    gpioSetDir(RB_BUSINT_PORT, RB_BUSINT_PIN, gpioDirection_Input);
    gpioSetPullup (&RB_BUSINT_IO, gpioPullupMode_PullUp);
    gpioSetInterrupt(RB_BUSINT_PORT, RB_BUSINT_PIN, gpioInterruptSense_Edge, gpioInterruptEdge_Single, gpioInterruptEvent_ActiveLow);
    gpioIntEnable(RB_BUSINT_PORT, RB_BUSINT_PIN);
    //add this to catch interrupt:
    /*
	void PIOINT3_IRQHandler(void) {    
	    if (gpioIntStatus(RB_BUSINT_PORT, RB_BUSINT_PIN)) {
		// do something
		gpioIntClear(RB_BUSINT_PORT, RB_BUSINT_PIN);
	    }
	}
    */
}
