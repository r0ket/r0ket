/*****************************************************************************
 *   uarttest.c:  UART test C file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.08.20  ver 1.00    Preliminary version, first Release
 *   2010.07.21  ver 1.01    Added OpenBeacon stakc and debug_printf
 *
******************************************************************************/

#include <openbeacon.h>

int
main (void)
{
  volatile int i;

  /* Basic chip initialization is taken care of in SystemInit() called
   * from the startup code. SystemInit() and chip settings are defined
   * in the CMSIS system_<part family>.c file.
   */

  /* NVIC is installed inside UARTInit file. */
  UARTInit (115200, 0);

  /* Initialize GPIO (sets up clock) */
  GPIOInit ();

  /* Set LED port pin to output */
  GPIOSetDir (LED_PORT, LED_BIT, 1);

  debug_printf ("Hello World!\n");

  while (1)
    {				/* Loop forever */
      if (UARTCount != 0)
	{
	  /* Send back everything we receive */
	  UARTSend ((uint8_t *) UARTBuffer, UARTCount);
	  UARTCount = 0;

	  /* Blink on every TX packet */
	  GPIOSetValue (LED_PORT, LED_BIT, LED_OFF);
	  /* Small Delay to make blink visible */
	  for(i=0;i<200000;i++);
	  /* Turn off LED on exit */
	  GPIOSetValue (LED_PORT, LED_BIT, LED_ON);
	}
    }
}
