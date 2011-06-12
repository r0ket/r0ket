/***************************************************************
 *
 * OpenBeacon.org - Power Management
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

#ifndef  __PMU_H__
#define  __PMU_H__

#define PMU_IRCOUT_PD         (1UL << 0)
#define PMU_IRC_PD            (1UL << 1)
#define PMU_FLASH_PD          (1UL << 2)
#define PMU_BOD_PD            (1UL << 3)
#define PMU_ADC_PD            (1UL << 4)
#define PMU_SYSOSC_PD         (1UL << 5)
#define PMU_WDTOSC_PD         (1UL << 6)
#define PMU_SYSPLL_PD         (1UL << 7)
#define PMU_USBPLL_PD         (1UL << 8)
#define PMU_RESERVED1_DEEP_PD (1UL << 9)
#define PMU_USBPAD_PD         (1UL <<10)
#define PMU_RESERVED2_DEEP_PD (1UL <<11)

#define NVIC_LP_SEVONPEND     (0x10)
#define NVIC_LP_SLEEPDEEP     (0x04)
#define NVIC_LP_SLEEPONEXIT   (0x02)

extern void pmu_init (void);
extern void pmu_status (void);
extern void pmu_off (uint32_t reason);
extern uint32_t pmu_reason (void);

#endif /*__PMU_H__*/
