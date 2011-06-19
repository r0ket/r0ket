//*****************************************************************************
//   +--+       
//   | ++----+   
//   +-++    |  
//     |     |  
//   +-+--+  |   
//   | +--+--+  
//   +----+    Copyright (c) 2009 Code Red Technologies Ltd. 
//
// Microcontroller Startup code for use with Red Suite
//
// Software License Agreement
// 
// The software is owned by Code Red Technologies and/or its suppliers, and is 
// protected under applicable copyright laws.  All rights are reserved.  Any 
// use in violation of the foregoing restrictions may subject the user to criminal 
// sanctions under applicable laws, as well as to civil liability for the breach 
// of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// USE OF THIS SOFTWARE FOR COMMERCIAL DEVELOPMENT AND/OR EDUCATION IS SUBJECT
// TO A CURRENT END USER LICENSE AGREEMENT (COMMERCIAL OR EDUCATIONAL) WITH
// CODE RED TECHNOLOGIES LTD. 
//
//*****************************************************************************
#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

// Code Red - if CMSIS is being used, then SystemInit() routine
// will be called by startup code rather than in application's main()
#ifdef __USE_CMSIS
#include "system_LPC13xx.h"
#endif

//*****************************************************************************
//
// Forward declaration of the default handlers. These are aliased.
// When the application defines a handler (with the same name), this will 
// automatically take precedence over these weak definitions
//
//*****************************************************************************
void Reset_Handler(void);
void ResetISR(void) ALIAS(Reset_Handler);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void MemManage_Handler(void);
WEAK void BusFault_Handler(void);
WEAK void UsageFault_Handler(void);
WEAK void SVCall_Handler(void);
WEAK void DebugMon_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);

//*****************************************************************************
//
// Forward declaration of the specific IRQ handlers. These are aliased
// to the IntDefaultHandler, which is a 'forever' loop. When the application
// defines a handler (with the same name), this will automatically take 
// precedence over these weak definitions
//
//*****************************************************************************

void I2C_IRQHandler(void) ALIAS(IntDefaultHandler);
void TIMER16_0_IRQHandler(void) ALIAS(IntDefaultHandler);
void TIMER16_1_IRQHandler(void) ALIAS(IntDefaultHandler);
void TIMER32_0_IRQHandler(void) ALIAS(IntDefaultHandler);
void TIMER32_1_IRQHandler(void) ALIAS(IntDefaultHandler);
void SSP_IRQHandler(void) ALIAS(IntDefaultHandler);
void UART_IRQHandler(void) ALIAS(IntDefaultHandler);
void USB_IRQHandler(void) ALIAS(IntDefaultHandler);
void USB_FIQHandler(void) ALIAS(IntDefaultHandler);
void ADC_IRQHandler(void) ALIAS(IntDefaultHandler);
void WDT_IRQHandler(void) ALIAS(IntDefaultHandler);
void BOD_IRQHandler(void) ALIAS(IntDefaultHandler);
void FMC_IRQHandler(void) ALIAS(IntDefaultHandler);
void PIOINT3_IRQHandler(void) ALIAS(IntDefaultHandler);
void PIOINT2_IRQHandler(void) ALIAS(IntDefaultHandler);
void PIOINT1_IRQHandler(void) ALIAS(IntDefaultHandler);
void PIOINT0_IRQHandler(void) ALIAS(IntDefaultHandler);

void WAKEUP_IRQHandlerPIO0_0 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO0_1 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO0_2 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO0_3 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO0_4 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO0_5 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO0_6 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO0_7 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO0_8 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO0_9 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO0_10(void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO0_11(void) ALIAS(IntDefaultHandler);

void WAKEUP_IRQHandlerPIO1_0 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO1_1 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO1_2 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO1_3 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO1_4 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO1_5 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO1_6 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO1_7 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO1_8 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO1_9 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO1_10(void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO1_11(void) ALIAS(IntDefaultHandler);

void WAKEUP_IRQHandlerPIO2_0 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO2_1 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO2_2 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO2_3 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO2_4 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO2_5 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO2_6 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO2_7 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO2_8 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO2_9 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO2_10(void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO2_11(void) ALIAS(IntDefaultHandler);

void WAKEUP_IRQHandlerPIO3_0 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO3_1 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO3_2 (void) ALIAS(IntDefaultHandler);
void WAKEUP_IRQHandlerPIO3_3 (void) ALIAS(IntDefaultHandler);

//*****************************************************************************
//
// The entry point for the application.
// __main() is the entry point for redlib based applications
// main() is the entry point for newlib based applications
//
//*****************************************************************************
extern WEAK void __main(void);
extern WEAK void main(void);
//*****************************************************************************
//
// External declaration for the pointer to the stack top from the Linker Script
//
//*****************************************************************************
extern void __stack_end__(void);

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector")))
void (*const g_pfnVectors[]) (void) =
{
	// Core Level - CM3
	__stack_end__,		// The initial stack pointer
	Reset_Handler,		// The reset handler
	NMI_Handler,		// The NMI handler
	HardFault_Handler,	// The hard fault handler
	MemManage_Handler,	// The MPU fault handler
	BusFault_Handler,	// The bus fault handler
	UsageFault_Handler,	// The usage fault handler
	0,			// Reserved
	0,			// Reserved
	0,			// Reserved
	0,			// Reserved
	SVCall_Handler,		// SVCall handler
	DebugMon_Handler,	// Debug monitor handler
	0,			// Reserved
	PendSV_Handler,		// The PendSV handler
	SysTick_Handler,	// The SysTick handler
	// Wakeup sources (40 ea.) for the I/O pins:
	//   PIO0 (0:11)
	//   PIO1 (0:11)
	//   PIO2 (0:11)
	//   PIO3 (0:3)
	WAKEUP_IRQHandlerPIO0_0,	// PIO0_0  Wakeup
	WAKEUP_IRQHandlerPIO0_1,	// PIO0_1  Wakeup
	WAKEUP_IRQHandlerPIO0_2,	// PIO0_2  Wakeup
	WAKEUP_IRQHandlerPIO0_3,	// PIO0_3  Wakeup
	WAKEUP_IRQHandlerPIO0_4,	// PIO0_4  Wakeup
	WAKEUP_IRQHandlerPIO0_5,	// PIO0_5  Wakeup
	WAKEUP_IRQHandlerPIO0_6,	// PIO0_6  Wakeup
	WAKEUP_IRQHandlerPIO0_7,	// PIO0_7  Wakeup
	WAKEUP_IRQHandlerPIO0_8,	// PIO0_8  Wakeup
	WAKEUP_IRQHandlerPIO0_9,	// PIO0_9  Wakeup
	WAKEUP_IRQHandlerPIO0_10,	// PIO0_10 Wakeup
	WAKEUP_IRQHandlerPIO0_11,	// PIO0_11 Wakeup
	WAKEUP_IRQHandlerPIO1_0,	// PIO1_0  Wakeup
	WAKEUP_IRQHandlerPIO1_1,	// PIO1_1  Wakeup
	WAKEUP_IRQHandlerPIO1_2,	// PIO1_2  Wakeup
	WAKEUP_IRQHandlerPIO1_3,	// PIO1_3  Wakeup
	WAKEUP_IRQHandlerPIO1_4,	// PIO1_4  Wakeup
	WAKEUP_IRQHandlerPIO1_5,	// PIO1_5  Wakeup
	WAKEUP_IRQHandlerPIO1_6,	// PIO1_6  Wakeup
	WAKEUP_IRQHandlerPIO1_7,	// PIO1_7  Wakeup
	WAKEUP_IRQHandlerPIO1_8,	// PIO1_8  Wakeup
	WAKEUP_IRQHandlerPIO1_9,	// PIO1_9  Wakeup
	WAKEUP_IRQHandlerPIO1_10,	// PIO1_10 Wakeup
	WAKEUP_IRQHandlerPIO1_11,	// PIO1_11 Wakeup
	WAKEUP_IRQHandlerPIO2_0,	// PIO2_0  Wakeup
	WAKEUP_IRQHandlerPIO2_1,	// PIO2_1  Wakeup
	WAKEUP_IRQHandlerPIO2_2,	// PIO2_2  Wakeup
	WAKEUP_IRQHandlerPIO2_3,	// PIO2_3  Wakeup
	WAKEUP_IRQHandlerPIO2_4,	// PIO2_4  Wakeup
	WAKEUP_IRQHandlerPIO2_5,	// PIO2_5  Wakeup
	WAKEUP_IRQHandlerPIO2_6,	// PIO2_6  Wakeup
	WAKEUP_IRQHandlerPIO2_7,	// PIO2_7  Wakeup
	WAKEUP_IRQHandlerPIO2_8,	// PIO2_8  Wakeup
	WAKEUP_IRQHandlerPIO2_9,	// PIO2_9  Wakeup
	WAKEUP_IRQHandlerPIO2_10,	// PIO2_10 Wakeup
	WAKEUP_IRQHandlerPIO2_11,	// PIO2_11 Wakeup
	WAKEUP_IRQHandlerPIO3_0,	// PIO3_0  Wakeup
	WAKEUP_IRQHandlerPIO3_1,	// PIO3_1  Wakeup
	WAKEUP_IRQHandlerPIO3_2,	// PIO3_2  Wakeup
	WAKEUP_IRQHandlerPIO3_3,	// PIO3_3  Wakeup
	I2C_IRQHandler,		// I2C0
	TIMER16_0_IRQHandler,	// CT16B0 (16-bit Timer 0)
	TIMER16_1_IRQHandler,	// CT16B1 (16-bit Timer 1)
	TIMER32_0_IRQHandler,	// CT32B0 (32-bit Timer 0)
	TIMER32_1_IRQHandler,	// CT32B1 (32-bit Timer 1)
	SSP_IRQHandler,		// SSP0
	UART_IRQHandler,	// UART0
	USB_IRQHandler,		// USB IRQ
	USB_FIQHandler,		// USB FIQ
	ADC_IRQHandler,		// ADC   (A/D Converter)
	WDT_IRQHandler,		// WDT   (Watchdog Timer)
	BOD_IRQHandler,		// BOD   (Brownout Detect)
	FMC_IRQHandler,		// Flash (IP2111 Flash Memory Controller)
	PIOINT3_IRQHandler,	// PIO INT3
	PIOINT2_IRQHandler,	// PIO INT2
	PIOINT1_IRQHandler,	// PIO INT1
	PIOINT0_IRQHandler,	// PIO INT0
};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern unsigned long __end_of_text__;
extern unsigned long __data_beg__;
extern unsigned long __data_end__;
extern unsigned long __bss_beg__;
extern unsigned long __bss_end__;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void
//ResetISR(void)
Reset_Handler(void)
{
    unsigned long *pulSrc, *pulDest;

    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &__end_of_text__;
    for (pulDest = &__data_beg__; pulDest < &__data_end__;) {
	*pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment.  This is done with inline assembly since this
    // will clear the value of pulDest if it is not kept in a register.
    //
    __asm("    ldr     r0, =__bss_beg__\n" "    ldr     r1, =__bss_end__\n"
	  "    mov     r2, #0\n" "    .thumb_func\n" "zero_loop:\n"
	  "        cmp     r0, r1\n" "        it      lt\n"
	  "        strlt   r2, [r0], #4\n" "        blt     zero_loop");

#ifdef __USE_CMSIS
    SystemInit();
#endif

    main();

    //
    // main() shouldn't return, but if it does, we'll just enter an infinite loop 
    //
    while (1) {
	;
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
void NMI_Handler(void)
{
    while (1) {
    }
}

void HardFault_Handler(void)
{
    while (1) {
    }
}

void MemManage_Handler(void)
{
    while (1) {
    }
}

void BusFault_Handler(void)
{
    while (1) {
    }
}

void UsageFault_Handler(void)
{
    while (1) {
    }
}

void SVCall_Handler(void)
{
    while (1) {
    }
}

void DebugMon_Handler(void)
{
    while (1) {
    }
}

void PendSV_Handler(void)
{
    while (1) {
    }
}

void SysTick_Handler(void)
{
    while (1) {
    }
}

//*****************************************************************************
//
// Processor ends up here if an unexpected interrupt occurs or a handler
// is not present in the application code.
//
//*****************************************************************************

static void IntDefaultHandler(void)
{
    //
    // Go into an infinite loop.
    //
    while (1) {
    }
}
