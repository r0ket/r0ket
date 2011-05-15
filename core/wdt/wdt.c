/**************************************************************************/
/*! 
    @file     wdt.c
    @author   K. Townsend (microBuilder.eu)
    @date     22 March 2010
    @version  0.10

    @section DESCRIPTION

    Sets up the watchdog timer (WDT).  The WDT allows you to monitor
    whether the device is still executing properly.  If the watchdog
    isn't 'fed' within a pre-determined delay, it will raise an interrupt
    allowing you to decide if you want to reset the device, etc.

    @code 
    #include "core/cpu/cpu.h"
    #include "core/wdt/wdt.h"
    ...
    cpuInit();

    // Initialise wdt with no reset on timeout
    wdtInit(false);

    // Pat the watchdog (to start the timer)
    wdtFeed();

    while (1)
    {
      // Keep the watchdog happy by regularly feeding it
      wdtFeed();
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

#include "wdt.h"

#define WDT_FEED_VALUE		(0x003FFFFF)

volatile uint32_t wdt_counter;

/**************************************************************************/
/*! 
    IRQ Handler when the watchdog times out.  Any actions that you wish
    to take when a timeout occurs should be called from here.
*/
/**************************************************************************/
void WDT_IRQHandler(void)
{
  /* Clear the time-out interrupt flag */
  WDT_WDMOD &= ~WDT_WDMOD_WDTOF;
  wdt_counter++;
}

/**************************************************************************/
/*! 
    Setup the clock for the watchdog timer.  The default setting is 250kHz.
*/
/**************************************************************************/
static void wdtClockSetup (void)
{
  /* Watchdog Configuration */
  /* Freq. = 0.5MHz, div = 2: WDT_OSC = 250kHz  */
  SCB_WDTOSCCTRL = SCB_WDTOSCCTRL_FREQSEL_0_5MHZ | 
                   SCB_WDTOSCCTRL_DIVSEL_DIV2;
  
  /* Set clock source (use WDT oscillator) */
  SCB_WDTCLKSEL = SCB_WDTCLKSEL_SOURCE_WATCHDOGOSC;
  SCB_WDTCLKUEN = SCB_WDTCLKUEN_UPDATE;
  SCB_WDTCLKUEN = SCB_WDTCLKUEN_DISABLE;
  SCB_WDTCLKUEN = SCB_WDTCLKUEN_UPDATE;

  /* Wait until updated */
  while (!(SCB_WDTCLKUEN & SCB_WDTCLKUEN_UPDATE));

  /* Set divider */
  SCB_WDTCLKDIV = SCB_WDTCLKDIV_DIV1;

  /* Enable WDT clock */
  SCB_PDRUNCFG &= ~(SCB_PDRUNCFG_WDTOSC);
}

/**************************************************************************/
/*! 
    Initialises the watchdog timer and sets up the interrupt.
*/
/**************************************************************************/
void wdtInit (bool reset)
{
  /* Setup the WDT clock */
  wdtClockSetup();

  /* Enable AHB clock to the WDT domain. */
  SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_WDT;

  wdt_counter = 0;

  /* Enable the WDT interrupt */
  NVIC_EnableIRQ(WDT_IRQn);

  /* Set timeout value (must be at least 0x000000FF) */
  WDT_WDTC = WDT_FEED_VALUE;

  /* Enable the watchdog timer (without system reset) */
  WDT_WDMOD = WDT_WDMOD_WDEN_ENABLED |
              reset ? WDT_WDMOD_WDRESET_ENABLED : WDT_WDMOD_WDRESET_DISABLED ;
}

/**************************************************************************/
/*! 
    Feeds the watchdog to keep it from timing out.  Interrupts will be
    disabled while feeding the watchdog.
*/
/**************************************************************************/
void wdtFeed (void)
{
  /* Pet the watchdog */
  __disable_irq();
  WDT_WDFEED = WDT_WDFEED_FEED1;
  WDT_WDFEED = WDT_WDFEED_FEED2;
  __enable_irq();
}

