/*
 * geigerct.c
 *
 *
 *  Created on: 11.08.2011
 *      Author: Turbo J <turboj@web.de>
 *
 *  Implements simple Geiger Counter
 *  Counts rising edges on P3_0 = BUSINT
 *  so you can directly connect the Geiger board
 *  from http://mightyohm.com/blog/products/geiger-counter/
 *
 */
#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "usetable.h"

// Liberated from ARM Cortex M3 CMSIS core_cm3.h
// The processor definition headers for R0ket are incomplete :-/

#define __I
#define __IO volatile

typedef struct {
	__I uint32_t CPUID; /*!< Offset: 0x000 (R/ )  CPU ID Base Register                                  */
	__IO uint32_t ICSR; /*!< Offset: 0x004 (R/W)  Interrupt Control State Register                      */
	__IO uint32_t VTOR; /*!< Offset: 0x008 (R/W)  Vector Table Offset Register                          */
	__IO uint32_t AIRCR; /*!< Offset: 0x00C (R/W)  Application Interrupt / Reset Control Register        */
	__IO uint32_t SCR; /*!< Offset: 0x010 (R/W)  System Control Register                               */
	__IO uint32_t CCR; /*!< Offset: 0x014 (R/W)  Configuration Control Register                        */
	__IO uint8_t SHP[12]; /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
	__IO uint32_t SHCSR; /*!< Offset: 0x024 (R/W)  System Handler Control and State Register             */
	__IO uint32_t CFSR; /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register                    */
	__IO uint32_t HFSR; /*!< Offset: 0x02C (R/W)  Hard Fault Status Register                            */
	__IO uint32_t DFSR; /*!< Offset: 0x030 (R/W)  Debug Fault Status Register                           */
	__IO uint32_t MMFAR; /*!< Offset: 0x034 (R/W)  Mem Manage Address Register                           */
	__IO uint32_t BFAR; /*!< Offset: 0x038 (R/W)  Bus Fault Address Register                            */
	__IO uint32_t AFSR; /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register                       */
	__I uint32_t PFR[2]; /*!< Offset: 0x040 (R/ )  Processor Feature Register                            */
	__I uint32_t DFR; /*!< Offset: 0x048 (R/ )  Debug Feature Register                                */
	__I uint32_t ADR; /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register                            */
	__I uint32_t MMFR[4]; /*!< Offset: 0x050 (R/ )  Memory Model Feature Register                         */
	__I uint32_t ISAR[5]; /*!< Offset: 0x060 (R/ )  ISA Feature Register                                  */
} SCB_Type;

#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address */
#define SCB_BASE            (SCS_BASE +  0x0D00UL)                    /*!< System Control Block Base Address */
#define SCB                 ((SCB_Type *)           SCB_BASE)         /*!< SCB configuration struct          */

uint32_t VectorTableInRAM[73] __attribute__ ((aligned(1024))); // VTOR needs 1024 Byte alignment, see UM10375.PDF

void (*orig_handler_extint3)(void);  // original EINT3 handler

uint32_t volatile IntCtr;

void ExtInt3_Handler();

// Remember: ram() must be the first function, place all other code AFTER
// because the Implementer seem not to know how to use section attributes

static uint8_t mainloop();

void ram(void) {
	uint8_t button;
	uint32_t LEDs;

	// populate my Vector table
	memcpy(VectorTableInRAM, 0, sizeof(VectorTableInRAM));
	orig_handler_extint3 = (void*) VectorTableInRAM[EINT3_IRQn + 16];
	VectorTableInRAM[EINT3_IRQn + 16] = (uint32_t) &ExtInt3_Handler;
	// HACK: use RAM vector table to implement own External IRQ handler
	SCB->VTOR = (uint32_t) &VectorTableInRAM[0];
	// TODO add DMB() here, as VTOR updates are NOT effective immediately
	//
	GPIO_GPIO3IEV |= 1;
	GPIO_GPIO3IE |= 1;
	GPIO_GPIO3DIR &= ~1;
	GPIO_GPIO3IS &= ~1;
	GPIO_GPIO0DATA &= ~1;
	IOCON_PIO3_0 = (1 << 3) | (1 << 5); // Pull DOWN not Up, Hyst on
	NVIC_EnableIRQ(EINT3_IRQn);
	IntCtr = 0;
	LEDs = 0;
	mainloop();

	GPIO_GPIO3IE &= ~1; // disable GPIO IRQ
	NVIC_DisableIRQ(EINT3_IRQn);
	// restore VTOR
	SCB->VTOR = 0;
	//TODO DMB(); Cortex Manual suggests DMB after setting VTOR
	// not really needed in this case
}

void ExtInt3_Handler() {
	if (GPIO_GPIO3RIS & 0x01) {
		GPIO_GPIO3IC |= (0x01); // ACK BUSINT


		//GPIO_GPIO0DATA|=(1<<11);
		IOCON_PIO1_11 = 0;
		GPIO_GPIO1DATA |= (1 << 7);
		GPIO_GPIO1DATA |= (1 << 11);
		IntCtr++;
	} else {
		orig_handler_extint3();
	}
}

static uint8_t mainloop() {
	uint32_t ioconbak = IOCON_PIO1_11;

	uint32_t volatile oldCount=IntCtr;
	uint32_t perMin=0; // counts in last 60 s
	uint32_t startTime=_timectr;
	uint8_t button;
		IOCON_PIO1_11 = 0;
		while (1) {
			//GPIO_GPIO0DATA&=~(1<<11);
			IOCON_PIO1_11 = ioconbak;
			GPIO_GPIO1DATA &= ~(1 << 7);
			GPIO_GPIO1DATA &= ~(1 << 11);
			lcdClear();

			lcdPrintln("   Geiger");
			lcdPrintln("   Counter");
			// ####
			for (int i=0; i< (14*( _timectr-startTime))/(60*100);i++) {
				lcdPrint("#");
			}

			lcdPrintln("");
			lcdPrintln("Counts:");
			lcdPrint(" ");
			lcdPrintInt(IntCtr);
			lcdPrintln(" total");
			lcdPrint(" ");
			lcdPrintInt(  perMin);
			lcdPrintln("/min");
			// remember: We have a 10ms Timer counter
			if ((startTime+60 *100 ) < _timectr) {
				// dumb algo: Just use last 60 seconds count
				perMin=IntCtr-oldCount;
				startTime=_timectr;
				oldCount=IntCtr;
			}
			lcdRefresh();
			delayms(42);
			button = getInputRaw();

			if (button != BTN_NONE) {
				delayms(23);// debounce and wait till user release button
				while (getInputRaw()!=BTN_NONE) delayms(23);
				break;
			}
		}
		IOCON_PIO1_11 = ioconbak;
		return button;

}

