/***************************************************************
 *
 * OpenBeacon.org - LPC13xx Power Management Functions
 *
 * Copyright 2011 Milosch Meriac <meriac@openbeacon.de>
 *
 ***************************************************************

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along
 with this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 */
#include <openbeacon.h>
#include "pmu.h"
#include "spi.h"
#include "nRF_API.h"
#include "nRF_CMD.h"
#include "openbeacon-proto.h"

static uint32_t g_sysahbclkctrl;

#define MAINCLKSEL_IRC 0
#define MAINCLKSEL_SYSPLL_IN 1
#define MAINCLKSEL_WDT 2
#define MAINCLKSEL_SYSPLL_OUT 3

#define SYSTEM_TMR16B0_PRESCALER 10000

void
WAKEUP_IRQHandlerPIO0_8 (void)
{
  if(LPC_SYSCON->MAINCLKSEL != MAINCLKSEL_SYSPLL_OUT)
  {
    /* switch to IRC oscillator */
    LPC_SYSCON->MAINCLKSEL = MAINCLKSEL_SYSPLL_OUT;
    /* push clock change */
    LPC_SYSCON->MAINCLKUEN = 0;
    LPC_SYSCON->MAINCLKUEN = 1;
    /* wait for clock change to be finished */
    while (!(LPC_SYSCON->MAINCLKUEN & 1));
    /* power down watchdog oscillator */
    LPC_SYSCON->PDRUNCFG |= WDTOSC_PD;
  }

  /* re-trigger match output */
  LPC_TMR16B0->EMR &= ~1;
  /* reset wakeup logic */
  LPC_SYSCON->STARTRSRP0CLR = STARTxPRP0_PIO0_8;
  /* disable deep sleep */
  SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

  /* enable previous clock settings  */
  LPC_SYSCON->SYSAHBCLKCTRL = g_sysahbclkctrl;
  /* select MISO function for PIO0_8 */
  LPC_IOCON->PIO0_8 = 1;

  /* vodoo -NOP */
  __NOP ();
}

void
pmu_wait_ms (uint16_t ms)
{
  LPC_IOCON->PIO0_8 = 2;

  g_sysahbclkctrl = LPC_SYSCON->SYSAHBCLKCTRL;
  LPC_SYSCON->SYSAHBCLKCTRL |= EN_CT16B0;

  /* prepare 16B0 timer */
  LPC_TMR16B0->TCR = 2;
  LPC_TMR16B0->PR = SYSTEM_CORE_CLOCK/SYSTEM_TMR16B0_PRESCALER;
  LPC_TMR16B0->EMR = 2 << 4;
  LPC_TMR16B0->MR0 = ms*10;
  /* enable IRQ, reset and timer stop in MR0 match */
  LPC_TMR16B0->MCR = 7;

  /* prepare sleep */
  LPC_PMU->PCON = (1 << 11) | (1 << 8);
  SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

  /* start timer */
  LPC_TMR16B0->TCR = 1;
  /* sleep */
  __WFI ();
}

void
pmu_sleep_ms (uint16_t ms)
{
  if (ms < 10)
    ms = 10;

  /* select CT16B0_MAT0 function for PIO0_8 */
  LPC_IOCON->PIO0_8 = 2;

  /* Turn off all other peripheral dividers FIXME save settings */
/*  LPC_SYSCON->SSPCLKDIV = 0;
  LPC_SYSCON->USBCLKDIV = 0;
  LPC_SYSCON->WDTCLKDIV = 0;
  LPC_SYSCON->SYSTICKCLKDIV = 0;*/

  g_sysahbclkctrl = LPC_SYSCON->SYSAHBCLKCTRL;
  LPC_SYSCON->SYSAHBCLKCTRL = EN_RAM | EN_GPIO | EN_CT16B0 | EN_FLASHARRAY | EN_IOCON;

  /* prepare 16B0 timer */
  LPC_TMR16B0->TCR = 2;
  LPC_TMR16B0->PR = 8;
  LPC_TMR16B0->EMR = 2 << 4;
  LPC_TMR16B0->MR0 = ms - 7;
  /* enable IRQ, reset and timer stop in MR0 match */
  LPC_TMR16B0->MCR = 7;

  /* prepare sleep */
  LPC_PMU->PCON = (1 << 11) | (1 << 8);
  SCB->SCR = SCB_SCR_SLEEPDEEP_Msk;

  /* power up watchdog */
  LPC_SYSCON->PDRUNCFG &= ~WDTOSC_PD;
  /* save current power settings, power WDT on wake */
  LPC_SYSCON->PDAWAKECFG = LPC_SYSCON->PDRUNCFG;
  /* power watchdog oscillator in deep sleep mode */
  LPC_SYSCON->PDSLEEPCFG = (~WDTOSC_PD) & 0xFFF;
  /* switch MAINCLKSEL to Watchdog Oscillator */
  LPC_SYSCON->MAINCLKSEL = MAINCLKSEL_WDT;
  /* push clock change */
  LPC_SYSCON->MAINCLKUEN = 0;
  LPC_SYSCON->MAINCLKUEN = 1;
  /* wait for clock change to be executed */
  while (!(LPC_SYSCON->MAINCLKUEN & 1));

  /* start timer */
  LPC_TMR16B0->TCR = 1;
  /* sleep */
  __WFI ();
}

void
pmu_init (void)
{
  /* reset 16B0 timer */
  LPC_TMR16B0->TCR = 2;
  /* Turn on the watchdog oscillator */
  LPC_SYSCON->WDTOSCCTRL = 0x3F;
  /* enable IRQ routine for PIO0_8 */
  NVIC_EnableIRQ (WAKEUP_PIO0_8_IRQn);
  /* initialize start logic for PIO0_8 */
  LPC_SYSCON->STARTAPRP0 |= STARTxPRP0_PIO0_8;
  LPC_SYSCON->STARTRSRP0CLR = STARTxPRP0_PIO0_8;
  LPC_SYSCON->STARTERP0 |= STARTxPRP0_PIO0_8;
}
