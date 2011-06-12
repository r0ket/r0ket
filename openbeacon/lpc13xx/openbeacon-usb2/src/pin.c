/***************************************************************
 *
 * OpenBeacon.org - GPIO declaration
 *
 * Copyright 2010 Milosch Meriac <meriac@openbeacon.de>
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
#include "pin.h"

/* IO definitions */
#define BUTTON0_PORT 2
#define BUTTON0_BIT 7
#define LED0_PORT 2
#define LED0_BIT 5
#define LED1_PORT 2
#define LED1_BIT 4
#define CPU_MODE_PMU_PORT 2
#define CPU_MODE_PMU_BIT 8

void
pin_led (uint8_t led)
{
  GPIOSetValue (LED0_PORT, LED0_BIT, (led & GPIO_LED0) > 0);
  GPIOSetValue (LED1_PORT, LED1_BIT, (led & GPIO_LED1) > 0);
}

void
pin_mode_pmu (uint8_t mode)
{
  GPIOSetValue (CPU_MODE_PMU_PORT, CPU_MODE_PMU_BIT, mode);
}

uint8_t
pin_button0 (void)
{
  return GPIOGetValue (BUTTON0_PORT, BUTTON0_BIT);
}

void
pin_init (void)
{
  /* Initialize GPIO (sets up clock) */
  GPIOInit ();

  /* switch ISP button pin to input */
  GPIOSetDir (BUTTON0_PORT, BUTTON0_BIT, 0);

  /* Set LED0 port pin to output */
  //LPC_IOCON->JTAG_nTRST_PIO1_2 = 1;
  GPIOSetDir (LED0_PORT, LED0_BIT, 1);
  GPIOSetValue (LED0_PORT, LED0_BIT, 0);

  /* Set LED1 port pin to output */
  //LPC_IOCON->JTAG_TDO_PIO1_1 = 1;
  GPIOSetDir (LED1_PORT, LED1_BIT, 1);
  GPIOSetValue (LED1_PORT, LED1_BIT, 0);

  /* Set to PMU high power mode by default */
  //LPC_IOCON->PIO0_5 = 0;
  GPIOSetDir (CPU_MODE_PMU_PORT, CPU_MODE_PMU_BIT, 1);
  pin_mode_pmu (0);
}
