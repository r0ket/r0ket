/**************************************************************************/
/*! 
    @file     gpio.h
    @author   K. Townsend (microBuilder.eu)
    @date     22 March 2010
    @version  0.10

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

#ifndef _GPIO_H_
#define _GPIO_H_

#include "projectconfig.h"

/**************************************************************************/
/*! 
    Indicates whether the interrupt should be configured as edge or 
    level sensitive.
*/
/**************************************************************************/
typedef enum gpioInterruptSense_e
{
  gpioInterruptSense_Edge = 0,
  gpioInterruptSense_Level
} 
gpioInterruptSense_t;

/**************************************************************************/
/*! 
    Indicates whether one edge or both edges trigger an interrupt.
    Setting this to Double will cause both edges to trigger an interrupt.
*/
/**************************************************************************/
typedef enum gpioInterruptEdge_e
{
  gpioInterruptEdge_Single = 0,
  gpioInterruptEdge_Double
} 
gpioInterruptEdge_t;

/**************************************************************************/
/*! 
    Indicates whether the interrupt should be triggered in the rising
    or falling edge.  ActiveHigh means that a HIGH level on the pin will
    trigger an interrupt, ActiveLow means that a LOW level on the pin
    will trigger an interrupt.
*/
/**************************************************************************/
typedef enum gpioInterruptEvent_e
{
  gpioInterruptEvent_ActiveHigh = 0,
  gpioInterruptEvent_ActiveLow
} 
gpioInterruptEvent_t;

typedef enum gpioDirection_e
{
  gpioDirection_Input = 0,
  gpioDirection_Output
}
gpioDirection_t;

typedef enum gpioPullupMode_e
{
  gpioPullupMode_Inactive = IOCON_COMMON_MODE_INACTIVE,
  gpioPullupMode_PullDown = IOCON_COMMON_MODE_PULLDOWN,
  gpioPullupMode_PullUp =   IOCON_COMMON_MODE_PULLUP,
  gpioPullupMode_Repeater = IOCON_COMMON_MODE_REPEATER
}
gpioPullupMode_t;

void gpioInit (void);
void gpioSetDir (uint32_t portNum, uint32_t bitPos, gpioDirection_t dir);
uint32_t gpioGetValue (uint32_t portNum, uint32_t bitPos);
void gpioSetValue (uint32_t portNum, uint32_t bitPos, uint32_t bitVal);
void gpioSetInterrupt (uint32_t portNum, uint32_t bitPos, gpioInterruptSense_t sense, gpioInterruptEdge_t edge, gpioInterruptEvent_t event);
void gpioIntEnable (uint32_t portNum, uint32_t bitPos);
void gpioIntDisable (uint32_t portNum, uint32_t bitPos);
uint32_t  gpioIntStatus (uint32_t portNum, uint32_t bitPos);
void gpioIntClear (uint32_t portNum, uint32_t bitPos);
void gpioSetPullup (volatile uint32_t *ioconRegister, gpioPullupMode_t mode);

#endif