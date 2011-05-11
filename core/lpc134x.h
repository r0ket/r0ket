/**************************************************************************/
/*! 
    @file     lpc134x.h
    @author   K. Townsend (microBuilder.eu)
    @date     22 March 2010
    @version  0.10

	@section DESCRIPTION

    LPC1343 header file, based on V0.10 of the LPC1343 User Manual.
	
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

#ifndef _LPC134X_H_
#define _LPC134X_H_

#include "sysdefs.h"
#include "projectconfig.h"

/*##############################################################################
## System Control Block
##############################################################################*/

#define SCB_BASE_ADDRESS                          (*(pREG32 (0x40048000)))    // System control block base address

#define SCB_MEMREMAP                              (*(pREG32 (0x40048000)))    // System memory remap
#define SCB_PRESETCTRL                            (*(pREG32 (0x40048004)))    // Peripheral reset control
#define SCB_PLLCTRL                               (*(pREG32 (0x40048008)))    // System PLL control
#define SCB_PLLSTAT                               (*(pREG32 (0x4004800C)))    // System PLL status
#define SCB_USBPLLCTRL                            (*(pREG32 (0x40048010)))    // USB PLL control
#define SCB_USBPLLSTAT                            (*(pREG32 (0x40048014)))    // USB PLL status
#define SCB_SYSOSCCTRL                            (*(pREG32 (0x40048020)))    // System oscillator control
#define SCB_WDTOSCCTRL                            (*(pREG32 (0x40048024)))    // Watchdog oscillator control
#define SCB_IRCCTRL                               (*(pREG32 (0x40048028)))    // IRC control
#define SCB_RESETSTAT                             (*(pREG32 (0x40048030)))    // System reset status register
#define SCB_PLLCLKSEL                             (*(pREG32 (0x40048040)))    // System PLL clock source select
#define SCB_PLLCLKUEN                             (*(pREG32 (0x40048044)))    // System PLL clock source update enable
#define SCB_USBPLLCLKSEL                          (*(pREG32 (0x40048048)))    // USB PLL clock source select
#define SCB_USBPLLCLKUEN                          (*(pREG32 (0x4004804C)))    // USB PLL clock source update enable
#define SCB_MAINCLKSEL                            (*(pREG32 (0x40048070)))    // Main clock source select
#define SCB_MAINCLKUEN                            (*(pREG32 (0x40048074)))    // Main clock source update enable
#define SCB_SYSAHBCLKDIV                          (*(pREG32 (0x40048078)))    // System AHB clock divider
#define SCB_SYSAHBCLKCTRL                         (*(pREG32 (0x40048080)))    // System AHB clock control
#define SCB_SSP0CLKDIV                            (*(pREG32 (0x40048094)))    // SSP0 clock divider
#define SCB_UARTCLKDIV                            (*(pREG32 (0x40048098)))    // UART clock divider
#define SCB_SYSTICKCLKDIV                         (*(pREG32 (0x400480B0)))    // System tick clock divider
#define SCB_USBCLKSEL                             (*(pREG32 (0x400480C0)))    // USB clock source select
#define SCB_USBCLKUEN                             (*(pREG32 (0x400480C4)))    // USB clock source update enable
#define SCB_USBCLKDIV                             (*(pREG32 (0x400480C8)))    // USB clock divider
#define SCB_WDTCLKSEL                             (*(pREG32 (0x400480D0)))    // Watchdog clock source select
#define SCB_WDTCLKUEN                             (*(pREG32 (0x400480D4)))    // Watchdog clock source update enable
#define SCB_WDTCLKDIV                             (*(pREG32 (0x400480D8)))    // Watchdog clock divider
#define SCB_CLKOUTCLKSEL                          (*(pREG32 (0x400480E0)))    // CLKOUT clock source select
#define SCB_CLKOUTCLKUEN                          (*(pREG32 (0x400480E4)))    // CLKOUT clock source update enable
#define SCB_CLKOUTCLKDIV                          (*(pREG32 (0x400480E8)))    // CLKOUT clock divider
#define SCB_PIOPORCAP0                            (*(pREG32 (0x40048100)))    // POR captured PIO status 0
#define SCB_PIOPORCAP1                            (*(pREG32 (0x40048104)))    // POR captured PIO status 1
#define SCB_BODCTRL                               (*(pREG32 (0x40048150)))    // Brown-out detector control
#define SCB_SYSTICKCCAL                           (*(pREG32 (0x40048158)))    // System tick counter calibration
#define SCB_STARTAPRP0                            (*(pREG32 (0x40048200)))    // Start logic edge control register 0; bottom 32 interrupts
#define SCB_STARTERP0                             (*(pREG32 (0x40048204)))    // Start logic signal enable register 0; bottom 32 interrupts
#define SCB_STARTRSRP0CLR                         (*(pREG32 (0x40048208)))    // Start logic reset register 0; bottom 32 interrupts
#define SCB_STARTSRP0                             (*(pREG32 (0x4004820C)))    // Start logic status register 0; bottom 32 interrupts
#define SCB_STARTAPRP1                            (*(pREG32 (0x40048210)))    // Start logic edge control register 1; top 8 interrupts
#define SCB_STARTERP1                             (*(pREG32 (0x40048214)))    // Start logic signal enable register 1; top 8 interrupts
#define SCB_STARTRSRP1CLR                         (*(pREG32 (0x40048218)))    // Start logic reset register 1; top 8 interrupts
#define SCB_STARTSRP1                             (*(pREG32 (0x4004821C)))    // Start logic status register 1; top 8 interrupts
#define SCB_PDSLEEPCFG                            (*(pREG32 (0x40048230)))    // Power-down states in Deep-sleep mode
#define SCB_PDAWAKECFG                            (*(pREG32 (0x40048234)))    // Power-down states after wake-up from Deep-sleep mode
#define SCB_PDRUNCFG                              (*(pREG32 (0x40048238)))    // Power-down configuration register
#define SCB_DEVICEID                              (*(pREG32 (0x400483F4)))    // Device ID
#define SCB_MMFAR                                 (*(pREG32 (0xE000ED34)))    // Memory Manage Address Register (MMAR)
#define SCB_BFAR                                  (*(pREG32 (0xE000ED38)))    // Bus Fault Manage Address Register (BFAR)
#define SCB_DEMCR                                 (*(pREG32 (0xE000EDFC)))

/*  CPU ID Base Register */
#define SCB_CPUID                                 (*(pREG32 (0xE000ED00)))
#define SCB_CPUID_REVISION_MASK                   ((unsigned int) 0x0000000F) // Revision Code
#define SCB_CPUID_PARTNO_MASK                     ((unsigned int) 0x0000FFF0) // Part Number
#define SCB_CPUID_CONSTANT_MASK                   ((unsigned int) 0x000F0000) // Constant
#define SCB_CPUID_VARIANT_MASK                    ((unsigned int) 0x00F00000) // Variant
#define SCB_CPUID_IMPLEMENTER_MASK                ((unsigned int) 0xFF000000) // Implementer

/*  System Control Register */

#define SCB_SCR                                   (*(pREG32 (0xE000ED10)))
#define SCB_SCR_SLEEPONEXIT_MASK                  ((unsigned int) 0x00000002) // Enable sleep on exit
#define SCB_SCR_SLEEPONEXIT                       ((unsigned int) 0x00000002)
#define SCB_SCR_SLEEPDEEP_MASK                    ((unsigned int) 0x00000004)
#define SCB_SCR_SLEEPDEEP                         ((unsigned int) 0x00000004) // Enable deep sleep
#define SCB_SCR_SEVONPEND_MASK                    ((unsigned int) 0x00000010) // Wake up from WFE is new int is pended regardless of priority
#define SCB_SCR_SEVONPEND                         ((unsigned int) 0x00000010)

/*  Application Interrupt and Reset Control Register */

#define SCB_AIRCR                                 (*(pREG32 (0xE000ED0C)))
#define SCB_AIRCR_VECTKEY_VALUE                   ((unsigned int) 0x05FA0000) // Vect key needs to be set to 05FA for reset to work
#define SCB_AIRCR_VECTKEY_MASK                    ((unsigned int) 0xFFFF0000)
#define SCB_AIRCR_ENDIANESS                       ((unsigned int) 0x00008000) // Read Endianness (1=Big, 0=Little)
#define SCB_AIRCR_ENDIANESS_MASK                  ((unsigned int) 0x00008000)
#define SCB_AIRCR_PRIGROUP                        ((unsigned int) 0x00000700)
#define SCB_AIRCR_PRIGROUP_MASK                   ((unsigned int) 0x00000700)
#define SCB_AIRCR_SYSRESETREQ                     ((unsigned int) 0x00000004) // Request system reset
#define SCB_AIRCR_SYSRESETREQ_MASK                ((unsigned int) 0x00000004)
#define SCB_AIRCR_VECTCLRACTIVE                   ((unsigned int) 0x00000002) // Used to prevent accidental reset
#define SCB_AIRCR_VECTCLRACTIVE_MASK              ((unsigned int) 0x00000002)
#define SCB_AIRCR_VECTRESET                       ((unsigned int) 0x00000001)
#define SCB_AIRCR_VECTRESET_MASK                  ((unsigned int) 0x00000001)

/*  Memory Management Fault Status Register */

#define SCB_MMFSR                                 (*(pREG32 (0xE000ED28)))    
#define SCB_MMFSR_IACCVIOL_MASK                   ((unsigned int) 0x00000001) // Instruction access violation
#define SCB_MMFSR_IACCVIOL                        ((unsigned int) 0x00000001)
#define SCB_MMFSR_DACCVIOL_MASK                   ((unsigned int) 0x00000002) // Data access violation
#define SCB_MMFSR_DACCVIOL                        ((unsigned int) 0x00000002)
#define SCB_MMFSR_MUNSTKERR_MASK                  ((unsigned int) 0x00000008) // Unstacking error
#define SCB_MMFSR_MUNSTKERR                       ((unsigned int) 0x00000008)
#define SCB_MMFSR_MSTKERR_MASK                    ((unsigned int) 0x00000010) // Stacking error
#define SCB_MMFSR_MSTKERR                         ((unsigned int) 0x00000010)
#define SCB_MMFSR_MMARVALID_MASK                  ((unsigned int) 0x00000080) // Indicates MMAR is valid
#define SCB_MMFSR_MMARVALID                       ((unsigned int) 0x00000080)

/*  Bus Fault Status Register */

#define SCB_BFSR                                  (*(pREG32 (0xE000ED29)))
#define SCB_BFSR_IBUSERR_MASK                     ((unsigned int) 0x00000001) // Instruction access violation
#define SCB_BFSR_IBUSERR                          ((unsigned int) 0x00000001)
#define SCB_BFSR_PRECISERR_MASK                   ((unsigned int) 0x00000002) // Precise data access violation
#define SCB_BFSR_PRECISERR                        ((unsigned int) 0x00000002)
#define SCB_BFSR_IMPRECISERR_MASK                 ((unsigned int) 0x00000004) // Imprecise data access violation
#define SCB_BFSR_IMPRECISERR                      ((unsigned int) 0x00000004)
#define SCB_BFSR_UNSTKERR_MASK                    ((unsigned int) 0x00000008) // Unstacking error
#define SCB_BFSR_UNSTKERR                         ((unsigned int) 0x00000008)
#define SCB_BFSR_STKERR_MASK                      ((unsigned int) 0x00000010) // Stacking error
#define SCB_BFSR_STKERR                           ((unsigned int) 0x00000010)
#define SCB_BFSR_BFARVALID_MASK                   ((unsigned int) 0x00000080) // Indicates BFAR is valid
#define SCB_BFSR_BFARVALID                        ((unsigned int) 0x00000080)

/*  Usage Fault Status Register */

#define SCB_UFSR                                  (*(pREG32 (0xE000ED2A)))
#define SCB_UFSR_UNDEFINSTR_MASK                  ((unsigned int) 0x00000001) // Attempt to execute an undefined instruction
#define SCB_UFSR_UNDEFINSTR                       ((unsigned int) 0x00000001)
#define SCB_UFSR_INVSTATE_MASK                    ((unsigned int) 0x00000002) // Attempt to switch to invalid state (i.e. ARM)
#define SCB_UFSR_INVSTATE                         ((unsigned int) 0x00000002)
#define SCB_UFSR_INVPC_MASK                       ((unsigned int) 0x00000004) // Attempt to do exception with bad value in EXC_RETURN number
#define SCB_UFSR_INVPC                            ((unsigned int) 0x00000004)
#define SCB_UFSR_NOCP_MASK                        ((unsigned int) 0x00000008) // Attempt to execute a coprocessor instruction
#define SCB_UFSR_NOCP                             ((unsigned int) 0x00000008)
#define SCB_UFSR_UNALIGNED_MASK                   ((unsigned int) 0x00000100) // Unaligned access
#define SCB_UFSR_UNALIGNED                        ((unsigned int) 0x00000100)
#define SCB_UFSR_DIVBYZERO_MASK                   ((unsigned int) 0x00000200) // Divide by zero
#define SCB_UFSR_DIVBYZERO                        ((unsigned int) 0x00000200)

/*  Hard Fault Status Register */

#define SCB_HFSR                                  (*(pREG32 (0xE000ED2C)))
#define SCB_HFSR_VECTTBL_MASK                     ((unsigned int) 0x00000002) // Hard fault caused by failed vector fetch
#define SCB_HFSR_VECTTBL                          ((unsigned int) 0x00000002)
#define SCB_HFSR_FORCED_MASK                      ((unsigned int) 0x40000000) // Hard fault taken because of bus/mem man/usage fault
#define SCB_HFSR_FORCED                           ((unsigned int) 0x40000000)
#define SCB_HFSR_DEBUGEVT_MASK                    ((unsigned int) 0x80000000) // Hard fault triggered by debug event
#define SCB_HFSR_DEBUGEVT                         ((unsigned int) 0x80000000)

/*  Debug Fault Status Register */

#define SCB_DFSR                                  (*(pREG32 (0xE000ED30)))
#define SCB_DFSR_HALTED_MASK                      ((unsigned int) 0x00000001) // Halt requested in NVIC
#define SCB_DFSR_HALTED                           ((unsigned int) 0x00000001)
#define SCB_DFSR_BKPT_MASK                        ((unsigned int) 0x00000002) // BKPT instruction executed
#define SCB_DFSR_BKPT                             ((unsigned int) 0x00000002)
#define SCB_DFSR_DWTTRAP_MASK                     ((unsigned int) 0x00000004) // DWT match occurred
#define SCB_DFSR_DWTTRAP                          ((unsigned int) 0x00000004)
#define SCB_DFSR_VCATCH_MASK                      ((unsigned int) 0x00000008) // Vector fetch occurred
#define SCB_DFSR_VCATCH                           ((unsigned int) 0x00000008)
#define SCB_DFSR_EXTERNAL_MASK                    ((unsigned int) 0x00000010) // EDBGRQ signal asserted
#define SCB_DFSR_EXTERNAL                         ((unsigned int) 0x00000010)

/*  SCB_MEMREMAP (System memory remap register)
    The system memory remap register selects whether the ARM interrupt vectors are read
    from the boot ROM, the flash, or the SRAM.  */

#define SCB_MEMREMAP_MODE_BOOTLOADER              ((unsigned int) 0x00000000) // Interrupt vectors are remapped to Boot ROM
#define SCB_MEMREMAP_MODE_RAM                     ((unsigned int) 0x00000001) // Interrupt vectors are remapped to Static ROM
#define SCB_MEMREMAP_MODE_FLASH                   ((unsigned int) 0x00000002) // Interrupt vectors are not remapped and reside in Flash
#define SCB_MEMREMAP_MASK                         ((unsigned int) 0x00000003)

/*  PRESETCTRL (Peripheral reset control register) */

#define SCB_PRESETCTRL_SSP0_RESETENABLED          ((unsigned int) 0x00000000)
#define SCB_PRESETCTRL_SSP0_RESETDISABLED         ((unsigned int) 0x00000001)
#define SCB_PRESETCTRL_SSP0_MASK                  ((unsigned int) 0x00000001)
#define SCB_PRESETCTRL_I2C_RESETENABLED           ((unsigned int) 0x00000000)
#define SCB_PRESETCTRL_I2C_RESETDISABLED          ((unsigned int) 0x00000002)
#define SCB_PRESETCTRL_I2C_MASK                   ((unsigned int) 0x00000002)

/*  SYSPLLCTRL (System PLL control register)
    This register connects and enables the system PLL and configures the PLL multiplier and
    divider values. The PLL accepts an input frequency from 10 MHz to 25 MHz from various
    clock sources. The input frequency is multiplied up to a high frequency, then divided down
    to provide the actual clock used by the CPU, peripherals, and optionally the USB
    subsystem. Note that the USB subsystem has its own dedicated PLL. The PLL can
    produce a clock up to the maximum allowed for the CPU, which is 72 MHz. */

#define SCB_PLLCTRL_MULT_1                        ((unsigned int) 0x00000000)
#define SCB_PLLCTRL_MULT_2                        ((unsigned int) 0x00000001)
#define SCB_PLLCTRL_MULT_3                        ((unsigned int) 0x00000002)
#define SCB_PLLCTRL_MULT_4                        ((unsigned int) 0x00000003)
#define SCB_PLLCTRL_MULT_5                        ((unsigned int) 0x00000004)
#define SCB_PLLCTRL_MULT_6                        ((unsigned int) 0x00000005)
#define SCB_PLLCTRL_MULT_7                        ((unsigned int) 0x00000006)
#define SCB_PLLCTRL_MULT_8                        ((unsigned int) 0x00000007)
#define SCB_PLLCTRL_MULT_9                        ((unsigned int) 0x00000008)
#define SCB_PLLCTRL_MULT_10                       ((unsigned int) 0x00000009)
#define SCB_PLLCTRL_MULT_11                       ((unsigned int) 0x0000000A)
#define SCB_PLLCTRL_MULT_12                       ((unsigned int) 0x0000000B)
#define SCB_PLLCTRL_MULT_13                       ((unsigned int) 0x0000000C)
#define SCB_PLLCTRL_MULT_14                       ((unsigned int) 0x0000000D)
#define SCB_PLLCTRL_MULT_15                       ((unsigned int) 0x0000000E)
#define SCB_PLLCTRL_MULT_16                       ((unsigned int) 0x0000000F)
#define SCB_PLLCTRL_MULT_17                       ((unsigned int) 0x00000010)
#define SCB_PLLCTRL_MULT_18                       ((unsigned int) 0x00000011)
#define SCB_PLLCTRL_MULT_19                       ((unsigned int) 0x00000012)
#define SCB_PLLCTRL_MULT_20                       ((unsigned int) 0x00000013)
#define SCB_PLLCTRL_MULT_21                       ((unsigned int) 0x00000014)
#define SCB_PLLCTRL_MULT_22                       ((unsigned int) 0x00000015)
#define SCB_PLLCTRL_MULT_23                       ((unsigned int) 0x00000016)
#define SCB_PLLCTRL_MULT_24                       ((unsigned int) 0x00000017)
#define SCB_PLLCTRL_MULT_25                       ((unsigned int) 0x00000018)
#define SCB_PLLCTRL_MULT_26                       ((unsigned int) 0x00000019)
#define SCB_PLLCTRL_MULT_27                       ((unsigned int) 0x0000001A)
#define SCB_PLLCTRL_MULT_28                       ((unsigned int) 0x0000001B)
#define SCB_PLLCTRL_MULT_29                       ((unsigned int) 0x0000001C)
#define SCB_PLLCTRL_MULT_30                       ((unsigned int) 0x0000001D)
#define SCB_PLLCTRL_MULT_31                       ((unsigned int) 0x0000001E)
#define SCB_PLLCTRL_MULT_32                       ((unsigned int) 0x0000001F)
#define SCB_PLLCTRL_MULT_MASK                     ((unsigned int) 0x0000001F)    
#define SCB_PLLCTRL_DIV_2                         ((unsigned int) 0x00000000)       
#define SCB_PLLCTRL_DIV_4                         ((unsigned int) 0x00000020)       
#define SCB_PLLCTRL_DIV_8                         ((unsigned int) 0x00000040)       
#define SCB_PLLCTRL_DIV_16                        ((unsigned int) 0x00000060)
#define SCB_PLLCTRL_DIV_BIT                       (5)
#define SCB_PLLCTRL_DIV_MASK                      ((unsigned int) 0x00000060)    
#define SCB_PLLCTRL_DIRECT_MASK                   ((unsigned int) 0x00000080) // Direct CCO clock output control
#define SCB_PLLCTRL_BYPASS_MASK                   ((unsigned int) 0x00000100) // Input clock bypass control
#define SCB_PLLCTRL_MASK                          ((unsigned int) 0x000001FF)    

/*  SYSPLLSTAT (System PLL status register)
    This register is a Read-only register and supplies the PLL lock status */

#define SCB_PLLSTAT_LOCK                          ((unsigned int) 0x00000001) // 0 = PLL not locked, 1 = PLL locked
#define SCB_PLLSTAT_LOCK_MASK                     ((unsigned int) 0x00000001)    

/*  USBPLLCTRL (USB PLL control register)
    The USB PLL is identical to the system PLL and is used to provide a dedicated clock to
    the USB block if available.  The USB PLL should be always connected to the system
    oscillator to produce a stable USB clock. */

#define SCB_USBPLLCTRL_MULT_1                     ((unsigned int) 0x00000000)
#define SCB_USBPLLCTRL_MULT_2                     ((unsigned int) 0x00000001)
#define SCB_USBPLLCTRL_MULT_3                     ((unsigned int) 0x00000002)
#define SCB_USBPLLCTRL_MULT_4                     ((unsigned int) 0x00000003)
#define SCB_USBPLLCTRL_MULT_5                     ((unsigned int) 0x00000004)
#define SCB_USBPLLCTRL_MULT_6                     ((unsigned int) 0x00000005)
#define SCB_USBPLLCTRL_MULT_7                     ((unsigned int) 0x00000006)
#define SCB_USBPLLCTRL_MULT_8                     ((unsigned int) 0x00000007)
#define SCB_USBPLLCTRL_MULT_9                     ((unsigned int) 0x00000008)
#define SCB_USBPLLCTRL_MULT_10                    ((unsigned int) 0x00000009)
#define SCB_USBPLLCTRL_MULT_11                    ((unsigned int) 0x0000000A)
#define SCB_USBPLLCTRL_MULT_12                    ((unsigned int) 0x0000000B)
#define SCB_USBPLLCTRL_MULT_13                    ((unsigned int) 0x0000000C)
#define SCB_USBPLLCTRL_MULT_14                    ((unsigned int) 0x0000000D)
#define SCB_USBPLLCTRL_MULT_15                    ((unsigned int) 0x0000000E)
#define SCB_USBPLLCTRL_MULT_16                    ((unsigned int) 0x0000000F)
#define SCB_USBPLLCTRL_MULT_17                    ((unsigned int) 0x00000010)
#define SCB_USBPLLCTRL_MULT_18                    ((unsigned int) 0x00000011)
#define SCB_USBPLLCTRL_MULT_19                    ((unsigned int) 0x00000012)
#define SCB_USBPLLCTRL_MULT_20                    ((unsigned int) 0x00000013)
#define SCB_USBPLLCTRL_MULT_21                    ((unsigned int) 0x00000014)
#define SCB_USBPLLCTRL_MULT_22                    ((unsigned int) 0x00000015)
#define SCB_USBPLLCTRL_MULT_23                    ((unsigned int) 0x00000016)
#define SCB_USBPLLCTRL_MULT_24                    ((unsigned int) 0x00000017)
#define SCB_USBPLLCTRL_MULT_25                    ((unsigned int) 0x00000018)
#define SCB_USBPLLCTRL_MULT_26                    ((unsigned int) 0x00000019)
#define SCB_USBPLLCTRL_MULT_27                    ((unsigned int) 0x0000001A)
#define SCB_USBPLLCTRL_MULT_28                    ((unsigned int) 0x0000001B)
#define SCB_USBPLLCTRL_MULT_29                    ((unsigned int) 0x0000001C)
#define SCB_USBPLLCTRL_MULT_30                    ((unsigned int) 0x0000001D)
#define SCB_USBPLLCTRL_MULT_31                    ((unsigned int) 0x0000001E)
#define SCB_USBPLLCTRL_MULT_32                    ((unsigned int) 0x0000001F)
#define SCB_USBPLLCTRL_MULT_MASK                  ((unsigned int) 0x0000001F)    
#define SCB_USBPLLCTRL_DIV_2                      ((unsigned int) 0x00000000)       
#define SCB_USBPLLCTRL_DIV_4                      ((unsigned int) 0x00000020)       
#define SCB_USBPLLCTRL_DIV_8                      ((unsigned int) 0x00000040)       
#define SCB_USBPLLCTRL_DIV_16                     ((unsigned int) 0x00000060)
#define SCB_USBPLLCTRL_DIV_BIT                    (5)
#define SCB_USBPLLCTRL_DIV_MASK                   ((unsigned int) 0x00000060)    
#define SCB_USBPLLCTRL_DIRECT_MASK                ((unsigned int) 0x00000080) // Direct CCO clock output control
#define SCB_USBPLLCTRL_BYPASS_MASK                ((unsigned int) 0x00000100) // Input clock bypass control
#define SCB_USBPLLCTRL_MASK                       ((unsigned int) 0x000001FF)    

/*  USBPLLSTAT (System PLL status register)
    This register is a Read-only register and supplies the PLL lock status. */

#define SCB_USBPLLSTAT_LOCK                       ((unsigned int) 0x00000001) // 0 = PLL not locked, 1 = PLL locked
#define SCB_USBPLLSTAT_LOCK_MASK                  ((unsigned int) 0x00000001)    

/*  SYSOSCCTRL (System oscillator control register)
    This register configures the frequency range for the system oscillator. */

#define SCB_SYSOSCCTRL_BYPASS_DISABLED            ((unsigned int) 0x00000000) // Oscillator is not bypassed.
#define SCB_SYSOSCCTRL_BYPASS_ENABLED             ((unsigned int) 0x00000001) // Bypass enabled
#define SCB_SYSOSCCTRL_BYPASS_MASK                ((unsigned int) 0x00000001)
#define SCB_SYSOSCCTRL_FREQRANGE_1TO20MHZ         ((unsigned int) 0x00000000) // 1-20 MHz frequency range
#define SCB_SYSOSCCTRL_FREQRANGE_15TO25MHZ        ((unsigned int) 0x00000002) // 15-25 MHz frequency range
#define SCB_SYSOSCCTRL_FREQRANGE_MASK             ((unsigned int) 0x00000002)

/*  WDTOSCTRL (Watchdog oscillator control register)
    This register configures the watchdog oscillator. The oscillator consists of an analog and a
    digital part. The analog part contains the oscillator function and generates an analog clock
    (Fclkana). With the digital part, the analog output clock (Fclkana) can be divided to the
    required output clock frequency wdt_osc_clk. The analog output frequency (Fclkana) can
    be adjusted with the FREQSEL bits between 500 kHz and 3.7 MHz. With the digital part
    Fclkana will be divided (divider ratios = 2, 4,...,64) to wdt_osc_clk using the DIVSEL bits.*/

#define SCB_WDTOSCCTRL_DIVSEL_DIV2                ((unsigned int) 0x00000000) // Reset value
#define SCB_WDTOSCCTRL_DIVSEL_DIV4                ((unsigned int) 0x00000001)
#define SCB_WDTOSCCTRL_DIVSEL_DIV6                ((unsigned int) 0x00000002)
#define SCB_WDTOSCCTRL_DIVSEL_DIV8                ((unsigned int) 0x00000003)
#define SCB_WDTOSCCTRL_DIVSEL_DIV10               ((unsigned int) 0x00000004)
#define SCB_WDTOSCCTRL_DIVSEL_DIV12               ((unsigned int) 0x00000005)
#define SCB_WDTOSCCTRL_DIVSEL_DIV14               ((unsigned int) 0x00000006)
#define SCB_WDTOSCCTRL_DIVSEL_DIV16               ((unsigned int) 0x00000007)
#define SCB_WDTOSCCTRL_DIVSEL_DIV18               ((unsigned int) 0x00000008)
#define SCB_WDTOSCCTRL_DIVSEL_DIV20               ((unsigned int) 0x00000009)
#define SCB_WDTOSCCTRL_DIVSEL_DIV22               ((unsigned int) 0x0000000A)
#define SCB_WDTOSCCTRL_DIVSEL_DIV24               ((unsigned int) 0x0000000B)
#define SCB_WDTOSCCTRL_DIVSEL_DIV26               ((unsigned int) 0x0000000C)
#define SCB_WDTOSCCTRL_DIVSEL_DIV28               ((unsigned int) 0x0000000D)
#define SCB_WDTOSCCTRL_DIVSEL_DIV30               ((unsigned int) 0x0000000E)
#define SCB_WDTOSCCTRL_DIVSEL_DIV32               ((unsigned int) 0x0000000F)
#define SCB_WDTOSCCTRL_DIVSEL_DIV34               ((unsigned int) 0x00000010)
#define SCB_WDTOSCCTRL_DIVSEL_DIV36               ((unsigned int) 0x00000011)
#define SCB_WDTOSCCTRL_DIVSEL_DIV38               ((unsigned int) 0x00000012)
#define SCB_WDTOSCCTRL_DIVSEL_DIV40               ((unsigned int) 0x00000013)
#define SCB_WDTOSCCTRL_DIVSEL_DIV42               ((unsigned int) 0x00000014)
#define SCB_WDTOSCCTRL_DIVSEL_DIV44               ((unsigned int) 0x00000015)
#define SCB_WDTOSCCTRL_DIVSEL_DIV46               ((unsigned int) 0x00000016)
#define SCB_WDTOSCCTRL_DIVSEL_DIV48               ((unsigned int) 0x00000017)
#define SCB_WDTOSCCTRL_DIVSEL_DIV50               ((unsigned int) 0x00000018)
#define SCB_WDTOSCCTRL_DIVSEL_DIV52               ((unsigned int) 0x00000019)
#define SCB_WDTOSCCTRL_DIVSEL_DIV54               ((unsigned int) 0x0000001A)
#define SCB_WDTOSCCTRL_DIVSEL_DIV56               ((unsigned int) 0x0000001B)
#define SCB_WDTOSCCTRL_DIVSEL_DIV58               ((unsigned int) 0x0000001C)
#define SCB_WDTOSCCTRL_DIVSEL_DIV60               ((unsigned int) 0x0000001D)
#define SCB_WDTOSCCTRL_DIVSEL_DIV62               ((unsigned int) 0x0000001E)
#define SCB_WDTOSCCTRL_DIVSEL_DIV64               ((unsigned int) 0x0000001F)
#define SCB_WDTOSCCTRL_DIVSEL_MASK                ((unsigned int) 0x0000001F)
#define SCB_WDTOSCCTRL_FREQSEL_0_5MHZ             ((unsigned int) 0x00000020)
#define SCB_WDTOSCCTRL_FREQSEL_0_8MHZ             ((unsigned int) 0x00000040)
#define SCB_WDTOSCCTRL_FREQSEL_1_1MHZ             ((unsigned int) 0x00000060)
#define SCB_WDTOSCCTRL_FREQSEL_1_4MHZ             ((unsigned int) 0x00000080)
#define SCB_WDTOSCCTRL_FREQSEL_1_6MHZ             ((unsigned int) 0x000000A0) // Reset value
#define SCB_WDTOSCCTRL_FREQSEL_1_8MHZ             ((unsigned int) 0x000000C0)
#define SCB_WDTOSCCTRL_FREQSEL_2_0MHZ             ((unsigned int) 0x000000E0)
#define SCB_WDTOSCCTRL_FREQSEL_2_2MHZ             ((unsigned int) 0x00000100)
#define SCB_WDTOSCCTRL_FREQSEL_2_4MHZ             ((unsigned int) 0x00000120)
#define SCB_WDTOSCCTRL_FREQSEL_2_6MHZ             ((unsigned int) 0x00000140)
#define SCB_WDTOSCCTRL_FREQSEL_2_7MHZ             ((unsigned int) 0x00000160)
#define SCB_WDTOSCCTRL_FREQSEL_2_9MHZ             ((unsigned int) 0x00000180)
#define SCB_WDTOSCCTRL_FREQSEL_3_1MHZ             ((unsigned int) 0x000001A0)
#define SCB_WDTOSCCTRL_FREQSEL_3_2MHZ             ((unsigned int) 0x000001C0)
#define SCB_WDTOSCCTRL_FREQSEL_3_4MHZ             ((unsigned int) 0x000001E0)
#define SCB_WDTOSCCTRL_FREQSEL_MASK               ((unsigned int) 0x000001E0)

/*  IRCCTRL (Internal resonant crystal control register)
    This register is used to trim the on-chip 12 MHz oscillator. The trim value is factory-preset
    and written by the boot code on start-up. */

#define SCB_IRCCTRL_MASK                          ((unsigned int) 0x000000FF)

/*  SYSRSTSTAT (System reset status register)
    The SYSRSTSTAT register shows the source of the latest reset event. The bits are
    cleared by writing a one to any of the bits. The POR event clears all other bits in this
    register, but if another reset signal (e.g., EXTRST) remains asserted after the POR signal
    is negated, then its bit is set to detected. */

#define SCB_RESETSTAT_POR_MASK                    ((unsigned int) 0x00000001) // POR reset status
#define SCB_RESETSTAT_EXTRST_MASK                 ((unsigned int) 0x00000002) // Status of the external reset pin
#define SCB_RESETSTAT_WDT_MASK                    ((unsigned int) 0x00000004) // Status of the watchdog reset
#define SCB_RESETSTAT_BOD_MASK                    ((unsigned int) 0x00000008) // Status of the brown-out detect reset
#define SCB_RESETSTAT_SYSRST_MASK                 ((unsigned int) 0x00000010) // Status of the software system reset
#define SCB_RESETSTAT_MASK                        ((unsigned int) 0x00000010)

/*  SYSPLLCLKSEL (System PLL clock source select register)
    This register selects the clock source for the system PLL. The SYSPLLCLKUEN register
    must be toggled from LOW to HIGH for the update to take effect.
    Remark: The system oscillator must be selected if the system PLL is used to generate a
    48 MHz clock to the USB block.
*/

#define SCB_CLKSEL_SOURCE_INTERNALOSC             ((unsigned int) 0x00000000)
#define SCB_CLKSEL_SOURCE_MAINOSC                 ((unsigned int) 0x00000001)
#define SCB_CLKSEL_SOURCE_RTCOSC                  ((unsigned int) 0x00000002)
#define SCB_CLKSEL_SOURCE_MASK                    ((unsigned int) 0x00000002)

/*  SYSPLLUEN (System PLL clock source update enable register)
    This register updates the clock source of the system PLL with the new input clock after the
    SYSPLLCLKSEL register has been written to. In order for the update to take effect, first
    write a zero to the SYSPLLUEN register and then write a one to SYSPLLUEN. */

#define SCB_PLLCLKUEN_DISABLE                     ((unsigned int) 0x00000000)
#define SCB_PLLCLKUEN_UPDATE                      ((unsigned int) 0x00000001)
#define SCB_PLLCLKUEN_MASK                        ((unsigned int) 0x00000001)

/*  USBPLLCLKSEL (USB PLL clock source select register)
    his register selects the clock source for the dedicated USB PLL. The SYSPLLCLKUEN
    register must be toggled from LOW to HIGH for the update to take effect.
    Remark: Always select the system oscillator to produce a stable 48 MHz clock for
    the USB block. */

#define SCB_USBPLLCLKSEL_SOURCE_INTERNALOSC       ((unsigned int) 0x00000000) // Do NOT use (even though this is the default value)
#define SCB_USBPLLCLKSEL_SOURCE_MAINOSC           ((unsigned int) 0x00000001) // Main oscillator should always be used for USB clock
#define SCB_USBPLLCLKSEL_SOURCE_MASK              ((unsigned int) 0x00000002)

/*  USBPLLUEN (USB PLL clock source update enable register)
    This register updates the clock source of the USB PLL with the new input clock after the
    USBPLLCLKSEL register has been written to. In order for the update to take effect at the
    USB PLL input, first write a zero to the USBPLLUEN register and then write a one to
    USBPLLUEN. */

#define SCB_USBPLLCLKUEN_DISABLE                  ((unsigned int) 0x00000000)
#define SCB_USBPLLCLKUEN_UPDATE                   ((unsigned int) 0x00000001)
#define SCB_USBPLLCLKUEN_MASK                     ((unsigned int) 0x00000001)

/*  MAINCLKSEL (Main clock source select register)
    This register selects the main system clock which can be either the output from the
    system PLL or the IRC, system, or Watchdog oscillators directly. The main system clock
    clocks the core, the peripherals, and optionally the USB block.
    The MAINCLKUEN register must be toggled from LOW to HIGH for the update to take effect.*/

#define SCB_MAINCLKSEL_SOURCE_INTERNALOSC         ((unsigned int) 0x00000000) // Use IRC oscillator for main clock source
#define SCB_MAINCLKSEL_SOURCE_INPUTCLOCK          ((unsigned int) 0x00000001) // Use Input clock to system PLL for main clock source
#define SCB_MAINCLKSEL_SOURCE_WDTOSC              ((unsigned int) 0x00000002) // Use watchdog oscillator for main clock source
#define SCB_MAINCLKSEL_SOURCE_SYSPLLCLKOUT        ((unsigned int) 0x00000003) // Use system PLL clock out for main clock source
#define SCB_MAINCLKSEL_MASK                       ((unsigned int) 0x00000003)

/*  MAINCLKUEN (Main clock source update enable register)
    This register updates the clock source of the main clock with the new input clock after the
    MAINCLKSEL register has been written to. In order for the update to take effect, first write
    a zero to the MAINUEN register and then write a one to MAINCLKUEN. */

#define SCB_MAINCLKUEN_DISABLE                    ((unsigned int) 0x00000000)
#define SCB_MAINCLKUEN_UPDATE                     ((unsigned int) 0x00000001)
#define SCB_MAINCLKUEN_MASK                       ((unsigned int) 0x00000001)

/*  SYSAHBCLKDIV (System AHB clock divider register)
    This register divides the main clock to provide the system clock to the core, memories,
    and the peripherals. The system clock can be shut down completely by setting the DIV
    bits to 0x0. */

#define SCB_SYSAHBCLKDIV_DISABLE                  ((unsigned int) 0x00000000) // 0 will shut the system clock down completely
#define SCB_SYSAHBCLKDIV_DIV1                     ((unsigned int) 0x00000001) // 1, 2 or 4 are the most common values
#define SCB_SYSAHBCLKDIV_DIV2                     ((unsigned int) 0x00000002)    
#define SCB_SYSAHBCLKDIV_DIV4                     ((unsigned int) 0x00000004)    
#define SCB_SYSAHBCLKDIV_MASK                     ((unsigned int) 0x000000FF) // AHB clock divider can be from 0 to 255

/*  AHBCLKCTRL (System AHB clock control register)
    The AHBCLKCTRL register enables the clocks to individual system and peripheral blocks.
    The system clock (sys_ahb_clk[0], bit 0 in the AHBCLKCTRL register) provides the clock
    for the AHB to APB bridge, the AHB matrix, the ARM Cortex-M3, the Syscon block, and
    the PMU. This clock cannot be disabled. */

#define SCB_SYSAHBCLKCTRL_SYS                     ((unsigned int) 0x00000001) // Enables clock for AHB and APB bridges, FCLK, HCLK, SysCon and PMU
#define SCB_SYSAHBCLKCTRL_SYS_MASK                ((unsigned int) 0x00000001)
#define SCB_SYSAHBCLKCTRL_ROM                     ((unsigned int) 0x00000002) // Enables clock for ROM
#define SCB_SYSAHBCLKCTRL_ROM_MASK                ((unsigned int) 0x00000002)
#define SCB_SYSAHBCLKCTRL_RAM                     ((unsigned int) 0x00000004) // Enables clock for SRAM
#define SCB_SYSAHBCLKCTRL_RAM_MASK                ((unsigned int) 0x00000004)
#define SCB_SYSAHBCLKCTRL_FLASH1                  ((unsigned int) 0x00000008) // Enables clock for flash1
#define SCB_SYSAHBCLKCTRL_FLASH1_MASK             ((unsigned int) 0x00000008)
#define SCB_SYSAHBCLKCTRL_FLASH2                  ((unsigned int) 0x00000010) // Enables clock for flash2
#define SCB_SYSAHBCLKCTRL_FLASH2_MASK             ((unsigned int) 0x00000010)
#define SCB_SYSAHBCLKCTRL_I2C                     ((unsigned int) 0x00000020) // Enables clock for I2C
#define SCB_SYSAHBCLKCTRL_I2C_MASK                ((unsigned int) 0x00000020)
#define SCB_SYSAHBCLKCTRL_GPIO                    ((unsigned int) 0x00000040) // Enables clock for GPIO
#define SCB_SYSAHBCLKCTRL_GPIO_MASK               ((unsigned int) 0x00000040)
#define SCB_SYSAHBCLKCTRL_CT16B0                  ((unsigned int) 0x00000080) // Enables clock for 16-bit counter/timer 0
#define SCB_SYSAHBCLKCTRL_CT16B0_MASK             ((unsigned int) 0x00000080)
#define SCB_SYSAHBCLKCTRL_CT16B1                  ((unsigned int) 0x00000100) // Enables clock for 16-bit counter/timer 1
#define SCB_SYSAHBCLKCTRL_CT16B1_MASK             ((unsigned int) 0x00000100)
#define SCB_SYSAHBCLKCTRL_CT32B0                  ((unsigned int) 0x00000200) // Enables clock for 32-bit counter/timer 0
#define SCB_SYSAHBCLKCTRL_CT32B0_MASK             ((unsigned int) 0x00000200)
#define SCB_SYSAHBCLKCTRL_CT32B1                  ((unsigned int) 0x00000400) // Enables clock for 32-bit counter/timer 1
#define SCB_SYSAHBCLKCTRL_CT32B1_MASK             ((unsigned int) 0x00000400)
#define SCB_SYSAHBCLKCTRL_SSP0                    ((unsigned int) 0x00000800) // Enables clock for SSP0
#define SCB_SYSAHBCLKCTRL_SSP0_MASK               ((unsigned int) 0x00000800)
#define SCB_SYSAHBCLKCTRL_UART                    ((unsigned int) 0x00001000) // Enables clock for UART.  UART pins must be configured
#define SCB_SYSAHBCLKCTRL_UART_MASK               ((unsigned int) 0x00001000) // in the IOCON block before the UART clock can be enabled.
#define SCB_SYSAHBCLKCTRL_ADC                     ((unsigned int) 0x00002000) // Enables clock for ADC
#define SCB_SYSAHBCLKCTRL_ADC_MASK                ((unsigned int) 0x00002000)
#define SCB_SYSAHBCLKCTRL_USB_REG                 ((unsigned int) 0x00004000) // Enables clock for USB_REG
#define SCB_SYSAHBCLKCTRL_USB_REG_MASK            ((unsigned int) 0x00004000)
#define SCB_SYSAHBCLKCTRL_WDT                     ((unsigned int) 0x00008000) // Enables clock for watchdog timer
#define SCB_SYSAHBCLKCTRL_WDT_MASK                ((unsigned int) 0x00008000)
#define SCB_SYSAHBCLKCTRL_IOCON                   ((unsigned int) 0x00010000) // Enables clock for IO configuration block
#define SCB_SYSAHBCLKCTRL_IOCON_MASK              ((unsigned int) 0x00010000)
#define SCB_SYSAHBCLKCTRL_ALL_MASK                ((unsigned int) 0x0001FFFF)

/*  SSP0CLKDIV (SSP0 clock divider register)
    This register configures the SSP0 peripheral clock SSP_PCLK. The SSP_PCLK can be
    shut down by setting the DIV bits to 0x0.  It can be set from 1..255. */

#define SCB_SSP0CLKDIV_DISABLE                    ((unsigned int) 0x00000000)
#define SCB_SSP0CLKDIV_DIV1                       ((unsigned int) 0x00000001) // Divide SSP0 clock by 1 (can be set from 1..255)
#define SCB_SSP0CLKDIV_DIV2                       ((unsigned int) 0x00000002)
#define SCB_SSP0CLKDIV_DIV3                       ((unsigned int) 0x00000003)
#define SCB_SSP0CLKDIV_DIV4                       ((unsigned int) 0x00000004)
#define SCB_SSP0CLKDIV_DIV6                       ((unsigned int) 0x00000006)
#define SCB_SSP0CLKDIV_DIV10                      ((unsigned int) 0x0000000A)
#define SCB_SSP0CLKDIV_DIV12                      ((unsigned int) 0x0000000C)
#define SCB_SSP0CLKDIV_DIV20                      ((unsigned int) 0x00000014)
#define SCB_SSP0CLKDIV_DIV40                      ((unsigned int) 0x00000028)
#define SCB_SSP0CLKDIV_MASK                       ((unsigned int) 0x000000FF)

/*  UARTCLKDIV (UART clock divider register)
    This register configures the UART peripheral. The UART_PCLK can be shut down by
    setting the DIV bits to 0x0.
    Remark: Note that the UART pins must be configured in the IOCON block before the
    UART clock can be enabled.  */

#define SCB_UARTCLKDIV_DISABLE                    ((unsigned int) 0x00000000)
#define SCB_UARTCLKDIV_DIV1                       ((unsigned int) 0x00000001) // Divide UART clock by 1 (can be set from 1..255)
#define SCB_UARTCLKDIV_DIV2                       ((unsigned int) 0x00000002)
#define SCB_UARTCLKDIV_DIV4                       ((unsigned int) 0x00000004)
#define SCB_UARTCLKDIV_MASK                       ((unsigned int) 0x000000FF)

/*  SYSTICKCLKDIV (SYSTICK clock divider register)
    This register configures the SYSTICK peripheral clock. The SYSTICK timer clock can be
    shut down by setting the DIV bits to 0x0. */

#define SCB_SYSTICKCLKDIV_DISABLE                 ((unsigned int) 0x00000000)
#define SCB_SYSTICKCLKDIV_DIV1                    ((unsigned int) 0x00000001) // Divide SYSTICK clock by 1 (can be set from 1..255)
#define SCB_SYSTICKCLKDIV_DIV2                    ((unsigned int) 0x00000002) // Divide SYSTICK clock by 2
#define SCB_SYSTICKCLKDIV_DIV4                    ((unsigned int) 0x00000004) // Divide SYSTICK clock by 4
#define SCB_SYSTICKCLKDIV_DIV8                    ((unsigned int) 0x00000008) // Divide SYSTICK clock by 8
#define SCB_SYSTICKCLKDIV_MASK                    ((unsigned int) 0x000000FF)

/*  USBCLKSEL (USB clock source select register)
    This register selects the clock source for the USB usb_clk. The clock source can be either
    the USB PLL output or the main clock, and the clock can be further divided by the
    USBCLKDIV register to obtain a 48 MHz clock.  The USBCLKUEN register must be toggled from
    LOW to HIGH for the update to take effect.  */

#define SCB_USBCLKSEL_SOURCE_USBPLLOUT            ((unsigned int) 0x00000000) // USB PLL output
#define SCB_USBCLKSEL_SOURCE_INPUTCLOCK           ((unsigned int) 0x00000001) // Use the main clock
#define SCB_USBCLKSEL_MASK                        ((unsigned int) 0x00000003)

/*  USBCLKUEN (USB clock source update enable register)
    This register updates the clock source of the USB with the new input clock after the
    USBCLKSEL register has been written to. In order for the update to take effect, first write
    a zero to the USBCLKUEN register and then write a one to USBCLKUEN.  */

#define SCB_USBCLKUEN_DISABLE                     ((unsigned int) 0x00000000)
#define SCB_USBCLKUEN_UPDATE                      ((unsigned int) 0x00000001)
#define SCB_USBCLKUEN_MASK                        ((unsigned int) 0x00000001)

/*  USBCLKDIV (USB clock divider register)
    This register allows the USB clock usb_clk to be divided to 48 MHz. The usb_clk can be
    shut down by setting the DIV bits to 0x0.  */

#define SCB_USBCLKDIV_DISABLE                     ((unsigned int) 0x00000000)
#define SCB_USBCLKDIV_DIV1                        ((unsigned int) 0x00000001) // Divide USB clock by 1 (can be set from 1..255)
#define SCB_USBCLKDIV_MASK                        ((unsigned int) 0x000000FF)

/*  WDTCLKSEL (WDT clock source select register)
    This register selects the clock source for the watchdog timer. The WDTCLKUEN register
    must be toggled from LOW to HIGH for the update to take effect.  */

#define SCB_WDTCLKSEL_SOURCE_INTERNALOSC          ((unsigned int) 0x00000000) // Use the internal oscillator
#define SCB_WDTCLKSEL_SOURCE_INPUTCLOCK           ((unsigned int) 0x00000001) // Use the main clock
#define SCB_WDTCLKSEL_SOURCE_WATCHDOGOSC          ((unsigned int) 0x00000002) // Use the watchdog oscillator
#define SCB_WDTCLKSEL_MASK                        ((unsigned int) 0x00000003)    

/*  WDTCLKUEN (WDT clock source update enable register)
    This register updates the clock source of the watchdog timer with the new input clock after
    the WDTCLKSEL register has been written to. In order for the update to take effect at the
    input of the watchdog timer, first write a zero to the WDTCLKUEN register and then write
    a one to WDTCLKUEN.  */

#define SCB_WDTCLKUEN_DISABLE                     ((unsigned int) 0x00000000)
#define SCB_WDTCLKUEN_UPDATE                      ((unsigned int) 0x00000001)
#define SCB_WDTCLKUEN_MASK                        ((unsigned int) 0x00000001)

/*  WDTCLKDIV (WDT clock divider register)
    This register determines the divider values for the watchdog clock wdt_clk. */

#define SCB_WDTCLKDIV_DISABLE                     ((unsigned int) 0x00000000)
#define SCB_WDTCLKDIV_DIV1                        ((unsigned int) 0x00000001) // Divide clock by 1 (can be set from 1..255)
#define SCB_WDTCLKDIV_MASK                        ((unsigned int) 0x000000FF)

/*  CLKOUTCLKSEL (CLKOUT clock source select register)
    This register configures the clkout_clk signal to be output on the CLKOUT pin. All three
    oscillators and the main clock can be selected for the clkout_clk clock.
    The CLKOUTCLKUEN register must be toggled from LOW to HIGH for the update to take effect. */

#define SCB_CLKOUTCLKSEL_SOURCE_USBPLLOUT         ((unsigned int) 0x00000000) // USB PLL output
#define SCB_CLKOUTCLKSEL_SOURCE_INPUTCLOCK        ((unsigned int) 0x00000001) // Use the main clock
#define SCB_CLKOUTCLKSEL_MASK                     ((unsigned int) 0x00000003)

/*  CLKOUTUEN (CLKOUT clock source update enable register)
    This register updates the clock source of the CLKOUT pin with the new clock after the
    CLKOUTCLKSEL register has been written to. In order for the update to take effect at the
    input of the CLKOUT pin, first write a zero to the CLKCLKUEN register and then write a
    one to CLKCLKUEN. */

#define SCB_CLKOUTCLKUEN_DISABLE                  ((unsigned int) 0x00000000)
#define SCB_CLKOUTCLKUEN_UPDATE                   ((unsigned int) 0x00000001)
#define SCB_CLKOUTCLKUEN_MASK                     ((unsigned int) 0x00000001)

/*  CLKOUTCLKDIV (CLKOUT clock divider register)
    This register determines the divider value for the clkout_clk signal on the CLKOUT pin. */

#define SCB_CLKOUTCLKDIV_DISABLE                  ((unsigned int) 0x00000000)
#define SCB_CLKOUTCLKDIV_DIV1                     ((unsigned int) 0x00000001) // Divide clock by 1 (can be set from 1..255)
#define SCB_CLKOUTCLKDIV_MASK                     ((unsigned int) 0x000000FF)


/*  PIOPORCAP0 (POR captured PIO status register 0)
    The PIOPORCAP0 register captures the state (HIGH or LOW) of the PIO pins of ports 0,1,
    and 2 (pins PIO2_0 to PIO2_7) at power-on-reset. Each bit represents the reset state of
    one GPIO pin. This register is a read-only status register.  */

#define SCB_PIOPORCAP0_PIO0_0                     ((unsigned int) 0x00000001)
#define SCB_PIOPORCAP0_PIO0_0_MASK                ((unsigned int) 0x00000001)
#define SCB_PIOPORCAP0_PIO0_1                     ((unsigned int) 0x00000002)
#define SCB_PIOPORCAP0_PIO0_1_MASK                ((unsigned int) 0x00000002)
#define SCB_PIOPORCAP0_PIO0_2                     ((unsigned int) 0x00000004)
#define SCB_PIOPORCAP0_PIO0_2_MASK                ((unsigned int) 0x00000004)
#define SCB_PIOPORCAP0_PIO0_3                     ((unsigned int) 0x00000008)
#define SCB_PIOPORCAP0_PIO0_3_MASK                ((unsigned int) 0x00000008)
#define SCB_PIOPORCAP0_PIO0_4                     ((unsigned int) 0x00000010)
#define SCB_PIOPORCAP0_PIO0_4_MASK                ((unsigned int) 0x00000010)
#define SCB_PIOPORCAP0_PIO0_5                     ((unsigned int) 0x00000020)
#define SCB_PIOPORCAP0_PIO0_5_MASK                ((unsigned int) 0x00000020)
#define SCB_PIOPORCAP0_PIO0_6                     ((unsigned int) 0x00000040)
#define SCB_PIOPORCAP0_PIO0_6_MASK                ((unsigned int) 0x00000040)
#define SCB_PIOPORCAP0_PIO0_7                     ((unsigned int) 0x00000080)
#define SCB_PIOPORCAP0_PIO0_7_MASK                ((unsigned int) 0x00000080)
#define SCB_PIOPORCAP0_PIO0_8                     ((unsigned int) 0x00000100)
#define SCB_PIOPORCAP0_PIO0_8_MASK                ((unsigned int) 0x00000100)
#define SCB_PIOPORCAP0_PIO0_9                     ((unsigned int) 0x00000200)
#define SCB_PIOPORCAP0_PIO0_9_MASK                ((unsigned int) 0x00000200)
#define SCB_PIOPORCAP0_PIO0_10                    ((unsigned int) 0x00000400)
#define SCB_PIOPORCAP0_PIO0_10_MASK               ((unsigned int) 0x00000400)
#define SCB_PIOPORCAP0_PIO0_11                    ((unsigned int) 0x00000800)
#define SCB_PIOPORCAP0_PIO0_11_MASK               ((unsigned int) 0x00000800)
#define SCB_PIOPORCAP0_PIO1_0                     ((unsigned int) 0x00001000)
#define SCB_PIOPORCAP0_PIO1_0_MASK                ((unsigned int) 0x00001000)
#define SCB_PIOPORCAP0_PIO1_1                     ((unsigned int) 0x00002000)
#define SCB_PIOPORCAP0_PIO1_1_MASK                ((unsigned int) 0x00002000)
#define SCB_PIOPORCAP0_PIO1_2                     ((unsigned int) 0x00004000)
#define SCB_PIOPORCAP0_PIO1_2_MASK                ((unsigned int) 0x00004000)
#define SCB_PIOPORCAP0_PIO1_3                     ((unsigned int) 0x00008000)
#define SCB_PIOPORCAP0_PIO1_3_MASK                ((unsigned int) 0x00008000)
#define SCB_PIOPORCAP0_PIO1_4                     ((unsigned int) 0x00010000)
#define SCB_PIOPORCAP0_PIO1_4_MASK                ((unsigned int) 0x00010000)
#define SCB_PIOPORCAP0_PIO1_5                     ((unsigned int) 0x00020000)
#define SCB_PIOPORCAP0_PIO1_5_MASK                ((unsigned int) 0x00020000)
#define SCB_PIOPORCAP0_PIO1_6                     ((unsigned int) 0x00040000)
#define SCB_PIOPORCAP0_PIO1_6_MASK                ((unsigned int) 0x00040000)
#define SCB_PIOPORCAP0_PIO1_7                     ((unsigned int) 0x00080000)
#define SCB_PIOPORCAP0_PIO1_7_MASK                ((unsigned int) 0x00080000)
#define SCB_PIOPORCAP0_PIO1_8                     ((unsigned int) 0x00100000)
#define SCB_PIOPORCAP0_PIO1_8_MASK                ((unsigned int) 0x00100000)
#define SCB_PIOPORCAP0_PIO1_9                     ((unsigned int) 0x00200000)
#define SCB_PIOPORCAP0_PIO1_9_MASK                ((unsigned int) 0x00200000)
#define SCB_PIOPORCAP0_PIO1_10                    ((unsigned int) 0x00400000)
#define SCB_PIOPORCAP0_PIO1_10_MASK               ((unsigned int) 0x00400000)
#define SCB_PIOPORCAP0_PIO1_11                    ((unsigned int) 0x00800000)
#define SCB_PIOPORCAP0_PIO1_11_MASK               ((unsigned int) 0x00800000)
#define SCB_PIOPORCAP0_PIO2_0                     ((unsigned int) 0x01000000)
#define SCB_PIOPORCAP0_PIO2_0_MASK                ((unsigned int) 0x01000000)
#define SCB_PIOPORCAP0_PIO2_1                     ((unsigned int) 0x02000000)
#define SCB_PIOPORCAP0_PIO2_1_MASK                ((unsigned int) 0x02000000)
#define SCB_PIOPORCAP0_PIO2_2                     ((unsigned int) 0x04000000)
#define SCB_PIOPORCAP0_PIO2_2_MASK                ((unsigned int) 0x04000000)
#define SCB_PIOPORCAP0_PIO2_3                     ((unsigned int) 0x08000000)
#define SCB_PIOPORCAP0_PIO2_3_MASK                ((unsigned int) 0x08000000)
#define SCB_PIOPORCAP0_PIO2_4                     ((unsigned int) 0x10000000)
#define SCB_PIOPORCAP0_PIO2_4_MASK                ((unsigned int) 0x10000000)
#define SCB_PIOPORCAP0_PIO2_5                     ((unsigned int) 0x20000000)
#define SCB_PIOPORCAP0_PIO2_5_MASK                ((unsigned int) 0x20000000)
#define SCB_PIOPORCAP0_PIO2_6                     ((unsigned int) 0x40000000)
#define SCB_PIOPORCAP0_PIO2_6_MASK                ((unsigned int) 0x40000000)
#define SCB_PIOPORCAP0_PIO2_7                     ((unsigned int) 0x80000000)
#define SCB_PIOPORCAP0_PIO2_7_MASK                ((unsigned int) 0x80000000)

/*  PIOPORCAP1 (POR captured PIO status register 1)
    The PIOPORCAP1 register captures the state (HIGH or LOW) of the PIO pins of port 2
    (PIO2_8 to PIO2_11) and port 3 at power-on-reset. Each bit represents the reset state of
    one PIO pin. This register is a read-only status register.  */

#define SCB_PIOPORCAP1_PIO2_8                     ((unsigned int) 0x00000001)
#define SCB_PIOPORCAP1_PIO2_8_MASK                ((unsigned int) 0x00000001)
#define SCB_PIOPORCAP1_PIO2_9                     ((unsigned int) 0x00000002)
#define SCB_PIOPORCAP1_PIO2_9_MASK                ((unsigned int) 0x00000002)
#define SCB_PIOPORCAP1_PIO2_10                    ((unsigned int) 0x00000004)
#define SCB_PIOPORCAP1_PIO2_10_MASK               ((unsigned int) 0x00000004)
#define SCB_PIOPORCAP1_PIO2_11                    ((unsigned int) 0x00000008)
#define SCB_PIOPORCAP1_PIO2_11_MASK               ((unsigned int) 0x00000008)
#define SCB_PIOPORCAP1_PIO3_0                     ((unsigned int) 0x00000010)
#define SCB_PIOPORCAP1_PIO3_0_MASK                ((unsigned int) 0x00000010)
#define SCB_PIOPORCAP1_PIO3_1                     ((unsigned int) 0x00000020)
#define SCB_PIOPORCAP1_PIO3_1_MASK                ((unsigned int) 0x00000020)
#define SCB_PIOPORCAP1_PIO3_2                     ((unsigned int) 0x00000040)
#define SCB_PIOPORCAP1_PIO3_2_MASK                ((unsigned int) 0x00000040)
#define SCB_PIOPORCAP1_PIO3_3                     ((unsigned int) 0x00000080)
#define SCB_PIOPORCAP1_PIO3_3_MASK                ((unsigned int) 0x00000080)
#define SCB_PIOPORCAP1_PIO3_4                     ((unsigned int) 0x00000100)
#define SCB_PIOPORCAP1_PIO3_4_MASK                ((unsigned int) 0x00000100)
#define SCB_PIOPORCAP1_PIO3_5                     ((unsigned int) 0x00000200)
#define SCB_PIOPORCAP1_PIO3_5_MASK                ((unsigned int) 0x00000200)

/*  BODCTRL (Brown-out detection control register)
    The BOD control register selects four separate threshold values for sending a BOD
    interrupt to the NVIC. Only one level is allowed for forced reset.  */

#define SCB_BODCTRL_RSTLEVEL_MASK                 ((unsigned int) 0x00000003)
#define SCB_BODCTRL_INTLEVEL_1_69V_1_84V          ((unsigned int) 0x00000000)
#define SCB_BODCTRL_INTLEVEL_2_29V_2_44V          ((unsigned int) 0x00000004)
#define SCB_BODCTRL_INTLEVEL_2_59V_2_74V          ((unsigned int) 0x00000008)
#define SCB_BODCTRL_INTLEVEL_2_87V_2_98V          ((unsigned int) 0x0000000C)
#define SCB_BODCTRL_INTLEVEL_MASK                 ((unsigned int) 0x0000000C)
#define SCB_BODCTRL_RSTENABLE_DISABLE             ((unsigned int) 0x00000000)
#define SCB_BODCTRL_RSTENABLE_ENABLE              ((unsigned int) 0x00000010)
#define SCB_BODCTRL_RSTENABLE_MASK                ((unsigned int) 0x00000010)

/*  SYSTCKCAL (System tick counter calibration register) */

#define SCB_SYSTICKCCAL_MASK                      ((unsigned int) 0x03FFFFFF) // Undefined as of v0.07 of the LPC1343 User Manual

/*  STARTAPRP0 (Start logic edge control register 0)
    The STARTAPRP0 register controls the start logic inputs of ports 0 (PIO0_0 to PIO0_11)
    and 1 (PIO1_0 to PIO1_11) and the lower 8 inputs of port 2 (PIO2_0 to PIO2_7). This
    register selects a falling or rising edge on the corresponding PIO input to produce a falling
    or rising clock edge, respectively, for the start logic (see Section 39.3).
    Every bit in the STARTAPRP0 register controls one port input and is connected to one
    wake-up interrupt in the NVIC. Bit 0 in the STARTAPRP0 register corresponds to interrupt
    0, bit 1 to interrupt 1, etc.. The bottom 32 interrupts are contained this register,
    the top 8 interrupts are contained in the STARTAPRP1 register for total of 40 wake-up
    interrupts.
    Remark: Each interrupt connected to a start logic input must be enabled in the NVIC if the
    corresponding PIO pin is used to wake up the chip from Deep-sleep mode. */

#define SCB_STARTAPRP0_APRPIO0_0                  ((unsigned int) 0x00000001)
#define SCB_STARTAPRP0_APRPIO0_0_MASK             ((unsigned int) 0x00000001)
#define SCB_STARTAPRP0_APRPIO0_1                  ((unsigned int) 0x00000002)
#define SCB_STARTAPRP0_APRPIO0_1_MASK             ((unsigned int) 0x00000002)
#define SCB_STARTAPRP0_APRPIO0_2                  ((unsigned int) 0x00000004)
#define SCB_STARTAPRP0_APRPIO0_2_MASK             ((unsigned int) 0x00000004)
#define SCB_STARTAPRP0_APRPIO0_3                  ((unsigned int) 0x00000008)
#define SCB_STARTAPRP0_APRPIO0_3_MASK             ((unsigned int) 0x00000008)
#define SCB_STARTAPRP0_APRPIO0_4                  ((unsigned int) 0x00000010)
#define SCB_STARTAPRP0_APRPIO0_4_MASK             ((unsigned int) 0x00000010)
#define SCB_STARTAPRP0_APRPIO0_5                  ((unsigned int) 0x00000020)
#define SCB_STARTAPRP0_APRPIO0_5_MASK             ((unsigned int) 0x00000020)
#define SCB_STARTAPRP0_APRPIO0_6                  ((unsigned int) 0x00000040)
#define SCB_STARTAPRP0_APRPIO0_6_MASK             ((unsigned int) 0x00000040)
#define SCB_STARTAPRP0_APRPIO0_7                  ((unsigned int) 0x00000080)
#define SCB_STARTAPRP0_APRPIO0_7_MASK             ((unsigned int) 0x00000080)
#define SCB_STARTAPRP0_APRPIO0_8                  ((unsigned int) 0x00000100)
#define SCB_STARTAPRP0_APRPIO0_8_MASK             ((unsigned int) 0x00000100)
#define SCB_STARTAPRP0_APRPIO0_9                  ((unsigned int) 0x00000200)
#define SCB_STARTAPRP0_APRPIO0_9_MASK             ((unsigned int) 0x00000200)
#define SCB_STARTAPRP0_APRPIO0_10                 ((unsigned int) 0x00000400)
#define SCB_STARTAPRP0_APRPIO0_10_MASK            ((unsigned int) 0x00000400)
#define SCB_STARTAPRP0_APRPIO0_11                 ((unsigned int) 0x00000800)
#define SCB_STARTAPRP0_APRPIO0_11_MASK            ((unsigned int) 0x00000800)
#define SCB_STARTAPRP0_APRPIO1_0                  ((unsigned int) 0x00001000)
#define SCB_STARTAPRP0_APRPIO1_0_MASK             ((unsigned int) 0x00001000)
#define SCB_STARTAPRP0_APRPIO1_1                  ((unsigned int) 0x00002000)
#define SCB_STARTAPRP0_APRPIO1_1_MASK             ((unsigned int) 0x00002000)
#define SCB_STARTAPRP0_APRPIO1_2                  ((unsigned int) 0x00004000)
#define SCB_STARTAPRP0_APRPIO1_2_MASK             ((unsigned int) 0x00004000)
#define SCB_STARTAPRP0_APRPIO1_3                  ((unsigned int) 0x00008000)
#define SCB_STARTAPRP0_APRPIO1_3_MASK             ((unsigned int) 0x00008000)
#define SCB_STARTAPRP0_APRPIO1_4                  ((unsigned int) 0x00010000)
#define SCB_STARTAPRP0_APRPIO1_4_MASK             ((unsigned int) 0x00010000)
#define SCB_STARTAPRP0_APRPIO1_5                  ((unsigned int) 0x00020000)
#define SCB_STARTAPRP0_APRPIO1_5_MASK             ((unsigned int) 0x00020000)
#define SCB_STARTAPRP0_APRPIO1_6                  ((unsigned int) 0x00040000)
#define SCB_STARTAPRP0_APRPIO1_6_MASK             ((unsigned int) 0x00040000)
#define SCB_STARTAPRP0_APRPIO1_7                  ((unsigned int) 0x00080000)
#define SCB_STARTAPRP0_APRPIO1_7_MASK             ((unsigned int) 0x00080000)
#define SCB_STARTAPRP0_APRPIO1_8                  ((unsigned int) 0x00100000)
#define SCB_STARTAPRP0_APRPIO1_8_MASK             ((unsigned int) 0x00100000)
#define SCB_STARTAPRP0_APRPIO1_9                  ((unsigned int) 0x00200000)
#define SCB_STARTAPRP0_APRPIO1_9_MASK             ((unsigned int) 0x00200000)
#define SCB_STARTAPRP0_APRPIO1_10                 ((unsigned int) 0x00400000)
#define SCB_STARTAPRP0_APRPIO1_10_MASK            ((unsigned int) 0x00400000)
#define SCB_STARTAPRP0_APRPIO1_11                 ((unsigned int) 0x00800000)
#define SCB_STARTAPRP0_APRPIO1_11_MASK            ((unsigned int) 0x00800000)
#define SCB_STARTAPRP0_APRPIO2_0                  ((unsigned int) 0x01000000)
#define SCB_STARTAPRP0_APRPIO2_0_MASK             ((unsigned int) 0x01000000)
#define SCB_STARTAPRP0_APRPIO2_1                  ((unsigned int) 0x02000000)
#define SCB_STARTAPRP0_APRPIO2_1_MASK             ((unsigned int) 0x02000000)
#define SCB_STARTAPRP0_APRPIO2_2                  ((unsigned int) 0x04000000)
#define SCB_STARTAPRP0_APRPIO2_2_MASK             ((unsigned int) 0x04000000)
#define SCB_STARTAPRP0_APRPIO2_3                  ((unsigned int) 0x08000000)
#define SCB_STARTAPRP0_APRPIO2_3_MASK             ((unsigned int) 0x08000000)
#define SCB_STARTAPRP0_APRPIO2_4                  ((unsigned int) 0x10000000)
#define SCB_STARTAPRP0_APRPIO2_4_MASK             ((unsigned int) 0x10000000)
#define SCB_STARTAPRP0_APRPIO2_5                  ((unsigned int) 0x20000000)
#define SCB_STARTAPRP0_APRPIO2_5_MASK             ((unsigned int) 0x20000000)
#define SCB_STARTAPRP0_APRPIO2_6                  ((unsigned int) 0x40000000)
#define SCB_STARTAPRP0_APRPIO2_6_MASK             ((unsigned int) 0x40000000)
#define SCB_STARTAPRP0_APRPIO2_7                  ((unsigned int) 0x80000000)
#define SCB_STARTAPRP0_APRPIO2_7_MASK             ((unsigned int) 0x80000000)
#define SCB_STARTAPRP0_MASK                       ((unsigned int) 0xFFFFFFFF)

/*  STARTERP0 (Start logic signal enable register 0)
    This STARTERP0 register enables or disables the start signal bits in the start logic.  */

#define SCB_STARTERP0_ERPIO0_0                    ((unsigned int) 0x00000001)
#define SCB_STARTERP0_ERPIO0_0_MASK               ((unsigned int) 0x00000001)
#define SCB_STARTERP0_ERPIO0_1                    ((unsigned int) 0x00000002)
#define SCB_STARTERP0_ERPIO0_1_MASK               ((unsigned int) 0x00000002)
#define SCB_STARTERP0_ERPIO0_2                    ((unsigned int) 0x00000004)
#define SCB_STARTERP0_ERPIO0_2_MASK               ((unsigned int) 0x00000004)
#define SCB_STARTERP0_ERPIO0_3                    ((unsigned int) 0x00000008)
#define SCB_STARTERP0_ERPIO0_3_MASK               ((unsigned int) 0x00000008)
#define SCB_STARTERP0_ERPIO0_4                    ((unsigned int) 0x00000010)
#define SCB_STARTERP0_ERPIO0_4_MASK               ((unsigned int) 0x00000010)
#define SCB_STARTERP0_ERPIO0_5                    ((unsigned int) 0x00000020)
#define SCB_STARTERP0_ERPIO0_5_MASK               ((unsigned int) 0x00000020)
#define SCB_STARTERP0_ERPIO0_6                    ((unsigned int) 0x00000040)
#define SCB_STARTERP0_ERPIO0_6_MASK               ((unsigned int) 0x00000040)
#define SCB_STARTERP0_ERPIO0_7                    ((unsigned int) 0x00000080)
#define SCB_STARTERP0_ERPIO0_7_MASK               ((unsigned int) 0x00000080)
#define SCB_STARTERP0_ERPIO0_8                    ((unsigned int) 0x00000100)
#define SCB_STARTERP0_ERPIO0_8_MASK               ((unsigned int) 0x00000100)
#define SCB_STARTERP0_ERPIO0_9                    ((unsigned int) 0x00000200)
#define SCB_STARTERP0_ERPIO0_9_MASK               ((unsigned int) 0x00000200)
#define SCB_STARTERP0_ERPIO0_10                   ((unsigned int) 0x00000400)
#define SCB_STARTERP0_ERPIO0_10_MASK              ((unsigned int) 0x00000400)
#define SCB_STARTERP0_ERPIO0_11                   ((unsigned int) 0x00000800)
#define SCB_STARTERP0_ERPIO0_11_MASK              ((unsigned int) 0x00000800)
#define SCB_STARTERP0_ERPIO1_0                    ((unsigned int) 0x00001000)
#define SCB_STARTERP0_ERPIO1_0_MASK               ((unsigned int) 0x00001000)
#define SCB_STARTERP0_ERPIO1_1                    ((unsigned int) 0x00002000)
#define SCB_STARTERP0_ERPIO1_1_MASK               ((unsigned int) 0x00002000)
#define SCB_STARTERP0_ERPIO1_2                    ((unsigned int) 0x00004000)
#define SCB_STARTERP0_ERPIO1_2_MASK               ((unsigned int) 0x00004000)
#define SCB_STARTERP0_ERPIO1_3                    ((unsigned int) 0x00008000)
#define SCB_STARTERP0_ERPIO1_3_MASK               ((unsigned int) 0x00008000)
#define SCB_STARTERP0_ERPIO1_4                    ((unsigned int) 0x00010000)
#define SCB_STARTERP0_ERPIO1_4_MASK               ((unsigned int) 0x00010000)
#define SCB_STARTERP0_ERPIO1_5                    ((unsigned int) 0x00020000)
#define SCB_STARTERP0_ERPIO1_5_MASK               ((unsigned int) 0x00020000)
#define SCB_STARTERP0_ERPIO1_6                    ((unsigned int) 0x00040000)
#define SCB_STARTERP0_ERPIO1_6_MASK               ((unsigned int) 0x00040000)
#define SCB_STARTERP0_ERPIO1_7                    ((unsigned int) 0x00080000)
#define SCB_STARTERP0_ERPIO1_7_MASK               ((unsigned int) 0x00080000)
#define SCB_STARTERP0_ERPIO1_8                    ((unsigned int) 0x00100000)
#define SCB_STARTERP0_ERPIO1_8_MASK               ((unsigned int) 0x00100000)
#define SCB_STARTERP0_ERPIO1_9                    ((unsigned int) 0x00200000)
#define SCB_STARTERP0_ERPIO1_9_MASK               ((unsigned int) 0x00200000)
#define SCB_STARTERP0_ERPIO1_10                   ((unsigned int) 0x00400000)
#define SCB_STARTERP0_ERPIO1_10_MASK              ((unsigned int) 0x00400000)
#define SCB_STARTERP0_ERPIO1_11                   ((unsigned int) 0x00800000)
#define SCB_STARTERP0_ERPIO1_11_MASK              ((unsigned int) 0x00800000)
#define SCB_STARTERP0_ERPIO2_0                    ((unsigned int) 0x01000000)
#define SCB_STARTERP0_ERPIO2_0_MASK               ((unsigned int) 0x01000000)
#define SCB_STARTERP0_ERPIO2_1                    ((unsigned int) 0x02000000)
#define SCB_STARTERP0_ERPIO2_1_MASK               ((unsigned int) 0x02000000)
#define SCB_STARTERP0_ERPIO2_2                    ((unsigned int) 0x04000000)
#define SCB_STARTERP0_ERPIO2_2_MASK               ((unsigned int) 0x04000000)
#define SCB_STARTERP0_ERPIO2_3                    ((unsigned int) 0x08000000)
#define SCB_STARTERP0_ERPIO2_3_MASK               ((unsigned int) 0x08000000)
#define SCB_STARTERP0_ERPIO2_4                    ((unsigned int) 0x10000000)
#define SCB_STARTERP0_ERPIO2_4_MASK               ((unsigned int) 0x10000000)
#define SCB_STARTERP0_ERPIO2_5                    ((unsigned int) 0x20000000)
#define SCB_STARTERP0_ERPIO2_5_MASK               ((unsigned int) 0x20000000)
#define SCB_STARTERP0_ERPIO2_6                    ((unsigned int) 0x40000000)
#define SCB_STARTERP0_ERPIO2_6_MASK               ((unsigned int) 0x40000000)
#define SCB_STARTERP0_ERPIO2_7                    ((unsigned int) 0x80000000)
#define SCB_STARTERP0_ERPIO2_7_MASK               ((unsigned int) 0x80000000)
#define SCB_STARTERP0_MASK                        ((unsigned int) 0xFFFFFFFF)

/*  STARTRSRP0CLR (Start logic reset register 0)
    Writing a one to a bit in the STARTRSRP0CLR register resets the start logic state. The
    start-up logic uses the input signals to generate a clock edge for registering a start
    signal. This clock edge (falling or rising) sets the interrupt for waking up from
    Deep-sleep mode. Therefore, the start-up logic states must be cleared before being used. */

#define SCB_STARTRSRP0CLR_RSRPIO0_0               ((unsigned int) 0x00000001)
#define SCB_STARTRSRP0CLR_RSRPIO0_0_MASK          ((unsigned int) 0x00000001)
#define SCB_STARTRSRP0CLR_RSRPIO0_1               ((unsigned int) 0x00000002)
#define SCB_STARTRSRP0CLR_RSRPIO0_1_MASK          ((unsigned int) 0x00000002)
#define SCB_STARTRSRP0CLR_RSRPIO0_2               ((unsigned int) 0x00000004)
#define SCB_STARTRSRP0CLR_RSRPIO0_2_MASK          ((unsigned int) 0x00000004)
#define SCB_STARTRSRP0CLR_RSRPIO0_3               ((unsigned int) 0x00000008)
#define SCB_STARTRSRP0CLR_RSRPIO0_3_MASK          ((unsigned int) 0x00000008)
#define SCB_STARTRSRP0CLR_RSRPIO0_4               ((unsigned int) 0x00000010)
#define SCB_STARTRSRP0CLR_RSRPIO0_4_MASK          ((unsigned int) 0x00000010)
#define SCB_STARTRSRP0CLR_RSRPIO0_5               ((unsigned int) 0x00000020)
#define SCB_STARTRSRP0CLR_RSRPIO0_5_MASK          ((unsigned int) 0x00000020)
#define SCB_STARTRSRP0CLR_RSRPIO0_6               ((unsigned int) 0x00000040)
#define SCB_STARTRSRP0CLR_RSRPIO0_6_MASK          ((unsigned int) 0x00000040)
#define SCB_STARTRSRP0CLR_RSRPIO0_7               ((unsigned int) 0x00000080)
#define SCB_STARTRSRP0CLR_RSRPIO0_7_MASK          ((unsigned int) 0x00000080)
#define SCB_STARTRSRP0CLR_RSRPIO0_8               ((unsigned int) 0x00000100)
#define SCB_STARTRSRP0CLR_RSRPIO0_8_MASK          ((unsigned int) 0x00000100)
#define SCB_STARTRSRP0CLR_RSRPIO0_9               ((unsigned int) 0x00000200)
#define SCB_STARTRSRP0CLR_RSRPIO0_9_MASK          ((unsigned int) 0x00000200)
#define SCB_STARTRSRP0CLR_RSRPIO0_10              ((unsigned int) 0x00000400)
#define SCB_STARTRSRP0CLR_RSRPIO0_10_MASK         ((unsigned int) 0x00000400)
#define SCB_STARTRSRP0CLR_RSRPIO0_11              ((unsigned int) 0x00000800)
#define SCB_STARTRSRP0CLR_RSRPIO0_11_MASK         ((unsigned int) 0x00000800)
#define SCB_STARTRSRP0CLR_RSRPIO1_0               ((unsigned int) 0x00001000)
#define SCB_STARTRSRP0CLR_RSRPIO1_0_MASK          ((unsigned int) 0x00001000)
#define SCB_STARTRSRP0CLR_RSRPIO1_1               ((unsigned int) 0x00002000)
#define SCB_STARTRSRP0CLR_RSRPIO1_1_MASK          ((unsigned int) 0x00002000)
#define SCB_STARTRSRP0CLR_RSRPIO1_2               ((unsigned int) 0x00004000)
#define SCB_STARTRSRP0CLR_RSRPIO1_2_MASK          ((unsigned int) 0x00004000)
#define SCB_STARTRSRP0CLR_RSRPIO1_3               ((unsigned int) 0x00008000)
#define SCB_STARTRSRP0CLR_RSRPIO1_3_MASK          ((unsigned int) 0x00008000)
#define SCB_STARTRSRP0CLR_RSRPIO1_4               ((unsigned int) 0x00010000)
#define SCB_STARTRSRP0CLR_RSRPIO1_4_MASK          ((unsigned int) 0x00010000)
#define SCB_STARTRSRP0CLR_RSRPIO1_5               ((unsigned int) 0x00020000)
#define SCB_STARTRSRP0CLR_RSRPIO1_5_MASK          ((unsigned int) 0x00020000)
#define SCB_STARTRSRP0CLR_RSRPIO1_6               ((unsigned int) 0x00040000)
#define SCB_STARTRSRP0CLR_RSRPIO1_6_MASK          ((unsigned int) 0x00040000)
#define SCB_STARTRSRP0CLR_RSRPIO1_7               ((unsigned int) 0x00080000)
#define SCB_STARTRSRP0CLR_RSRPIO1_7_MASK          ((unsigned int) 0x00080000)
#define SCB_STARTRSRP0CLR_RSRPIO1_8               ((unsigned int) 0x00100000)
#define SCB_STARTRSRP0CLR_RSRPIO1_8_MASK          ((unsigned int) 0x00100000)
#define SCB_STARTRSRP0CLR_RSRPIO1_9               ((unsigned int) 0x00200000)
#define SCB_STARTRSRP0CLR_RSRPIO1_9_MASK          ((unsigned int) 0x00200000)
#define SCB_STARTRSRP0CLR_RSRPIO1_10              ((unsigned int) 0x00400000)
#define SCB_STARTRSRP0CLR_RSRPIO1_10_MASK         ((unsigned int) 0x00400000)
#define SCB_STARTRSRP0CLR_RSRPIO1_11              ((unsigned int) 0x00800000)
#define SCB_STARTRSRP0CLR_RSRPIO1_11_MASK         ((unsigned int) 0x00800000)
#define SCB_STARTRSRP0CLR_RSRPIO2_0               ((unsigned int) 0x01000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_0_MASK          ((unsigned int) 0x01000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_1               ((unsigned int) 0x02000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_1_MASK          ((unsigned int) 0x02000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_2               ((unsigned int) 0x04000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_2_MASK          ((unsigned int) 0x04000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_3               ((unsigned int) 0x08000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_3_MASK          ((unsigned int) 0x08000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_4               ((unsigned int) 0x10000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_4_MASK          ((unsigned int) 0x10000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_5               ((unsigned int) 0x20000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_5_MASK          ((unsigned int) 0x20000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_6               ((unsigned int) 0x40000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_6_MASK          ((unsigned int) 0x40000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_7               ((unsigned int) 0x80000000)
#define SCB_STARTRSRP0CLR_RSRPIO2_7_MASK          ((unsigned int) 0x80000000)
#define SCB_STARTRSRP0CLR_MASK                    ((unsigned int) 0xFFFFFFFF)

/*  (Start logic status register 0)
    This register reflects the status of the enabled start signal bits. Each bit
    (if enabled) reflects the state of the start logic, i.e. whether or not a
    wake-up signal has been received for a given pin.  */

#define SCB_STARTSRP0_SRPIO0_0                    ((unsigned int) 0x00000001)
#define SCB_STARTSRP0_SRPIO0_0_MASK               ((unsigned int) 0x00000001)
#define SCB_STARTSRP0_SRPIO0_1                    ((unsigned int) 0x00000002)
#define SCB_STARTSRP0_SRPIO0_1_MASK               ((unsigned int) 0x00000002)
#define SCB_STARTSRP0_SRPIO0_2                    ((unsigned int) 0x00000004)
#define SCB_STARTSRP0_SRPIO0_2_MASK               ((unsigned int) 0x00000004)
#define SCB_STARTSRP0_SRPIO0_3                    ((unsigned int) 0x00000008)
#define SCB_STARTSRP0_SRPIO0_3_MASK               ((unsigned int) 0x00000008)
#define SCB_STARTSRP0_SRPIO0_4                    ((unsigned int) 0x00000010)
#define SCB_STARTSRP0_SRPIO0_4_MASK               ((unsigned int) 0x00000010)
#define SCB_STARTSRP0_SRPIO0_5                    ((unsigned int) 0x00000020)
#define SCB_STARTSRP0_SRPIO0_5_MASK               ((unsigned int) 0x00000020)
#define SCB_STARTSRP0_SRPIO0_6                    ((unsigned int) 0x00000040)
#define SCB_STARTSRP0_SRPIO0_6_MASK               ((unsigned int) 0x00000040)
#define SCB_STARTSRP0_SRPIO0_7                    ((unsigned int) 0x00000080)
#define SCB_STARTSRP0_SRPIO0_7_MASK               ((unsigned int) 0x00000080)
#define SCB_STARTSRP0_SRPIO0_8                    ((unsigned int) 0x00000100)
#define SCB_STARTSRP0_SRPIO0_8_MASK               ((unsigned int) 0x00000100)
#define SCB_STARTSRP0_SRPIO0_9                    ((unsigned int) 0x00000200)
#define SCB_STARTSRP0_SRPIO0_9_MASK               ((unsigned int) 0x00000200)
#define SCB_STARTSRP0_SRPIO0_10                   ((unsigned int) 0x00000400)
#define SCB_STARTSRP0_SRPIO0_10_MASK              ((unsigned int) 0x00000400)
#define SCB_STARTSRP0_SRPIO0_11                   ((unsigned int) 0x00000800)
#define SCB_STARTSRP0_SRPIO0_11_MASK              ((unsigned int) 0x00000800)
#define SCB_STARTSRP0_SRPIO1_0                    ((unsigned int) 0x00001000)
#define SCB_STARTSRP0_SRPIO1_0_MASK               ((unsigned int) 0x00001000)
#define SCB_STARTSRP0_SRPIO1_1                    ((unsigned int) 0x00002000)
#define SCB_STARTSRP0_SRPIO1_1_MASK               ((unsigned int) 0x00002000)
#define SCB_STARTSRP0_SRPIO1_2                    ((unsigned int) 0x00004000)
#define SCB_STARTSRP0_SRPIO1_2_MASK               ((unsigned int) 0x00004000)
#define SCB_STARTSRP0_SRPIO1_3                    ((unsigned int) 0x00008000)
#define SCB_STARTSRP0_SRPIO1_3_MASK               ((unsigned int) 0x00008000)
#define SCB_STARTSRP0_SRPIO1_4                    ((unsigned int) 0x00010000)
#define SCB_STARTSRP0_SRPIO1_4_MASK               ((unsigned int) 0x00010000)
#define SCB_STARTSRP0_SRPIO1_5                    ((unsigned int) 0x00020000)
#define SCB_STARTSRP0_SRPIO1_5_MASK               ((unsigned int) 0x00020000)
#define SCB_STARTSRP0_SRPIO1_6                    ((unsigned int) 0x00040000)
#define SCB_STARTSRP0_SRPIO1_6_MASK               ((unsigned int) 0x00040000)
#define SCB_STARTSRP0_SRPIO1_7                    ((unsigned int) 0x00080000)
#define SCB_STARTSRP0_SRPIO1_7_MASK               ((unsigned int) 0x00080000)
#define SCB_STARTSRP0_SRPIO1_8                    ((unsigned int) 0x00100000)
#define SCB_STARTSRP0_SRPIO1_8_MASK               ((unsigned int) 0x00100000)
#define SCB_STARTSRP0_SRPIO1_9                    ((unsigned int) 0x00200000)
#define SCB_STARTSRP0_SRPIO1_9_MASK               ((unsigned int) 0x00200000)
#define SCB_STARTSRP0_SRPIO1_10                   ((unsigned int) 0x00400000)
#define SCB_STARTSRP0_SRPIO1_10_MASK              ((unsigned int) 0x00400000)
#define SCB_STARTSRP0_SRPIO1_11                   ((unsigned int) 0x00800000)
#define SCB_STARTSRP0_SRPIO1_11_MASK              ((unsigned int) 0x00800000)
#define SCB_STARTSRP0_SRPIO2_0                    ((unsigned int) 0x01000000)
#define SCB_STARTSRP0_SRPIO2_0_MASK               ((unsigned int) 0x01000000)
#define SCB_STARTSRP0_SRPIO2_1                    ((unsigned int) 0x02000000)
#define SCB_STARTSRP0_SRPIO2_1_MASK               ((unsigned int) 0x02000000)
#define SCB_STARTSRP0_SRPIO2_2                    ((unsigned int) 0x04000000)
#define SCB_STARTSRP0_SRPIO2_2_MASK               ((unsigned int) 0x04000000)
#define SCB_STARTSRP0_SRPIO2_3                    ((unsigned int) 0x08000000)
#define SCB_STARTSRP0_SRPIO2_3_MASK               ((unsigned int) 0x08000000)
#define SCB_STARTSRP0_SRPIO2_4                    ((unsigned int) 0x10000000)
#define SCB_STARTSRP0_SRPIO2_4_MASK               ((unsigned int) 0x10000000)
#define SCB_STARTSRP0_SRPIO2_5                    ((unsigned int) 0x20000000)
#define SCB_STARTSRP0_SRPIO2_5_MASK               ((unsigned int) 0x20000000)
#define SCB_STARTSRP0_SRPIO2_6                    ((unsigned int) 0x40000000)
#define SCB_STARTSRP0_SRPIO2_6_MASK               ((unsigned int) 0x40000000)
#define SCB_STARTSRP0_SRPIO2_7                    ((unsigned int) 0x80000000)
#define SCB_STARTSRP0_SRPIO2_7_MASK               ((unsigned int) 0x80000000)
#define SCB_STARTSRP0_MASK                        ((unsigned int) 0xFFFFFFFF)


/*  STARTAPRP1 (Start logic edge control register 1)
    The STARTAPRP1 register controls the start logic inputs of ports 2 (PIO2_8 to PIO2_11)
    and 3 (PIO3_0 to PIO3_3). This register selects a falling or rising edge on the
    corresponding PIO input to produce a falling or rising clock edge, respectively, for the
    start-up logic.
    Every bit in the STARTAPRP1 register controls one port input and is connected to one
    wake-up interrupt in the NVIC. Bit 0 in the STARTAPRP1 register corresponds to interrupt
    32, bit 1 to interrupt 33, up to bit 7 corresponding to interrupt 39.
    Remark: Each interrupt connected to a start logic input must be enabled in the NVIC if the
    corresponding PIO pin is used to wake up the chip from Deep-sleep mode.*/

#define SCB_STARTAPRP1_APRPIO2_8                  ((unsigned int) 0x00000001)
#define SCB_STARTAPRP1_APRPIO2_8_MASK             ((unsigned int) 0x00000001)
#define SCB_STARTAPRP1_APRPIO2_8                  ((unsigned int) 0x00000001)
#define SCB_STARTAPRP1_APRPIO2_9_MASK             ((unsigned int) 0x00000002)
#define SCB_STARTAPRP1_APRPIO2_10                 ((unsigned int) 0x00000004)
#define SCB_STARTAPRP1_APRPIO2_10_MASK            ((unsigned int) 0x00000004)
#define SCB_STARTAPRP1_APRPIO2_11                 ((unsigned int) 0x00000008)
#define SCB_STARTAPRP1_APRPIO2_11_MASK            ((unsigned int) 0x00000008)
#define SCB_STARTAPRP1_APRPIO3_0                  ((unsigned int) 0x00000010)
#define SCB_STARTAPRP1_APRPIO3_0_MASK             ((unsigned int) 0x00000010)
#define SCB_STARTAPRP1_APRPIO3_1                  ((unsigned int) 0x00000020)
#define SCB_STARTAPRP1_APRPIO3_1_MASK             ((unsigned int) 0x00000020)
#define SCB_STARTAPRP1_APRPIO3_2                  ((unsigned int) 0x00000040)
#define SCB_STARTAPRP1_APRPIO3_2_MASK             ((unsigned int) 0x00000040)
#define SCB_STARTAPRP1_APRPIO3_3                  ((unsigned int) 0x00000080)
#define SCB_STARTAPRP1_APRPIO3_3_MASK             ((unsigned int) 0x00000080)
#define SCB_STARTAPRP1_MASK                       ((unsigned int) 0x000000FF)

/*  STARTERP1 (Start logic signal enable register 1)
    This STARTERP1 register enables or disables the start signal bits in the start logic. */

#define SCB_STARTERP1_ERPIO2_8                    ((unsigned int) 0x00000001)
#define SCB_STARTERP1_ERPIO2_8_MASK               ((unsigned int) 0x00000001)
#define SCB_STARTERP1_ERPIO2_8                    ((unsigned int) 0x00000001)
#define SCB_STARTERP1_ERPIO2_9_MASK               ((unsigned int) 0x00000002)
#define SCB_STARTERP1_ERPIO2_10                   ((unsigned int) 0x00000004)
#define SCB_STARTERP1_ERPIO2_10_MASK              ((unsigned int) 0x00000004)
#define SCB_STARTERP1_ERPIO2_11                   ((unsigned int) 0x00000008)
#define SCB_STARTERP1_ERPIO2_11_MASK              ((unsigned int) 0x00000008)
#define SCB_STARTERP1_ERPIO3_0                    ((unsigned int) 0x00000010)
#define SCB_STARTERP1_ERPIO3_0_MASK               ((unsigned int) 0x00000010)
#define SCB_STARTERP1_ERPIO3_1                    ((unsigned int) 0x00000020)
#define SCB_STARTERP1_ERPIO3_1_MASK               ((unsigned int) 0x00000020)
#define SCB_STARTERP1_ERPIO3_2                    ((unsigned int) 0x00000040)
#define SCB_STARTERP1_ERPIO3_2_MASK               ((unsigned int) 0x00000040)
#define SCB_STARTERP1_ERPIO3_3                    ((unsigned int) 0x00000080)
#define SCB_STARTERP1_ERPIO3_3_MASK               ((unsigned int) 0x00000080)
#define SCB_STARTERP1_MASK                        ((unsigned int) 0x000000FF)

/*  (Start logic reset register 1)
    Writing a one to a bit in the STARTRSRP1CLR register resets the start logic state. The
    start-up logic uses the input signals to generate a clock edge for registering a start
    signal. This clock edge (falling or rising) sets the interrupt for waking up from
    Deep-sleep mode. Therefore, the start-up logic states must be cleared before being used. */

#define SCB_STARTRSRP1CLR_RSRPIO2_8               ((unsigned int) 0x00000001)
#define SCB_STARTRSRP1CLR_RSRPIO2_8_MASK          ((unsigned int) 0x00000001)
#define SCB_STARTRSRP1CLR_RSRPIO2_8               ((unsigned int) 0x00000001)
#define SCB_STARTRSRP1CLR_RSRPIO2_9_MASK          ((unsigned int) 0x00000002)
#define SCB_STARTRSRP1CLR_RSRPIO2_10              ((unsigned int) 0x00000004)
#define SCB_STARTRSRP1CLR_RSRPIO2_10_MASK         ((unsigned int) 0x00000004)
#define SCB_STARTRSRP1CLR_RSRPIO2_11              ((unsigned int) 0x00000008)
#define SCB_STARTRSRP1CLR_RSRPIO2_11_MASK         ((unsigned int) 0x00000008)
#define SCB_STARTRSRP1CLR_RSRPIO3_0               ((unsigned int) 0x00000010)
#define SCB_STARTRSRP1CLR_RSRPIO3_0_MASK          ((unsigned int) 0x00000010)
#define SCB_STARTRSRP1CLR_RSRPIO3_1               ((unsigned int) 0x00000020)
#define SCB_STARTRSRP1CLR_RSRPIO3_1_MASK          ((unsigned int) 0x00000020)
#define SCB_STARTRSRP1CLR_RSRPIO3_2               ((unsigned int) 0x00000040)
#define SCB_STARTRSRP1CLR_RSRPIO3_2_MASK          ((unsigned int) 0x00000040)
#define SCB_STARTRSRP1CLR_RSRPIO3_3               ((unsigned int) 0x00000080)
#define SCB_STARTRSRP1CLR_RSRPIO3_3_MASK          ((unsigned int) 0x00000080)
#define SCB_STARTRSRP1CLR_MASK                    ((unsigned int) 0x000000FF)

/*  STARTSRP1 (Start logic status register 1)
    This register reflects the status of the enabled start signals. */

#define SCB_STARTSRP1_SRPIO2_8                    ((unsigned int) 0x00000001)
#define SCB_STARTSRP1_SRPIO2_8_MASK               ((unsigned int) 0x00000001)
#define SCB_STARTSRP1_SRPIO2_8                    ((unsigned int) 0x00000001)
#define SCB_STARTSRP1_SRPIO2_9_MASK               ((unsigned int) 0x00000002)
#define SCB_STARTSRP1_SRPIO2_10                   ((unsigned int) 0x00000004)
#define SCB_STARTSRP1_SRPIO2_10_MASK              ((unsigned int) 0x00000004)
#define SCB_STARTSRP1_SRPIO2_11                   ((unsigned int) 0x00000008)
#define SCB_STARTSRP1_SRPIO2_11_MASK              ((unsigned int) 0x00000008)
#define SCB_STARTSRP1_SRPIO3_0                    ((unsigned int) 0x00000010)
#define SCB_STARTSRP1_SRPIO3_0_MASK               ((unsigned int) 0x00000010)
#define SCB_STARTSRP1_SRPIO3_1                    ((unsigned int) 0x00000020)
#define SCB_STARTSRP1_SRPIO3_1_MASK               ((unsigned int) 0x00000020)
#define SCB_STARTSRP1_SRPIO3_2                    ((unsigned int) 0x00000040)
#define SCB_STARTSRP1_SRPIO3_2_MASK               ((unsigned int) 0x00000040)
#define SCB_STARTSRP1_SRPIO3_3                    ((unsigned int) 0x00000080)
#define SCB_STARTSRP1_SRPIO3_3_MASK               ((unsigned int) 0x00000080)
#define SCB_STARTSRP1_MASK                        ((unsigned int) 0x000000FF)

/*  PDSLEEPCFG (Deep-sleep mode configuration register)
    The bits in this register can be programmed to indicate the state the chip must enter when
    the Deep-sleep mode is asserted by the ARM. The value of the PDSLEEPCFG register
    will be automatically loaded into the PDRUNCFG register when the Sleep mode is
    entered. */

#define SCB_PDSLEEPCFG_IRCOUT_PD                  ((unsigned int) 0x00000001)
#define SCB_PDSLEEPCFG_IRCOUT_PD_MASK             ((unsigned int) 0x00000001)
#define SCB_PDSLEEPCFG_IRC_PD                     ((unsigned int) 0x00000002)
#define SCB_PDSLEEPCFG_IRC_PD_MASK                ((unsigned int) 0x00000002)
#define SCB_PDSLEEPCFG_FLASH_PD                   ((unsigned int) 0x00000004)
#define SCB_PDSLEEPCFG_FLASH_PD_MASK              ((unsigned int) 0x00000004)
#define SCB_PDSLEEPCFG_BOD_PD                     ((unsigned int) 0x00000008)
#define SCB_PDSLEEPCFG_BOD_PD_MASK                ((unsigned int) 0x00000008)
#define SCB_PDSLEEPCFG_ADC_PD                     ((unsigned int) 0x00000010)
#define SCB_PDSLEEPCFG_ADC_PD_MASK                ((unsigned int) 0x00000010)
#define SCB_PDSLEEPCFG_SYSOSC_PD                  ((unsigned int) 0x00000020)
#define SCB_PDSLEEPCFG_SYSOSC_PD_MASK             ((unsigned int) 0x00000020)
#define SCB_PDSLEEPCFG_WDTOSC_PD                  ((unsigned int) 0x00000040)
#define SCB_PDSLEEPCFG_WDTOSC_PD_MASK             ((unsigned int) 0x00000040)
#define SCB_PDSLEEPCFG_SYSPLL_PD                  ((unsigned int) 0x00000080)
#define SCB_PDSLEEPCFG_SYSPLL_PD_MASK             ((unsigned int) 0x00000080)
#define SCB_PDSLEEPCFG_USBPLL_PD                  ((unsigned int) 0x00000100)
#define SCB_PDSLEEPCFG_USBPLL_PD_MASK             ((unsigned int) 0x00000100)
#define SCB_PDSLEEPCFG_USBPAD_PD                  ((unsigned int) 0x00000400)
#define SCB_PDSLEEPCFG_USBPAD_PD_MASK             ((unsigned int) 0x00000400)

/*  PDAWAKECFG (Wake-up configuration register)
    The bits in this register can be programmed to indicate the state the chip must enter when
    it is waking up from Deep-sleep mode. */

#define SCB_PDAWAKECFG_IRCOUT_PD                  ((unsigned int) 0x00000001)
#define SCB_PDAWAKECFG_IRCOUT_PD_MASK             ((unsigned int) 0x00000001)
#define SCB_PDAWAKECFG_IRC_PD                     ((unsigned int) 0x00000002)
#define SCB_PDAWAKECFG_IRC_PD_MASK                ((unsigned int) 0x00000002)
#define SCB_PDAWAKECFG_FLASH_PD                   ((unsigned int) 0x00000004)
#define SCB_PDAWAKECFG_FLASH_PD_MASK              ((unsigned int) 0x00000004)
#define SCB_PDAWAKECFG_BOD_PD                     ((unsigned int) 0x00000008)
#define SCB_PDAWAKECFG_BOD_PD_MASK                ((unsigned int) 0x00000008)
#define SCB_PDAWAKECFG_ADC_PD                     ((unsigned int) 0x00000010)
#define SCB_PDAWAKECFG_ADC_PD_MASK                ((unsigned int) 0x00000010)
#define SCB_PDAWAKECFG_SYSOSC_PD                  ((unsigned int) 0x00000020)
#define SCB_PDAWAKECFG_SYSOSC_PD_MASK             ((unsigned int) 0x00000020)
#define SCB_PDAWAKECFG_WDTOSC_PD                  ((unsigned int) 0x00000040)
#define SCB_PDAWAKECFG_WDTOSC_PD_MASK             ((unsigned int) 0x00000040)
#define SCB_PDAWAKECFG_SYSPLL_PD                  ((unsigned int) 0x00000080)
#define SCB_PDAWAKECFG_SYSPLL_PD_MASK             ((unsigned int) 0x00000080)
#define SCB_PDAWAKECFG_USBPLL_PD                  ((unsigned int) 0x00000100)
#define SCB_PDAWAKECFG_USBPLL_PD_MASK             ((unsigned int) 0x00000100)
#define SCB_PDAWAKECFG_USBPAD_PD                  ((unsigned int) 0x00000400)
#define SCB_PDAWAKECFG_USBPAD_PD_MASK             ((unsigned int) 0x00000400)

/*  PDRUNCFG (Power-down configuration register)
    The bits in the PDRUNCFG register control the power to the various analog blocks. This
    register can be written to at any time while the chip is running, and a write will take effect
    immediately with the exception of the power-down signal to the IRC.  Setting a 1 powers-down
    a peripheral and 0 enables it. */

#define SCB_PDRUNCFG_IRCOUT                       ((unsigned int) 0x00000001) // IRC oscillator output power-down
#define SCB_PDRUNCFG_IRCOUT_MASK                  ((unsigned int) 0x00000001)    
#define SCB_PDRUNCFG_IRC                          ((unsigned int) 0x00000002) // IRC oscillator power-down
#define SCB_PDRUNCFG_IRC_MASK                     ((unsigned int) 0x00000002)
#define SCB_PDRUNCFG_FLASH                        ((unsigned int) 0x00000004) // Flash power-down
#define SCB_PDRUNCFG_FLASH_MASK                   ((unsigned int) 0x00000004)
#define SCB_PDRUNCFG_BOD                          ((unsigned int) 0x00000008) // Brown-out detector power-down
#define SCB_PDRUNCFG_BOD_MASK                     ((unsigned int) 0x00000008)
#define SCB_PDRUNCFG_ADC                          ((unsigned int) 0x00000010) // ADC power-down
#define SCB_PDRUNCFG_ADC_MASK                     ((unsigned int) 0x00000010)
#define SCB_PDRUNCFG_SYSOSC                       ((unsigned int) 0x00000020) // System oscillator power-down
#define SCB_PDRUNCFG_SYSOSC_MASK                  ((unsigned int) 0x00000020)
#define SCB_PDRUNCFG_WDTOSC                       ((unsigned int) 0x00000040) // Watchdog oscillator power-down
#define SCB_PDRUNCFG_WDTOSC_MASK                  ((unsigned int) 0x00000040)
#define SCB_PDRUNCFG_SYSPLL                       ((unsigned int) 0x00000080) // System PLL power-down
#define SCB_PDRUNCFG_SYSPLL_MASK                  ((unsigned int) 0x00000080)
#define SCB_PDRUNCFG_USBPLL                       ((unsigned int) 0x00000100) // USB PLL power-down
#define SCB_PDRUNCFG_USBPLL_MASK                  ((unsigned int) 0x00000100)
#define SCB_PDRUNCFG_USBPAD                       ((unsigned int) 0x00000400) // USB PHY power-down
#define SCB_PDRUNCFG_USBPAD_MASK                  ((unsigned int) 0x00000400)

/*  DEVICE_ID (Device ID Register)
    This device ID register is a read-only register and contains the device ID for each
    LPC13xx part. This register is also read by the ISP/IAP commands. */

#define SCB_DEVICEID_LPC1311FHN33                 ((unsigned int) 0x2C42502B)
#define SCB_DEVICEID_LPC1313FHN33                 ((unsigned int) 0x2C40102B)
#define SCB_DEVICEID_LPC1313FBD48                 ((unsigned int) 0x2C40102B)
#define SCB_DEVICEID_LPC1342FHN33                 ((unsigned int) 0x3D01402B)
#define SCB_DEVICEID_LPC1343FHN33                 ((unsigned int) 0x3D00002B)
#define SCB_DEVICEID_LPC1343FBD48                 ((unsigned int) 0x3D00002B)

/*##############################################################################
## Data Watchpoint and Trace Unit (DWT)
##############################################################################*/
// For more information, see Cortex-M3 Technical Reference Manual 8.3
// This block is optional and not all comparators or functionality may
// be present on all chips, though basic DWT functionality is present
// on the LPC1343 since CYCNT works

#define DWT_CTRL                                  (*(pREG32 (0xE0001000)))    // Control register
#define DWT_CYCCNT                                (*(pREG32 (0xE0001004)))    // Cycle counter (useful for rough performance testing)
#define DWT_CPICNT                                (*(pREG32 (0xE0001008)))    // CPI Count Register
#define DWT_EXCCNT                                (*(pREG32 (0xE000100C)))    // Exception overhead count register
#define DWT_SLEEPCNT                              (*(pREG32 (0xE0001010)))    // Sleep count register
#define DWT_LSUCNT                                (*(pREG32 (0xE0001014)))    // LSU count register
#define DWT_FOLDCNT                               (*(pREG32 (0xE0001018)))    // Folder-instruction count register
#define DWT_PCSR                                  (*(pREG32 (0xE000101C)))    // Program counter sample register
#define DWT_COMP0                                 (*(pREG32 (0xE0001020)))    // Comparator register 0
#define DWT_MASK0                                 (*(pREG32 (0xE0001024)))    // Mask register 0
#define DWT_FUNCTION0                             (*(pREG32 (0xE0001028)))    // Function register 0
#define DWT_COMP1                                 (*(pREG32 (0xE0001030)))    // Comparator register 1
#define DWT_MASK1                                 (*(pREG32 (0xE0001034)))    // Mask register 1
#define DWT_FUNCTION1                             (*(pREG32 (0xE0001038)))    // Function register 1
#define DWT_COMP2                                 (*(pREG32 (0xE0001040)))    // Comparator register 2
#define DWT_MASK2                                 (*(pREG32 (0xE0001044)))    // Mask register 2
#define DWT_FUNCTION2                             (*(pREG32 (0xE0001048)))    // Function register 2
#define DWT_COMP3                                 (*(pREG32 (0xE0001050)))    // Comparator register 3
#define DWT_MASK3                                 (*(pREG32 (0xE0001054)))    // Mask register 3
#define DWT_FUNCTION3                             (*(pREG32 (0xE0001058)))    // Function register 3

/*##############################################################################
## Power Management Unit (PMU)
##############################################################################*/

#define PMU_BASE_ADDRESS                          (0x40038000)

#define PMU_PMUCTRL                               (*(pREG32 (0x40038000)))    // Power control register
#define PMU_GPREG0                                (*(pREG32 (0x40038004)))    // General purpose register 0
#define PMU_GPREG1                                (*(pREG32 (0x40038008)))    // General purpose register 1
#define PMU_GPREG2                                (*(pREG32 (0x4003800C)))    // General purpose register 2
#define PMU_GPREG3                                (*(pREG32 (0x40038010)))    // General purpose register 3
#define PMU_GPREG4                                (*(pREG32 (0x40038014)))    // General purpose register 4

#define PMU_PMUCTRL_DPDEN_MASK                    ((unsigned int) 0x00000002) // Deep power-down enable
#define PMU_PMUCTRL_DPDEN_DEEPPOWERDOWN           ((unsigned int) 0x00000002) // WFI will enter deep power-down mode
#define PMU_PMUCTRL_DPDEN_SLEEP                   ((unsigned int) 0x00000000) // WFI will enter sleep mode
#define PMU_PMUCTRL_DPDFLAG_MASK                  ((unsigned int) 0x00000800) // Deep power-down flag
#define PMU_PMUCTRL_DPDFLAG                       ((unsigned int) 0x00000800)

/*  GPREG0..3 (General purpose registers 0 to 3)
    The general purpose registers retain data through the Deep power-down mode when
    power is still applied to the VDD(3V3) pin but the chip has entered Deep power-down mode.
    Only a cold boot when all power has been completely removed from the chip will reset
    the general purpose registers.  */

#define PMU_GPREG0_GPDATA_MASK                    ((unsigned int) 0xFFFFFFFF)
#define PMU_GPREG1_GPDATA_MASK                    ((unsigned int) 0xFFFFFFFF)
#define PMU_GPREG2_GPDATA_MASK                    ((unsigned int) 0xFFFFFFFF)
#define PMU_GPREG3_GPDATA_MASK                    ((unsigned int) 0xFFFFFFFF)

/*  GPREG4 (General purpose register 4)
    The general purpose register 4 retains data through the Deep power-down mode when
    power is still applied to the VDD(3V3) pin but the chip has entered Deep power-down mode.
    Only a cold boot, when all power has been completely removed from the chip, will reset
    the general purpose registers.
      
    Remark: If the external voltage applied on pin VDD(3V3) drops below <tbd> V, the
    hysteresis of the WAKEUP input pin has to be disabled in order for the chip to wake up
    from Deep power-down mode.  */

#define PMU_GPREG4_GPDATA_MASK                    ((unsigned int) 0xFFFFF800)
#define PMU_GPREG4_WAKEUPHYS_MASK                 ((unsigned int) 0x00000400)
#define PMU_GPREG4_WAKEUPHYS_HYSTERESISENABLED    ((unsigned int) 0x00000400)
#define PMU_GPREG4_WAKEUPHYS_HYSTERESISDISABLED   ((unsigned int) 0x00000000)
#define PMU_GPREG4_GPDATA_MASK                    ((unsigned int) 0xFFFFF800)

/*##############################################################################
## I/O Control (IOCON)
##############################################################################*/

#define IOCON_BASE_ADDRESS                        (0x40044000)

/* Values that should be common to all pins, though they are also defined
   on the individual pin level in case they change with a pin on any future
   device */

#define IOCON_COMMON_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_COMMON_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_COMMON_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_COMMON_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_COMMON_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_COMMON_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_COMMON_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_COMMON_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_COMMON_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO2_6                              (*(pREG32 (0x40044000)))
#define IOCON_PIO2_6_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO2_6_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO2_6_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO2_6_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO2_6_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO2_6_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO2_6_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO2_6_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO2_6_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO2_6_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO2_0                              (*(pREG32 (0x40044008)))
#define IOCON_PIO2_0_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO2_0_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO2_0_FUNC_DTR                     ((unsigned int) 0x00000001)
#define IOCON_PIO2_0_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO2_0_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO2_0_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO2_0_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO2_0_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO2_0_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO2_0_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO2_0_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_nRESET_PIO0_0                       (*(pREG32 (0x4004400C)))
#define IOCON_nRESET_PIO0_0_FUNC_MASK             ((unsigned int) 0x00000007)
#define IOCON_nRESET_PIO0_0_FUNC_RESET            ((unsigned int) 0x00000000)
#define IOCON_nRESET_PIO0_0_FUNC_GPIO             ((unsigned int) 0x00000001)
#define IOCON_nRESET_PIO0_0_MODE_MASK             ((unsigned int) 0x00000018)
#define IOCON_nRESET_PIO0_0_MODE_INACTIVE         ((unsigned int) 0x00000000)
#define IOCON_nRESET_PIO0_0_MODE_PULLDOWN         ((unsigned int) 0x00000008)
#define IOCON_nRESET_PIO0_0_MODE_PULLUP           ((unsigned int) 0x00000010)
#define IOCON_nRESET_PIO0_0_MODE_REPEATER         ((unsigned int) 0x00000018)
#define IOCON_nRESET_PIO0_0_HYS_MASK              ((unsigned int) 0x00000020)
#define IOCON_nRESET_PIO0_0_HYS_DISABLE           ((unsigned int) 0x00000000)
#define IOCON_nRESET_PIO0_0_HYS_ENABLE            ((unsigned int) 0x00000020)

#define IOCON_PIO0_1                              (*(pREG32 (0x40044010)))
#define IOCON_PIO0_1_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO0_1_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO0_1_FUNC_CLKOUT                  ((unsigned int) 0x00000001)
#define IOCON_PIO0_1_FUNC_CT32B0_MAT2             ((unsigned int) 0x00000002)
#define IOCON_PIO0_1_FUNC_USB_FTOGGLE             ((unsigned int) 0x00000003)
#define IOCON_PIO0_1_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO0_1_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO0_1_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO0_1_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO0_1_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO0_1_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO0_1_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO0_1_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO1_8                              (*(pREG32 (0x40044014)))
#define IOCON_PIO1_8_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO1_8_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO1_8_FUNC_CT16B1_CAP0             ((unsigned int) 0x00000001)
#define IOCON_PIO1_8_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO1_8_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO1_8_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO1_8_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO1_8_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO1_8_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO1_8_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO1_8_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO0_2                              (*(pREG32 (0x4004401C)))
#define IOCON_PIO0_2_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO0_2_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO0_2_FUNC_SSEL                    ((unsigned int) 0x00000001)
#define IOCON_PIO0_2_FUNC_CT16B0_CAP0             ((unsigned int) 0x00000002)
#define IOCON_PIO0_2_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO0_2_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO0_2_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO0_2_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO0_2_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO0_2_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO0_2_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO0_2_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO2_7                              (*(pREG32 (0x40044020)))
#define IOCON_PIO2_7_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO2_7_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO2_7_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO2_7_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO2_7_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO2_7_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO2_7_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO2_7_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO2_7_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO2_7_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO2_8                              (*(pREG32 (0x40044024)))
#define IOCON_PIO2_8_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO2_8_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO2_8_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO2_8_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO2_8_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO2_8_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO2_8_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO2_8_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO2_8_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO2_8_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO2_1                              (*(pREG32 (0x40044028)))
#define IOCON_PIO2_1_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO2_1_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO2_1_FUNC_DSR                     ((unsigned int) 0x00000001)
#define IOCON_PIO2_1_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO2_1_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO2_1_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO2_1_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO2_1_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO2_1_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO2_1_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO2_1_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO0_3                              (*(pREG32 (0x4004402C)))
#define IOCON_PIO0_3_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO0_3_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO0_3_FUNC_USB_VBUS                ((unsigned int) 0x00000001)
#define IOCON_PIO0_3_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO0_3_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO0_3_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO0_3_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO0_3_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO0_3_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO0_3_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO0_3_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO0_4                              (*(pREG32 (0x40044030)))
#define IOCON_PIO0_4_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO0_4_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO0_4_FUNC_I2CSCL                  ((unsigned int) 0x00000001)
#define IOCON_PIO0_4_I2CMODE_MASK                 ((unsigned int) 0x00000300)
#define IOCON_PIO0_4_I2CMODE_STANDARDI2C          ((unsigned int) 0x00000000)
#define IOCON_PIO0_4_I2CMODE_STANDARDIO           ((unsigned int) 0x00000100)
#define IOCON_PIO0_4_I2CMODE_FASTPLUSI2C          ((unsigned int) 0x00000200)

#define IOCON_PIO0_5                              (*(pREG32 (0x40044034)))
#define IOCON_PIO0_5_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO0_5_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO0_5_FUNC_I2CSDA                  ((unsigned int) 0x00000001)
#define IOCON_PIO0_5_I2CMODE_MASK                 ((unsigned int) 0x00000300)
#define IOCON_PIO0_5_I2CMODE_STANDARDI2C          ((unsigned int) 0x00000000)
#define IOCON_PIO0_5_I2CMODE_STANDARDIO           ((unsigned int) 0x00000100)
#define IOCON_PIO0_5_I2CMODE_FASTPLUSI2C          ((unsigned int) 0x00000200)

#define IOCON_PIO1_9                              (*(pREG32 (0x40044038)))
#define IOCON_PIO1_9_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO1_9_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO1_9_FUNC_CT16B1_MAT0             ((unsigned int) 0x00000001)
#define IOCON_PIO1_9_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO1_9_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO1_9_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO1_9_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO1_9_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO1_9_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO1_9_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO1_9_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO3_4                              (*(pREG32 (0x4004403C)))
#define IOCON_PIO3_4_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO3_4_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO3_4_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO3_4_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO3_4_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO3_4_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO3_4_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO3_4_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO3_4_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO3_4_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO2_4                              (*(pREG32 (0x40044040)))
#define IOCON_PIO2_4_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO2_4_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO2_4_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO2_4_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO2_4_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO2_4_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO2_4_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO2_4_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO2_4_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO2_4_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO2_5                              (*(pREG32 (0x40044044)))
#define IOCON_PIO2_5_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO2_5_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO2_5_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO2_5_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO2_5_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO2_5_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO2_5_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO2_5_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO2_5_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO2_5_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO3_5                              (*(pREG32 (0x40044048)))
#define IOCON_PIO3_5_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO3_5_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO3_5_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO3_5_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO3_5_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO3_5_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO3_5_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO3_5_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO3_5_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO3_5_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO0_6                              (*(pREG32 (0x4004404C)))
#define IOCON_PIO0_6_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO0_6_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO0_6_FUNC_USB_CONNECT             ((unsigned int) 0x00000001)
#define IOCON_PIO0_6_FUNC_SCK                     ((unsigned int) 0x00000002)
#define IOCON_PIO0_6_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO0_6_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO0_6_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO0_6_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO0_6_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO0_6_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO0_6_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO0_6_HYS_ENABLE                   ((unsigned int) 0x00000020)


#define IOCON_PIO0_7                              (*(pREG32 (0x40044050)))
#define IOCON_PIO0_7_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO0_7_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO0_7_FUNC_CTS                     ((unsigned int) 0x00000001)
#define IOCON_PIO0_7_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO0_7_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO0_7_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO0_7_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO0_7_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO0_7_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO0_7_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO0_7_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO2_9                              (*(pREG32 (0x40044054)))
#define IOCON_PIO2_9_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO2_9_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO2_9_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO2_9_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO2_9_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO2_9_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO2_9_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO2_9_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO2_9_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO2_9_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO2_10                              (*(pREG32 (0x40044058)))
#define IOCON_PIO2_10_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO2_10_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO2_10_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO2_10_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO2_10_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO2_10_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO2_10_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO2_10_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO2_10_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO2_10_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO2_2 (*(pREG32 (0x4004405C)))
#define IOCON_PIO2_2_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO2_2_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO2_2_FUNC_DCD                     ((unsigned int) 0x00000001)
#define IOCON_PIO2_2_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO2_2_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO2_2_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO2_2_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO2_2_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO2_2_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO2_2_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO2_2_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO0_8                              (*(pREG32 (0x40044060)))
#define IOCON_PIO0_8_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO0_8_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO0_8_FUNC_MISO0                   ((unsigned int) 0x00000001)
#define IOCON_PIO0_8_FUNC_CT16B0_MAT0             ((unsigned int) 0x00000002)
#define IOCON_PIO0_8_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO0_8_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO0_8_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO0_8_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO0_8_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO0_8_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO0_8_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO0_8_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO0_9                              (*(pREG32 (0x40044064)))
#define IOCON_PIO0_9_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO0_9_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO0_9_FUNC_MOSI0                   ((unsigned int) 0x00000001)
#define IOCON_PIO0_9_FUNC_CT16B0_MAT1             ((unsigned int) 0x00000002)
#define IOCON_PIO0_9_FUNC_SWO                     ((unsigned int) 0x00000003)
#define IOCON_PIO0_9_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO0_9_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO0_9_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO0_9_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO0_9_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO0_9_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO0_9_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO0_9_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_JTAG_TCK_PIO0_10                    (*(pREG32 (0x40044068)))
#define IOCON_JTAG_TCK_PIO0_10_FUNC_MASK          ((unsigned int) 0x00000007)
#define IOCON_JTAG_TCK_PIO0_10_FUNC_SWCLK         ((unsigned int) 0x00000000)
#define IOCON_JTAG_TCK_PIO0_10_FUNC_GPIO          ((unsigned int) 0x00000001)
#define IOCON_JTAG_TCK_PIO0_10_FUNC_SCK           ((unsigned int) 0x00000002)
#define IOCON_JTAG_TCK_PIO0_10_FUNC_CT16B0_MAT2   ((unsigned int) 0x00000003)
#define IOCON_JTAG_TCK_PIO0_10_MODE_MASK          ((unsigned int) 0x00000018)
#define IOCON_JTAG_TCK_PIO0_10_MODE_INACTIVE      ((unsigned int) 0x00000000)
#define IOCON_JTAG_TCK_PIO0_10_MODE_PULLDOWN      ((unsigned int) 0x00000008)
#define IOCON_JTAG_TCK_PIO0_10_MODE_PULLUP        ((unsigned int) 0x00000010)
#define IOCON_JTAG_TCK_PIO0_10_MODE_REPEATER      ((unsigned int) 0x00000018)
#define IOCON_JTAG_TCK_PIO0_10_HYS_MASK           ((unsigned int) 0x00000020)
#define IOCON_JTAG_TCK_PIO0_10_HYS_DISABLE        ((unsigned int) 0x00000000)
#define IOCON_JTAG_TCK_PIO0_10_HYS_ENABLE         ((unsigned int) 0x00000020)

#define IOCON_PIO1_10                             (*(pREG32 (0x4004406C)))
#define IOCON_PIO1_10_FUNC_MASK                   ((unsigned int) 0x00000007)
#define IOCON_PIO1_10_FUNC_GPIO                   ((unsigned int) 0x00000000)
#define IOCON_PIO1_10_FUNC_AD6                    ((unsigned int) 0x00000001)
#define IOCON_PIO1_10_FUNC_CT16B1_MAT1            ((unsigned int) 0x00000002)
#define IOCON_PIO1_10_MODE_MASK                   ((unsigned int) 0x00000018)
#define IOCON_PIO1_10_MODE_INACTIVE               ((unsigned int) 0x00000000)
#define IOCON_PIO1_10_MODE_PULLDOWN               ((unsigned int) 0x00000008)
#define IOCON_PIO1_10_MODE_PULLUP                 ((unsigned int) 0x00000010)
#define IOCON_PIO1_10_MODE_REPEATER               ((unsigned int) 0x00000018)
#define IOCON_PIO1_10_HYS_MASK                    ((unsigned int) 0x00000020)
#define IOCON_PIO1_10_HYS_DISABLE                 ((unsigned int) 0x00000000)
#define IOCON_PIO1_10_HYS_ENABLE                  ((unsigned int) 0x00000020)
#define IOCON_PIO1_10_ADMODE_MASK                 ((unsigned int) 0x00000080)
#define IOCON_PIO1_10_ADMODE_ANALOG               ((unsigned int) 0x00000000)
#define IOCON_PIO1_10_ADMODE_DIGITAL              ((unsigned int) 0x00000080)

#define IOCON_PIO2_11                             (*(pREG32 (0x40044070)))
#define IOCON_PIO2_11_FUNC_MASK                   ((unsigned int) 0x00000007)
#define IOCON_PIO2_11_FUNC_GPIO                   ((unsigned int) 0x00000000)
#define IOCON_PIO2_11_FUNC_SCK0                   ((unsigned int) 0x00000001)
#define IOCON_PIO2_11_MODE_MASK                   ((unsigned int) 0x00000018)
#define IOCON_PIO2_11_MODE_INACTIVE               ((unsigned int) 0x00000000)
#define IOCON_PIO2_11_MODE_PULLDOWN               ((unsigned int) 0x00000008)
#define IOCON_PIO2_11_MODE_PULLUP                 ((unsigned int) 0x00000010)
#define IOCON_PIO2_11_MODE_REPEATER               ((unsigned int) 0x00000018)
#define IOCON_PIO2_11_HYS_MASK                    ((unsigned int) 0x00000020)
#define IOCON_PIO2_11_HYS_DISABLE                 ((unsigned int) 0x00000000)
#define IOCON_PIO2_11_HYS_ENABLE                  ((unsigned int) 0x00000020)

#define IOCON_JTAG_TDI_PIO0_11                    (*(pREG32 (0x40044074)))
#define IOCON_JTAG_TDI_PIO0_11_FUNC_MASK          ((unsigned int) 0x00000007)
#define IOCON_JTAG_TDI_PIO0_11_FUNC_TDI           ((unsigned int) 0x00000000)
#define IOCON_JTAG_TDI_PIO0_11_FUNC_GPIO          ((unsigned int) 0x00000001)
#define IOCON_JTAG_TDI_PIO0_11_FUNC_AD0           ((unsigned int) 0x00000002)
#define IOCON_JTAG_TDI_PIO0_11_FUNC_CT32B0_MAT3   ((unsigned int) 0x00000003)
#define IOCON_JTAG_TDI_PIO0_11_MODE_MASK          ((unsigned int) 0x00000018)
#define IOCON_JTAG_TDI_PIO0_11_MODE_INACTIVE      ((unsigned int) 0x00000000)
#define IOCON_JTAG_TDI_PIO0_11_MODE_PULLDOWN      ((unsigned int) 0x00000008)
#define IOCON_JTAG_TDI_PIO0_11_MODE_PULLUP        ((unsigned int) 0x00000010)
#define IOCON_JTAG_TDI_PIO0_11_MODE_REPEATER      ((unsigned int) 0x00000018)
#define IOCON_JTAG_TDI_PIO0_11_HYS_MASK           ((unsigned int) 0x00000020)
#define IOCON_JTAG_TDI_PIO0_11_HYS_DISABLE        ((unsigned int) 0x00000000)
#define IOCON_JTAG_TDI_PIO0_11_HYS_ENABLE         ((unsigned int) 0x00000020)
#define IOCON_JTAG_TDI_PIO0_11_ADMODE_MASK        ((unsigned int) 0x00000080)
#define IOCON_JTAG_TDI_PIO0_11_ADMODE_ANALOG      ((unsigned int) 0x00000000)
#define IOCON_JTAG_TDI_PIO0_11_ADMODE_DIGITAL     ((unsigned int) 0x00000080)

#define IOCON_JTAG_TMS_PIO1_0                     (*(pREG32 (0x40044078)))
#define IOCON_JTAG_TMS_PIO1_0_FUNC_MASK           ((unsigned int) 0x00000007)
#define IOCON_JTAG_TMS_PIO1_0_FUNC_TMS            ((unsigned int) 0x00000000)
#define IOCON_JTAG_TMS_PIO1_0_FUNC_GPIO           ((unsigned int) 0x00000001)
#define IOCON_JTAG_TMS_PIO1_0_FUNC_AD1            ((unsigned int) 0x00000002)
#define IOCON_JTAG_TMS_PIO1_0_FUNC_CT32B1_CAP0    ((unsigned int) 0x00000003)
#define IOCON_JTAG_TMS_PIO1_0_MODE_MASK           ((unsigned int) 0x00000018)
#define IOCON_JTAG_TMS_PIO1_0_MODE_INACTIVE       ((unsigned int) 0x00000000)
#define IOCON_JTAG_TMS_PIO1_0_MODE_PULLDOWN       ((unsigned int) 0x00000008)
#define IOCON_JTAG_TMS_PIO1_0_MODE_PULLUP         ((unsigned int) 0x00000010)
#define IOCON_JTAG_TMS_PIO1_0_MODE_REPEATER       ((unsigned int) 0x00000018)
#define IOCON_JTAG_TMS_PIO1_0_HYS_MASK            ((unsigned int) 0x00000020)
#define IOCON_JTAG_TMS_PIO1_0_HYS_DISABLE         ((unsigned int) 0x00000000)
#define IOCON_JTAG_TMS_PIO1_0_HYS_ENABLE          ((unsigned int) 0x00000020)
#define IOCON_JTAG_TMS_PIO1_0_ADMODE_MASK         ((unsigned int) 0x00000080)
#define IOCON_JTAG_TMS_PIO1_0_ADMODE_ANALOG       ((unsigned int) 0x00000000)
#define IOCON_JTAG_TMS_PIO1_0_ADMODE_DIGITAL      ((unsigned int) 0x00000080)

#define IOCON_JTAG_TDO_PIO1_1                     (*(pREG32 (0x4004407C)))
#define IOCON_JTAG_TDO_PIO1_1_FUNC_MASK           ((unsigned int) 0x00000007)
#define IOCON_JTAG_TDO_PIO1_1_FUNC_TDO            ((unsigned int) 0x00000000)
#define IOCON_JTAG_TDO_PIO1_1_FUNC_GPIO           ((unsigned int) 0x00000001)
#define IOCON_JTAG_TDO_PIO1_1_FUNC_AD2            ((unsigned int) 0x00000002)
#define IOCON_JTAG_TDO_PIO1_1_FUNC_CT32B1_MAT0    ((unsigned int) 0x00000003)
#define IOCON_JTAG_TDO_PIO1_1_MODE_MASK           ((unsigned int) 0x00000018)
#define IOCON_JTAG_TDO_PIO1_1_MODE_INACTIVE       ((unsigned int) 0x00000000)
#define IOCON_JTAG_TDO_PIO1_1_MODE_PULLDOWN       ((unsigned int) 0x00000008)
#define IOCON_JTAG_TDO_PIO1_1_MODE_PULLUP         ((unsigned int) 0x00000010)
#define IOCON_JTAG_TDO_PIO1_1_MODE_REPEATER       ((unsigned int) 0x00000018)
#define IOCON_JTAG_TDO_PIO1_1_HYS_MASK            ((unsigned int) 0x00000020)
#define IOCON_JTAG_TDO_PIO1_1_HYS_DISABLE         ((unsigned int) 0x00000000)
#define IOCON_JTAG_TDO_PIO1_1_HYS_ENABLE          ((unsigned int) 0x00000020)
#define IOCON_JTAG_TDO_PIO1_1_ADMODE_MASK         ((unsigned int) 0x00000080)
#define IOCON_JTAG_TDO_PIO1_1_ADMODE_ANALOG       ((unsigned int) 0x00000000)
#define IOCON_JTAG_TDO_PIO1_1_ADMODE_DIGITAL      ((unsigned int) 0x00000080)

#define IOCON_JTAG_nTRST_PIO1_2                   (*(pREG32 (0x40044080)))
#define IOCON_JTAG_nTRST_PIO1_2_FUNC_MASK         ((unsigned int) 0x00000007)
#define IOCON_JTAG_nTRST_PIO1_2_FUNC_TRST         ((unsigned int) 0x00000000)
#define IOCON_JTAG_nTRST_PIO1_2_FUNC_GPIO         ((unsigned int) 0x00000001)
#define IOCON_JTAG_nTRST_PIO1_2_FUNC_AD3          ((unsigned int) 0x00000002)
#define IOCON_JTAG_nTRST_PIO1_2_FUNC_CT32B1_MAT1  ((unsigned int) 0x00000003)
#define IOCON_JTAG_nTRST_PIO1_2_MODE_MASK         ((unsigned int) 0x00000018)
#define IOCON_JTAG_nTRST_PIO1_2_MODE_INACTIVE     ((unsigned int) 0x00000000)
#define IOCON_JTAG_nTRST_PIO1_2_MODE_PULLDOWN     ((unsigned int) 0x00000008)
#define IOCON_JTAG_nTRST_PIO1_2_MODE_PULLUP       ((unsigned int) 0x00000010)
#define IOCON_JTAG_nTRST_PIO1_2_MODE_REPEATER     ((unsigned int) 0x00000018)
#define IOCON_JTAG_nTRST_PIO1_2_HYS_MASK          ((unsigned int) 0x00000020)
#define IOCON_JTAG_nTRST_PIO1_2_HYS_DISABLE       ((unsigned int) 0x00000000)
#define IOCON_JTAG_nTRST_PIO1_2_HYS_ENABLE        ((unsigned int) 0x00000020)
#define IOCON_JTAG_nTRST_PIO1_2_ADMODE_MASK       ((unsigned int) 0x00000080)
#define IOCON_JTAG_nTRST_PIO1_2_ADMODE_ANALOG     ((unsigned int) 0x00000000)
#define IOCON_JTAG_nTRST_PIO1_2_ADMODE_DIGITAL    ((unsigned int) 0x00000080)

#define IOCON_PIO3_0                              (*(pREG32 (0x40044084)))
#define IOCON_PIO3_0_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO3_0_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO3_0_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO3_0_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO3_0_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO3_0_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO3_0_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO3_0_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO3_0_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO3_0_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO3_1                              (*(pREG32 (0x40044088)))
#define IOCON_PIO3_1_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO3_1_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO3_1_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO3_1_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO3_1_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO3_1_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO3_1_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO3_1_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO3_1_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO3_1_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO2_3 (*(pREG32 (0x4004408C)))
#define IOCON_PIO2_3_FUNC_MASK 0x7
#define IOCON_PIO2_3_MODE_MASK 0x18
#define IOCON_PIO2_3_HYS_MASK 0x20
#define IOCON_PIO2_3_HYS 0x20

#define IOCON_SWDIO_PIO1_3                        (*(pREG32 (0x40044090)))
#define IOCON_SWDIO_PIO1_3_FUNC_MASK              ((unsigned int) 0x00000007)
#define IOCON_SWDIO_PIO1_3_FUNC_SWDIO             ((unsigned int) 0x00000000)
#define IOCON_SWDIO_PIO1_3_FUNC_GPIO              ((unsigned int) 0x00000001)
#define IOCON_SWDIO_PIO1_3_FUNC_AD4               ((unsigned int) 0x00000002)
#define IOCON_SWDIO_PIO1_3_FUNC_CT32B1_MAT2       ((unsigned int) 0x00000004)
#define IOCON_SWDIO_PIO1_3_HYS_MASK               ((unsigned int) 0x00000020)
#define IOCON_SWDIO_PIO1_3_HYS_DISABLE            ((unsigned int) 0x00000000)
#define IOCON_SWDIO_PIO1_3_HYS_ENABLE             ((unsigned int) 0x00000020)
#define IOCON_SWDIO_PIO1_3_ADMODE_MASK            ((unsigned int) 0x00000080)
#define IOCON_SWDIO_PIO1_3_ADMODE_ANALOG          ((unsigned int) 0x00000000)
#define IOCON_SWDIO_PIO1_3_ADMODE_DIGITAL         ((unsigned int) 0x00000080)

#define IOCON_PIO1_4                              (*(pREG32 (0x40044094)))
#define IOCON_PIO1_4_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO1_4_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO1_4_FUNC_AD5                     ((unsigned int) 0x00000001)
#define IOCON_PIO1_4_FUNC_CT32B1_MAT3             ((unsigned int) 0x00000002)
#define IOCON_PIO1_4_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO1_4_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO1_4_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO1_4_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO1_4_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO1_4_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO1_4_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO1_4_HYS_ENABLE                   ((unsigned int) 0x00000020)
#define IOCON_PIO1_4_ADMODE_MASK                  ((unsigned int) 0x00000080)
#define IOCON_PIO1_4_ADMODE_ANALOG                ((unsigned int) 0x00000000)
#define IOCON_PIO1_4_ADMODE_DIGITAL               ((unsigned int) 0x00000080)

#define IOCON_PIO1_11                             (*(pREG32 (0x40044098)))
#define IOCON_PIO1_11_FUNC_MASK                   ((unsigned int) 0x00000007)
#define IOCON_PIO1_11_FUNC_GPIO                   ((unsigned int) 0x00000000)
#define IOCON_PIO1_11_FUNC_AD7                    ((unsigned int) 0x00000001)
#define IOCON_PIO1_11_MODE_MASK                   ((unsigned int) 0x00000018)
#define IOCON_PIO1_11_MODE_INACTIVE               ((unsigned int) 0x00000000)
#define IOCON_PIO1_11_MODE_PULLDOWN               ((unsigned int) 0x00000008)
#define IOCON_PIO1_11_MODE_PULLUP                 ((unsigned int) 0x00000010)
#define IOCON_PIO1_11_MODE_REPEATER               ((unsigned int) 0x00000018)
#define IOCON_PIO1_11_HYS_MASK                    ((unsigned int) 0x00000020)
#define IOCON_PIO1_11_HYS_DISABLE                 ((unsigned int) 0x00000000)
#define IOCON_PIO1_11_HYS_ENABLE                  ((unsigned int) 0x00000020)
#define IOCON_PIO1_11_ADMODE_MASK                 ((unsigned int) 0x00000080)
#define IOCON_PIO1_11_ADMODE_ANALOG               ((unsigned int) 0x00000000)
#define IOCON_PIO1_11_ADMODE_DIGITAL              ((unsigned int) 0x00000080)

#define IOCON_PIO3_2                              (*(pREG32 (0x4004409C)))
#define IOCON_PIO3_2_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO3_2_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO3_2_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO3_2_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO3_2_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO3_2_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO3_2_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO3_2_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO3_2_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO3_2_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO1_5                              (*(pREG32 (0x400440A0)))
#define IOCON_PIO1_5_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO1_5_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO1_5_FUNC_RTS                     ((unsigned int) 0x00000001)
#define IOCON_PIO1_5_FUNC_CT32B0_CAP0             ((unsigned int) 0x00000002)
#define IOCON_PIO1_5_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO1_5_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO1_5_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO1_5_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO1_5_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO1_5_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO1_5_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO1_5_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO1_6                              (*(pREG32 (0x400440A4)))
#define IOCON_PIO1_6_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO1_6_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO1_6_FUNC_UART_RXD                ((unsigned int) 0x00000001)
#define IOCON_PIO1_6_FUNC_CT32B0_MAT0             ((unsigned int) 0x00000002)
#define IOCON_PIO1_6_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO1_6_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO1_6_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO1_6_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO1_6_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO1_6_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO1_6_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO1_6_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO1_7                              (*(pREG32 (0x400440A8)))
#define IOCON_PIO1_7_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO1_7_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO1_7_FUNC_UART_TXD                ((unsigned int) 0x00000001)
#define IOCON_PIO1_7_FUNC_CT32B0_MAT1             ((unsigned int) 0x00000002)
#define IOCON_PIO1_7_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO1_7_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO1_7_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO1_7_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO1_7_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO1_7_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO1_7_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO1_7_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_PIO3_3                              (*(pREG32 (0x400440AC)))
#define IOCON_PIO3_3_FUNC_MASK                    ((unsigned int) 0x00000007)
#define IOCON_PIO3_3_FUNC_GPIO                    ((unsigned int) 0x00000000)
#define IOCON_PIO3_3_MODE_MASK                    ((unsigned int) 0x00000018)
#define IOCON_PIO3_3_MODE_INACTIVE                ((unsigned int) 0x00000000)
#define IOCON_PIO3_3_MODE_PULLDOWN                ((unsigned int) 0x00000008)
#define IOCON_PIO3_3_MODE_PULLUP                  ((unsigned int) 0x00000010)
#define IOCON_PIO3_3_MODE_REPEATER                ((unsigned int) 0x00000018)
#define IOCON_PIO3_3_HYS_MASK                     ((unsigned int) 0x00000020)
#define IOCON_PIO3_3_HYS_DISABLE                  ((unsigned int) 0x00000000)
#define IOCON_PIO3_3_HYS_ENABLE                   ((unsigned int) 0x00000020)

#define IOCON_SCKLOC                              (*(pREG32 (0x400440B0)))
#define IOCON_SCKLOC_SCKPIN_MASK                  ((unsigned int) 0x00000003)
#define IOCON_SCKLOC_SCKPIN_PIO0_10               ((unsigned int) 0x00000000) // Set SCK function to pin 0.10
#define IOCON_SCKLOC_SCKPIN_PIO2_11               ((unsigned int) 0x00000001) // Set SCK function to pin 2.11
#define IOCON_SCKLOC_SCKPIN_PIO0_6                ((unsigned int) 0x00000003) // Set SCK function to pin 0.6

#define IOCON_SWCLK_PIO0_10                       (*(pREG32 (0x40044068)))

/*##############################################################################
## Nested Vectored Interrupt Controller
##############################################################################*/

#define NVIC_BASE_ADDRESS                         (0xE000E100)

typedef struct
{
  volatile uint32_t ISER[8];                      /*!< Offset: 0x000  Interrupt Set Enable Register           */
	 uint32_t RESERVED0[24];                                   
  volatile uint32_t ICER[8];                      /*!< Offset: 0x080  Interrupt Clear Enable Register         */
	  uint32_t RSERVED1[24];                                    
  volatile uint32_t ISPR[8];                      /*!< Offset: 0x100  Interrupt Set Pending Register          */
	 uint32_t RESERVED2[24];                                   
  volatile uint32_t ICPR[8];                      /*!< Offset: 0x180  Interrupt Clear Pending Register        */
	 uint32_t RESERVED3[24];                                   
  volatile uint32_t IABR[8];                      /*!< Offset: 0x200  Interrupt Active bit Register           */
	 uint32_t RESERVED4[56];                                   
  volatile uint8_t  IP[240];                      /*!< Offset: 0x300  Interrupt Priority Register (8Bit wide) */
	uint32_t RESERVED5[644];                                  
  volatile  uint32_t STIR;                        /*!< Offset: 0xE00  Software Trigger Interrupt Register     */
}  NVIC_Type;                                               

#define NVIC                                      ((NVIC_Type *) NVIC_BASE_ADDRESS)

static inline void __enable_irq()                 { __asm volatile ("cpsie i"); }
static inline void __disable_irq()                { __asm volatile ("cpsid i"); }

typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn           = -14,      /*!< 2 Non Maskable Interrupt                         */
  MemoryManagement_IRQn         = -12,      /*!< 4 Cortex-M3 Memory Management Interrupt          */
  BusFault_IRQn                 = -11,      /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
  UsageFault_IRQn               = -10,      /*!< 6 Cortex-M3 Usage Fault Interrupt                */
  SVCall_IRQn                   = -5,       /*!< 11 Cortex-M3 SV Call Interrupt                   */
  DebugMonitor_IRQn             = -4,       /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
  PendSV_IRQn                   = -2,       /*!< 14 Cortex-M3 Pend SV Interrupt                   */
  SysTick_IRQn                  = -1,       /*!< 15 Cortex-M3 System Tick Interrupt               */

/******  LPC13xx Specific Interrupt Numbers *******************************************************/
  WAKEUP0_IRQn                  = 0,        /*!< All I/O pins can be used as wakeup source.       */
  WAKEUP1_IRQn                  = 1,        /*!< There are 40 pins in total for LPC17xx           */
  WAKEUP2_IRQn                  = 2,
  WAKEUP3_IRQn                  = 3,
  WAKEUP4_IRQn                  = 4,   
  WAKEUP5_IRQn                  = 5,        
  WAKEUP6_IRQn                  = 6,        
  WAKEUP7_IRQn                  = 7,        
  WAKEUP8_IRQn                  = 8,        
  WAKEUP9_IRQn                  = 9,        
  WAKEUP10_IRQn                 = 10,       
  WAKEUP11_IRQn                 = 11,       
  WAKEUP12_IRQn                 = 12,       
  WAKEUP13_IRQn                 = 13,       
  WAKEUP14_IRQn                 = 14,       
  WAKEUP15_IRQn                 = 15,       
  WAKEUP16_IRQn                 = 16,       
  WAKEUP17_IRQn                 = 17,       
  WAKEUP18_IRQn                 = 18,       
  WAKEUP19_IRQn                 = 19,       
  WAKEUP20_IRQn                 = 20,       
  WAKEUP21_IRQn                 = 21,       
  WAKEUP22_IRQn                 = 22,       
  WAKEUP23_IRQn                 = 23,       
  WAKEUP24_IRQn                 = 24,       
  WAKEUP25_IRQn                 = 25,       
  WAKEUP26_IRQn                 = 26,       
  WAKEUP27_IRQn                 = 27,       
  WAKEUP28_IRQn                 = 28,       
  WAKEUP29_IRQn                 = 29,       
  WAKEUP30_IRQn                 = 30,       
  WAKEUP31_IRQn                 = 31,       
  WAKEUP32_IRQn                 = 32,       
  WAKEUP33_IRQn                 = 33,       
  WAKEUP34_IRQn                 = 34,       
  WAKEUP35_IRQn                 = 35,       
  WAKEUP36_IRQn                 = 36,       
  WAKEUP37_IRQn                 = 37,       
  WAKEUP38_IRQn                 = 38,       
  WAKEUP39_IRQn                 = 39,       
  I2C_IRQn                      = 40,       /*!< I2C Interrupt                                    */
  TIMER_16_0_IRQn               = 41,       /*!< 16-bit Timer0 Interrupt                          */
  TIMER_16_1_IRQn               = 42,       /*!< 16-bit Timer1 Interrupt                          */
  TIMER_32_0_IRQn               = 43,       /*!< 32-bit Timer0 Interrupt                          */
  TIMER_32_1_IRQn               = 44,       /*!< 32-bit Timer1 Interrupt                          */
  SSP_IRQn                      = 45,       /*!< SSP Interrupt                                    */
  UART_IRQn                     = 46,       /*!< UART Interrupt                                   */
  USB_IRQn                      = 47,       /*!< USB Regular Interrupt                            */
  USB_FIQn                      = 48,       /*!< USB Fast Interrupt                               */
  ADC_IRQn                      = 49,       /*!< A/D Converter Interrupt                          */
  WDT_IRQn                      = 50,       /*!< Watchdog timer Interrupt                         */  
  BOD_IRQn                      = 51,       /*!< Brown Out Detect(BOD) Interrupt                  */
  EINT3_IRQn                    = 53,       /*!< External Interrupt 3 Interrupt                   */
  EINT2_IRQn                    = 54,       /*!< External Interrupt 2 Interrupt                   */
  EINT1_IRQn                    = 55,       /*!< External Interrupt 1 Interrupt                   */
  EINT0_IRQn                    = 56,       /*!< External Interrupt 0 Interrupt                   */
} IRQn_t;

static inline void NVIC_EnableIRQ(IRQn_t IRQn)
{
  NVIC->ISER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

static inline void NVIC_DisableIRQ(IRQn_t IRQn)
{
  NVIC->ICER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/*##############################################################################
## GPIO - General Purpose I/O
##############################################################################*/

#define GPIO_GPIO0_BASE                           (0x50000000)
#define GPIO_GPIO1_BASE                           (0x50010000)
#define GPIO_GPIO2_BASE                           (0x50020000)
#define GPIO_GPIO3_BASE                           (0x50030000)

#define GPIO_GPIO0DATA                            (*(pREG32 (0x50003FFC)))    // Port data register
#define GPIO_GPIO0DIR                             (*(pREG32 (0x50008000)))    // Data direction register
#define GPIO_GPIO0IS                              (*(pREG32 (0x50008004)))    // Interrupt sense register
#define GPIO_GPIO0IBE                             (*(pREG32 (0x50008008)))    // Interrupt both edges register
#define GPIO_GPIO0IEV                             (*(pREG32 (0x5000800C)))    // Interrupt event register
#define GPIO_GPIO0IE                              (*(pREG32 (0x50008010)))    // Interrupt mask register
#define GPIO_GPIO0RIS                             (*(pREG32 (0x50008014)))    // Raw interrupt status register
#define GPIO_GPIO0MIS                             (*(pREG32 (0x50008018)))    // Masked interrupt status register
#define GPIO_GPIO0IC                              (*(pREG32 (0x5000801C)))    // Interrupt clear register

#define GPIO_GPIO1DATA                            (*(pREG32 (0x50013FFC)))    // Port data register
#define GPIO_GPIO1DIR                             (*(pREG32 (0x50018000)))    // Data direction register
#define GPIO_GPIO1IS                              (*(pREG32 (0x50018004)))    // Interrupt sense register
#define GPIO_GPIO1IBE                             (*(pREG32 (0x50018008)))    // Interrupt both edges register
#define GPIO_GPIO1IEV                             (*(pREG32 (0x5001800C)))    // Interrupt event register
#define GPIO_GPIO1IE                              (*(pREG32 (0x50018010)))    // Interrupt mask register
#define GPIO_GPIO1RIS                             (*(pREG32 (0x50018014)))    // Raw interrupt status register
#define GPIO_GPIO1MIS                             (*(pREG32 (0x50018018)))    // Masked interrupt status register
#define GPIO_GPIO1IC                              (*(pREG32 (0x5001801C)))    // Interrupt clear register

#define GPIO_GPIO2DATA                            (*(pREG32 (0x50023FFC)))    // Port data register
#define GPIO_GPIO2DIR                             (*(pREG32 (0x50028000)))    // Data direction register
#define GPIO_GPIO2IS                              (*(pREG32 (0x50028004)))    // Interrupt sense register
#define GPIO_GPIO2IBE                             (*(pREG32 (0x50028008)))    // Interrupt both edges register
#define GPIO_GPIO2IEV                             (*(pREG32 (0x5002800C)))    // Interrupt event register
#define GPIO_GPIO2IE                              (*(pREG32 (0x50028010)))    // Interrupt mask register
#define GPIO_GPIO2RIS                             (*(pREG32 (0x50028014)))    // Raw interrupt status register
#define GPIO_GPIO2MIS                             (*(pREG32 (0x50028018)))    // Masked interrupt status register
#define GPIO_GPIO2IC                              (*(pREG32 (0x5002801C)))    // Interrupt clear register

#define GPIO_GPIO3DATA                            (*(pREG32 (0x50033FFC)))    // Port data register
#define GPIO_GPIO3DIR                             (*(pREG32 (0x50038000)))    // Data direction register
#define GPIO_GPIO3IS                              (*(pREG32 (0x50038004)))    // Interrupt sense register
#define GPIO_GPIO3IBE                             (*(pREG32 (0x50038008)))    // Interrupt both edges register
#define GPIO_GPIO3IEV                             (*(pREG32 (0x5003800C)))    // Interrupt event register
#define GPIO_GPIO3IE                              (*(pREG32 (0x50038010)))    // Interrupt mask register
#define GPIO_GPIO3RIS                             (*(pREG32 (0x50038014)))    // Raw interrupt status register
#define GPIO_GPIO3MIS                             (*(pREG32 (0x50038018)))    // Masked interrupt status register
#define GPIO_GPIO3IC                              (*(pREG32 (0x5003801C)))    // Interrupt clear register

#define GPIO_IO_P0                                ((unsigned int) 0x00000001)
#define GPIO_IO_P1                                ((unsigned int) 0x00000002)
#define GPIO_IO_P2                                ((unsigned int) 0x00000004)
#define GPIO_IO_P3                                ((unsigned int) 0x00000008)
#define GPIO_IO_P4                                ((unsigned int) 0x00000010)
#define GPIO_IO_P5                                ((unsigned int) 0x00000020)
#define GPIO_IO_P6                                ((unsigned int) 0x00000040)
#define GPIO_IO_P7                                ((unsigned int) 0x00000080)
#define GPIO_IO_P8                                ((unsigned int) 0x00000100)
#define GPIO_IO_P9                                ((unsigned int) 0x00000200)
#define GPIO_IO_P10                               ((unsigned int) 0x00000400)
#define GPIO_IO_P11                               ((unsigned int) 0x00000800)
#define GPIO_IO_ALL                               ((unsigned int) 0x00000FFF)

/*##############################################################################
## USB
##############################################################################*/

/* USB registers are defined in USB code */
#define USB_BASE_ADDRESS                          (0x40020000)

/* USB Device Interrupt Status Register */
#define USB_DEVINTST                              (*(pREG32 (0x40020000)))
#define USB_DEVINTST_FRAME_MASK                   ((unsigned int) 0x00000001) 
#define USB_DEVINTST_FRAME                        ((unsigned int) 0x00000001) // Frame interrupt
#define USB_DEVINTST_EP0_MASK                     ((unsigned int) 0x00000002) 
#define USB_DEVINTST_EP0                          ((unsigned int) 0x00000002) // USB core interrupt for EP0
#define USB_DEVINTST_EP1_MASK                     ((unsigned int) 0x00000004) 
#define USB_DEVINTST_EP1                          ((unsigned int) 0x00000004) // USB core interrupt for EP1
#define USB_DEVINTST_EP2_MASK                     ((unsigned int) 0x00000008) 
#define USB_DEVINTST_EP2                          ((unsigned int) 0x00000008) // USB core interrupt for EP2
#define USB_DEVINTST_EP3_MASK                     ((unsigned int) 0x00000010)
#define USB_DEVINTST_EP3                          ((unsigned int) 0x00000010) // USB core interrupt for EP3
#define USB_DEVINTST_EP4_MASK                     ((unsigned int) 0x00000020)
#define USB_DEVINTST_EP4                          ((unsigned int) 0x00000020) // USB core interrupt for EP4
#define USB_DEVINTST_EP5_MASK                     ((unsigned int) 0x00000040)
#define USB_DEVINTST_EP5                          ((unsigned int) 0x00000040) // USB core interrupt for EP5
#define USB_DEVINTST_EP6_MASK                     ((unsigned int) 0x00000080)
#define USB_DEVINTST_EP6                          ((unsigned int) 0x00000080) // USB core interrupt for EP6
#define USB_DEVINTST_EP7_MASK                     ((unsigned int) 0x00000100)
#define USB_DEVINTST_EP7                          ((unsigned int) 0x00000100) // USB core interrupt for EP7
#define USB_DEVINTST_DEV_START_MASK               ((unsigned int) 0x00000200)
#define USB_DEVINTST_DEV_START                    ((unsigned int) 0x00000200)
#define USB_DEVINTST_CC_EMPTY_MASK                ((unsigned int) 0x00000400)
#define USB_DEVINTST_CC_EMPTY                     ((unsigned int) 0x00000400)
#define USB_DEVINTST_CD_FULL_MASK                 ((unsigned int) 0x00000800)
#define USB_DEVINTST_CD_FULL                      ((unsigned int) 0x00000800)
#define USB_DEVINTST_RxENDPKT_MASK                ((unsigned int) 0x00001000)
#define USB_DEVINTST_RxENDPKT                     ((unsigned int) 0x00001000)
#define USB_DEVINTST_TxENDPKT_MASK                ((unsigned int) 0x00002000)
#define USB_DEVINTST_TxENDPKT                     ((unsigned int) 0x00002000)

/* USB Device Interrupt Enable Register  */
#define USB_DEVINTEN                              (*(pREG32 (0x40020004)))
#define USB_DEVINTEN_FRAME_MASK                   ((unsigned int) 0x00000001)
#define USB_DEVINTEN_FRAME                        ((unsigned int) 0x00000001)
#define USB_DEVINTEN_EP0_MASK                     ((unsigned int) 0x00000002)
#define USB_DEVINTEN_EP0                          ((unsigned int) 0x00000002)
#define USB_DEVINTEN_EP1_MASK                     ((unsigned int) 0x00000004)
#define USB_DEVINTEN_EP1                          ((unsigned int) 0x00000004)
#define USB_DEVINTEN_EP2_MASK                     ((unsigned int) 0x00000008)
#define USB_DEVINTEN_EP2                          ((unsigned int) 0x00000008)
#define USB_DEVINTEN_EP3_MASK                     ((unsigned int) 0x00000010)
#define USB_DEVINTEN_EP3                          ((unsigned int) 0x00000010)
#define USB_DEVINTEN_EP4_MASK                     ((unsigned int) 0x00000020)
#define USB_DEVINTEN_EP4                          ((unsigned int) 0x00000020)
#define USB_DEVINTEN_EP5_MASK                     ((unsigned int) 0x00000040)
#define USB_DEVINTEN_EP5                          ((unsigned int) 0x00000040)
#define USB_DEVINTEN_EP6_MASK                     ((unsigned int) 0x00000080)
#define USB_DEVINTEN_EP6                          ((unsigned int) 0x00000080)
#define USB_DEVINTEN_EP7_MASK                     ((unsigned int) 0x00000100)
#define USB_DEVINTEN_EP7                          ((unsigned int) 0x00000100)
#define USB_DEVINTEN_DEV_START_MASK               ((unsigned int) 0x00000200)
#define USB_DEVINTEN_DEV_START                    ((unsigned int) 0x00000200)
#define USB_DEVINTEN_CC_EMPTY_MASK                ((unsigned int) 0x00000400)
#define USB_DEVINTEN_CC_EMPTY                     ((unsigned int) 0x00000400)
#define USB_DEVINTEN_CD_FULL_MASK                 ((unsigned int) 0x00000800)
#define USB_DEVINTEN_CD_FULL                      ((unsigned int) 0x00000800)
#define USB_DEVINTEN_RxENDPKT_MASK                ((unsigned int) 0x00001000)
#define USB_DEVINTEN_RxENDPKT                     ((unsigned int) 0x00001000)
#define USB_DEVINTEN_TxENDPKT_MASK                ((unsigned int) 0x00002000)
#define USB_DEVINTEN_TxENDPKT                     ((unsigned int) 0x00002000)

/* USB Device Interrupt Clear Register  */
#define USB_DEVINTCLR                             (*(pREG32 (0x40020008)))
#define USB_DEVINTCLR_FRAME_MASK                  ((unsigned int) 0x00000001)
#define USB_DEVINTCLR_FRAME                       ((unsigned int) 0x00000001)
#define USB_DEVINTCLR_EP0_MASK                    ((unsigned int) 0x00000002)
#define USB_DEVINTCLR_EP0                         ((unsigned int) 0x00000002)
#define USB_DEVINTCLR_EP1_MASK                    ((unsigned int) 0x00000004)
#define USB_DEVINTCLR_EP1                         ((unsigned int) 0x00000004)
#define USB_DEVINTCLR_EP2_MASK                    ((unsigned int) 0x00000008)
#define USB_DEVINTCLR_EP2                         ((unsigned int) 0x00000008)
#define USB_DEVINTCLR_EP3_MASK                    ((unsigned int) 0x00000010)
#define USB_DEVINTCLR_EP3                         ((unsigned int) 0x00000010)
#define USB_DEVINTCLR_EP4_MASK                    ((unsigned int) 0x00000020)
#define USB_DEVINTCLR_EP4                         ((unsigned int) 0x00000020)
#define USB_DEVINTCLR_EP5_MASK                    ((unsigned int) 0x00000040)
#define USB_DEVINTCLR_EP5                         ((unsigned int) 0x00000040)
#define USB_DEVINTCLR_EP6_MASK                    ((unsigned int) 0x00000080)
#define USB_DEVINTCLR_EP6                         ((unsigned int) 0x00000080)
#define USB_DEVINTCLR_EP7_MASK                    ((unsigned int) 0x00000100)
#define USB_DEVINTCLR_EP7                         ((unsigned int) 0x00000100)
#define USB_DEVINTCLR_DEV_START_MASK              ((unsigned int) 0x00000200)
#define USB_DEVINTCLR_DEV_START                   ((unsigned int) 0x00000200)
#define USB_DEVINTCLR_CC_EMPTY_MASK               ((unsigned int) 0x00000400)
#define USB_DEVINTCLR_CC_EMPTY                    ((unsigned int) 0x00000400)
#define USB_DEVINTCLR_CD_FULL_MASK                ((unsigned int) 0x00000800)
#define USB_DEVINTCLR_CD_FULL                     ((unsigned int) 0x00000800)
#define USB_DEVINTCLR_RxENDPKT_MASK               ((unsigned int) 0x00001000)
#define USB_DEVINTCLR_RxENDPKT                    ((unsigned int) 0x00001000)
#define USB_DEVINTCLR_TxENDPKT_MASK               ((unsigned int) 0x00002000)
#define USB_DEVINTCLR_TxENDPKT                    ((unsigned int) 0x00002000)

/* USB Device Interrupt Set Register  */
#define USB_DEVINTSET                             (*(pREG32 (0x4002000C)))
#define USB_DEVINTSET_FRAME_MASK                  ((unsigned int) 0x00000001)
#define USB_DEVINTSET_FRAME                       ((unsigned int) 0x00000001)
#define USB_DEVINTSET_EP0_MASK                    ((unsigned int) 0x00000002)
#define USB_DEVINTSET_EP0                         ((unsigned int) 0x00000002)
#define USB_DEVINTSET_EP1_MASK                    ((unsigned int) 0x00000004)
#define USB_DEVINTSET_EP1                         ((unsigned int) 0x00000004)
#define USB_DEVINTSET_EP2_MASK                    ((unsigned int) 0x00000008)
#define USB_DEVINTSET_EP2                         ((unsigned int) 0x00000008)
#define USB_DEVINTSET_EP3_MASK                    ((unsigned int) 0x00000010)
#define USB_DEVINTSET_EP3                         ((unsigned int) 0x00000010)
#define USB_DEVINTSET_EP4_MASK                    ((unsigned int) 0x00000020)
#define USB_DEVINTSET_EP4                         ((unsigned int) 0x00000020)
#define USB_DEVINTSET_EP5_MASK                    ((unsigned int) 0x00000040)
#define USB_DEVINTSET_EP5                         ((unsigned int) 0x00000040)
#define USB_DEVINTSET_EP6_MASK                    ((unsigned int) 0x00000080)
#define USB_DEVINTSET_EP6                         ((unsigned int) 0x00000080)
#define USB_DEVINTSET_EP7_MASK                    ((unsigned int) 0x00000100)
#define USB_DEVINTSET_EP7                         ((unsigned int) 0x00000100)
#define USB_DEVINTSET_DEV_START_MASK              ((unsigned int) 0x00000200)
#define USB_DEVINTSET_DEV_START                   ((unsigned int) 0x00000200)
#define USB_DEVINTSET_CC_EMPTY_MASK               ((unsigned int) 0x00000400)
#define USB_DEVINTSET_CC_EMPTY                    ((unsigned int) 0x00000400)
#define USB_DEVINTSET_CD_FULL_MASK                ((unsigned int) 0x00000800)
#define USB_DEVINTSET_CD_FULL                     ((unsigned int) 0x00000800)
#define USB_DEVINTSET_RxENDPKT_MASK               ((unsigned int) 0x00001000)
#define USB_DEVINTSET_RxENDPKT                    ((unsigned int) 0x00001000)
#define USB_DEVINTSET_TxENDPKT_MASK               ((unsigned int) 0x00002000)
#define USB_DEVINTSET_TxENDPKT                    ((unsigned int) 0x00002000)

/* USB Command Code Register */
#define USB_CMDCODE                               (*(pREG32 (0x40020010)))
#define USB_CMDCODE_CMD_PHASE_WRITE               ((unsigned int) 0x00000100)
#define USB_CMDCODE_CMD_PHASE_READ                ((unsigned int) 0x00000200)
#define USB_CMDCODE_CMD_PHASE_COMMAND             ((unsigned int) 0x00000500)
#define USB_CMDCODE_CMD_PHASE_MASK                ((unsigned int) 0x0000FF00)
#define USB_CMDCODE_CMD_CODE_MASK                 ((unsigned int) 0x00FF0000)
#define USB_CMDCODE_CMD_WDATA_MASK                ((unsigned int) 0x00FF0000)

/* USB Command Data Register */
#define USB_CMDDATA                               (*(pREG32 (0x40020014)))
#define USB_CMDDATA_CMD_RDATA_MASK                ((unsigned int) 0x000000FF)

/* USB Receive Data Register */
#define USB_RXDATA                                (*(pREG32 (0x40020018)))

/* USB Transmit Data Register */
#define USB_TXDATA                                (*(pREG32 (0x4002001C)))

/* USB Receive Packet Length Register */
#define USB_RXPLEN                                (*(pREG32 (0x40020020)))
#define USB_RXPLEN_PKT_LNGTH_MASK                 ((unsigned int) 0x000003FF)
#define USB_RXPLEN_DV_MASK                        ((unsigned int) 0x00000400)
#define USB_RXPLEN_DV                             ((unsigned int) 0x00000400)

/* USB Transmit Packet Length Register */
#define USB_TXPLEN                                (*(pREG32 (0x40020024)))
#define USB_TXPLEN_PKT_LNGTH_MASK 0x3FF

/* USB Control Register */
#define USB_CTRL                                  (*(pREG32 (0x40020028)))
#define USB_CTRL_RD_EN_MASK                       ((unsigned int) 0x00000001)
#define USB_CTRL_RD_EN                            ((unsigned int) 0x00000001)
#define USB_CTRL_WR_EN_MASK                       ((unsigned int) 0x00000002)
#define USB_CTRL_WR_EN                            ((unsigned int) 0x00000002)
#define USB_CTRL_LOG_ENDPOINT_MASK                ((unsigned int) 0x0000003C)

/* USB Device FIQ Select Register */
#define USB_DEVFIQSEL                             (*(pREG32 (0x4002002C)))
#define USB_DEVFIQSEL_FRAME_MASK                  ((unsigned int) 0x00000001)
#define USB_DEVFIQSEL_FRAME                       ((unsigned int) 0x00000001)
#define USB_DEVFIQSEL_BULKOUT_MASK                ((unsigned int) 0x00000002)
#define USB_DEVFIQSEL_BULKOUT                     ((unsigned int) 0x00000002)
#define USB_DEVFIQSEL_BULKIN_MASK                 ((unsigned int) 0x00000004)
#define USB_DEVFIQSEL_BULKIN                      ((unsigned int) 0x00000004)

/*##############################################################################
## UART
##############################################################################*/

#define UART_BASE_ADDRESS                         (0x40008000)

#define UART_U0RBR                                (*(pREG32 (0x40008000)))    // Receive buffer
#define UART_U0THR                                (*(pREG32 (0x40008000)))    // Transmitter holding register
#define UART_U0DLL                                (*(pREG32 (0x40008000)))    // Divisor latch LSB
#define UART_U0DLM                                (*(pREG32 (0x40008004)))    // Divisor latch MSB
#define UART_U0IER                                (*(pREG32 (0x40008004)))    // Interrupt enable
#define UART_U0IIR                                (*(pREG32 (0x40008008)))    // Interrupt identification
#define UART_U0FCR                                (*(pREG32 (0x40008008)))    // FIFO control
#define UART_U0MCR                                (*(pREG32 (0x40008010)))    // Modem control
#define UART_U0LCR                                (*(pREG32 (0x4000800C)))    // Line control
#define UART_U0LSR                                (*(pREG32 (0x40008014)))    // Line status
#define UART_U0MSR                                (*(pREG32 (0x40008018)))    // Modem status
#define UART_U0SCR                                (*(pREG32 (0x4000801C)))    // Scratch pad
#define UART_U0ACR                                (*(pREG32 (0x40008020)))    // Auto-baud control
#define UART_U0FDR                                (*(pREG32 (0x40008028)))    // Fractional divider
#define UART_U0TER                                (*(pREG32 (0x40008030)))    // Transmit enable
#define UART_U0RS485CTRL                          (*(pREG32 (0x4000804C)))    // RS485 control register
#define UART_U0RS485ADRMATCH                      (*(pREG32 (0x40008050)))    // RS485 address match
#define UART_U0RS485DLY                           (*(pREG32 (0x40008054)))    // RS485 Delay value
#define UART_U0FIFOLVL                            (*(pREG32 (0x40008058)))    // UART FIFO level

#define UART_U0RBR_MASK                           ((unsigned int) 0x000000FF)

#define UART_U0IER_RBR_Interrupt_MASK             ((unsigned int) 0x00000001) // Enables the received data available interrupt
#define UART_U0IER_RBR_Interrupt_Enabled          ((unsigned int) 0x00000001)
#define UART_U0IER_RBR_Interrupt_Disabled         ((unsigned int) 0x00000000)
#define UART_U0IER_THRE_Interrupt_MASK            ((unsigned int) 0x00000002) // Enables the THRE interrupt
#define UART_U0IER_THRE_Interrupt_Enabled         ((unsigned int) 0x00000002)
#define UART_U0IER_THRE_Interrupt_Disabled        ((unsigned int) 0x00000000)
#define UART_U0IER_RLS_Interrupt_MASK             ((unsigned int) 0x00000004) // Enables the Rx line status interrupt
#define UART_U0IER_RLS_Interrupt_Enabled          ((unsigned int) 0x00000004)
#define UART_U0IER_RLS_Interrupt_Disabled         ((unsigned int) 0x00000000)
#define UART_U0IER_ABEOIntEn_MASK                 ((unsigned int) 0x00000100) // End of auto-baud interrupt
#define UART_U0IER_ABEOIntEn_Enabled              ((unsigned int) 0x00000100)
#define UART_U0IER_ABEOIntEn_Disabled             ((unsigned int) 0x00000000)
#define UART_U0IER_ABTOIntEn_MASK                 ((unsigned int) 0x00000200) // Auto-baud timeout interrupt
#define UART_U0IER_ABTOIntEn_Enabled              ((unsigned int) 0x00000200)
#define UART_U0IER_ABTOIntEn_Disabled             ((unsigned int) 0x00000000)

#define UART_U0IIR_IntStatus_MASK                 ((unsigned int) 0x00000001) // Interrupt status
#define UART_U0IIR_IntStatus_InterruptPending     ((unsigned int) 0x00000001)
#define UART_U0IIR_IntStatus_NoInterruptPending   ((unsigned int) 0x00000000)
#define UART_U0IIR_IntId_MASK                     ((unsigned int) 0x0000000E) // Interrupt identification
#define UART_U0IIR_IntId_RLS                      ((unsigned int) 0x00000006) // Receive line status
#define UART_U0IIR_IntId_RDA                      ((unsigned int) 0x00000004) // Receive data available
#define UART_U0IIR_IntId_CTI                      ((unsigned int) 0x0000000C) // Character time-out indicator
#define UART_U0IIR_IntId_THRE                     ((unsigned int) 0x00000002) // THRE interrupt
#define UART_U0IIR_IntId_MODEM                    ((unsigned int) 0x00000000) // Modem interrupt
#define UART_U0IIR_FIFO_Enable_MASK               ((unsigned int) 0x000000C0)
#define UART_U0IIR_ABEOInt_MASK                   ((unsigned int) 0x00000100) // End of auto-baud interrupt
#define UART_U0IIR_ABEOInt                        ((unsigned int) 0x00000100)
#define UART_U0IIR_ABTOInt_MASK                   ((unsigned int) 0x00000200) // Auto-baud time-out interrupt
#define UART_U0IIR_ABTOInt                        ((unsigned int) 0x00000200)

#define UART_U0FCR_FIFO_Enable_MASK               ((unsigned int) 0x00000001) // UART FIFOs enabled/disabled
#define UART_U0FCR_FIFO_Enabled                   ((unsigned int) 0x00000001)
#define UART_U0FCR_FIFO_Disabled                  ((unsigned int) 0x00000000)
#define UART_U0FCR_Rx_FIFO_Reset_MASK             ((unsigned int) 0x00000002)
#define UART_U0FCR_Rx_FIFO_Reset                  ((unsigned int) 0x00000002) // Clear Rx FIFO
#define UART_U0FCR_Tx_FIFO_Reset_MASK             ((unsigned int) 0x00000004)
#define UART_U0FCR_Tx_FIFO_Reset                  ((unsigned int) 0x00000004) // Clear Tx FIFO
#define UART_U0FCR_Rx_Trigger_Level_Select_MASK   ((unsigned int) 0x000000C0) // Chars written before before interrupt
#define UART_U0FCR_Rx_Trigger_Level_Select_1Char  ((unsigned int) 0x00000000) 
#define UART_U0FCR_Rx_Trigger_Level_Select_4Char  ((unsigned int) 0x00000040) 
#define UART_U0FCR_Rx_Trigger_Level_Select_8Char  ((unsigned int) 0x00000080) 
#define UART_U0FCR_Rx_Trigger_Level_Select_12Char ((unsigned int) 0x000000C0) 

#define UART_U0MCR_DTR_Control_MASK               ((unsigned int) 0x00000001) // Source for modem output pin DTR
#define UART_U0MCR_DTR_Control                    ((unsigned int) 0x00000001)
#define UART_U0MCR_RTS_Control_MASK               ((unsigned int) 0x00000002) // Source for modem output pin RTS
#define UART_U0MCR_RTS_Control                    ((unsigned int) 0x00000002)
#define UART_U0MCR_Loopback_Mode_Select_MASK      ((unsigned int) 0x00000010) // Diagnostic loopback mode
#define UART_U0MCR_Loopback_Mode_Select_Enabled   ((unsigned int) 0x00000010)
#define UART_U0MCR_Loopback_Mode_Select_Disabled  ((unsigned int) 0x00000000)
#define UART_U0MCR_RTSen_MASK                     ((unsigned int) 0x00000040) // Disable auto-rts flow control
#define UART_U0MCR_RTSen_Enabled                  ((unsigned int) 0x00000040)
#define UART_U0MCR_RTSen_Disabled                 ((unsigned int) 0x00000000)
#define UART_U0MCR_CTSen_MASK                     ((unsigned int) 0x00000080) // Disable auto-cts flow control
#define UART_U0MCR_CTSen_Enabled                  ((unsigned int) 0x00000080)
#define UART_U0MCR_CTSen_Disabled                 ((unsigned int) 0x00000000)

#define UART_U0LCR_Word_Length_Select_MASK        ((unsigned int) 0x00000003) // Word Length Selector
#define UART_U0LCR_Word_Length_Select_5Chars      ((unsigned int) 0x00000000)
#define UART_U0LCR_Word_Length_Select_6Chars      ((unsigned int) 0x00000001)
#define UART_U0LCR_Word_Length_Select_7Chars      ((unsigned int) 0x00000002)
#define UART_U0LCR_Word_Length_Select_8Chars      ((unsigned int) 0x00000003)
#define UART_U0LCR_Stop_Bit_Select_MASK           ((unsigned int) 0x00000004) // Stop bit select
#define UART_U0LCR_Stop_Bit_Select_1Bits          ((unsigned int) 0x00000000)
#define UART_U0LCR_Stop_Bit_Select_2Bits          ((unsigned int) 0x00000004)
#define UART_U0LCR_Parity_Enable_MASK             ((unsigned int) 0x00000008) // Parity enable
#define UART_U0LCR_Parity_Enabled                 ((unsigned int) 0x00000008)
#define UART_U0LCR_Parity_Disabled                ((unsigned int) 0x00000000)
#define UART_U0LCR_Parity_Select_MASK             ((unsigned int) 0x00000030) // Parity select
#define UART_U0LCR_Parity_Select_OddParity        ((unsigned int) 0x00000000)
#define UART_U0LCR_Parity_Select_EvenParity       ((unsigned int) 0x00000010)
#define UART_U0LCR_Parity_Select_Forced1          ((unsigned int) 0x00000020)
#define UART_U0LCR_Parity_Select_Forced0          ((unsigned int) 0x00000030)
#define UART_U0LCR_Break_Control_MASK             ((unsigned int) 0x00000040) // Break transmission control
#define UART_U0LCR_Break_Control_Enabled          ((unsigned int) 0x00000040)
#define UART_U0LCR_Break_Control_Disabled         ((unsigned int) 0x00000000)
#define UART_U0LCR_Divisor_Latch_Access_MASK      ((unsigned int) 0x00000080) // Divisor latch access
#define UART_U0LCR_Divisor_Latch_Access_Enabled   ((unsigned int) 0x00000080)
#define UART_U0LCR_Divisor_Latch_Access_Disabled  ((unsigned int) 0x00000000)

#define UART_U0LSR_RDR_MASK                       ((unsigned int) 0x00000001) // Receiver data ready
#define UART_U0LSR_RDR_EMPTY                      ((unsigned int) 0x00000000) // U0RBR is empty
#define UART_U0LSR_RDR_DATA                       ((unsigned int) 0x00000001) // U0RBR contains valid data
#define UART_U0LSR_OE_MASK                        ((unsigned int) 0x00000002) // Overrun error
#define UART_U0LSR_OE                             ((unsigned int) 0x00000002)
#define UART_U0LSR_PE_MASK                        ((unsigned int) 0x00000004) // Parity error
#define UART_U0LSR_PE                             ((unsigned int) 0x00000004)
#define UART_U0LSR_FE_MASK                        ((unsigned int) 0x00000008) // Framing error
#define UART_U0LSR_FE                             ((unsigned int) 0x00000008)
#define UART_U0LSR_BI_MASK                        ((unsigned int) 0x00000010) // Break interrupt
#define UART_U0LSR_BI                             ((unsigned int) 0x00000010)
#define UART_U0LSR_THRE_MASK                      ((unsigned int) 0x00000020) // Transmitter holding register empty
#define UART_U0LSR_THRE                           ((unsigned int) 0x00000020)
#define UART_U0LSR_TEMT_MASK                      ((unsigned int) 0x00000040) // Transmitter empty
#define UART_U0LSR_TEMT                           ((unsigned int) 0x00000040)
#define UART_U0LSR_RXFE_MASK                      ((unsigned int) 0x00000080) // Error in Rx FIFO
#define UART_U0LSR_RXFE                           ((unsigned int) 0x00000080)

#define UART_U0MSR_Delta_CTS_MASK                 ((unsigned int) 0x00000001) // State change of input CTS
#define UART_U0MSR_Delta_CTS                      ((unsigned int) 0x00000001)
#define UART_U0MSR_Delta_DSR_MASK                 ((unsigned int) 0x00000002) // State change of input DSR
#define UART_U0MSR_Delta_DSR                      ((unsigned int) 0x00000002)
#define UART_U0MSR_Trailing_Edge_RI_MASK          ((unsigned int) 0x00000004) // Low to high transition of input RI
#define UART_U0MSR_Trailing_Edge_RI               ((unsigned int) 0x00000004)
#define UART_U0MSR_Delta_DCD_MASK                 ((unsigned int) 0x00000008) // State change of input DCD
#define UART_U0MSR_Delta_DCD                      ((unsigned int) 0x00000008)
#define UART_U0MSR_CTS_MASK                       ((unsigned int) 0x00000010) // Clear to send state
#define UART_U0MSR_CTS                            ((unsigned int) 0x00000010)
#define UART_U0MSR_DSR_MASK                       ((unsigned int) 0x00000020) // Data set ready state
#define UART_U0MSR_DSR                            ((unsigned int) 0x00000020)
#define UART_U0MSR_RI_MASK                        ((unsigned int) 0x00000040) // Ring indicator state
#define UART_U0MSR_RI                             ((unsigned int) 0x00000040)
#define UART_U0MSR_DCD_MASK                       ((unsigned int) 0x00000080) // Data carrier detect state
#define UART_U0MSR_DCD                            ((unsigned int) 0x00000080)

#define UART_U0ACR_Start_MASK                     ((unsigned int) 0x00000001) // Auto-baud start/stop
#define UART_U0ACR_Start                          ((unsigned int) 0x00000001)
#define UART_U0ACR_Stop                           ((unsigned int) 0x00000000)
#define UART_U0ACR_Mode_MASK                      ((unsigned int) 0x00000002) // Auto-baud mode select
#define UART_U0ACR_Mode_Mode1                     ((unsigned int) 0x00000000)
#define UART_U0ACR_Mode_Mode2                     ((unsigned int) 0x00000002)
#define UART_U0ACR_AutoRestart_MASK               ((unsigned int) 0x00000004)
#define UART_U0ACR_AutoRestart_NoRestart          ((unsigned int) 0x00000000)
#define UART_U0ACR_AutoRestart_Restart            ((unsigned int) 0x00000004) // Restart in case of time-out
#define UART_U0ACR_ABEOIntClr_MASK                ((unsigned int) 0x00000100) // End of auto-baud interrupt clear bit
#define UART_U0ACR_ABEOIntClr                     ((unsigned int) 0x00000100) 
#define UART_U0ACR_ABTOIntClr_MASK                ((unsigned int) 0x00000200) // Auto-baud timeout interrupt clear bit
#define UART_U0ACR_ABTOIntClr                     ((unsigned int) 0x00000200)

#define UART_U0FDR_DIVADDVAL_MASK                 ((unsigned int) 0x0000000F) // Fractional divider: prescaler register
#define UART_U0FDR_MULVAL_MASK                    ((unsigned int) 0x000000F0) // Fractional divider: prescaler multiplier

#define UART_U0TER_TXEN_MASK                      ((unsigned int) 0x00000080) // UART transmit enable
#define UART_U0TER_TXEN_Enabled                   ((unsigned int) 0x00000080)
#define UART_U0TER_TXEN_Disabled                  ((unsigned int) 0x00000000)

#define UART_U0RS485CTRL_NMMEN_MASK               ((unsigned int) 0x00000001) // Normal multi-drop mode
#define UART_U0RS485CTRL_NMMEN                    ((unsigned int) 0x00000001)
#define UART_U0RS485CTRL_RXDIS_MASK               ((unsigned int) 0x00000002) // Receiver
#define UART_U0RS485CTRL_RXDIS                    ((unsigned int) 0x00000002)
#define UART_U0RS485CTRL_AADEN_MASK               ((unsigned int) 0x00000004) // Auto-address detect
#define UART_U0RS485CTRL_AADEN                    ((unsigned int) 0x00000004)
#define UART_U0RS485CTRL_SEL_MASK                 ((unsigned int) 0x00000008) 
#define UART_U0RS485CTRL_SEL_RTS                  ((unsigned int) 0x00000000) // Use RTS for direction control
#define UART_U0RS485CTRL_SEL_DTS                  ((unsigned int) 0x00000008) // Use DTS for direction control
#define UART_U0RS485CTRL_DCTRL_MASK               ((unsigned int) 0x00000010) // Enable/Disable auto-direction control
#define UART_U0RS485CTRL_DCTRL_Disabled           ((unsigned int) 0x00000000)
#define UART_U0RS485CTRL_DCTRL_Enabled            ((unsigned int) 0x00000010)
#define UART_U0RS485CTRL_OINV_MASK                ((unsigned int) 0x00000020) // Reverse polarity of direction control signal on RTS/DTR pin
#define UART_U0RS485CTRL_OINV_Normal              ((unsigned int) 0x00000000)
#define UART_U0RS485CTRL_OINV_Inverted            ((unsigned int) 0x00000020)

#define UART_U0FIFOLVL_RXFIFOLVL_MASK             ((unsigned int) 0x0000000F)
#define UART_U0FIFOLVL_RXFIFOLVL_Empty            ((unsigned int) 0x00000000)
#define UART_U0FIFOLVL_RXFIFOLVL_Full             ((unsigned int) 0x0000000F)
#define UART_U0FIFOLVL_TXFIFOLVL_MASK             ((unsigned int) 0x00000F00)
#define UART_U0FIFOLVL_TXFIFOLVL_Empty            ((unsigned int) 0x00000000)
#define UART_U0FIFOLVL_TXFIFOLVL_Full             ((unsigned int) 0x00000F00)

/*##############################################################################
## SSP - Synchronous Serial Port
##############################################################################*/

#define SSP_SSP0_BASE_ADDRESS                     (0x40040000)

#define SSP_SSP0CR0                               (*(pREG32 (0x40040000)))    // Control register 0
#define SSP_SSP0CR1                               (*(pREG32 (0x40040004)))    // Control register 1
#define SSP_SSP0DR                                (*(pREG32 (0x40040008)))    // Data register
#define SSP_SSP0SR                                (*(pREG32 (0x4004000C)))    // Status register
#define SSP_SSP0CPSR                              (*(pREG32 (0x40040010)))    // Clock prescale register
#define SSP_SSP0IMSC                              (*(pREG32 (0x40040014)))    // Interrupt mask set/clear register
#define SSP_SSP0RIS                               (*(pREG32 (0x40040018)))    // Raw interrupt status register
#define SSP_SSP0MIS                               (*(pREG32 (0x4004001C)))    // Masked interrupt status register
#define SSP_SSP0ICR                               (*(pREG32 (0x40040020)))    // SSPICR interrupt clear register

/*  SSP0CR0 (SSP0 Control Register 0)
    This register controls the basic operation of the SSP controller. */

#define SSP_SSP0CR0_DSS_MASK                      ((unsigned int) 0x0000000F) // Data size select
#define SSP_SSP0CR0_DSS_4BIT                      ((unsigned int) 0x00000003)
#define SSP_SSP0CR0_DSS_5BIT                      ((unsigned int) 0x00000004)
#define SSP_SSP0CR0_DSS_6BIT                      ((unsigned int) 0x00000005)
#define SSP_SSP0CR0_DSS_7BIT                      ((unsigned int) 0x00000006)
#define SSP_SSP0CR0_DSS_8BIT                      ((unsigned int) 0x00000007)
#define SSP_SSP0CR0_DSS_9BIT                      ((unsigned int) 0x00000008)
#define SSP_SSP0CR0_DSS_10BIT                     ((unsigned int) 0x00000009)
#define SSP_SSP0CR0_DSS_11BIT                     ((unsigned int) 0x0000000A)
#define SSP_SSP0CR0_DSS_12BIT                     ((unsigned int) 0x0000000B)
#define SSP_SSP0CR0_DSS_13BIT                     ((unsigned int) 0x0000000C)
#define SSP_SSP0CR0_DSS_14BIT                     ((unsigned int) 0x0000000D)
#define SSP_SSP0CR0_DSS_15BIT                     ((unsigned int) 0x0000000E)
#define SSP_SSP0CR0_DSS_16BIT                     ((unsigned int) 0x0000000F)
#define SSP_SSP0CR0_FRF_MASK                      ((unsigned int) 0x00000030) // Frame format
#define SSP_SSP0CR0_FRF_SPI                       ((unsigned int) 0x00000000)
#define SSP_SSP0CR0_FRF_TI                        ((unsigned int) 0x00000010)
#define SSP_SSP0CR0_FRF_MWIRE                     ((unsigned int) 0x00000020)
#define SSP_SSP0CR0_CPOL_MASK                     ((unsigned int) 0x00000040) // Clock out polarity
#define SSP_SSP0CR0_CPOL_LOW                      ((unsigned int) 0x00000000)
#define SSP_SSP0CR0_CPOL_HIGH                     ((unsigned int) 0x00000040)
#define SSP_SSP0CR0_CPHA_MASK                     ((unsigned int) 0x00000080) // Clock out phase
#define SSP_SSP0CR0_CPHA_FIRST                    ((unsigned int) 0x00000000)
#define SSP_SSP0CR0_CPHA_SECOND                   ((unsigned int) 0x00000080)

/*  Serial Clock Rate. The number of prescaler-output clocks per
    bit on the bus, minus one. Given that CPSDVSR is the
    prescale divider, and the APB clock PCLK clocks the
    prescaler, the bit frequency is PCLK / (CPSDVSR � [SCR+1]). */

#define SSP_SSP0CR0_SCR_MASK                      ((unsigned int) 0x0000FF00) // Serial clock rate
#define SSP_SSP0CR0_SCR_1                         ((unsigned int) 0x00000100)
#define SSP_SSP0CR0_SCR_2                         ((unsigned int) 0x00000200)
#define SSP_SSP0CR0_SCR_3                         ((unsigned int) 0x00000300)
#define SSP_SSP0CR0_SCR_4                         ((unsigned int) 0x00000400)
#define SSP_SSP0CR0_SCR_5                         ((unsigned int) 0x00000500)
#define SSP_SSP0CR0_SCR_6                         ((unsigned int) 0x00000600)
#define SSP_SSP0CR0_SCR_7                         ((unsigned int) 0x00000700)
#define SSP_SSP0CR0_SCR_8                         ((unsigned int) 0x00000800)
#define SSP_SSP0CR0_SCR_9                         ((unsigned int) 0x00000900)
#define SSP_SSP0CR0_SCR_10                        ((unsigned int) 0x00000A00)
#define SSP_SSP0CR0_SCR_11                        ((unsigned int) 0x00000B00)
#define SSP_SSP0CR0_SCR_12                        ((unsigned int) 0x00000C00)
#define SSP_SSP0CR0_SCR_13                        ((unsigned int) 0x00000D00)
#define SSP_SSP0CR0_SCR_14                        ((unsigned int) 0x00000E00)
#define SSP_SSP0CR0_SCR_15                        ((unsigned int) 0x00000F00)
#define SSP_SSP0CR0_SCR_16                        ((unsigned int) 0x00001000)

/*  SSP0CR1 (SSP0 Control Register 1)
    This register controls certain aspects of the operation of the SSP controller.  */

#define SSP_SSP0CR1_LBM_MASK                      ((unsigned int) 0x00000001) // Loop back mode
#define SSP_SSP0CR1_LBM_NORMAL                    ((unsigned int) 0x00000000)
#define SSP_SSP0CR1_LBM_INVERTED                  ((unsigned int) 0x00000001) // MISO/MOSI are reversed
#define SSP_SSP0CR1_SSE_MASK                      ((unsigned int) 0x00000002) // SSP enable
#define SSP_SSP0CR1_SSE_DISABLED                  ((unsigned int) 0x00000000)
#define SSP_SSP0CR1_SSE_ENABLED                   ((unsigned int) 0x00000002)
#define SSP_SSP0CR1_MS_MASK                       ((unsigned int) 0x00000004) // Master/Slave Mode
#define SSP_SSP0CR1_MS_MASTER                     ((unsigned int) 0x00000000)
#define SSP_SSP0CR1_MS_SLAVE                      ((unsigned int) 0x00000004)
#define SSP_SSP0CR1_SOD_MASK                      ((unsigned int) 0x00000008) // Slave output disable

/*  SSP0DR (SSP0 Data Register)
    Software can write data to be transmitted to this register, and read data that has been
    received. */

#define SSP_SSP0DR_MASK                           ((unsigned int) 0x0000FFFF) // Data

/*  SSP0SR (SSP0 Status Register)
    This read-only register reflects the current status of the SSP controller.  */

#define SSP_SSP0SR_TFE_MASK                       ((unsigned int) 0x00000001) // Transmit FIFO empty
#define SSP_SSP0SR_TFE_EMPTY                      ((unsigned int) 0x00000001)
#define SSP_SSP0SR_TFE_NOTEMPTY                   ((unsigned int) 0x00000000)
#define SSP_SSP0SR_TNF_MASK                       ((unsigned int) 0x00000002) // Transmit FIFO not full
#define SSP_SSP0SR_TNF_NOTFULL                    ((unsigned int) 0x00000002)
#define SSP_SSP0SR_TNF_FULL                       ((unsigned int) 0x00000000)
#define SSP_SSP0SR_RNE_MASK                       ((unsigned int) 0x00000004) // Receive FIFO not empty
#define SSP_SSP0SR_RNE_NOTEMPTY                   ((unsigned int) 0x00000004)
#define SSP_SSP0SR_RNE_EMPTY                      ((unsigned int) 0x00000000)
#define SSP_SSP0SR_RFF_MASK                       ((unsigned int) 0x00000008) // Receive FIFO full
#define SSP_SSP0SR_RFF_FULL                       ((unsigned int) 0x00000008)
#define SSP_SSP0SR_RFF_NOTFULL                    ((unsigned int) 0x00000000)
#define SSP_SSP0SR_BSY_MASK                       ((unsigned int) 0x00000010) // Busy Flag
#define SSP_SSP0SR_BSY_IDLE                       ((unsigned int) 0x00000000)
#define SSP_SSP0SR_BSY_BUSY                       ((unsigned int) 0x00000010)

/*  SSP0CPSR (SSP0 Clock Prescale Register)
    This register controls the factor by which the Prescaler divides the SSP peripheral clock
    SSP_PCLK to yield the prescaler clock that is, in turn, divided by the SCR factor in
    SSP0CR0, to determine the bit clock.  */

#define SSP_SSP0CPSR_CPSDVSR_MASK                 ((unsigned int) 0x000000FF)
#define SSP_SSP0CPSR_CPSDVSR_DIV2                 ((unsigned int) 0x00000002)
#define SSP_SSP0CPSR_CPSDVSR_DIV4                 ((unsigned int) 0x00000004)
#define SSP_SSP0CPSR_CPSDVSR_DIV10                ((unsigned int) 0x0000000A)
#define SSP_SSP0CPSR_CPSDVSR_DIV12                ((unsigned int) 0x0000000C)
#define SSP_SSP0CPSR_CPSDVSR_DIV16                ((unsigned int) 0x00000010)
#define SSP_SSP0CPSR_CPSDVSR_DIV20                ((unsigned int) 0x00000014)

/*  SSP0IMSC (SSP0 Interrupt Mask Set/Clear Register)
    This register controls whether each of the four possible interrupt conditions in the SSP
    controller are enabled. Note that ARM uses the word masked in the opposite sense from
    classic computer terminology, in which masked meant disabled. ARM uses the word
    masked to mean enabled. To avoid confusion we will not use the word masked. */

#define SSP_SSP0IMSC_RORIM_MASK                   ((unsigned int) 0x00000001) // Receive overrun interrupt
#define SSP_SSP0IMSC_RORIM_ENBL                   ((unsigned int) 0x00000001)
#define SSP_SSP0IMSC_RORIM_DSBL                   ((unsigned int) 0x00000000)
#define SSP_SSP0IMSC_RTIM_MASK                    ((unsigned int) 0x00000002) // Receive timeout interrupt
#define SSP_SSP0IMSC_RTIM_ENBL                    ((unsigned int) 0x00000002)
#define SSP_SSP0IMSC_RTIM_DSBL                    ((unsigned int) 0x00000000)
#define SSP_SSP0IMSC_RXIM_MASK                    ((unsigned int) 0x00000004) // Rx FIFO >= 1/2 full interrupt
#define SSP_SSP0IMSC_RXIM_ENBL                    ((unsigned int) 0x00000004)
#define SSP_SSP0IMSC_RXIM_DSBL                    ((unsigned int) 0x00000000)
#define SSP_SSP0IMSC_TXIM_MASK                    ((unsigned int) 0x00000008) // Tx FIFO >= 1/2 empty interrupt
#define SSP_SSP0IMSC_TXIM_ENBL                    ((unsigned int) 0x00000008)
#define SSP_SSP0IMSC_TXIM_DSBL                    ((unsigned int) 0x00000000)

/*  SSP0RIS (SSP0 Raw Interrupt Status Register)
    This read-only register contains a 1 for each interrupt condition that is asserted,
    regardless of whether or not the interrupt is enabled in the SSP0IMSC.  */

#define SSP_SSP0RIS_RORRIS_MASK                   ((unsigned int) 0x00000001) // Frame received while Rx FIFO full
#define SSP_SSP0RIS_RORRIS_RCVD                   ((unsigned int) 0x00000001)
#define SSP_SSP0RIS_RTRIS_MASK                    ((unsigned int) 0x00000002) // Rx FIFO not empty no read within timeout
#define SSP_SSP0RIS_RTRIS_NOTEMPTY                ((unsigned int) 0x00000002)
#define SSP_SSP0RIS_RXRIS_MASK                    ((unsigned int) 0x00000004) // Rx FIFO >= half full
#define SSP_SSP0RIS_RXRIS_HALFFULL                ((unsigned int) 0x00000004)
#define SSP_SSP0RIS_TXRIS_MASK                    ((unsigned int) 0x00000008) // Tx FIF0 >= half-empty
#define SSP_SSP0RIS_TXRIS_HALFEMPTY               ((unsigned int) 0x00000008)

/*  SSP0MIS (SSP0 Masked Interrupt Status Register)
    This read-only register contains a 1 for each interrupt condition that is asserted and
    enabled in the SSP0IMSC. When an SSP interrupt occurs, the interrupt service routine
    should read this register to determine the cause(s) of the interrupt. */

#define SSP_SSP0MIS_RORMIS_MASK                   ((unsigned int) 0x00000001) // Frame received while Rx FIFO full
#define SSP_SSP0MIS_RORMIS_FRMRCVD                ((unsigned int) 0x00000001)
#define SSP_SSP0MIS_RTMIS_MASK                    ((unsigned int) 0x00000002) // Rx FIFO not empty no read withing timeout
#define SSP_SSP0MIS_RTMIS_NOTEMPTY                ((unsigned int) 0x00000002)
#define SSP_SSP0MIS_RXMIS_MASK                    ((unsigned int) 0x00000004) // Rx FIFO >= half full
#define SSP_SSP0MIS_RXMIS_HALFFULL                ((unsigned int) 0x00000004)
#define SSP_SSP0MIS_TXMIS_MASK                    ((unsigned int) 0x00000008) // Tx FIFO >= half-empty
#define SSP_SSP0MIS_TXMIS_HALFEMPTY               ((unsigned int) 0x00000008)

/*  SSP0ICR (SSP0 Interrupt Clear Register)
    Software can write one or more one(s) to this write-only register, to clear the
    corresponding interrupt condition(s) in the SSP controller. Note that the other two interrupt
    conditions can be cleared by writing or reading the appropriate FIFO, or disabled by
    clearing the corresponding bit in SSP0IMSC. */

#define SSP_SSP0ICR_RORIC_MASK                    ((unsigned int) 0x00000001) // Clears RORIC interrupt flag
#define SSP_SSP0ICR_RORIC_CLEAR                   ((unsigned int) 0x00000001)
#define SSP_SSP0ICR_RTIC_MASK                     ((unsigned int) 0x00000002) // Clear Rx FIFO not empty/no read flag
#define SSP_SSP0ICR_RTIC_CLEAR                    ((unsigned int) 0x00000002)

/*##############################################################################
## I2C
##############################################################################*/

#define I2C_BASE_ADDRESS                          (0x40000000)

#define I2C_I2CCONSET                             (*(pREG32 (0x40000000)))    // I2C control set register
#define I2C_I2CSTAT                               (*(pREG32 (0x40000004)))    // I2C status register
#define I2C_I2CDAT                                (*(pREG32 (0x40000008)))    // I2C data register
#define I2C_I2CADR0                               (*(pREG32 (0x4000000C)))    // I2C slave address register
#define I2C_I2CSCLH                               (*(pREG32 (0x40000010)))    // I2C SCL HIGH/LOW duty cycle register
#define I2C_I2CSCLL                               (*(pREG32 (0x40000014)))
#define I2C_I2CCONCLR                             (*(pREG32 (0x40000018)))    // I2C control clear register
#define I2C_I2CMMCTRL                             (*(pREG32 (0x4000001C)))    // I2C monitor control register
#define I2C_I2CADR1                               (*(pREG32 (0x40000020)))    // I2C slave address register 1
#define I2C_I2CADR2                               (*(pREG32 (0x40000024)))    // I2C slave address register 2
#define I2C_I2CADR3                               (*(pREG32 (0x40000028)))    // I2C slave address register 3
#define I2C_I2CDATA_BUFFER                        (*(pREG32 (0x4000002C)))    // I2C data buffer register
#define I2C_I2CMASK0                              (*(pREG32 (0x40000030)))    // I2C mask register 0
#define I2C_I2CMASK1                              (*(pREG32 (0x40000034)))    // I2C mask register 1
#define I2C_I2CMASK2                              (*(pREG32 (0x40000038)))    // I2C mask register 2
#define I2C_I2CMASK3                              (*(pREG32 (0x4000003C)))    // I2C mask register 3

/*  I2CCONSET (I2C Control Set register)
    The I2CONSET registers control setting of bits in the I2CON register that controls
    operation of the I2C interface. Writing a one to a bit of this register causes the
    corresponding bit in the I2C control register to be set. Writing a zero has no effect. */

#define I2C_I2CCONSET_AA_MASK                     ((unsigned int) 0x00000004)
#define I2C_I2CCONSET_AA                          ((unsigned int) 0x00000004) // Asset acknowlegde flag
#define I2C_I2CCONSET_SI_MASK                     ((unsigned int) 0x00000008)
#define I2C_I2CCONSET_SI                          ((unsigned int) 0x00000008) // I2C interrupt flag
#define I2C_I2CCONSET_STO_MASK                    ((unsigned int) 0x00000010)
#define I2C_I2CCONSET_STO                         ((unsigned int) 0x00000010) // Stop flag
#define I2C_I2CCONSET_STA_MASK                    ((unsigned int) 0x00000020)
#define I2C_I2CCONSET_STA                         ((unsigned int) 0x00000020) // Start flag
#define I2C_I2CCONSET_I2EN_MASK                   ((unsigned int) 0x00000040)
#define I2C_I2CCONSET_I2EN                        ((unsigned int) 0x00000040) // I2C interface enable

/*  I2CSTAT (I2C Status register)
    Each I2C Status register reflects the condition of the corresponding I2C interface. The I2C
    Status register is Read-Only. */

#define I2C_I2CSTAT_Status_MASK                   ((unsigned int) 0x000000F8) // Status information

/*  I2CADR0 (I2C Slave Address register)
    These registers are readable and writable and are only used when an I2C interface is set
    to slave mode.  */

#define I2C_I2CADR0_GC_MASK                       ((unsigned int) 0x00000001)
#define I2C_I2CADR0_GC                            ((unsigned int) 0x00000001) // General call enable bit
#define I2C_I2CADR0_Address_MASK                  ((unsigned int) 0x000000FE) // I2C device address for slave mode

/*  I2CCONCLR (I2C Control Clear register)
    The I2CONCLR registers control clearing of bits in the I2CON register that controls
    operation of the I2C interface. Writing a one to a bit of this register causes the
    corresponding bit in the I2C control register to be cleared. Writing a zero has no effect.  */

#define I2C_I2CCONCLR_AAC_MASK                    ((unsigned int) 0x00000004) // Assert acknowledge clear bit
#define I2C_I2CCONCLR_AAC                         ((unsigned int) 0x00000004)
#define I2C_I2CCONCLR_SIC_MASK                    ((unsigned int) 0x00000008) // I2C interrupt clear bit
#define I2C_I2CCONCLR_SIC                         ((unsigned int) 0x00000008)
#define I2C_I2CCONCLR_STAC_MASK                   ((unsigned int) 0x00000020) // Start flag clear bit
#define I2C_I2CCONCLR_STAC                        ((unsigned int) 0x00000020)
#define I2C_I2CCONCLR_I2ENC_MASK                  ((unsigned int) 0x00000040) // I2C interface disable bit
#define I2C_I2CCONCLR_I2ENC                       ((unsigned int) 0x00000040)

/*  I2CMMCTRL (I2C Monitor mode control register)
    This register controls the Monitor mode which allows the I2C module to monitor traffic on
    the I2C bus without actually participating in traffic or interfering with the I2C bus.  */

#define I2C_I2CMMCTRL_MM_ENA_MASK                 ((unsigned int) 0x00000001) // Monitor mode enable
#define I2C_I2CMMCTRL_MM_ENA_ENABLED              ((unsigned int) 0x00000001)
#define I2C_I2CMMCTRL_MM_ENA_DISABLED             ((unsigned int) 0x00000000)
#define I2C_I2CMMCTRL_ENA_SCL_MASK                ((unsigned int) 0x00000002) // SCL output enable
#define I2C_I2CMMCTRL_ENA_SCL_HOLDLOW             ((unsigned int) 0x00000002)
#define I2C_I2CMMCTRL_ENA_SCL_FORCEHIGH           ((unsigned int) 0x00000000)
#define I2C_I2CMMCTRL_MATCH_ALL_MASK              ((unsigned int) 0x00000008) // Select interrupt register match
#define I2C_I2CMMCTRL_MATCH_ALL_NORMAL            ((unsigned int) 0x00000000)
#define I2C_I2CMMCTRL_MATCH_ALL_ANYADDRESS        ((unsigned int) 0x00000008)

/*  I2CADR1..3 (I2C Slave Address registers)
    These registers are readable and writable and are only used when an I2C interface is set
    to slave mode. In master mode, this register has no effect. The LSB of I2ADR is the
    General Call bit. When this bit is set, the General Call address (0x00) is recognized. */

#define I2C_I2CADR1_GC_MASK                       ((unsigned int) 0x00000001) // General call enable bit
#define I2C_I2CADR1_GC                            ((unsigned int) 0x00000001)
#define I2C_I2CADR1_Address_MASK                  ((unsigned int) 0x000000FE)

#define I2C_I2CADR2_GC_MASK                       ((unsigned int) 0x00000001) // General call enable bit
#define I2C_I2CADR2_GC                            ((unsigned int) 0x00000001)
#define I2C_I2CADR2_Address_MASK                  ((unsigned int) 0x000000FE)

#define I2C_I2CADR3_GC_MASK                       ((unsigned int) 0x00000001) // General call enable bit
#define I2C_I2CADR3_GC                            ((unsigned int) 0x00000001)
#define I2C_I2CADR3_Address_MASK                  ((unsigned int) 0x000000FE)

/*  I2CMASK0..3 (I2C Mask registers) */

#define I2C_I2CMASK0_MASK_MASK                    ((unsigned int) 0x000000FE)

#define I2C_I2CMASK1_MASK_MASK                    ((unsigned int) 0x000000FE)

#define I2C_I2CMASK2_MASK_MASK                    ((unsigned int) 0x000000FE)

#define I2C_I2CMASK3_MASK_MASK                    ((unsigned int) 0x000000FE)

/*##############################################################################
## 16-Bit Timers (CT16B0/1)
##############################################################################*/

#define TMR_CT16B0_BASE_ADDRESS                   (0x4000C000)

#define TMR_TMR16B0IR                             (*(pREG32 (0x4000C000)))    // Interrupt register
#define TMR_TMR16B0TCR                            (*(pREG32 (0x4000C004)))    // Timer control register
#define TMR_TMR16B0TC                             (*(pREG32 (0x4000C008)))    // Timer counter
#define TMR_TMR16B0PR                             (*(pREG32 (0x4000C00C)))    // Prescale register
#define TMR_TMR16B0PC                             (*(pREG32 (0x4000C010)))    // Prescale counter register
#define TMR_TMR16B0MCR                            (*(pREG32 (0x4000C014)))    // Match control register
#define TMR_TMR16B0MR0                            (*(pREG32 (0x4000C018)))    // Match register 0
#define TMR_TMR16B0MR1                            (*(pREG32 (0x4000C01C)))    // Match register 1
#define TMR_TMR16B0MR2                            (*(pREG32 (0x4000C020)))    // Match register 2
#define TMR_TMR16B0MR3                            (*(pREG32 (0x4000C024)))    // Match register 3
#define TMR_TMR16B0CCR                            (*(pREG32 (0x4000C028)))    // Capture control register
#define TMR_TMR16B0CR0                            (*(pREG32 (0x4000C02C)))    // Capture register
#define TMR_TMR16B0EMR                            (*(pREG32 (0x4000C03C)))    // External match register
#define TMR_TMR16B0CTCR                           (*(pREG32 (0x4000C070)))    // Count control register
#define TMR_TMR16B0PWMC                           (*(pREG32 (0x4000C074)))    // PWM control register

#define TMR_TMR16B0IR_MR0_MASK                    ((unsigned int) 0x00000001) // Interrupt flag for match channel 0
#define TMR_TMR16B0IR_MR0                         ((unsigned int) 0x00000001)
#define TMR_TMR16B0IR_MR1_MASK                    ((unsigned int) 0x00000002) // Interrupt flag for match channel 1
#define TMR_TMR16B0IR_MR1                         ((unsigned int) 0x00000002)
#define TMR_TMR16B0IR_MR2_MASK                    ((unsigned int) 0x00000004) // Interrupt flag for match channel 2
#define TMR_TMR16B0IR_MR2                         ((unsigned int) 0x00000004)
#define TMR_TMR16B0IR_MR3_MASK                    ((unsigned int) 0x00000008) // Interrupt flag for match channel 3
#define TMR_TMR16B0IR_MR3                         ((unsigned int) 0x00000008)
#define TMR_TMR16B0IR_CR0_MASK                    ((unsigned int) 0x00000010) // Interrupt flag for capture channel 0 event
#define TMR_TMR16B0IR_CR0                         ((unsigned int) 0x00000010)
#define TMR_TMR16B0IR_MASK_ALL                    ((unsigned int) 0x0000001F)

#define TMR_TMR16B0TCR_COUNTERENABLE_MASK         ((unsigned int) 0x00000001) // Counter enable
#define TMR_TMR16B0TCR_COUNTERENABLE_ENABLED      ((unsigned int) 0x00000001)
#define TMR_TMR16B0TCR_COUNTERENABLE_DISABLED     ((unsigned int) 0x00000000)
#define TMR_TMR16B0TCR_COUNTERRESET_MASK          ((unsigned int) 0x00000002)
#define TMR_TMR16B0TCR_COUNTERRESET_ENABLED       ((unsigned int) 0x00000002)
#define TMR_TMR16B0TCR_COUNTERRESET_DISABLED      ((unsigned int) 0x00000002)

#define TMR_TMR16B0MCR_MR0_INT_MASK               ((unsigned int) 0x00000001) // Interrupt on MRO
#define TMR_TMR16B0MCR_MR0_INT_ENABLED            ((unsigned int) 0x00000001)
#define TMR_TMR16B0MCR_MR0_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR16B0MCR_MR0_RESET_MASK             ((unsigned int) 0x00000002) // Reset on MR0
#define TMR_TMR16B0MCR_MR0_RESET_ENABLED          ((unsigned int) 0x00000002)
#define TMR_TMR16B0MCR_MR0_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR16B0MCR_MR0_STOP_MASK              ((unsigned int) 0x00000004) // Stop on MR0
#define TMR_TMR16B0MCR_MR0_STOP_ENABLED           ((unsigned int) 0x00000004)
#define TMR_TMR16B0MCR_MR0_STOP_DISABLED          ((unsigned int) 0x00000000)
#define TMR_TMR16B0MCR_MR1_INT_MASK               ((unsigned int) 0x00000008) // Interrupt on MR1
#define TMR_TMR16B0MCR_MR1_INT_ENABLED            ((unsigned int) 0x00000008)
#define TMR_TMR16B0MCR_MR1_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR16B0MCR_MR1_RESET_MASK             ((unsigned int) 0x00000010) // Reset on MR1
#define TMR_TMR16B0MCR_MR1_RESET_ENABLED          ((unsigned int) 0x00000010)
#define TMR_TMR16B0MCR_MR1_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR16B0MCR_MR1_STOP_MASK              ((unsigned int) 0x00000020) // Stop on MR1
#define TMR_TMR16B0MCR_MR1_STOP_ENABLED           ((unsigned int) 0x00000020)
#define TMR_TMR16B0MCR_MR1_STOP_DISABLED          ((unsigned int) 0x00000000)
#define TMR_TMR16B0MCR_MR2_INT_MASK               ((unsigned int) 0x00000040) // Interrupt on MR2
#define TMR_TMR16B0MCR_MR2_INT_ENABLED            ((unsigned int) 0x00000040)
#define TMR_TMR16B0MCR_MR2_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR16B0MCR_MR2_RESET_MASK             ((unsigned int) 0x00000080) // Reset on MR2
#define TMR_TMR16B0MCR_MR2_RESET_ENABLED          ((unsigned int) 0x00000080)
#define TMR_TMR16B0MCR_MR2_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR16B0MCR_MR2_STOP_MASK              ((unsigned int) 0x00000100) // Stop on MR2
#define TMR_TMR16B0MCR_MR2_STOP_ENABLED           ((unsigned int) 0x00000100)
#define TMR_TMR16B0MCR_MR2_STOP_DISABLED          ((unsigned int) 0x00000000)
#define TMR_TMR16B0MCR_MR3_INT_MASK               ((unsigned int) 0x00000200) // Interrupt on MR3
#define TMR_TMR16B0MCR_MR3_INT_ENABLED            ((unsigned int) 0x00000200)
#define TMR_TMR16B0MCR_MR3_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR16B0MCR_MR3_RESET_MASK             ((unsigned int) 0x00000400) // Reset on MR3
#define TMR_TMR16B0MCR_MR3_RESET_ENABLED          ((unsigned int) 0x00000400)
#define TMR_TMR16B0MCR_MR3_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR16B0MCR_MR3_STOP_MASK              ((unsigned int) 0x00000800) // Stop on MR3
#define TMR_TMR16B0MCR_MR3_STOP_ENABLED           ((unsigned int) 0x00000800)
#define TMR_TMR16B0MCR_MR3_STOP_DISABLED          ((unsigned int) 0x00000000)

#define TMR_TMR16B0CCR_CAP0RE_MASK                ((unsigned int) 0x00000001) // Capture on rising edge
#define TMR_TMR16B0CCR_CAP0RE_ENABLED             ((unsigned int) 0x00000001)
#define TMR_TMR16B0CCR_CAP0RE_DISABLED            ((unsigned int) 0x00000000)
#define TMR_TMR16B0CCR_CAP0FE_MASK                ((unsigned int) 0x00000002) // Capture on falling edge
#define TMR_TMR16B0CCR_CAP0FE_ENABLED             ((unsigned int) 0x00000002)
#define TMR_TMR16B0CCR_CAP0FE_DISABLED            ((unsigned int) 0x00000000)
#define TMR_TMR16B0CCR_CAP0I_MASK                 ((unsigned int) 0x00000004) // Interrupt on CAP0 event
#define TMR_TMR16B0CCR_CAP0I_ENABLED              ((unsigned int) 0x00000004)
#define TMR_TMR16B0CCR_CAP0I_DISABLED             ((unsigned int) 0x00000000)

#define TMR_TMR16B0EMR_EM0_MASK                   ((unsigned int) 0x00000001) // External match 0
#define TMR_TMR16B0EMR_EM0                        ((unsigned int) 0x00000001)
#define TMR_TMR16B0EMR_EMC0_MASK                  ((unsigned int) 0x00000030)
#define TMR_TMR16B0EMR_EMC0_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR16B0EMR_EMC0_LOW                   ((unsigned int) 0x00000010)
#define TMR_TMR16B0EMR_EMC0_HIGH                  ((unsigned int) 0x00000020)
#define TMR_TMR16B0EMR_EMC0_TOGGLE                ((unsigned int) 0x00000030)
#define TMR_TMR16B0EMR_EM1_MASK                   ((unsigned int) 0x00000002) // External match 1
#define TMR_TMR16B0EMR_EM1                        ((unsigned int) 0x00000002)
#define TMR_TMR16B0EMR_EMC1_MASK                  ((unsigned int) 0x000000C0)
#define TMR_TMR16B0EMR_EMC1_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR16B0EMR_EMC1_LOW                   ((unsigned int) 0x00000040)
#define TMR_TMR16B0EMR_EMC1_HIGH                  ((unsigned int) 0x00000080)
#define TMR_TMR16B0EMR_EMC1_TOGGLE                ((unsigned int) 0x000000C0)
#define TMR_TMR16B0EMR_EM2_MASK                   ((unsigned int) 0x00000004) // External match 2
#define TMR_TMR16B0EMR_EM2                        ((unsigned int) 0x00000004)
#define TMR_TMR16B0EMR_EMC2_MASK                  ((unsigned int) 0x00000300)
#define TMR_TMR16B0EMR_EMC2_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR16B0EMR_EMC2_LOW                   ((unsigned int) 0x00000100)
#define TMR_TMR16B0EMR_EMC2_HIGH                  ((unsigned int) 0x00000200)
#define TMR_TMR16B0EMR_EMC2_TOGGLE                ((unsigned int) 0x00000300)
#define TMR_TMR16B0EMR_EM3_MASK                   ((unsigned int) 0x00000008) // External match 3
#define TMR_TMR16B0EMR_EM3                        ((unsigned int) 0x00000008)
#define TMR_TMR16B0EMR_EMC3_MASK                  ((unsigned int) 0x00000C00)
#define TMR_TMR16B0EMR_EMC3_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR16B0EMR_EMC3_LOW                   ((unsigned int) 0x00000400)
#define TMR_TMR16B0EMR_EMC3_HIGH                  ((unsigned int) 0x00000800)
#define TMR_TMR16B0EMR_EMC3_TOGGLE                ((unsigned int) 0x00000C00)

#define TMR_TMR16B0CTCR_CTMODE_MASK               ((unsigned int) 0x00000003) // Counter/Timer mode
#define TMR_TMR16B0CTCR_CTMODE_TIMER              ((unsigned int) 0x00000000) // Timer Mode: Every rising PCLK edge
#define TMR_TMR16B0CTCR_CTMODE_COUNTERRISING      ((unsigned int) 0x00000001) // Counter: TC increments on rising edge of input
#define TMR_TMR16B0CTCR_CTMODE_COUNTERFALLING     ((unsigned int) 0x00000002) // Counter: TC increments on falling edge of input
#define TMR_TMR16B0CTCR_CTMODE_COUNTERBOTH        ((unsigned int) 0x00000003) // Counter: TC increments on both edges of input
#define TMR_TMR16B0CTCR_CINPUTSELECT_MASK         ((unsigned int) 0x0000000C)   
#define TMR_TMR16B0CTCR_CINPUTSELECT              ((unsigned int) 0x00000000) // CINPUTSELECT must be set to 00

#define TMR_TMR16B0PWMC_PWM0_MASK                 ((unsigned int) 0x00000001)   
#define TMR_TMR16B0PWMC_PWM0_ENABLED              ((unsigned int) 0x00000001) // PWM mode is enabled for CT16Bn_MAT0
#define TMR_TMR16B0PWMC_PWM0_DISABLED             ((unsigned int) 0x00000000)
#define TMR_TMR16B0PWMC_PWM1_MASK                 ((unsigned int) 0x00000002)   
#define TMR_TMR16B0PWMC_PWM1_ENABLED              ((unsigned int) 0x00000002) // PWM mode is enabled for CT16Bn_MAT1
#define TMR_TMR16B0PWMC_PWM1_DISABLED             ((unsigned int) 0x00000000)
#define TMR_TMR16B0PWMC_PWM2_MASK                 ((unsigned int) 0x00000004)   
#define TMR_TMR16B0PWMC_PWM2_ENABLED              ((unsigned int) 0x00000004) // PWM mode is enabled for CT16Bn_MAT2
#define TMR_TMR16B0PWMC_PWM2_DISABLED             ((unsigned int) 0x00000000)
#define TMR_TMR16B0PWMC_PWM3_MASK                 ((unsigned int) 0x00000008)   
#define TMR_TMR16B0PWMC_PWM3_ENABLED              ((unsigned int) 0x00000008)
#define TMR_TMR16B0PWMC_PWM3_DISABLED             ((unsigned int) 0x00000000)

#define TMR_CT16B1_BASE_ADDRESS                   (0x40010000)

#define TMR_TMR16B1IR                             (*(pREG32 (0x40010000)))    // Interrupt register
#define TMR_TMR16B1TCR                            (*(pREG32 (0x40010004)))    // Timer control register
#define TMR_TMR16B1TC                             (*(pREG32 (0x40010008)))    // Timer counter
#define TMR_TMR16B1PR                             (*(pREG32 (0x4001000C)))    // Prescale register
#define TMR_TMR16B1PC                             (*(pREG32 (0x40010010)))    // Prescale counter register
#define TMR_TMR16B1MCR                            (*(pREG32 (0x40010014)))    // Match control register
#define TMR_TMR16B1MR0                            (*(pREG32 (0x40010018)))    // Match register 0
#define TMR_TMR16B1MR1                            (*(pREG32 (0x4001001C)))    // Match register 1
#define TMR_TMR16B1MR2                            (*(pREG32 (0x40010020)))    // Match register 2
#define TMR_TMR16B1MR3                            (*(pREG32 (0x40010024)))    // Match register 3
#define TMR_TMR16B1CCR                            (*(pREG32 (0x40010028)))    // Capture control register
#define TMR_TMR16B1CR0                            (*(pREG32 (0x4001002C)))    // Capture register
#define TMR_TMR16B1EMR                            (*(pREG32 (0x4001003C)))    // External match register
#define TMR_TMR16B1CTCR                           (*(pREG32 (0x40010070)))    // Count control register
#define TMR_TMR16B1PWMC                           (*(pREG32 (0x40010074)))    // PWM control register

#define TMR_TMR16B1IR_MR0_MASK                    ((unsigned int) 0x00000001) // Interrupt flag for match channel 0
#define TMR_TMR16B1IR_MR0                         ((unsigned int) 0x00000001)
#define TMR_TMR16B1IR_MR1_MASK                    ((unsigned int) 0x00000002) // Interrupt flag for match channel 1
#define TMR_TMR16B1IR_MR1                         ((unsigned int) 0x00000002)
#define TMR_TMR16B1IR_MR2_MASK                    ((unsigned int) 0x00000004) // Interrupt flag for match channel 2
#define TMR_TMR16B1IR_MR2                         ((unsigned int) 0x00000004)
#define TMR_TMR16B1IR_MR3_MASK                    ((unsigned int) 0x00000008) // Interrupt flag for match channel 3
#define TMR_TMR16B1IR_MR3                         ((unsigned int) 0x00000008)
#define TMR_TMR16B1IR_CR0_MASK                    ((unsigned int) 0x00000010) // Interrupt flag for capture channel 0 event
#define TMR_TMR16B1IR_CR0                         ((unsigned int) 0x00000010)
#define TMR_TMR16B1IR_MASK_ALL                    ((unsigned int) 0x0000001F)

#define TMR_TMR16B1TCR_COUNTERENABLE_MASK         ((unsigned int) 0x00000001) // Counter enable
#define TMR_TMR16B1TCR_COUNTERENABLE_ENABLED      ((unsigned int) 0x00000001)
#define TMR_TMR16B1TCR_COUNTERENABLE_DISABLED     ((unsigned int) 0x00000000)
#define TMR_TMR16B1TCR_COUNTERRESET_MASK          ((unsigned int) 0x00000002)
#define TMR_TMR16B1TCR_COUNTERRESET_ENABLED       ((unsigned int) 0x00000002)
#define TMR_TMR16B1TCR_COUNTERRESET_DISABLED      ((unsigned int) 0x00000002)

#define TMR_TMR16B1MCR_MR0_INT_MASK               ((unsigned int) 0x00000001) // Interrupt on MRO
#define TMR_TMR16B1MCR_MR0_INT_ENABLED            ((unsigned int) 0x00000001)
#define TMR_TMR16B1MCR_MR0_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR16B1MCR_MR0_RESET_MASK             ((unsigned int) 0x00000002) // Reset on MR0
#define TMR_TMR16B1MCR_MR0_RESET_ENABLED          ((unsigned int) 0x00000002)
#define TMR_TMR16B1MCR_MR0_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR16B1MCR_MR0_STOP_MASK              ((unsigned int) 0x00000004) // Stop on MR0
#define TMR_TMR16B1MCR_MR0_STOP_ENABLED           ((unsigned int) 0x00000004)
#define TMR_TMR16B1MCR_MR0_STOP_DISABLED          ((unsigned int) 0x00000000)
#define TMR_TMR16B1MCR_MR1_INT_MASK               ((unsigned int) 0x00000008) // Interrupt on MR1
#define TMR_TMR16B1MCR_MR1_INT_ENABLED            ((unsigned int) 0x00000008)
#define TMR_TMR16B1MCR_MR1_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR16B1MCR_MR1_RESET_MASK             ((unsigned int) 0x00000010) // Reset on MR1
#define TMR_TMR16B1MCR_MR1_RESET_ENABLED          ((unsigned int) 0x00000010)
#define TMR_TMR16B1MCR_MR1_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR16B1MCR_MR1_STOP_MASK              ((unsigned int) 0x00000020) // Stop on MR1
#define TMR_TMR16B1MCR_MR1_STOP_ENABLED           ((unsigned int) 0x00000020)
#define TMR_TMR16B1MCR_MR1_STOP_DISABLED          ((unsigned int) 0x00000000)
#define TMR_TMR16B1MCR_MR2_INT_MASK               ((unsigned int) 0x00000040) // Interrupt on MR2
#define TMR_TMR16B1MCR_MR2_INT_ENABLED            ((unsigned int) 0x00000040)
#define TMR_TMR16B1MCR_MR2_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR16B1MCR_MR2_RESET_MASK             ((unsigned int) 0x00000080) // Reset on MR2
#define TMR_TMR16B1MCR_MR2_RESET_ENABLED          ((unsigned int) 0x00000080)
#define TMR_TMR16B1MCR_MR2_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR16B1MCR_MR2_STOP_MASK              ((unsigned int) 0x00000100) // Stop on MR2
#define TMR_TMR16B1MCR_MR2_STOP_ENABLED           ((unsigned int) 0x00000100)
#define TMR_TMR16B1MCR_MR2_STOP_DISABLED          ((unsigned int) 0x00000000)
#define TMR_TMR16B1MCR_MR3_INT_MASK               ((unsigned int) 0x00000200) // Interrupt on MR3
#define TMR_TMR16B1MCR_MR3_INT_ENABLED            ((unsigned int) 0x00000200)
#define TMR_TMR16B1MCR_MR3_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR16B1MCR_MR3_RESET_MASK             ((unsigned int) 0x00000400) // Reset on MR3
#define TMR_TMR16B1MCR_MR3_RESET_ENABLED          ((unsigned int) 0x00000400)
#define TMR_TMR16B1MCR_MR3_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR16B1MCR_MR3_STOP_MASK              ((unsigned int) 0x00000800) // Stop on MR3
#define TMR_TMR16B1MCR_MR3_STOP_ENABLED           ((unsigned int) 0x00000800)
#define TMR_TMR16B1MCR_MR3_STOP_DISABLED          ((unsigned int) 0x00000000)

#define TMR_TMR16B1CCR_CAP0RE_MASK                ((unsigned int) 0x00000001) // Capture on rising edge
#define TMR_TMR16B1CCR_CAP0RE_ENABLED             ((unsigned int) 0x00000001)
#define TMR_TMR16B1CCR_CAP0RE_DISABLED            ((unsigned int) 0x00000000)
#define TMR_TMR16B1CCR_CAP0FE_MASK                ((unsigned int) 0x00000002) // Capture on falling edge
#define TMR_TMR16B1CCR_CAP0FE_ENABLED             ((unsigned int) 0x00000002)
#define TMR_TMR16B1CCR_CAP0FE_DISABLED            ((unsigned int) 0x00000000)
#define TMR_TMR16B1CCR_CAP0I_MASK                 ((unsigned int) 0x00000004) // Interrupt on CAP0 event
#define TMR_TMR16B1CCR_CAP0I_ENABLED              ((unsigned int) 0x00000004)
#define TMR_TMR16B1CCR_CAP0I_DISABLED             ((unsigned int) 0x00000000)

#define TMR_TMR16B1EMR_EM0_MASK                   ((unsigned int) 0x00000001) // External match 0
#define TMR_TMR16B1EMR_EM0                        ((unsigned int) 0x00000001)
#define TMR_TMR16B1EMR_EMC0_MASK                  ((unsigned int) 0x00000030)
#define TMR_TMR16B1EMR_EMC0_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR16B1EMR_EMC0_LOW                   ((unsigned int) 0x00000010)
#define TMR_TMR16B1EMR_EMC0_HIGH                  ((unsigned int) 0x00000020)
#define TMR_TMR16B1EMR_EMC0_TOGGLE                ((unsigned int) 0x00000030)
#define TMR_TMR16B1EMR_EM1_MASK                   ((unsigned int) 0x00000002) // External match 1
#define TMR_TMR16B1EMR_EM1                        ((unsigned int) 0x00000002)
#define TMR_TMR16B1EMR_EMC1_MASK                  ((unsigned int) 0x000000C0)
#define TMR_TMR16B1EMR_EMC1_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR16B1EMR_EMC1_LOW                   ((unsigned int) 0x00000040)
#define TMR_TMR16B1EMR_EMC1_HIGH                  ((unsigned int) 0x00000080)
#define TMR_TMR16B1EMR_EMC1_TOGGLE                ((unsigned int) 0x000000C0)
#define TMR_TMR16B1EMR_EM2_MASK                   ((unsigned int) 0x00000004) // External match 2
#define TMR_TMR16B1EMR_EM2                        ((unsigned int) 0x00000004)
#define TMR_TMR16B1EMR_EMC2_MASK                  ((unsigned int) 0x00000300)
#define TMR_TMR16B1EMR_EMC2_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR16B1EMR_EMC2_LOW                   ((unsigned int) 0x00000100)
#define TMR_TMR16B1EMR_EMC2_HIGH                  ((unsigned int) 0x00000200)
#define TMR_TMR16B1EMR_EMC2_TOGGLE                ((unsigned int) 0x00000300)
#define TMR_TMR16B1EMR_EM3_MASK                   ((unsigned int) 0x00000008) // External match 3
#define TMR_TMR16B1EMR_EM3                        ((unsigned int) 0x00000008)
#define TMR_TMR16B1EMR_EMC3_MASK                  ((unsigned int) 0x00000C00)
#define TMR_TMR16B1EMR_EMC3_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR16B1EMR_EMC3_LOW                   ((unsigned int) 0x00000400)
#define TMR_TMR16B1EMR_EMC3_HIGH                  ((unsigned int) 0x00000800)
#define TMR_TMR16B1EMR_EMC3_TOGGLE                ((unsigned int) 0x00000C00)

#define TMR_TMR16B1CTCR_CTMODE_MASK               ((unsigned int) 0x00000003) // Counter/Timer mode
#define TMR_TMR16B1CTCR_CTMODE_TIMER              ((unsigned int) 0x00000000) // Timer Mode: Every rising PCLK edge
#define TMR_TMR16B1CTCR_CTMODE_COUNTERRISING      ((unsigned int) 0x00000001) // Counter: TC increments on rising edge of input
#define TMR_TMR16B1CTCR_CTMODE_COUNTERFALLING     ((unsigned int) 0x00000002) // Counter: TC increments on falling edge of input
#define TMR_TMR16B1CTCR_CTMODE_COUNTERBOTH        ((unsigned int) 0x00000003) // Counter: TC increments on both edges of input
#define TMR_TMR16B1CTCR_CINPUTSELECT_MASK         ((unsigned int) 0x0000000C)   
#define TMR_TMR16B1CTCR_CINPUTSELECT              ((unsigned int) 0x00000000) // CINPUTSELECT must be set to 00

#define TMR_TMR16B1PWMC_PWM0_MASK                 ((unsigned int) 0x00000001)   
#define TMR_TMR16B1PWMC_PWM0_ENABLED              ((unsigned int) 0x00000001) // PWM mode is enabled for CT16Bn_MAT0
#define TMR_TMR16B1PWMC_PWM0_DISABLED             ((unsigned int) 0x00000000)
#define TMR_TMR16B1PWMC_PWM1_MASK                 ((unsigned int) 0x00000002)   
#define TMR_TMR16B1PWMC_PWM1_ENABLED              ((unsigned int) 0x00000002) // PWM mode is enabled for CT16Bn_MAT1
#define TMR_TMR16B1PWMC_PWM1_DISABLED             ((unsigned int) 0x00000000)
#define TMR_TMR16B1PWMC_PWM2_MASK                 ((unsigned int) 0x00000004)   
#define TMR_TMR16B1PWMC_PWM2_ENABLED              ((unsigned int) 0x00000004) // PWM mode is enabled for CT16Bn_MAT2
#define TMR_TMR16B1PWMC_PWM2_DISABLED             ((unsigned int) 0x00000000)
#define TMR_TMR16B1PWMC_PWM3_MASK                 ((unsigned int) 0x00000008)   
#define TMR_TMR16B1PWMC_PWM3_ENABLED              ((unsigned int) 0x00000008)
#define TMR_TMR16B1PWMC_PWM3_DISABLED             ((unsigned int) 0x00000000)

/*##############################################################################
## 32-Bit Timers (CT32B0/1)
##############################################################################*/

#define TMR_CT32B0_BASE_ADDRESS                   (0x40014000)

#define TMR_TMR32B0IR                             (*(pREG32 (0x40014000)))      // Interrupt register
#define TMR_TMR32B0TCR                            (*(pREG32 (0x40014004)))      // Timer control register
#define TMR_TMR32B0TC                             (*(pREG32 (0x40014008)))      // Timer counter
#define TMR_TMR32B0PR                             (*(pREG32 (0x4001400C)))      // Prescale register
#define TMR_TMR32B0PC                             (*(pREG32 (0x40014010)))      // Prescale counter register
#define TMR_TMR32B0MCR                            (*(pREG32 (0x40014014)))      // Match control register
#define TMR_TMR32B0MR0                            (*(pREG32 (0x40014018)))      // Match register 0
#define TMR_TMR32B0MR1                            (*(pREG32 (0x4001401C)))      // Match register 1
#define TMR_TMR32B0MR2                            (*(pREG32 (0x40014020)))      // Match register 2
#define TMR_TMR32B0MR3                            (*(pREG32 (0x40014024)))      // Match register 3
#define TMR_TMR32B0CCR                            (*(pREG32 (0x40014028)))      // Capture control register
#define TMR_TMR32B0CR0                            (*(pREG32 (0x4001402C)))      // Capture register
#define TMR_TMR32B0EMR                            (*(pREG32 (0x4001403C)))      // External match register
#define TMR_TMR32B0CTCR                           (*(pREG32 (0x40014070)))      // Count control register
#define TMR_TMR32B0PWMC                           (*(pREG32 (0x40014074)))      // PWM control register

#define TMR_TMR32B0IR_MR0_MASK                    ((unsigned int) 0x00000001)   // Interrupt flag for match channel 0
#define TMR_TMR32B0IR_MR0                         ((unsigned int) 0x00000001)
#define TMR_TMR32B0IR_MR1_MASK                    ((unsigned int) 0x00000002)   // Interrupt flag for match channel 1
#define TMR_TMR32B0IR_MR1                         ((unsigned int) 0x00000002)
#define TMR_TMR32B0IR_MR2_MASK                    ((unsigned int) 0x00000004)   // Interrupt flag for match channel 2
#define TMR_TMR32B0IR_MR2                         ((unsigned int) 0x00000004)
#define TMR_TMR32B0IR_MR3_MASK                    ((unsigned int) 0x00000008)   // Interrupt flag for match channel 3
#define TMR_TMR32B0IR_MR3                         ((unsigned int) 0x00000008)
#define TMR_TMR32B0IR_CR0_MASK                    ((unsigned int) 0x00000010)   // Interrupt flag for capture channel 0 event
#define TMR_TMR32B0IR_CR0                         ((unsigned int) 0x00000010)
#define TMR_TMR32B0IR_MASK_ALL                    ((unsigned int) 0x0000001F)

#define TMR_TMR32B0TCR_COUNTERENABLE_MASK         ((unsigned int) 0x00000001)   // Counter enable
#define TMR_TMR32B0TCR_COUNTERENABLE_ENABLED      ((unsigned int) 0x00000001)
#define TMR_TMR32B0TCR_COUNTERENABLE_DISABLED     ((unsigned int) 0x00000000)
#define TMR_TMR32B0TCR_COUNTERRESET_MASK          ((unsigned int) 0x00000002)
#define TMR_TMR32B0TCR_COUNTERRESET_ENABLED       ((unsigned int) 0x00000002)
#define TMR_TMR32B0TCR_COUNTERRESET_DISABLED      ((unsigned int) 0x00000002)

#define TMR_TMR32B0MCR_MR0_INT_MASK               ((unsigned int) 0x00000001)   // Interrupt on MRO
#define TMR_TMR32B0MCR_MR0_INT_ENABLED            ((unsigned int) 0x00000001)
#define TMR_TMR32B0MCR_MR0_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR32B0MCR_MR0_RESET_MASK             ((unsigned int) 0x00000002)   // Reset on MR0
#define TMR_TMR32B0MCR_MR0_RESET_ENABLED          ((unsigned int) 0x00000002)
#define TMR_TMR32B0MCR_MR0_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR32B0MCR_MR0_STOP_MASK              ((unsigned int) 0x00000004)   // Stop on MR0
#define TMR_TMR32B0MCR_MR0_STOP_ENABLED           ((unsigned int) 0x00000004)
#define TMR_TMR32B0MCR_MR0_STOP_DISABLED          ((unsigned int) 0x00000000)
#define TMR_TMR32B0MCR_MR1_INT_MASK               ((unsigned int) 0x00000008)   // Interrupt on MR1
#define TMR_TMR32B0MCR_MR1_INT_ENABLED            ((unsigned int) 0x00000008)
#define TMR_TMR32B0MCR_MR1_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR32B0MCR_MR1_RESET_MASK             ((unsigned int) 0x00000010)   // Reset on MR1
#define TMR_TMR32B0MCR_MR1_RESET_ENABLED          ((unsigned int) 0x00000010)
#define TMR_TMR32B0MCR_MR1_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR32B0MCR_MR1_STOP_MASK              ((unsigned int) 0x00000020)   // Stop on MR1
#define TMR_TMR32B0MCR_MR1_STOP_ENABLED           ((unsigned int) 0x00000020)
#define TMR_TMR32B0MCR_MR1_STOP_DISABLED          ((unsigned int) 0x00000000)
#define TMR_TMR32B0MCR_MR2_INT_MASK               ((unsigned int) 0x00000040)   // Interrupt on MR2
#define TMR_TMR32B0MCR_MR2_INT_ENABLED            ((unsigned int) 0x00000040)
#define TMR_TMR32B0MCR_MR2_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR32B0MCR_MR2_RESET_MASK             ((unsigned int) 0x00000080)   // Reset on MR2
#define TMR_TMR32B0MCR_MR2_RESET_ENABLED          ((unsigned int) 0x00000080)
#define TMR_TMR32B0MCR_MR2_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR32B0MCR_MR2_STOP_MASK              ((unsigned int) 0x00000100)   // Stop on MR2
#define TMR_TMR32B0MCR_MR2_STOP_ENABLED           ((unsigned int) 0x00000100)
#define TMR_TMR32B0MCR_MR2_STOP_DISABLED          ((unsigned int) 0x00000000)
#define TMR_TMR32B0MCR_MR3_INT_MASK               ((unsigned int) 0x00000200)   // Interrupt on MR3
#define TMR_TMR32B0MCR_MR3_INT_ENABLED            ((unsigned int) 0x00000200)
#define TMR_TMR32B0MCR_MR3_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR32B0MCR_MR3_RESET_MASK             ((unsigned int) 0x00000400)   // Reset on MR3
#define TMR_TMR32B0MCR_MR3_RESET_ENABLED          ((unsigned int) 0x00000400)
#define TMR_TMR32B0MCR_MR3_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR32B0MCR_MR3_STOP_MASK              ((unsigned int) 0x00000800)   // Stop on MR3
#define TMR_TMR32B0MCR_MR3_STOP_ENABLED           ((unsigned int) 0x00000800)
#define TMR_TMR32B0MCR_MR3_STOP_DISABLED          ((unsigned int) 0x00000000)

#define TMR_TMR32B0CCR_CAP0RE_MASK                ((unsigned int) 0x00000001)   // Capture on rising edge
#define TMR_TMR32B0CCR_CAP0RE_ENABLED             ((unsigned int) 0x00000001)
#define TMR_TMR32B0CCR_CAP0RE_DISABLED            ((unsigned int) 0x00000000)
#define TMR_TMR32B0CCR_CAP0FE_MASK                ((unsigned int) 0x00000002)   // Capture on falling edge
#define TMR_TMR32B0CCR_CAP0FE_ENABLED             ((unsigned int) 0x00000002)
#define TMR_TMR32B0CCR_CAP0FE_DISABLED            ((unsigned int) 0x00000000)
#define TMR_TMR32B0CCR_CAP0I_MASK                 ((unsigned int) 0x00000004)   // Interrupt on CAP0 event
#define TMR_TMR32B0CCR_CAP0I_ENABLED              ((unsigned int) 0x00000004)
#define TMR_TMR32B0CCR_CAP0I_DISABLED             ((unsigned int) 0x00000000)

#define TMR_TMR32B0EMR_EM0_MASK                   ((unsigned int) 0x00000001)   // External match 0
#define TMR_TMR32B0EMR_EM0                        ((unsigned int) 0x00000001)
#define TMR_TMR32B0EMR_EMC0_MASK                  ((unsigned int) 0x00000030)
#define TMR_TMR32B0EMR_EMC0_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR32B0EMR_EMC0_LOW                   ((unsigned int) 0x00000010)
#define TMR_TMR32B0EMR_EMC0_HIGH                  ((unsigned int) 0x00000020)
#define TMR_TMR32B0EMR_EMC0_TOGGLE                ((unsigned int) 0x00000030)
#define TMR_TMR32B0EMR_EM1_MASK                   ((unsigned int) 0x00000002)   // External match 1
#define TMR_TMR32B0EMR_EM1                        ((unsigned int) 0x00000002)
#define TMR_TMR32B0EMR_EMC1_MASK                  ((unsigned int) 0x000000C0)
#define TMR_TMR32B0EMR_EMC1_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR32B0EMR_EMC1_LOW                   ((unsigned int) 0x00000040)
#define TMR_TMR32B0EMR_EMC1_HIGH                  ((unsigned int) 0x00000080)
#define TMR_TMR32B0EMR_EMC1_TOGGLE                ((unsigned int) 0x000000C0)
#define TMR_TMR32B0EMR_EM2_MASK                   ((unsigned int) 0x00000004)   // External match 2
#define TMR_TMR32B0EMR_EM2                        ((unsigned int) 0x00000004)
#define TMR_TMR32B0EMR_EMC2_MASK                  ((unsigned int) 0x00000300)
#define TMR_TMR32B0EMR_EMC2_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR32B0EMR_EMC2_LOW                   ((unsigned int) 0x00000100)
#define TMR_TMR32B0EMR_EMC2_HIGH                  ((unsigned int) 0x00000200)
#define TMR_TMR32B0EMR_EMC2_TOGGLE                ((unsigned int) 0x00000300)
#define TMR_TMR32B0EMR_EM3_MASK                   ((unsigned int) 0x00000008)   // External match 3
#define TMR_TMR32B0EMR_EM3                        ((unsigned int) 0x00000008)
#define TMR_TMR32B0EMR_EMC3_MASK                  ((unsigned int) 0x00000C00)
#define TMR_TMR32B0EMR_EMC3_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR32B0EMR_EMC3_LOW                   ((unsigned int) 0x00000400)
#define TMR_TMR32B0EMR_EMC3_HIGH                  ((unsigned int) 0x00000800)
#define TMR_TMR32B0EMR_EMC3_TOGGLE                ((unsigned int) 0x00000C00)

#define TMR_TMR32B0CTCR_CTMODE_MASK               ((unsigned int) 0x00000003)   // Counter/Timer mode
#define TMR_TMR32B0CTCR_CTMODE_TIMER              ((unsigned int) 0x00000000)   // Timer Mode: Every rising PCLK edge
#define TMR_TMR32B0CTCR_CTMODE_COUNTERRISING      ((unsigned int) 0x00000001)   // Counter: TC increments on rising edge of input
#define TMR_TMR32B0CTCR_CTMODE_COUNTERFALLING     ((unsigned int) 0x00000002)   // Counter: TC increments on falling edge of input
#define TMR_TMR32B0CTCR_CTMODE_COUNTERBOTH        ((unsigned int) 0x00000003)   // Counter: TC increments on both edges of input
#define TMR_TMR32B0CTCR_CINPUTSELECT_MASK         ((unsigned int) 0x0000000C)   
#define TMR_TMR32B0CTCR_CINPUTSELECT              ((unsigned int) 0x00000000)   // CINPUTSELECT must be set to 00

#define TMR_TMR32B0PWMC_PWM0_MASK                 ((unsigned int) 0x00000001)   
#define TMR_TMR32B0PWMC_PWM0_ENABLED              ((unsigned int) 0x00000001)   // PWM mode is enabled for CT32Bn_MAT0
#define TMR_TMR32B0PWMC_PWM0_DISABLED             ((unsigned int) 0x00000000)
#define TMR_TMR32B0PWMC_PWM1_MASK                 ((unsigned int) 0x00000002)   
#define TMR_TMR32B0PWMC_PWM1_ENABLED              ((unsigned int) 0x00000002)   // PWM mode is enabled for CT32Bn_MAT1
#define TMR_TMR32B0PWMC_PWM1_DISABLED             ((unsigned int) 0x00000000)
#define TMR_TMR32B0PWMC_PWM2_MASK                 ((unsigned int) 0x00000004)   
#define TMR_TMR32B0PWMC_PWM2_ENABLED              ((unsigned int) 0x00000004)   // PWM mode is enabled for CT32Bn_MAT2
#define TMR_TMR32B0PWMC_PWM2_DISABLED             ((unsigned int) 0x00000000)
#define TMR_TMR32B0PWMC_PWM3_MASK                 ((unsigned int) 0x00000008)
#define TMR_TMR32B0PWMC_PWM3_ENABLED              ((unsigned int) 0x00000008)   // PWM mode is enabled for CT32Bn_MAT3
#define TMR_TMR32B0PWMC_PWM3_DISABLED             ((unsigned int) 0x00000000)

#define TMR_CT32B1_BASE_ADDRESS                   (0x40018000)

#define TMR_TMR32B1IR                             (*(pREG32 (0x40018000)))      // Interrupt register
#define TMR_TMR32B1TCR                            (*(pREG32 (0x40018004)))      // Timer control register
#define TMR_TMR32B1TC                             (*(pREG32 (0x40018008)))      // Timer counter
#define TMR_TMR32B1PR                             (*(pREG32 (0x4001800C)))      // Prescale register
#define TMR_TMR32B1PC                             (*(pREG32 (0x40018010)))      // Prescale counter register
#define TMR_TMR32B1MCR                            (*(pREG32 (0x40018014)))      // Match control register
#define TMR_TMR32B1MR0                            (*(pREG32 (0x40018018)))      // Match register 0
#define TMR_TMR32B1MR1                            (*(pREG32 (0x4001801C)))      // Match register 1
#define TMR_TMR32B1MR2                            (*(pREG32 (0x40018020)))      // Match register 2
#define TMR_TMR32B1MR3                            (*(pREG32 (0x40018024)))      // Match register 3
#define TMR_TMR32B1CCR                            (*(pREG32 (0x40018028)))      // Capture control register
#define TMR_TMR32B1CR0                            (*(pREG32 (0x4001802C)))      // Capture register
#define TMR_TMR32B1EMR                            (*(pREG32 (0x4001803C)))      // External match register
#define TMR_TMR32B1CTCR                           (*(pREG32 (0x40018070)))      // Count control register
#define TMR_TMR32B1PWMC                           (*(pREG32 (0x40018074)))      // PWM control register

#define TMR_TMR32B1IR_MR0_MASK                    ((unsigned int) 0x00000001)   // Interrupt flag for match channel 0
#define TMR_TMR32B1IR_MR0                         ((unsigned int) 0x00000001)
#define TMR_TMR32B1IR_MR1_MASK                    ((unsigned int) 0x00000002)   // Interrupt flag for match channel 1
#define TMR_TMR32B1IR_MR1                         ((unsigned int) 0x00000002)
#define TMR_TMR32B1IR_MR2_MASK                    ((unsigned int) 0x00000004)   // Interrupt flag for match channel 2
#define TMR_TMR32B1IR_MR2                         ((unsigned int) 0x00000004)
#define TMR_TMR32B1IR_MR3_MASK                    ((unsigned int) 0x00000008)   // Interrupt flag for match channel 3
#define TMR_TMR32B1IR_MR3                         ((unsigned int) 0x00000008)
#define TMR_TMR32B1IR_CR0_MASK                    ((unsigned int) 0x00000010)   // Interrupt flag for capture channel 0 event
#define TMR_TMR32B1IR_CR0                         ((unsigned int) 0x00000010)
#define TMR_TMR32B1IR_MASK_ALL                    ((unsigned int) 0x0000001F)

#define TMR_TMR32B1TCR_COUNTERENABLE_MASK         ((unsigned int) 0x00000001)   // Counter enable
#define TMR_TMR32B1TCR_COUNTERENABLE_ENABLED      ((unsigned int) 0x00000001)
#define TMR_TMR32B1TCR_COUNTERENABLE_DISABLED     ((unsigned int) 0x00000000)
#define TMR_TMR32B1TCR_COUNTERRESET_MASK          ((unsigned int) 0x00000002)
#define TMR_TMR32B1TCR_COUNTERRESET_ENABLED       ((unsigned int) 0x00000002)
#define TMR_TMR32B1TCR_COUNTERRESET_DISABLED      ((unsigned int) 0x00000002)

#define TMR_TMR32B1MCR_MR0_INT_MASK               ((unsigned int) 0x00000001)   // Interrupt on MRO
#define TMR_TMR32B1MCR_MR0_INT_ENABLED            ((unsigned int) 0x00000001)
#define TMR_TMR32B1MCR_MR0_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR32B1MCR_MR0_RESET_MASK             ((unsigned int) 0x00000002)   // Reset on MR0
#define TMR_TMR32B1MCR_MR0_RESET_ENABLED          ((unsigned int) 0x00000002)
#define TMR_TMR32B1MCR_MR0_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR32B1MCR_MR0_STOP_MASK              ((unsigned int) 0x00000004)   // Stop on MR0
#define TMR_TMR32B1MCR_MR0_STOP_ENABLED           ((unsigned int) 0x00000004)
#define TMR_TMR32B1MCR_MR0_STOP_DISABLED          ((unsigned int) 0x00000000)
#define TMR_TMR32B1MCR_MR1_INT_MASK               ((unsigned int) 0x00000008)   // Interrupt on MR1
#define TMR_TMR32B1MCR_MR1_INT_ENABLED            ((unsigned int) 0x00000008)
#define TMR_TMR32B1MCR_MR1_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR32B1MCR_MR1_RESET_MASK             ((unsigned int) 0x00000010)   // Reset on MR1
#define TMR_TMR32B1MCR_MR1_RESET_ENABLED          ((unsigned int) 0x00000010)
#define TMR_TMR32B1MCR_MR1_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR32B1MCR_MR1_STOP_MASK              ((unsigned int) 0x00000020)   // Stop on MR1
#define TMR_TMR32B1MCR_MR1_STOP_ENABLED           ((unsigned int) 0x00000020)
#define TMR_TMR32B1MCR_MR1_STOP_DISABLED          ((unsigned int) 0x00000000)
#define TMR_TMR32B1MCR_MR2_INT_MASK               ((unsigned int) 0x00000040)   // Interrupt on MR2
#define TMR_TMR32B1MCR_MR2_INT_ENABLED            ((unsigned int) 0x00000040)
#define TMR_TMR32B1MCR_MR2_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR32B1MCR_MR2_RESET_MASK             ((unsigned int) 0x00000080)   // Reset on MR2
#define TMR_TMR32B1MCR_MR2_RESET_ENABLED          ((unsigned int) 0x00000080)
#define TMR_TMR32B1MCR_MR2_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR32B1MCR_MR2_STOP_MASK              ((unsigned int) 0x00000100)   // Stop on MR2
#define TMR_TMR32B1MCR_MR2_STOP_ENABLED           ((unsigned int) 0x00000100)
#define TMR_TMR32B1MCR_MR2_STOP_DISABLED          ((unsigned int) 0x00000000)
#define TMR_TMR32B1MCR_MR3_INT_MASK               ((unsigned int) 0x00000200)   // Interrupt on MR3
#define TMR_TMR32B1MCR_MR3_INT_ENABLED            ((unsigned int) 0x00000200)
#define TMR_TMR32B1MCR_MR3_INT_DISABLED           ((unsigned int) 0x00000000)
#define TMR_TMR32B1MCR_MR3_RESET_MASK             ((unsigned int) 0x00000400)   // Reset on MR3
#define TMR_TMR32B1MCR_MR3_RESET_ENABLED          ((unsigned int) 0x00000400)
#define TMR_TMR32B1MCR_MR3_RESET_DISABLED         ((unsigned int) 0x00000000)
#define TMR_TMR32B1MCR_MR3_STOP_MASK              ((unsigned int) 0x00000800)   // Stop on MR3
#define TMR_TMR32B1MCR_MR3_STOP_ENABLED           ((unsigned int) 0x00000800)
#define TMR_TMR32B1MCR_MR3_STOP_DISABLED          ((unsigned int) 0x00000000)

#define TMR_TMR32B1CCR_CAP0RE_MASK                ((unsigned int) 0x00000001)   // Capture on rising edge
#define TMR_TMR32B1CCR_CAP0RE_ENABLED             ((unsigned int) 0x00000001)
#define TMR_TMR32B1CCR_CAP0RE_DISABLED            ((unsigned int) 0x00000000)
#define TMR_TMR32B1CCR_CAP0FE_MASK                ((unsigned int) 0x00000002)   // Capture on falling edge
#define TMR_TMR32B1CCR_CAP0FE_ENABLED             ((unsigned int) 0x00000002)
#define TMR_TMR32B1CCR_CAP0FE_DISABLED            ((unsigned int) 0x00000000)
#define TMR_TMR32B1CCR_CAP0I_MASK                 ((unsigned int) 0x00000004)   // Interrupt on CAP0 event
#define TMR_TMR32B1CCR_CAP0I_ENABLED              ((unsigned int) 0x00000004)
#define TMR_TMR32B1CCR_CAP0I_DISABLED             ((unsigned int) 0x00000000)

#define TMR_TMR32B1EMR_EM0_MASK                   ((unsigned int) 0x00000001)   // External match 0
#define TMR_TMR32B1EMR_EM0                        ((unsigned int) 0x00000001)
#define TMR_TMR32B1EMR_EMC0_MASK                  ((unsigned int) 0x00000030)
#define TMR_TMR32B1EMR_EMC0_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR32B1EMR_EMC0_LOW                   ((unsigned int) 0x00000010)
#define TMR_TMR32B1EMR_EMC0_HIGH                  ((unsigned int) 0x00000020)
#define TMR_TMR32B1EMR_EMC0_TOGGLE                ((unsigned int) 0x00000030)
#define TMR_TMR32B1EMR_EM1_MASK                   ((unsigned int) 0x00000002)   // External match 1
#define TMR_TMR32B1EMR_EM1                        ((unsigned int) 0x00000002)
#define TMR_TMR32B1EMR_EMC1_MASK                  ((unsigned int) 0x000000C0)
#define TMR_TMR32B1EMR_EMC1_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR32B1EMR_EMC1_LOW                   ((unsigned int) 0x00000040)
#define TMR_TMR32B1EMR_EMC1_HIGH                  ((unsigned int) 0x00000080)
#define TMR_TMR32B1EMR_EMC1_TOGGLE                ((unsigned int) 0x000000C0)
#define TMR_TMR32B1EMR_EM2_MASK                   ((unsigned int) 0x00000004)   // External match 2
#define TMR_TMR32B1EMR_EM2                        ((unsigned int) 0x00000004)
#define TMR_TMR32B1EMR_EMC2_MASK                  ((unsigned int) 0x00000300)
#define TMR_TMR32B1EMR_EMC2_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR32B1EMR_EMC2_LOW                   ((unsigned int) 0x00000100)
#define TMR_TMR32B1EMR_EMC2_HIGH                  ((unsigned int) 0x00000200)
#define TMR_TMR32B1EMR_EMC2_TOGGLE                ((unsigned int) 0x00000300)
#define TMR_TMR32B1EMR_EM3_MASK                   ((unsigned int) 0x00000008)   // External match 3
#define TMR_TMR32B1EMR_EM3                        ((unsigned int) 0x00000008)
#define TMR_TMR32B1EMR_EMC3_MASK                  ((unsigned int) 0x00000C00)
#define TMR_TMR32B1EMR_EMC3_DONOTHING             ((unsigned int) 0x00000000)
#define TMR_TMR32B1EMR_EMC3_LOW                   ((unsigned int) 0x00000400)
#define TMR_TMR32B1EMR_EMC3_HIGH                  ((unsigned int) 0x00000800)
#define TMR_TMR32B1EMR_EMC3_TOGGLE                ((unsigned int) 0x00000C00)

#define TMR_TMR32B1CTCR_CTMODE_MASK               ((unsigned int) 0x00000003)   // Counter/Timer mode
#define TMR_TMR32B1CTCR_CTMODE_TIMER              ((unsigned int) 0x00000000)   // Timer Mode: Every rising PCLK edge
#define TMR_TMR32B1CTCR_CTMODE_COUNTERRISING      ((unsigned int) 0x00000001)   // Counter: TC increments on rising edge of input
#define TMR_TMR32B1CTCR_CTMODE_COUNTERFALLING     ((unsigned int) 0x00000002)   // Counter: TC increments on falling edge of input
#define TMR_TMR32B1CTCR_CTMODE_COUNTERBOTH        ((unsigned int) 0x00000003)   // Counter: TC increments on both edges of input
#define TMR_TMR32B1CTCR_CINPUTSELECT_MASK         ((unsigned int) 0x0000000C)   
#define TMR_TMR32B1CTCR_CINPUTSELECT              ((unsigned int) 0x00000000)   // CINPUTSELECT must be set to 00

#define TMR_TMR32B1PWMC_PWM0_MASK                 ((unsigned int) 0x00000001)   
#define TMR_TMR32B1PWMC_PWM0_ENABLED              ((unsigned int) 0x00000001)   // PWM mode is enabled for CT32Bn_MAT0
#define TMR_TMR32B1PWMC_PWM0_DISABLED             ((unsigned int) 0x00000000)
#define TMR_TMR32B1PWMC_PWM1_MASK                 ((unsigned int) 0x00000002)   
#define TMR_TMR32B1PWMC_PWM1_ENABLED              ((unsigned int) 0x00000002)   // PWM mode is enabled for CT32Bn_MAT1
#define TMR_TMR32B1PWMC_PWM1_DISABLED             ((unsigned int) 0x00000000)
#define TMR_TMR32B1PWMC_PWM2_MASK                 ((unsigned int) 0x00000004)   
#define TMR_TMR32B1PWMC_PWM2_ENABLED              ((unsigned int) 0x00000004)   // PWM mode is enabled for CT32Bn_MAT2
#define TMR_TMR32B1PWMC_PWM2_DISABLED             ((unsigned int) 0x00000000)
#define TMR_TMR32B1PWMC_PWM3_MASK                 ((unsigned int) 0x00000008)   
#define TMR_TMR32B1PWMC_PWM3_ENABLED              ((unsigned int) 0x00000008)   // PWM mode is enabled for CT32Bn_MAT3
#define TMR_TMR32B1PWMC_PWM3_DISABLED             ((unsigned int) 0x00000000)

/*##############################################################################
## System Tick Timer
##############################################################################*/

#define SYSTICK_BASE_ADDRESS                      (0xE000E000)

#define SYSTICK_STCTRL                            (*(pREG32 (0xE000E010)))    // System tick control
#define SYSTICK_STRELOAD                          (*(pREG32 (0xE000E014)))    // System timer reload
#define SYSTICK_STCURR                            (*(pREG32 (0xE000E018)))    // System timer current
#define SYSTICK_STCALIB                           (*(pREG32 (0xE000E01C)))    // System timer calibration

/*  STCTRL (System Timer Control and status register)
    The STCTRL register contains control information for the System Tick Timer, and provides
    a status flag.  */

#define SYSTICK_STCTRL_ENABLE                     (0x00000001)    // System tick counter enable
#define SYSTICK_STCTRL_TICKINT                    (0x00000002)    // System tick interrupt enable
#define SYSTICK_STCTRL_CLKSOURCE                  (0x00000004)    // NOTE: This isn't documented but is based on NXP examples
#define SYSTICK_STCTRL_COUNTFLAG                  (0x00010000)    // System tick counter flag

/*  STRELOAD (System Timer Reload value register)
    The STRELOAD register is set to the value that will be loaded into the System Tick Timer
    whenever it counts down to zero. This register is loaded by software as part of timer
    initialization. The STCALIB register may be read and used as the value for STRELOAD if
    the CPU or external clock is running at the frequency intended for use with the STCALIB
    value.  */

#define SYSTICK_STRELOAD_MASK                     (0x00FFFFFF)

/*  STCURR (System Timer Current value register)
    The STCURR register returns the current count from the System Tick counter when it is
    read by software. */

#define SYSTICK_STCURR_MASK                       (0x00FFFFFF)

/*  STCALIB (System Timer Calibration value register) */

#define SYSTICK_STCALIB_TENMS_MASK                (0x00FFFFFF)
#define SYSTICK_STCALIB_SKEW_MASK                 (0x40000000)
#define SYSTICK_STCALIB_NOREF_MASK                (0x80000000)

/*##############################################################################
## ADC
##############################################################################*/

#define ADC_AD0_BASE_ADDRESS                      (0x4001C000)

#define ADC_AD0CR                                 (*(pREG32 (0x4001C000)))      // ADC Control Register
#define ADC_AD0GDR                                ((unsigned int) 0x4001C004)   // ADC Global Data Register
#define ADC_AD0INTEN                              ((unsigned int) 0x4001C00C)   // ADC Interrupt Enable Register
#define ADC_AD0DR0                                ((unsigned int) 0x4001C010)   // ADC Data Register 0
#define ADC_AD0DR1                                ((unsigned int) 0x4001C014)   // ADC Data Register 1
#define ADC_AD0DR2                                ((unsigned int) 0x4001C018)   // ADC Data Register 2
#define ADC_AD0DR3                                ((unsigned int) 0x4001C01C)   // ADC Data Register 3
#define ADC_AD0DR4                                ((unsigned int) 0x4001C020)   // ADC Data Register 4
#define ADC_AD0DR5                                ((unsigned int) 0x4001C024)   // ADC Data Register 5
#define ADC_AD0DR6                                ((unsigned int) 0x4001C028)   // ADC Data Register 6
#define ADC_AD0DR7                                ((unsigned int) 0x4001C02C)   // ADC Data Register 7
#define ADC_AD0STAT                               ((unsigned int) 0x4001C030)   // ADC Status Register

#define ADC_AD0CR_SEL_MASK                        (0x000000FF)
#define ADC_AD0CR_SEL_AD0                         (0x00000001)
#define ADC_AD0CR_SEL_AD1                         (0x00000002)
#define ADC_AD0CR_SEL_AD2                         (0x00000004)
#define ADC_AD0CR_SEL_AD3                         (0x00000008)
#define ADC_AD0CR_SEL_AD4                         (0x00000010)
#define ADC_AD0CR_SEL_AD5                         (0x00000020)
#define ADC_AD0CR_SEL_AD6                         (0x00000040)
#define ADC_AD0CR_SEL_AD7                         (0x00000080)
#define ADC_AD0CR_CLKDIV_MASK                     (0x0000FF00)
#define ADC_AD0CR_BURST_MASK                      (0x00010000)
#define ADC_AD0CR_BURST_SWMODE                    (0x00000000)
#define ADC_AD0CR_BURST_HWSCANMODE                (0x00010000)
#define ADC_AD0CR_CLKS_MASK                       (0x000E0000)
#define ADC_AD0CR_CLKS_10BITS                     (0x00000000)
#define ADC_AD0CR_CLKS_9BITS                      (0x00020000)
#define ADC_AD0CR_CLKS_8BITS                      (0x00040000)
#define ADC_AD0CR_CLKS_7BITS                      (0x00060000)
#define ADC_AD0CR_CLKS_6BITS                      (0x00080000)
#define ADC_AD0CR_CLKS_5BITS                      (0x000A0000)
#define ADC_AD0CR_CLKS_4BITS                      (0x000C0000)
#define ADC_AD0CR_CLKS_3BITS                      (0x000E0000)
#define ADC_AD0CR_START_MASK                      (0x07000000)
#define ADC_AD0CR_START_NOSTART                   (0x00000000)
#define ADC_AD0CR_START_STARTNOW                  (0x01000000)
#define ADC_AD0CR_EDGE_MASK                       (0x08000000)
#define ADC_AD0CR_EDGE_FALLING                    (0x08000000)
#define ADC_AD0CR_EDGE_RISING                     (0x00000000)

/*  AD9GDR (A/D Global Data Register)
    The A/D Global Data Register contains the result of the most recent A/D conversion. This
    includes the data, DONE, and Overrun flags, and the number of the A/D channel to which
    the data relates. */

#define ADC_AD0GDR_RESULT_MASK                    (0x0000FFC0)
#define ADC_AD0GDR_CHN_MASK                       (0x07000000)    // Channel from which the results were converted
#define ADC_AD0GDR_OVERUN_MASK                    (0x40000000)
#define ADC_AD0GDR_OVERUN                         (0x40000000)
#define ADC_AD0GDR_DONE_MASK                      (0x80000000)
#define ADC_AD0GDR_DONE                           (0x80000000)

/*  AD0STAT (A/D Status Register)
    The A/D Status register allows checking the status of all A/D channels simultaneously.
    The DONE and OVERRUN flags appearing in the ADDRn register for each A/D channel
    are mirrored in ADSTAT. The interrupt flag (the logical OR of all DONE flags) is also found
    in ADSTAT.  */

#define ADC_AD0STAT_DONE0_MASK                    (0x00000001)
#define ADC_AD0STAT_DONE0                         (0x00000001)
#define ADC_AD0STAT_DONE1_MASK                    (0x00000002)
#define ADC_AD0STAT_DONE1                         (0x00000002)
#define ADC_AD0STAT_DONE2_MASK                    (0x00000004)
#define ADC_AD0STAT_DONE2                         (0x00000004)
#define ADC_AD0STAT_DONE3_MASK                    (0x00000008)
#define ADC_AD0STAT_DONE3                         (0x00000008)
#define ADC_AD0STAT_DONE4_MASK                    (0x00000010)
#define ADC_AD0STAT_DONE4                         (0x00000010)
#define ADC_AD0STAT_DONE5_MASK                    (0x00000020)
#define ADC_AD0STAT_DONE5                         (0x00000020)
#define ADC_AD0STAT_DONE6_MASK                    (0x00000040)
#define ADC_AD0STAT_DONE6                         (0x00000040)
#define ADC_AD0STAT_DONE7_MASK                    (0x00000080)
#define ADC_AD0STAT_DONE7                         (0x00000080)
#define ADC_AD0STAT_OVERRUN0_MASK                 (0x00000100)
#define ADC_AD0STAT_OVERRUN0                      (0x00000100)
#define ADC_AD0STAT_OVERRUN1_MASK                 (0x00000200)
#define ADC_AD0STAT_OVERRUN1                      (0x00000200)
#define ADC_AD0STAT_OVERRUN2_MASK                 (0x00000400)
#define ADC_AD0STAT_OVERRUN2                      (0x00000400)
#define ADC_AD0STAT_OVERRUN3_MASK                 (0x00000800)
#define ADC_AD0STAT_OVERRUN3                      (0x00000800)
#define ADC_AD0STAT_OVERRUN4_MASK                 (0x00001000)
#define ADC_AD0STAT_OVERRUN4                      (0x00001000)
#define ADC_AD0STAT_OVERRUN5_MASK                 (0x00002000)
#define ADC_AD0STAT_OVERRUN5                      (0x00002000)
#define ADC_AD0STAT_OVERRUN6_MASK                 (0x00004000)
#define ADC_AD0STAT_OVERRUN6                      (0x00004000)
#define ADC_AD0STAT_OVERRUN7_MASK                 (0x00008000)
#define ADC_AD0STAT_OVERRUN7                      (0x00008000)
#define ADC_AD0STAT_ADINT_MASK                    (0x00010000)
#define ADC_AD0STAT_ADINT                         (0x00010000)

/*  ADINTEN0 (A/D Interrupt Enable Register)
    This register allows control over which A/D channels generate an interrupt when a
    conversion is complete. For example, it may be desirable to use some A/D channels to
    monitor sensors by continuously performing conversions on them. The most recent
    results are read by the application program whenever they are needed. In this case, an
    interrupt is not desirable at the end of each conversion for some A/D channels.  */

#define ADC_AD0INTEN_ADINTEN0_MASK                (0x00000001)
#define ADC_AD0INTEN_ADINTEN0                     (0x00000001)
#define ADC_AD0INTEN_ADINTEN1_MASK                (0x00000002)
#define ADC_AD0INTEN_ADINTEN1                     (0x00000002)
#define ADC_AD0INTEN_ADINTEN2_MASK                (0x00000004)
#define ADC_AD0INTEN_ADINTEN2                     (0x00000004)
#define ADC_AD0INTEN_ADINTEN3_MASK                (0x00000008)
#define ADC_AD0INTEN_ADINTEN3                     (0x00000008)
#define ADC_AD0INTEN_ADINTEN4_MASK                (0x00000010)
#define ADC_AD0INTEN_ADINTEN4                     (0x00000010)
#define ADC_AD0INTEN_ADINTEN5_MASK                (0x00000020)
#define ADC_AD0INTEN_ADINTEN5                     (0x00000020)
#define ADC_AD0INTEN_ADINTEN6_MASK                (0x00000040)
#define ADC_AD0INTEN_ADINTEN6                     (0x00000040)
#define ADC_AD0INTEN_ADINTEN7_MASK                (0x00000080)
#define ADC_AD0INTEN_ADINTEN7                     (0x00000080)
#define ADC_AD0INTEN_ADGINTEN_MASK                (0x00000100)
#define ADC_AD0INTEN_ADGINTEN_ENABLE              (0x00000100)
#define ADC_AD0INTEN_ADGINTEN_DISABLE             (0x00000000)

/*  AD0DR0..7 (A/D Data Registers)
    The A/D Data Register hold the result when an A/D conversion is complete, and also
    include the flags that indicate when a conversion has been completed and when a
    conversion overrun has occurred. */

#define ADC_DR_V_MASK                             (0x0000FFC0)
#define ADC_DR_OVERRUN_MASK                       (0x40000000)
#define ADC_DR_OVERRUN                            (0x40000000)
#define ADC_DR_DONE_MASK                          (0x80000000)
#define ADC_DR_DONE                               (0x80000000)

/*##############################################################################
## WDT - Watchdog Timer
##############################################################################*/

#define WDT_BASE_ADDRESS                          (0x40004000)

#define WDT_WDMOD                                 (*(pREG32 (0x40004000)))    // Watchdog mode register
#define WDT_WDTC                                  (*(pREG32 (0x40004004)))    // Watchdog timer constant register
#define WDT_WDFEED                                (*(pREG32 (0x40004008)))    // Watchdog feed sequence register
#define WDT_WDTV                                  (*(pREG32 (0x4000400C)))    // Watchdog timer value register

/*  WDMOD (Watchdog Mode register)
    The WDMOD register controls the operation of the Watchdog through the combination of
    WDEN and RESET bits. Note that a watchdog feed must be performed before any
    changes to the WDMOD register take effect.  */

#define WDT_WDMOD_WDEN_DISABLED                   (0x00000000)    // Watchdog enable bit
#define WDT_WDMOD_WDEN_ENABLED                    (0x00000001)
#define WDT_WDMOD_WDEN_MASK                       (0x00000001)
#define WDT_WDMOD_WDRESET_DISABLED                (0x00000000)    // Watchdog reset enable bit
#define WDT_WDMOD_WDRESET_ENABLED                 (0x00000002)
#define WDT_WDMOD_WDRESET_MASK                    (0x00000002)
#define WDT_WDMOD_WDTOF                           (0x00000004)    // Watchdog time-out interrupt flag
#define WDT_WDMOD_WDTOF_MASK                      (0x00000004)    // Set when the watchdog times out
#define WDT_WDMOD_WDINT                           (0x00000008)    // Watchdog timer interrupt flag
#define WDT_WDMOD_WDINT_MASK                      (0x00000008)

/*  WDFEED (Watchdog Feed register)
    Writing 0xAA followed by 0x55 to this register will reload the Watchdog timer with the
    WDTC value. This operation will also start the Watchdog if it is enabled via the WDMOD
    register. Setting the WDEN bit in the WDMOD register is not sufficient to enable the
    Watchdog. A valid feed sequence must be completed after setting WDEN before the
    Watchdog is capable of generating a reset. Until then, the Watchdog will ignore feed
    errors. After writing 0xAA to WDFEED, access to any Watchdog register other than writing
    0x55 to WDFEED causes an immediate reset/interrupt when the Watchdog is enabled.
    The reset will be generated during the second PCLK following an incorrect access to a
    Watchdog register during a feed sequence.
    Interrupts should be disabled during the feed sequence. An abort condition will occur if an
    interrupt happens during the feed sequence. */

#define WDT_WDFEED_FEED1                          (0x000000AA)
#define WDT_WDFEED_FEED2                          (0x00000055)

/*##############################################################################
## Misc. Inline Functions
##############################################################################*/

/**************************************************************************/
/*! 
    @brief  Reverses the bit order of a 32-bit value

    Allows single-cycle reversing of 32-bit values (ASM RBIT)

    @param[in]  value  
                The 32-bit value to reverse
    @returns    The reversed value
*/
/**************************************************************************/
static inline uint32_t RBIT(uint32_t value)     { uint32_t result=0; __asm volatile ("rbit %0, %1" : "=r" (result) : "r" (value) ); return(result); }

/**************************************************************************/
/*! 
    @brief  Causes a system reset and enters the USB Bootloader

    Resets the system using the AIRCR register, and waits in a loop until
    reset occurs.  The resistor/divider on the LPC1343 Reference Design
    Base Board [1] causes the AIRCR reset to enter the bootloader rather
    than executing the existing firmware.  If you wish to reset and execute
    the existing firmware, you need to use the watchdog timer to reset
    (see "wdt/wdt.c").

    [1] http://www.microbuilder.eu/Projects/LPC1343ReferenceDesign.aspx
*/
/**************************************************************************/
static inline void __resetBootloader()          { __disable_irq(); SCB_AIRCR = SCB_AIRCR_VECTKEY_VALUE | SCB_AIRCR_SYSRESETREQ; while(1); }

#endif
