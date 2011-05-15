/*
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2010, Roel Verdult
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the
 * names of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

// The GCC compiler defines the current architecture derived from the -mcpu argument.
// When target cpu is the cortex-m3, it automatically defines __ARM_ARCH_7M__
#ifndef __ARM_ARCH_7M__
  #error "The target ARM cpu must be Cortex-M3 compatible (-mcpu=cortex-m3)"
#endif

// Declare a weak alias macro as described in the GCC manual[1][2]
#define WEAK_ALIAS(f) __attribute__ ((weak, alias (#f)));
#define SECTION(s) __attribute__ ((section(s)))

/******************************************************************************
 * Forward undefined IRQ handlers to an infinite loop function. The Handlers
 * are weakly aliased which means that (re)definitions will overide these.
 *****************************************************************************/

void irq_undefined() {
  // Do nothing when occured interrupt is not defined, just keep looping
  while(1);
}

void I2C_IRQHandler(void)       WEAK_ALIAS(irq_undefined);
void TIMER16_0_IRQHandler(void) WEAK_ALIAS(irq_undefined);
void TIMER16_1_IRQHandler(void) WEAK_ALIAS(irq_undefined);
void TIMER32_0_IRQHandler(void) WEAK_ALIAS(irq_undefined);
void TIMER32_1_IRQHandler(void) WEAK_ALIAS(irq_undefined);
void SSP_IRQHandler(void)       WEAK_ALIAS(irq_undefined);
void UART_IRQHandler(void)      WEAK_ALIAS(irq_undefined);
void USB_IRQHandler(void)       WEAK_ALIAS(irq_undefined);
void USB_FIQHandler(void)       WEAK_ALIAS(irq_undefined);
void ADC_IRQHandler(void)       WEAK_ALIAS(irq_undefined);
void WDT_IRQHandler(void)       WEAK_ALIAS(irq_undefined);
void BOD_IRQHandler(void)       WEAK_ALIAS(irq_undefined);
void FMC_IRQHandler(void)       WEAK_ALIAS(irq_undefined);
void PIOINT3_IRQHandler(void)   WEAK_ALIAS(irq_undefined);
void PIOINT2_IRQHandler(void)   WEAK_ALIAS(irq_undefined);
void PIOINT1_IRQHandler(void)   WEAK_ALIAS(irq_undefined);
void PIOINT0_IRQHandler(void)   WEAK_ALIAS(irq_undefined);
void WAKEUP_IRQHandler(void)    WEAK_ALIAS(irq_undefined);

/*****************************************************************************
 * Forward undefined fault handlers to an infinite loop function. The Handlers
 * are weakly aliased which means that (re)definitions will overide these.
 ****************************************************************************/

void fault_undefined() {
  // Do nothing when occured interrupt is not defined, just keep looping
  while(1);
}

void NMI_Handler(void)          WEAK_ALIAS(fault_undefined);
void HardFault_Handler(void)    WEAK_ALIAS(fault_undefined);
void MemManage_Handler(void)    WEAK_ALIAS(fault_undefined);
void BusFault_Handler(void)     WEAK_ALIAS(fault_undefined);
void UsageFault_Handler(void)   WEAK_ALIAS(fault_undefined);
void SVCall_Handler(void)       WEAK_ALIAS(fault_undefined);
void DebugMon_Handler(void)     WEAK_ALIAS(fault_undefined);
void PendSV_Handler(void)       WEAK_ALIAS(fault_undefined);
void SysTick_Handler(void)      WEAK_ALIAS(fault_undefined);

/******************************************************************************
 * Forward undefined IRQ handlers to an infinite loop function. The Handlers
 * are weakly aliased which means that (re)definitions will overide these.
 *****************************************************************************/

// Prototype the entry values, which are handled by the linker script
extern void* stack_entry;
extern void boot_entry(void);

// Defined irq vectors using simple c code following the description in a white 
// paper from ARM[3] and code example from Simonsson Fun Technologies[4].
// These vectors are placed at the memory location defined in the linker script
const void *vectors[] SECTION(".irq_vectors") =
{
  // Stack and program reset entry point
  &stack_entry,          // The initial stack pointer
  boot_entry,            // The reset handler

  // Various fault handlers
  NMI_Handler,           // The NMI handler
  HardFault_Handler,     // The hard fault handler
  MemManage_Handler,     // The MPU fault handler
  BusFault_Handler,      // The bus fault handler
  UsageFault_Handler,    // The usage fault handler
  0,                     // Reserved
  0,                     // Reserved
  0,                     // Reserved
  0,                     // Reserved
  SVCall_Handler,        // SVCall handler
  DebugMon_Handler,      // Debug monitor handler
  0,                     // Reserved
  PendSV_Handler,        // The PendSV handler
  SysTick_Handler,       // The SysTick handler
  
  // Wakeup I/O pins handlers
  WAKEUP_IRQHandler,     // PIO0_0  Wakeup
  WAKEUP_IRQHandler,     // PIO0_1  Wakeup
  WAKEUP_IRQHandler,     // PIO0_2  Wakeup
  WAKEUP_IRQHandler,     // PIO0_3  Wakeup
  WAKEUP_IRQHandler,     // PIO0_4  Wakeup
  WAKEUP_IRQHandler,     // PIO0_5  Wakeup
  WAKEUP_IRQHandler,     // PIO0_6  Wakeup
  WAKEUP_IRQHandler,     // PIO0_7  Wakeup
  WAKEUP_IRQHandler,     // PIO0_8  Wakeup
  WAKEUP_IRQHandler,     // PIO0_9  Wakeup
  WAKEUP_IRQHandler,     // PIO0_10 Wakeup
  WAKEUP_IRQHandler,     // PIO0_11 Wakeup
  WAKEUP_IRQHandler,     // PIO1_0  Wakeup
  WAKEUP_IRQHandler,     // PIO1_1  Wakeup
  WAKEUP_IRQHandler,     // PIO1_2  Wakeup
  WAKEUP_IRQHandler,     // PIO1_3  Wakeup
  WAKEUP_IRQHandler,     // PIO1_4  Wakeup
  WAKEUP_IRQHandler,     // PIO1_5  Wakeup
  WAKEUP_IRQHandler,     // PIO1_6  Wakeup
  WAKEUP_IRQHandler,     // PIO1_7  Wakeup
  WAKEUP_IRQHandler,     // PIO1_8  Wakeup
  WAKEUP_IRQHandler,     // PIO1_9  Wakeup
  WAKEUP_IRQHandler,     // PIO1_10 Wakeup
  WAKEUP_IRQHandler,     // PIO1_11 Wakeup
  WAKEUP_IRQHandler,     // PIO2_0  Wakeup
  WAKEUP_IRQHandler,     // PIO2_1  Wakeup
  WAKEUP_IRQHandler,     // PIO2_2  Wakeup
  WAKEUP_IRQHandler,     // PIO2_3  Wakeup
  WAKEUP_IRQHandler,     // PIO2_4  Wakeup
  WAKEUP_IRQHandler,     // PIO2_5  Wakeup
  WAKEUP_IRQHandler,     // PIO2_6  Wakeup
  WAKEUP_IRQHandler,     // PIO2_7  Wakeup
  WAKEUP_IRQHandler,     // PIO2_8  Wakeup
  WAKEUP_IRQHandler,     // PIO2_9  Wakeup
  WAKEUP_IRQHandler,     // PIO2_10 Wakeup
  WAKEUP_IRQHandler,     // PIO2_11 Wakeup
  WAKEUP_IRQHandler,     // PIO3_0  Wakeup
  WAKEUP_IRQHandler,     // PIO3_1  Wakeup
  WAKEUP_IRQHandler,     // PIO3_2  Wakeup
  WAKEUP_IRQHandler,     // PIO3_3  Wakeup  

  // Specific peripheral irq handlers
  I2C_IRQHandler,        // I2C0
  TIMER16_0_IRQHandler,  // CT16B0 (16-bit Timer 0)
  TIMER16_1_IRQHandler,  // CT16B1 (16-bit Timer 1)
  TIMER32_0_IRQHandler,  // CT32B0 (32-bit Timer 0)
  TIMER32_1_IRQHandler,  // CT32B1 (32-bit Timer 1)
  SSP_IRQHandler,        // SSP0
  UART_IRQHandler,       // UART0
  USB_IRQHandler,        // USB IRQ
  USB_FIQHandler,        // USB FIQ
  ADC_IRQHandler,        // ADC (A/D Converter)
  WDT_IRQHandler,        // WDT (Watchdog Timer)
  BOD_IRQHandler,        // BOD (Brownout Detect)
  FMC_IRQHandler,        // Flash (IP2111 Flash Memory Controller)
  PIOINT3_IRQHandler,    // PIO INT3
  PIOINT2_IRQHandler,    // PIO INT2
  PIOINT1_IRQHandler,    // PIO INT1
  PIOINT0_IRQHandler,    // PIO INT0
};

/******************************************************************************
 * References
 *  [1] http://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
 *  [2] http://gcc.gnu.org/onlinedocs/gcc/Variable-Attributes.html
 *  [3] http://www.arm.com/files/pdf/Cortex-M3_programming_for_ARM7_developers.pdf
 *  [4] http://fun-tech.se/stm32/OlimexBlinky/mini.php
 *****************************************************************************/

