/**************************************************************************/
/*! 
    @file     cpu.c
    @author   K. Townsend (microBuilder.eu)
    @date     22 March 2010
    @version  0.10

    @section DESCRIPTION

    Initialises the CPU and any core clocks.  By default, the core clock
    is set to run at 72MHz.  In order to reduce power consumption all pins
    are set to GPIO and input by cpuInit.

    @section EXAMPLE
    @code
    #include "lpc134x.h"
    #include "core/cpu/cpu.h"

    int main (void)
    {
      // Initialise the CPU and setup the PLL
      cpuInit();
      
      while(1)
      {
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

#include "cpu.h"
#include "core/gpio/gpio.h"

/**************************************************************************/
/*! 
    @brief Configures the main clock/PLL
    
    The speed at which the MCU operates is set here using the SCB_PLLCTRL
    register, and the SCB_PLLCLKSEL register can be used to select which
    oscillator to use to generate the system clocks (the internal 12MHz
    oscillator or an external crystal).

    @param[in]  multiplier
                The PLL multiplier

*/
/**************************************************************************/
void cpuPllSetup (cpuMultiplier_t multiplier)
{
  uint32_t i;

  // Power up system oscillator
  SCB_PDRUNCFG &= ~(SCB_PDRUNCFG_SYSOSC_MASK);

  // Setup the crystal input (bypass disabled, 1-20MHz crystal)
  SCB_SYSOSCCTRL = (SCB_SYSOSCCTRL_BYPASS_DISABLED | SCB_SYSOSCCTRL_FREQRANGE_1TO20MHZ);

  for (i = 0; i < 200; i++)
  {
    __asm volatile ("NOP");
  }

  // Configure PLL
  SCB_PLLCLKSEL = SCB_CLKSEL_SOURCE_MAINOSC;  // Select external crystal as PLL clock source
  SCB_PLLCLKUEN = SCB_PLLCLKUEN_UPDATE;       // Update clock source
  SCB_PLLCLKUEN = SCB_PLLCLKUEN_DISABLE;      // Toggle update register once
  SCB_PLLCLKUEN = SCB_PLLCLKUEN_UPDATE;       // Update clock source again
  
  // Wait until the clock is updated
  while (!(SCB_PLLCLKUEN & SCB_PLLCLKUEN_UPDATE));

  // Set clock speed
  switch (multiplier)
  {
    case CPU_MULTIPLIER_2:
      SCB_PLLCTRL = (SCB_PLLCTRL_MULT_2 | (1 << SCB_PLLCTRL_DIV_BIT));
      break;
    case CPU_MULTIPLIER_3:
      SCB_PLLCTRL = (SCB_PLLCTRL_MULT_3 | (1 << SCB_PLLCTRL_DIV_BIT));
      break;
    case CPU_MULTIPLIER_4:
      SCB_PLLCTRL = (SCB_PLLCTRL_MULT_4 | (1 << SCB_PLLCTRL_DIV_BIT));
      break;
    case CPU_MULTIPLIER_5:
      SCB_PLLCTRL = (SCB_PLLCTRL_MULT_5 | (1 << SCB_PLLCTRL_DIV_BIT));
      break;
    case CPU_MULTIPLIER_6:
      SCB_PLLCTRL = (SCB_PLLCTRL_MULT_6 | (1 << SCB_PLLCTRL_DIV_BIT));
      break;
    case CPU_MULTIPLIER_1:
    default:
      SCB_PLLCTRL = (SCB_PLLCTRL_MULT_1 | (1 << SCB_PLLCTRL_DIV_BIT));
      break;
  }

  // Enable system PLL
  SCB_PDRUNCFG &= ~(SCB_PDRUNCFG_SYSPLL_MASK);

  // Wait for PLL to lock
  while (!(SCB_PLLSTAT & SCB_PLLSTAT_LOCK));

  // Setup main clock (use PLL output)
  SCB_MAINCLKSEL = SCB_MAINCLKSEL_SOURCE_SYSPLLCLKOUT;
  SCB_MAINCLKUEN = SCB_MAINCLKUEN_UPDATE;     // Update clock source
  SCB_MAINCLKUEN = SCB_MAINCLKUEN_DISABLE;    // Toggle update register once
  SCB_MAINCLKUEN = SCB_MAINCLKUEN_UPDATE;

  // Wait until the clock is updated
  while (!(SCB_MAINCLKUEN & SCB_MAINCLKUEN_UPDATE));

  // Disable USB clock by default (enabled in USB code)
  SCB_PDRUNCFG |= (SCB_PDSLEEPCFG_USBPAD_PD); // Power-down USB PHY
  SCB_PDRUNCFG |= (SCB_PDSLEEPCFG_USBPLL_PD); // Power-down USB PLL

  // Set system AHB clock
  SCB_SYSAHBCLKDIV = SCB_SYSAHBCLKDIV_DIV1;

  // Enabled IOCON clock for I/O related peripherals
  SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_IOCON;
}

/**************************************************************************/
/*! 
    @brief Initialises the CPU, setting up the PLL, etc.
*/
/**************************************************************************/
void cpuInit (void)
{
  gpioInit();

  // Set all GPIO pins to input by default
  GPIO_GPIO0DIR &= ~(GPIO_IO_ALL);
  GPIO_GPIO1DIR &= ~(GPIO_IO_ALL);
  GPIO_GPIO2DIR &= ~(GPIO_IO_ALL);
  GPIO_GPIO3DIR &= ~(GPIO_IO_ALL);

  // Setup PLL (etc.)
  cpuPllSetup(CPU_MULTIPLIER_6);
}
