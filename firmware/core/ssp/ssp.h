/**************************************************************************/
/*! 
    @file     ssp.h
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

#ifndef _SSP_H_
#define _SSP_H_

#include "projectconfig.h"
#include "core/gpio/gpio.h"

#define SSP_FIFOSIZE            8       /* SPI read and write buffer size */
#define SSP_MAX_TIMEOUT         0xFF

#define SSP0_CSPORT             0
#define SSP0_CSPIN              2

/* Macro definitions to enable and disable SPI */
#define ssp0Select()            do {gpioSetValue(SSP0_CSPORT, SSP0_CSPIN, 0);} while (0)
#define ssp0Deselect()          do {gpioSetValue(SSP0_CSPORT, SSP0_CSPIN, 1);} while (0)

/**************************************************************************/
/*! 
    Indicates whether the clock should be high or low between frames.
*/
/**************************************************************************/
typedef enum sspClockPolarity_e
{
  sspClockPolarity_Low = 0,
  sspClockPolarity_High
} 
sspClockPolarity_t;

/**************************************************************************/
/*! 
    Indicates whether the bits start at the rising or falling edge of
    the clock transition.
*/
/**************************************************************************/
typedef enum sspClockPhase_e
{
  sspClockPhase_RisingEdge = 0,
  sspClockPhase_FallingEdge
} 
sspClockPhase_t;

extern void SSP_IRQHandler (void);
void sspInit (uint8_t portNum, sspClockPolarity_t polarity, sspClockPhase_t phase);
void sspSend (uint8_t portNum, const uint8_t *buf, uint32_t length);
void sspReceive (uint8_t portNum, uint8_t *buf, uint32_t length);
void sspSendReceive(uint8_t portNum, uint8_t *buf, uint32_t length);
#endif
