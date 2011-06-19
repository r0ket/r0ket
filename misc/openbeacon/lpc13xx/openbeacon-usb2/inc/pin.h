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

#ifndef  __PIN_H__
#define  __PIN_H__

#define GPIO_LEDS_OFF 0
#define GPIO_LED0 1
#define GPIO_LED1 2

extern void pin_init (void);
extern void pin_led (uint8_t led);
extern void pin_mode_pmu (uint8_t mode);
extern uint8_t pin_button0 (void);

#endif /*__PIN_H__*/
