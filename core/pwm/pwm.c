/**************************************************************************/
/*! 
    @file     pwm.c
    @author   K. Townsend (microBuilder.eu)

    @brief    Simple PWM example that can be used to control a motor, dim
              an LED, etc.  Uses 16-bit Timer 1 and P1.9 for PWM output.

    @section Example

    @code 
    #include "core/pwm/pwm.h"
    ...

    // Initialises PWM output on 16-bit Timer 1 and
    // sets MAT0 (P1.9) as output
    pwmInit();

    // Setup the pulse-width and duty-cycle
    pwmSetDutyCycle(50);                  // Set 50% duty cycle
    pwmSetFrequencyInMicroseconds(100);   // 100 millisecond pulse width

    // Enable PWM output for exactly 50 pulses
    pwmStartFixed(50);

    // Alternatively, enable PWM output indefinately
    pwmStart();

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
#include "pwm.h"

uint32_t pwmPulseWidth = CFG_PWM_DEFAULT_PULSEWIDTH;
uint32_t pwmDutyCycle = CFG_PWM_DEFAULT_DUTYCYCLE;

// pwmMaxPulses is used by TIMER16_1_IRQHandler to turn PWM off after
// a specified number of pulses have been sent.  This only relevant when
// pwmStartFixed() is used.
volatile uint32_t pwmMaxPulses = 0;

/**************************************************************************/
/*! 
    Initialises 16-bit Timer 1, and configures the MAT0 output (pin 1.9) 
    to send the PWM output signal.
*/
/**************************************************************************/
void pwmInit(void)
{
  /* Enable the clock for CT16B1 */
  SCB_SYSAHBCLKCTRL |= (SCB_SYSAHBCLKCTRL_CT16B1);

  /* Configure PIO1.9 as Timer1_16 MAT0 Output */
  /* Alternatively, PIO1.10 (MAT1) can also be used though
     the initialisation code will need to be modified */
  IOCON_PIO1_9 &= ~IOCON_PIO1_9_FUNC_MASK;
  IOCON_PIO1_9 |= IOCON_PIO1_9_FUNC_CT16B1_MAT0;  

  /* Set default pulse width (MR3)*/
  TMR_TMR16B1MR3 = pwmPulseWidth;

  /* Set default duty cycle (MR0) */
  TMR_TMR16B1MR0 = (pwmPulseWidth * (100 - pwmDutyCycle)) / 100;

  /* Configure match control register to reset on MR3 */
  TMR_TMR16B1MCR = (TMR_TMR16B1MCR_MR3_RESET_ENABLED);

  /* External Match Register Settings for PWM */
  TMR_TMR16B1EMR = TMR_TMR16B1EMR_EMC0_TOGGLE | TMR_TMR16B1EMR_EM0;

  /* Disable Timer1 by default (enabled by pwmStart of pwmStartFixed) */
  TMR_TMR16B1TCR &= ~TMR_TMR16B1TCR_COUNTERENABLE_MASK;

  /* Enable PWM0 and PWM3 */
  TMR_TMR16B1PWMC = TMR_TMR16B1PWMC_PWM0_ENABLED | TMR_TMR16B1PWMC_PWM3_ENABLED;

  /* Make sure that the timer interrupt is enabled */
  NVIC_EnableIRQ(TIMER_16_1_IRQn);
}

/**************************************************************************/
/*! 
    Starts the PWM output
*/
/**************************************************************************/
void pwmStart(void)
{
  /* Disable interrupt on MR3 in case it was enabled by pwmStartFixed() */
  TMR_TMR16B1MCR  &= ~(TMR_TMR16B1MCR_MR3_INT_MASK);

  /* Enable Timer1 */
  TMR_TMR16B1TCR = TMR_TMR16B1TCR_COUNTERENABLE_ENABLED;
}

/**************************************************************************/
/*! 
    Stops the PWM output
*/
/**************************************************************************/
void pwmStop(void)
{
  /* Disable Timer1 */
  TMR_TMR16B1TCR &= ~(TMR_TMR16B1TCR_COUNTERENABLE_MASK);
}

/**************************************************************************/
/*! 
    Starts the PWM output, and stops after the specified number of
    pulses.

    @param[in]  pulses
                The number of pulses to generate before disabling the
                PWM output.  The output is actually disabled in the 
                timer ISR.

    @warning    The PWM output is actually stopped inside the 16-bit
                timer ISR in "core/timer16/timer16.h".
*/
/**************************************************************************/
void pwmStartFixed(uint32_t pulses)
{
  pwmMaxPulses = pulses;

  /* Configure match control register to also raise an interrupt on MR3 */
  TMR_TMR16B1MCR  |= (TMR_TMR16B1MCR_MR3_INT_ENABLED);

  /* Enable Timer1 (it will eventually be disabled in the ISR) */
  TMR_TMR16B1TCR = TMR_TMR16B1TCR_COUNTERENABLE_ENABLED;
}

/**************************************************************************/
/*! 
    Sets the signal's duty cycle in percent (1-100).

    @param[in]  percentage
                The duty-cycle in percentage (the amount of time that
                the signal is 'high' relative to the time its 'low').

    @returns    -1 if an invalid percentage was supplied.  Value must be
                between 1 and 100.
*/
/**************************************************************************/
int pwmSetDutyCycle(uint32_t percentage)
{
  if ((percentage < 1) || (percentage > 100))
  {
    /* Duty Cycle must be a value between 1 and 100 */
    return -1;
  }

  /* Set Duty Cycle (MR0) */
  TMR_TMR16B1MR0 = (pwmPulseWidth * (100 - (pwmDutyCycle = percentage))) / 100;

  return 0;
}

/**************************************************************************/
/*! 
    Sets the signal's frequency/pulse-width to the specified number
    of ticks.

    @param[in]  ticks
                The duration in clock ticks of each full pulse.

    @returns    -1 if a zero-value was provided for ticks.
*/
/**************************************************************************/
int pwmSetFrequencyInTicks(uint16_t ticks)
{
  if (ticks < 1)
  {
    return -1;
  }

  /* Set Pulse Width (MR3)*/
  TMR_TMR16B1MR3 = (pwmPulseWidth = ticks);

  /* Adjust Duty Cycle (MR0) */
  TMR_TMR16B1MR0 = (pwmPulseWidth * (100 - pwmDutyCycle)) / 100;

  return 0;  
}

/**************************************************************************/
/*! 
    Sets the signal's frequency/pulse-width to the specified number
    of microseconds.

    @param[in]  us
                The duration in microseconds of each full pulse.

    @returns    -1 if the supplied value exceeds the limits of the 16-bit
                timer, or if a zero-value was provided.

    @Warning    Because a 16-bit timer is used here by default, the
                maximum frequency is quite small.  Running at 36MHz, the
                largest possible pulse-width/frequency is ~1,82mS or
                1820 microSeconds.  At 12MHz its 5461 uS, and at 48MHz
                its 1365 uS.
*/
/**************************************************************************/
int pwmSetFrequencyInMicroseconds(uint16_t us)
{
  if (us < 1)
  {
    return -1;
  }

  uint32_t ticks = (((CFG_CPU_CCLK/SCB_SYSAHBCLKDIV) / 1000000) * us);
  if (ticks > 0xFFFF)
  {
    /* Delay exceeds the upper limit for the 16-bit timer */
    return -1;
  }

  /* Set Pulse Width (MR3)*/
  TMR_TMR16B1MR3 = (pwmPulseWidth = ticks);

  /* Adjust Duty Cycle (MR0) */
  TMR_TMR16B1MR0 = (pwmPulseWidth * (100 - pwmDutyCycle)) / 100;

  return 0;  
}


