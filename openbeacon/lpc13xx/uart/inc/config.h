/*****************************************************************************
 *   config.h:  config file for uart example for NXP LPC13xx Family
 *   Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.19  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/

/*
Overview:
   This example shows how to use a timer to blink an LED. It works by using
   libLPC to configure a I/O port and a 32-bit timer, then setting up a timer
   interrupt to increment a counter. The main loop polls the counter to
   determine when to toggle the LED state.

User #defines:
   LED_PORT	I/O port driving an LED
   LED_BIT  I/O port bit driving an LED
   LED_ON   Value to set the bit to turn on the LED
   LED_OFF  Value to set the bit to turn off the LED
   LED_TOGGLE_TICKS
		    Number of timer ticks per flash. The timer is configured to generate
		    an interrupt 100 times a second or every 10mS.

How to use:
   Click the debug toolbar button.

   * You should see an LED flashing.
*/

#define LED_PORT 1		// Port for led
#define LED_BIT 9		// Bit on port for led
#define LED_ON 1		// Level to set port to turn on led
#define LED_OFF 0		// Level to set port to turn off led
#define LED_TOGGLE_TICKS 100	// 100 ticks = 1 Hz flash rate

/*********************************************************************************
**                            End Of File
*********************************************************************************/
