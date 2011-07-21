
// dummy implementation instead of #include "../../../../firmware/core/i2c/i2c.c"


#include "i2c.h"

volatile uint32_t I2CMasterState = I2CSTATE_IDLE;
volatile uint32_t I2CSlaveState = I2CSTATE_IDLE;

volatile uint8_t I2CMasterBuffer[I2C_BUFSIZE];
volatile uint8_t I2CSlaveBuffer[I2C_BUFSIZE];
volatile uint32_t I2CReadLength;
volatile uint32_t I2CWriteLength;

volatile uint32_t RdIndex = 0;
volatile uint32_t WrIndex = 0;


void I2C_IRQHandler(void) {
}

uint32_t i2cInit( uint32_t I2cMode ) {
  return( TRUE );
}

uint32_t i2cEngine( void ) {
  return I2CSTATE_IDLE;
}

/******************************************************************************
**                            End Of File
******************************************************************************/


