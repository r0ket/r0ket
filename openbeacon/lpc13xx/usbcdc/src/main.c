/***************************************************************
 *
 * OpenBeacon.org - USB CDC + FreeRTOS Pipes Demo
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

static BOOL vTasksRunning = FALSE;

/*
 * overwrite default_putchar with USB CDC ACM
 * output to enable USB support for debug_printf
 */
BOOL default_putchar(uint8_t data)
{
	if (vTasksRunning)
		CDC_PutChar (data);
	/* always send out over serial port as well */
	UARTSendChar (data);

	return TRUE;
}

static void serial_task(void *handle)
{
	int i;
	(void) handle;
	portCHAR data;

	vTasksRunning = TRUE;

	i = 0;
	for (;;)
	{
		debug_printf("%04i: Hello Task! (%04i bytes free heap memory)\n", i++,
				xPortGetFreeHeapSize());

		GPIOSetValue(LED_PORT, LED_BIT, LED_ON);
		vTaskDelay(10 / portTICK_RATE_MS);
		GPIOSetValue(LED_PORT, LED_BIT, LED_OFF);

		while (CDC_Recv(&data, sizeof(data), 990))
			UARTSendChar(data);
	}
}

int main(void)
{
	/* Enable CT32B1 Timer, IOCON, and USB */
	LPC_SYSCON->SYSAHBCLKCTRL |= (EN_CT32B1 | EN_USB_REG | EN_IOCON);

	/* Initialize GPIO */
	GPIOInit();
	/* Set LED port pin to output */
	GPIOSetDir(LED_PORT, LED_BIT, 1);
	GPIOSetValue(LED_PORT, LED_BIT, 0);

	/* CDC Initialization */
	CDC_Init();
	/* USB Initialization */
	USB_Init();
	/* Connect to USB port */
	USB_Connect(1);

	/* Update System Core Clock Value */
	SystemCoreClockUpdate();

	/* NVIC is installed inside UARTInit file. */
	UARTInit(115200, 0);

	/* the following text is printed on the serial port as vTasksRunning
	 * is not yet set to TRUE in serial_task - which will redirect
	 * debug output to the CDC ACM USB serial device.
	 */
	debug_printf("Hello World!\n");

	/* Create CDC ACM task */
	xTaskCreate(serial_task, (const signed char*) "Serial", TASK_SERIAL_STACK_SIZE, NULL, TASK_SERIAL_PRIORITY, NULL);

	/* Start the tasks running. */
	vTaskStartScheduler();

	return 0;
}
