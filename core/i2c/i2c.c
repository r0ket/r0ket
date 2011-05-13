/*****************************************************************************
 *   i2c.c:  I2C C file for NXP LPC11xx/13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   Parts of this code are (C) 2010, MyVoice CAD/CAM Services
 *   All rights reserved.
 *
 *   History
 *   2009.12.07  ver 1.00    Preliminary version, first Release
 *   2010.07.19  ver 1.10    Rob Jansen - MyVoice CAD/CAM Services:
 *                           Major cleaning and a rewrite of some functions
 *                           - adding ACK/NACK handling to the state machine
 *                           - adding a return result to the I2CEngine()
 *
*****************************************************************************/
#include "i2c.h"

volatile uint32_t I2CMasterState = I2CSTATE_IDLE;
volatile uint32_t I2CSlaveState = I2CSTATE_IDLE;

volatile uint8_t I2CMasterBuffer[I2C_BUFSIZE];
volatile uint8_t I2CSlaveBuffer[I2C_BUFSIZE];
volatile uint32_t I2CReadLength;
volatile uint32_t I2CWriteLength;

volatile uint32_t RdIndex = 0;
volatile uint32_t WrIndex = 0;


/*****************************************************************************
** Function name:		I2C_IRQHandler
**
** Descriptions:		I2C interrupt handler, deal with master mode only.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2C_IRQHandler(void) 
{
	uint8_t StatValue;

	/* this handler deals with master read and master write only */
	StatValue = I2C_I2CSTAT;
	switch ( StatValue )
	{
	case 0x08:
		/*
		 * A START condition has been transmitted.
		 * We now send the slave address and initialize
		 * the write buffer
		 * (we always start with a write after START+SLA)
		 */
		WrIndex = 0;
		I2C_I2CDAT = I2CMasterBuffer[WrIndex++];
		I2C_I2CCONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
		I2CMasterState = I2CSTATE_PENDING;
		break;
	
	case 0x10:
		/*
		 * A repeated START condition has been transmitted.
		 * Now a second, read, transaction follows so we
		 * initialize the read buffer.
		 */
		RdIndex = 0;
		/* Send SLA with R bit set, */
		I2C_I2CDAT = I2CMasterBuffer[WrIndex++];
		I2C_I2CCONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	break;
	
	case 0x18:
		/*
		 * SLA+W has been transmitted; ACK has been received.
		 * We now start writing bytes.
		 */
		I2C_I2CDAT = I2CMasterBuffer[WrIndex++];
		I2C_I2CCONCLR = I2CONCLR_SIC;
		break;

	case 0x20:
		/*
		 * SLA+W has been transmitted; NOT ACK has been received.
		 * Send a stop condition to terminate the transaction
		 * and signal I2CEngine the transaction is aborted.
		 */
		I2C_I2CCONSET = I2CONSET_STO;
		I2C_I2CCONCLR = I2CONCLR_SIC;
		I2CMasterState = I2CSTATE_SLA_NACK;
		break;

	case 0x28:
		/*
		 * Data in I2DAT has been transmitted; ACK has been received.
		 * Continue sending more bytes as long as there are bytes to send
		 * and after this check if a read transaction should follow.
		 */
		if ( WrIndex < I2CWriteLength )
		{
			/* Keep writing as long as bytes avail */
			I2C_I2CDAT = I2CMasterBuffer[WrIndex++];
		}
		else
		{
			if ( I2CReadLength != 0 )
			{
				/* Send a Repeated START to initialize a read transaction */
				/* (handled in state 0x10)                                */
				I2C_I2CCONSET = I2CONSET_STA;	/* Set Repeated-start flag */
			}
			else
			{
				I2CMasterState = I2CSTATE_ACK;
				I2C_I2CCONSET = I2CONSET_STO;      /* Set Stop flag */
			}
		}
		I2C_I2CCONCLR = I2CONCLR_SIC;
		break;

	case 0x30:
		/*
		 * Data byte in I2DAT has been transmitted; NOT ACK has been received
		 * Send a STOP condition to terminate the transaction and inform the
		 * I2CEngine that the transaction failed.
		 */
		I2C_I2CCONSET = I2CONSET_STO;
		I2C_I2CCONCLR = I2CONCLR_SIC;
		I2CMasterState = I2CSTATE_NACK;
		break;

	case 0x38:
		/*
		 * Arbitration loss in SLA+R/W or Data bytes.
		 * This is a fatal condition, the transaction did not complete due
		 * to external reasons (e.g. hardware system failure).
		 * Inform the I2CEngine of this and cancel the transaction
		 * (this is automatically done by the I2C hardware)
		 */
		I2CMasterState = I2CSTATE_ARB_LOSS;
		I2C_I2CCONCLR = I2CONCLR_SIC;
		break;

	case 0x40:
		/*
		 * SLA+R has been transmitted; ACK has been received.
		 * Initialize a read.
		 * Since a NOT ACK is sent after reading the last byte,
		 * we need to prepare a NOT ACK in case we only read 1 byte.
		 */
		if ( I2CReadLength == 1 )
		{
			/* last (and only) byte: send a NACK after data is received */
			I2C_I2CCONCLR = I2CONCLR_AAC;
		}
		else
		{
			/* more bytes to follow: send an ACK after data is received */
			I2C_I2CCONSET = I2CONSET_AA;
		}
		I2C_I2CCONCLR = I2CONCLR_SIC;
		break;

	case 0x48:
		/*
		 * SLA+R has been transmitted; NOT ACK has been received.
		 * Send a stop condition to terminate the transaction
		 * and signal I2CEngine the transaction is aborted.
		 */
		I2C_I2CCONSET = I2CONSET_STO;
		I2C_I2CCONCLR = I2CONCLR_SIC;
		I2CMasterState = I2CSTATE_SLA_NACK;
		break;

	case 0x50:
		/*
		 * Data byte has been received; ACK has been returned.
		 * Read the byte and check for more bytes to read.
		 * Send a NOT ACK after the last byte is received
		 */
		I2CSlaveBuffer[RdIndex++] = I2C_I2CDAT;
		if ( RdIndex < (I2CReadLength-1) )
		{
			/* lmore bytes to follow: send an ACK after data is received */
			I2C_I2CCONSET = I2CONSET_AA;
		}
		else
		{
			/* last byte: send a NACK after data is received */
			I2C_I2CCONCLR = I2CONCLR_AAC;
		}
		I2C_I2CCONCLR = I2CONCLR_SIC;
		break;
	
	case 0x58:
		/*
		 * Data byte has been received; NOT ACK has been returned.
		 * This is the last byte to read.
		 * Generate a STOP condition and flag the I2CEngine that the
		 * transaction is finished.
		 */
		I2CSlaveBuffer[RdIndex++] = I2C_I2CDAT;
		I2CMasterState = I2CSTATE_ACK;
		I2C_I2CCONSET = I2CONSET_STO;	/* Set Stop flag */
		I2C_I2CCONCLR = I2CONCLR_SIC;	/* Clear SI flag */
		break;

	
	default:
		I2C_I2CCONCLR = I2CONCLR_SIC;
	break;
  }
  return;
}

/*****************************************************************************
** Function name:	I2CStart
**
** Descriptions:	Create I2C start condition, a timeout
**					value is set if the I2C never gets started,
**					and timed out. It's a fatal error.
**
** parameters:		None
** Returned value:	true or false, return false if timed out
** 
*****************************************************************************/
static uint32_t I2CStart( void )
{
	uint32_t timeout = 0;

	/*--- Issue a start condition ---*/
	I2C_I2CCONSET = I2CONSET_STA;	/* Set Start flag */
    
	while((I2CMasterState != I2CSTATE_PENDING) && (timeout < MAX_TIMEOUT))
	{
		timeout++;
	}

	return (timeout < MAX_TIMEOUT);
}

/*****************************************************************************
** Function name:	I2CStop
**
** Descriptions:	Set the I2C stop condition
**
** parameters:		None
** Returned value:	true or never return
** 
*****************************************************************************/
static uint32_t I2CStop( void )
{
	uint32_t timeout = 0;

	I2C_I2CCONSET = I2CONSET_STO;      /* Set Stop flag */
	I2C_I2CCONCLR = I2CONCLR_SIC;  /* Clear SI flag */

	/*--- Wait for STOP detected ---*/
	while((I2C_I2CCONSET & I2CONSET_STO) && (timeout < MAX_TIMEOUT))
	{
		timeout++;
	}
	return (timeout >= MAX_TIMEOUT);
}

/*****************************************************************************
** Function name:	I2CInit
**
** Descriptions:	Initialize I2C controller
**
** parameters:		I2c mode is either MASTER or SLAVE
** Returned value:	true or false, return false if the I2C
**					interrupt handler was not installed correctly
** 
*****************************************************************************/
uint32_t i2cInit( uint32_t I2cMode ) 
{
	SCB_PRESETCTRL |= (0x1<<1);

  // Enable I2C clock
  SCB_SYSAHBCLKCTRL |= (SCB_SYSAHBCLKCTRL_I2C);

  // Configure pin 0.4 for SCL
  IOCON_PIO0_4 &= ~(IOCON_PIO0_4_FUNC_MASK | IOCON_PIO0_4_I2CMODE_MASK);
  IOCON_PIO0_4 |= (IOCON_PIO0_4_FUNC_I2CSCL);

  // Configure pin 0.5 for SDA
  IOCON_PIO0_5 &= ~(IOCON_PIO0_5_FUNC_MASK | IOCON_PIO0_5_I2CMODE_MASK);
  IOCON_PIO0_5 |= IOCON_PIO0_5_FUNC_I2CSDA;

  // Clear flags
  I2C_I2CCONCLR = I2C_I2CCONCLR_AAC | 
                  I2C_I2CCONCLR_SIC | 
                  I2C_I2CCONCLR_STAC | 
                  I2C_I2CCONCLR_I2ENC;

  // See p.128 for appropriate values for SCLL and SCLH
#if I2C_FAST_MODE_PLUS
  IOCON_PIO0_4 |= (IOCON_PIO0_4_I2CMODE_FASTPLUSI2C);
  IOCON_PIO0_5 |= (IOCON_PIO0_5_I2CMODE_FASTPLUSI2C);
  I2C_I2CSCLL   = I2C_SCLL_HS_SCLL;
  I2C_I2CSCLH   = I2C_SCLH_HS_SCLH;
#else
  I2C_I2CSCLL   = I2SCLL_SCLL;
  I2C_I2CSCLH   = I2SCLH_SCLH;
#endif

  if ( I2cMode == I2CSLAVE )
  {
    I2C_I2CADR0 = SLAVE_ADDR;
  }    

  /* Enable the I2C Interrupt */
  NVIC_EnableIRQ(I2C_IRQn);
  I2C_I2CCONSET = I2C_I2CCONSET_I2EN;

  return( TRUE );
}

/*****************************************************************************
** Function name:	I2CEngine
**
** Descriptions:	The routine to complete a I2C transaction
**					from start to stop. All the intermitten
**					steps are handled in the interrupt handler.
**					Before this routine is called, the read
**					length, write length and I2C master buffer
**					need to be filled.
**
** parameters:		None
** Returned value:	Any of the I2CSTATE_... values. See i2c.h
** 
*****************************************************************************/
uint32_t i2cEngine( void ) 
{
  I2CMasterState = I2CSTATE_IDLE;
  RdIndex = 0;
  WrIndex = 0;
  if ( I2CStart() != TRUE )
  {
	I2CStop();
	return ( FALSE );
  }

  /* wait until the state is a terminal state */
  while (I2CMasterState < 0x100);

  return ( I2CMasterState );
}

/******************************************************************************
**                            End Of File
******************************************************************************/

