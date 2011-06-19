/**************************************************************************/
/*! 
    @file     timer32.h
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

#ifndef __TIMER32_H__ 
#define __TIMER32_H__

#include "projectconfig.h"

#define TIMER32_CCLK_1US        ((CFG_CPU_CCLK/SCB_SYSAHBCLKDIV) / 1000000)
#define TIMER32_CCLK_10US       ((CFG_CPU_CCLK/SCB_SYSAHBCLKDIV) / 100000)
#define TIMER32_CCLK_100US      ((CFG_CPU_CCLK/SCB_SYSAHBCLKDIV) / 10000)
#define TIMER32_CCLK_1MS        ((CFG_CPU_CCLK/SCB_SYSAHBCLKDIV) / 1000)
#define TIMER32_CCLK_10MS       ((CFG_CPU_CCLK/SCB_SYSAHBCLKDIV) / 100)
#define TIMER32_CCLK_100MS      ((CFG_CPU_CCLK/SCB_SYSAHBCLKDIV) / 10)
#define TIMER32_CCLK_1S         (CFG_CPU_CCLK/SCB_SYSAHBCLKDIV)
#define TIMER32_DEFAULTINTERVAL	(TIMER32_CCLK_100US)

#define TIMER32_DELAY_100US     (1)            // 100uS delay = 1 tick
#define TIMER32_DELAY_1MS       (10)           // 1mS delay = 10 ticks
#define TIMER32_DELAY_1S        (10000)        // 1S delay = 10000 ticks

void TIMER32_0_IRQHandler(void);
void TIMER32_1_IRQHandler(void);

void timer32Delay(uint8_t timerNum, uint32_t delay);
void timer32Enable(uint8_t timerNum);
void timer32Disable(uint8_t timerNum);
void timer32Reset(uint8_t timerNum);
void timer32Init(uint8_t timerNum, uint32_t timerInterval);

#endif
