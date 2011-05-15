/**************************************************************************/
/*! 
    @file     adc.c
    @author   K. Townsend (microBuilder.eu)
    @date     22 March 2010
    @version  0.10
	
    @section Description
	
    SW-based single-channel A/D conversion.  If you wish to convert
    multiple ADC channels simultaneously, this code will need to be
    modified to work in BURST mode.

    @section Example

    @code
    #include "core/cpu/cpu.h"
    #include "core/adc/adc.h"

    void main (void)
    {
      cpuInit();
      adcInit();

      uint32_t results = 0;  
      while(1)
      {
        // Get A/D conversion results from A/D channel 0
        results = adcRead(0);
      }
    }
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

#include "adc.h"

static bool _adcInitialised = false;
static uint8_t _adcLastChannel = 0;

/**************************************************************************/
/*! 
    @brief Returns the conversion results on the specified ADC channel.

    This function will manually start an A/D conversion on a single
    channel and return the results.  

    @param[in]  channelNum
                The A/D channel [0..7] that will be used during the A/D 
                conversion.  (Note that only A/D channel's 0..3 are 
                configured by default in adcInit.)

    @return     0 if an overrun error occured, otherwise a 10-bit value
                containing the A/D conversion results.
    @warning    Only AD channels 0..3 are configured for A/D in adcInit.
                If you wish to use A/D pins 4..7 they will also need to
                be added to the adcInit function.
*/
/**************************************************************************/
uint32_t adcRead (uint8_t channelNum)
{
  if (!_adcInitialised) adcInit();

  uint32_t regVal, adcData;

  /* make sure that channel number is 0..7 */
  if ( channelNum >= 8 )
  {
    // ToDo: Change this to throw an exception back 
    channelNum = 0;
  }

  /* Deselect all channels */
  ADC_AD0CR &= ~ADC_AD0CR_SEL_MASK;

  /* Start converting now on the appropriate channel */
  ADC_AD0CR |= ADC_AD0CR_START_STARTNOW | (1 << channelNum);
				
  /* wait until end of A/D convert */
  while ( 1 )			
  {
    // Get data register results for the requested channel
    switch (channelNum)
    {
      case 0:
        regVal = (*(pREG32(ADC_AD0DR0)));
        break;
      case 1:
        regVal = (*(pREG32(ADC_AD0DR1)));
        break;
      case 2:
        regVal = (*(pREG32(ADC_AD0DR2)));
        break;
      case 3:
        regVal = (*(pREG32(ADC_AD0DR3)));
        break;
      case 4:
        regVal = (*(pREG32(ADC_AD0DR4)));
        break;
      case 5:
        regVal = (*(pREG32(ADC_AD0DR5)));
        break;
      case 6:
        regVal = (*(pREG32(ADC_AD0DR6)));
        break;
      case 7:
        regVal = (*(pREG32(ADC_AD0DR7)));
        break;
      default:
        regVal = (*(pREG32(ADC_AD0DR0)));
        break;
    }

    /* read result of A/D conversion */
    if (regVal & ADC_DR_DONE)
    {
      break;
    }
  }

  /* stop ADC */
  ADC_AD0CR &= ~ADC_AD0CR_START_MASK;

  /* return 0 if an overrun occurred */
  if ( regVal & ADC_DR_OVERRUN )
  {
    return (1);
  }

  /* return conversion results */
  adcData = (regVal >> 6) & 0x3FF;
  return (adcData);
}

/**************************************************************************/
/*! 
    @brief      Initialises the A/D converter and configures channels 0..3
                for 10-bit, SW-controlled A/D conversion.

    @return     Nothing
*/
/**************************************************************************/
void adcInit (void)
{
  /* Disable Power down bit to the ADC block. */
  SCB_PDRUNCFG &= ~(SCB_PDRUNCFG_ADC);

  /* Enable AHB clock to the ADC. */
  SCB_SYSAHBCLKCTRL |= (SCB_SYSAHBCLKCTRL_ADC);

  /* Digital pins need to have the 'analog' bit set in addition
     to changing their pin function */

  /* Set AD0 to analog input */
  IOCON_JTAG_TDI_PIO0_11 &= ~(IOCON_JTAG_TDI_PIO0_11_ADMODE_MASK |
                              IOCON_JTAG_TDI_PIO0_11_FUNC_MASK |
                              IOCON_JTAG_TDI_PIO0_11_MODE_MASK);
  IOCON_JTAG_TDI_PIO0_11 |=  (IOCON_JTAG_TDI_PIO0_11_FUNC_AD0 &
                              IOCON_JTAG_TDI_PIO0_11_ADMODE_ANALOG);

  /* Set AD1 to analog input */
  IOCON_JTAG_TMS_PIO1_0 &=  ~(IOCON_JTAG_TMS_PIO1_0_ADMODE_MASK |
                              IOCON_JTAG_TMS_PIO1_0_FUNC_MASK |
                              IOCON_JTAG_TMS_PIO1_0_MODE_MASK);
  IOCON_JTAG_TMS_PIO1_0 |=   (IOCON_JTAG_TMS_PIO1_0_FUNC_AD1 &
                              IOCON_JTAG_TMS_PIO1_0_ADMODE_ANALOG);

  /* Set AD2 to analog input */
  IOCON_JTAG_TDO_PIO1_1 &=  ~(IOCON_JTAG_TDO_PIO1_1_ADMODE_MASK |
                              IOCON_JTAG_TDO_PIO1_1_FUNC_MASK |
                              IOCON_JTAG_TDO_PIO1_1_MODE_MASK);
  IOCON_JTAG_TDO_PIO1_1 |=   (IOCON_JTAG_TDO_PIO1_1_FUNC_AD2 &
                              IOCON_JTAG_TDO_PIO1_1_ADMODE_ANALOG);

  /* Set AD3 to analog input */
  IOCON_JTAG_nTRST_PIO1_2 &= ~(IOCON_JTAG_nTRST_PIO1_2_ADMODE_MASK |
                               IOCON_JTAG_nTRST_PIO1_2_FUNC_MASK |
                               IOCON_JTAG_nTRST_PIO1_2_MODE_MASK);
  IOCON_JTAG_nTRST_PIO1_2 |=  (IOCON_JTAG_nTRST_PIO1_2_FUNC_AD3 &
                               IOCON_JTAG_nTRST_PIO1_2_ADMODE_ANALOG);

  /* Note that in SW mode only one channel can be selected at a time (AD0 in this case)
     To select multiple channels, ADC_AD0CR_BURST_HWSCANMODE must be used */
  ADC_AD0CR = (ADC_AD0CR_SEL_AD0 |                     /* SEL=1,select channel 0 on ADC0 */
              (((CFG_CPU_CCLK / SCB_SYSAHBCLKDIV) / 1000000 - 1 ) << 8) |   /* CLKDIV = Fpclk / 1000000 - 1 */ 
              ADC_AD0CR_BURST_SWMODE |                 /* BURST = 0, no BURST, software controlled */
              ADC_AD0CR_CLKS_10BITS |                  /* CLKS = 0, 11 clocks/10 bits */
              ADC_AD0CR_START_NOSTART |                /* START = 0 A/D conversion stops */
              ADC_AD0CR_EDGE_RISING);                  /* EDGE = 0 (CAP/MAT signal falling, trigger A/D conversion) */ 

  /* Set initialisation flag */
  _adcInitialised = true;

  /* Set last channel flag to 0 (initialised above) */
  _adcLastChannel = 0;

  return;
}
