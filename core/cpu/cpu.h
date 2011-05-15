/**************************************************************************/
/*! 
    @file     cpu.h
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

#ifndef _CPU_H_
#define _CPU_H_

#include "projectconfig.h"

// Macro to initialise, reset and enable the cycle counter.
// This can be used for rough timing and performance tests
// by resetting the cycle counter before a function, and 
// then reading the value after with "int count = DWT_CYCCNT"
//
//    CPU_RESET_CYCLECOUNTER;
//    ... do something
//    int count = DWT_CYCCNT;
//
#define CPU_RESET_CYCLECOUNTER    do { SCB_DEMCR = SCB_DEMCR | 0x01000000;  \
                                       DWT_CYCCNT = 0;                      \
                                       DWT_CTRL = DWT_CTRL | 1 ; } while(0)

/**************************************************************************/
/*! 
    @brief Indicates the value for the PLL multiplier
*/
/**************************************************************************/
typedef enum
{
  CPU_MULTIPLIER_1 = 0,
  CPU_MULTIPLIER_2,
  CPU_MULTIPLIER_3,
  CPU_MULTIPLIER_4,
  CPU_MULTIPLIER_5,
  CPU_MULTIPLIER_6
}
cpuMultiplier_t;

void cpuPllSetup (cpuMultiplier_t multiplier);
void cpuInit (void);

#endif