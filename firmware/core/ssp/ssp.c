/**************************************************************************/
/*! 
    @file     ssp.c
    @author   K. Townsend (microBuilder.eu)
    @date     22 March 2010
    @version  0.10

    @section DESCRIPTION

    Generic code for SSP/SPI communications.  By default, the SSP block
    is initialised in SPI master mode.

    @section Example

    @code
    #include "core/cpu/cpu.h"
    #include "core/ssp/ssp.h"
    ...
    cpuInit();
    sspInit(0, sspClockPolarity_High, sspClockPhase_RisingEdge);
    ...
    uint8_t request[SSP_FIFOSIZE];
    uint8_t response[SSP_FIFOSIZE];
  
    // Send 0x9C to the slave device and wait for a response
    request[0] = 0x80 | 0x1C;
    // Toggle the select pin
    ssp0Select();
    // Send 1 byte from the request buffer
    sspSend(0, (uint8_t *)&request, 1);
    // Receive 1 byte into the response buffer
    sspReceive(0, (uint8_t *)&response, 1);
    // Reset the select pin
    ssp0Deselect();
    // Print the results
    debug_printf("Ox%x ", response[0]);
    @endcode
	
    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2010, microBuilder SARL
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#include "ssp.h"
#include "core/gpio/gpio.h"

/* Statistics for all interrupts */
volatile uint32_t interruptRxStat = 0;
volatile uint32_t interruptOverRunStat = 0;
volatile uint32_t interruptRxTimeoutStat = 0;

/**************************************************************************/
/*! 
    @brief SSP0 interrupt handler for SPI communication

    The algorithm is, if RXFIFO is at least half full, 
    start receive until it's empty; if TXFIFO is at least
    half empty, start transmit until it's full.
    This will maximize the use of both FIFOs and performance.
*/
/**************************************************************************/
void SSP_IRQHandler (void)
{
  uint32_t regValue;

  regValue = SSP_SSP0MIS;

  /* Check for overrun interrupt */
  if ( regValue & SSP_SSP0MIS_RORMIS_FRMRCVD )
  {
    interruptOverRunStat++;
    SSP_SSP0ICR = SSP_SSP0ICR_RORIC_CLEAR;      // Clear interrupt
  }

  /* Check for timeout interrupt */
  if ( regValue & SSP_SSP0MIS_RTMIS_NOTEMPTY )
  {
    interruptRxTimeoutStat++;
    SSP_SSP0ICR = SSP_SSP0ICR_RTIC_CLEAR;       // Clear interrupt
  }

  /* Check if Rx buffer is at least half-full */
  if ( regValue & SSP_SSP0MIS_RXMIS_HALFFULL )
  {
    // ToDo: Receive until it's empty
    interruptRxStat++;
  }
  return;
}

/**************************************************************************/
/*! 
    @brief Initialises the SSP0 port

    By default, SSP0 is set to SPI frame-format with 8-bit data.  Pin 2.11
    is routed to serve as serial clock (SCK), and SSEL (0.2) is set to
    GPIO to allow manual control of when the SPI port is enabled or
    disabled.  Overrun and timeout interrupts are both enabled.

    @param[in]  portNum
                The SPI port to use (0..1)
    @param[in]  polarity
                Indicates whether the clock should be held high
                (sspClockPolarity_High) or low (sspClockPolarity_Low)
                when inactive.
    @param[in]  phase
                Indicates whether new bits start on the leading
                (sspClockPhase_RisingEdge) or falling
                (sspClockPhase_FallingEdge) edge of clock transitions.

    @note   sspSelect() and sspDeselect() macros have been defined in 
            ssp.h to control the SSEL line without having to know the 
            specific pin being used.
*/
/**************************************************************************/
void sspInit (uint8_t portNum, sspClockPolarity_t polarity, sspClockPhase_t phase)
{
  gpioInit();

  if (portNum == 0)
  {
    /* Reset SSP */
    SCB_PRESETCTRL &= ~SCB_PRESETCTRL_SSP0_MASK;
    SCB_PRESETCTRL |= SCB_PRESETCTRL_SSP0_RESETDISABLED;
  
    /* Enable AHB clock to the SSP domain. */
    SCB_SYSAHBCLKCTRL |= (SCB_SYSAHBCLKCTRL_SSP0);
  
    /* Divide by 1 (SSPCLKDIV also enables to SSP CLK) */
    SCB_SSP0CLKDIV = SCB_SSP0CLKDIV_DIV1;
  
    /* Set P0.8 to SSP MISO */
    IOCON_PIO0_8 &= ~IOCON_PIO0_8_FUNC_MASK;
    IOCON_PIO0_8 |= IOCON_PIO0_8_FUNC_MISO0;
  
    /* Set P0.9 to SSP MOSI */
    IOCON_PIO0_9 &= ~IOCON_PIO0_9_FUNC_MASK;
    IOCON_PIO0_9 |= IOCON_PIO0_9_FUNC_MOSI0;

    /* Set 2.11 to SSP SCK (0.6 and 0.10 can also be used) */ 
    #ifdef CFG_SSP0_SCKPIN_2_11
    IOCON_SCKLOC = IOCON_SCKLOC_SCKPIN_PIO2_11; 
    IOCON_PIO2_11 = IOCON_PIO2_11_FUNC_SCK0;  
    #endif

    /* Set 0.6 to SSP SCK (2.11 and 0.10 can also be used) */ 
    #ifdef CFG_SSP0_SCKPIN_0_6
    IOCON_SCKLOC = IOCON_SCKLOC_SCKPIN_PIO0_6; 
    IOCON_PIO0_6 = IOCON_PIO0_6_FUNC_SCK;
    #endif

    /* Set P0.2/SSEL to GPIO output and high */
    IOCON_PIO0_2 &= ~IOCON_PIO0_2_FUNC_MASK;
    IOCON_PIO0_2 |= IOCON_PIO0_2_FUNC_GPIO;
    gpioSetDir(SSP0_CSPORT, SSP0_CSPIN, 1);
    gpioSetValue(SSP0_CSPORT, SSP0_CSPIN, 1);
    gpioSetPullup(&IOCON_PIO0_2, gpioPullupMode_Inactive);  // Board has external pull-up
  
    /* If SSP0CLKDIV = DIV1 -- (PCLK / (CPSDVSR × [SCR+1])) = (72,000,000 / (2 x [8 + 1])) = 4.0 MHz */
    uint32_t configReg = ( SSP_SSP0CR0_DSS_8BIT   // Data size = 8-bit
                  | SSP_SSP0CR0_FRF_SPI           // Frame format = SPI
                  | SSP_SSP0CR0_SCR_8);           // Serial clock rate = 8
  
    // Set clock polarity
    if (polarity == sspClockPolarity_High)
      configReg |= SSP_SSP0CR0_CPOL_HIGH;     // Clock polarity = High between frames
    else
      configReg &= ~SSP_SSP0CR0_CPOL_MASK;    // Clock polarity = Low between frames
  
    // Set edge transition
    if (phase == sspClockPhase_FallingEdge)
      configReg |= SSP_SSP0CR0_CPHA_SECOND;   // Clock out phase = Trailing edge clock transition
    else
      configReg &= ~SSP_SSP0CR0_CPHA_MASK;    // Clock out phase = Leading edge clock transition
  
    // Assign config values to SSP0CR0
    SSP_SSP0CR0 = configReg;
  
    /* Clock prescale register must be even and at least 2 in master mode */
    SSP_SSP0CPSR = SSP_SSP0CPSR_CPSDVSR_DIV2;
  
    /* Clear the Rx FIFO */
    uint8_t i, Dummy=Dummy;
    for ( i = 0; i < SSP_FIFOSIZE; i++ )
    {
      Dummy = SSP_SSP0DR;
    }
  
    /* Enable the SSP Interrupt */
    NVIC_EnableIRQ(SSP_IRQn);
  
    /* Set SSPINMS registers to enable interrupts
     * enable all error related interrupts        */
    SSP_SSP0IMSC = ( SSP_SSP0IMSC_RORIM_ENBL      // Enable overrun interrupt
                   | SSP_SSP0IMSC_RTIM_ENBL);     // Enable timeout interrupt
  
    /* Enable device and set it to master mode, no loopback */
    SSP_SSP0CR1 = SSP_SSP0CR1_SSE_ENABLED | SSP_SSP0CR1_MS_MASTER | SSP_SSP0CR1_LBM_NORMAL;
  }

  return;
}

/**************************************************************************/
/*! 
    @brief Sends a block of data to the SSP0 port

    @param[in]  portNum
                The SPI port to use (0..1)
    @param[in]  buf
                Pointer to the data buffer
    @param[in]  length
                Block length of the data buffer
*/
/**************************************************************************/
void sspSend (uint8_t portNum, uint8_t *buf, uint32_t length)
{
  uint32_t i;
  uint8_t Dummy = Dummy;

  if (portNum == 0)
  {
    for (i = 0; i < length; i++)
    {
      /* Move on only if NOT busy and TX FIFO not full. */
      while ((SSP_SSP0SR & (SSP_SSP0SR_TNF_NOTFULL | SSP_SSP0SR_BSY_BUSY)) != SSP_SSP0SR_TNF_NOTFULL);
      SSP_SSP0DR = *buf;
      buf++;
  
      while ( (SSP_SSP0SR & (SSP_SSP0SR_BSY_BUSY|SSP_SSP0SR_RNE_NOTEMPTY)) != SSP_SSP0SR_RNE_NOTEMPTY );
      /* Whenever a byte is written, MISO FIFO counter increments, Clear FIFO 
      on MISO. Otherwise, when SSP0Receive() is called, previous data byte
      is left in the FIFO. */
      Dummy = SSP_SSP0DR;
    }
  }

  return; 
}

/**************************************************************************/
/*! 
    @brief Receives a block of data from the SSP0 port

    @param[in]  portNum
                The SPI port to use (0..1)
    @param[in]  buf
                Pointer to the data buffer
    @param[in]  length
                Block length of the data buffer
*/
/**************************************************************************/
void sspReceive(uint8_t portNum, uint8_t *buf, uint32_t length)
{
  uint32_t i;

  if (portNum == 0)
  {
    for ( i = 0; i < length; i++ )
    {
      /* As long as the receive FIFO is not empty, data can be received. */
      SSP_SSP0DR = 0xFF;
  
      /* Wait until the Busy bit is cleared */
      while ( (SSP_SSP0SR & (SSP_SSP0SR_BSY_BUSY|SSP_SSP0SR_RNE_NOTEMPTY)) != SSP_SSP0SR_RNE_NOTEMPTY );
      
      *buf = SSP_SSP0DR;
      buf++;
    }
  }

  return; 
}

