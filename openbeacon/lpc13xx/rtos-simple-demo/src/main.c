/*
    FreeRTOS V6.1.0 - Copyright (C) 2010 Real Time Engineers Ltd.

    ***************************************************************************
    *                                                                         *
    * If you are:                                                             *
    *                                                                         *
    *    + New to FreeRTOS,                                                   *
    *    + Wanting to learn FreeRTOS or multitasking in general quickly       *
    *    + Looking for basic training,                                        *
    *    + Wanting to improve your FreeRTOS skills and productivity           *
    *                                                                         *
    * then take a look at the FreeRTOS books - available as PDF or paperback  *
    *                                                                         *
    *        "Using the FreeRTOS Real Time Kernel - a Practical Guide"        *
    *                  http://www.FreeRTOS.org/Documentation                  *
    *                                                                         *
    * A pdf reference manual is also available.  Both are usually delivered   *
    * to your inbox within 20 minutes to two hours when purchased between 8am *
    * and 8pm GMT (although please allow up to 24 hours in case of            *
    * exceptional circumstances).  Thank you for your support!                *
    *                                                                         *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/*
 * This is a very simple demo that demonstrates task and queue usages only.
 * Details of other FreeRTOS features (API functions, tracing features,
 * diagnostic hook functions, memory management, etc.) can be found on the
 * FreeRTOS web site (http://www.FreeRTOS.org) and in the FreeRTOS book.
 * Details of this demo (what it does, how it should behave, etc.) can be found
 * in the accompanying PDF application note.
 *
*/

/* Kernel includes. */
#include <FreeRTOS.h>
#include <openbeacon.h>
#include <task.h>
#include <queue.h>
#include "config.h"

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue, specified in milliseconds. */
#define mainQUEUE_SEND_FREQUENCY_MS		( 500 / portTICK_RATE_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH			( 1 )

/*
 * The tasks as described in the accompanying PDF application note.
 */
static void prvQueueReceiveTask (void *pvParameters);
static void prvQueueSendTask (void *pvParameters);

/* The queue used by both tasks. */
static xQueueHandle xQueue = NULL;
/*-----------------------------------------------------------*/

int
main (void)
{
  /* Initialize GPIO (sets up clock) */
  GPIOInit ();
  /* Set LED port pin to output */
  GPIOSetDir (LED_PORT, LED_BIT, 1);
  GPIOSetValue (LED_PORT, LED_BIT, 0);

  /* Create the queue. */
  xQueue = xQueueCreate (mainQUEUE_LENGTH, sizeof (unsigned long));

  if (xQueue != NULL)
    {
      /* Start the two tasks as described in the accompanying application
         note. */
      xTaskCreate (prvQueueReceiveTask, (signed char *) "Rx",
		   configMINIMAL_STACK_SIZE, NULL,
		   mainQUEUE_RECEIVE_TASK_PRIORITY, NULL);
      xTaskCreate (prvQueueSendTask, (signed char *) "TX",
		   configMINIMAL_STACK_SIZE, NULL,
		   mainQUEUE_SEND_TASK_PRIORITY, NULL);

      /* Start the tasks running. */
      vTaskStartScheduler ();
    }

  /* If all is well we will never reach here as the scheduler will now be
     running.  If we do reach here then it is likely that there was insufficient
     heap available for the idle task to be created. */
  for (;;);
}

/*-----------------------------------------------------------*/

static void
prvQueueSendTask (void *pvParameters)
{
  portTickType xNextWakeTime;
  unsigned long ulValueToSend;

  (void) pvParameters;

  /* Initialise xNextWakeTime - this only needs to be done once. */
  xNextWakeTime = xTaskGetTickCount ();

  for (;;)
    {
      /* Place this task in the blocked state until it is time to run again.
         The block state is specified in ticks, the constant used converts ticks
         to ms.  While in the blocked state this task will not consume any CPU
         time. */
      vTaskDelayUntil (&xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS);

      /* Send to the queue - causing the queue receive task to flash its LED.
         0 is used as the block time so the sending operation will not block -
         it shouldn't need to block as the queue should always be empty at this
         point in the code. */
      ulValueToSend = 100;
      xQueueSend (xQueue, &ulValueToSend, 0);

      /* Place this task in the blocked state until it is time to run again.
         The block state is specified in ticks, the constant used converts ticks
         to ms.  While in the blocked state this task will not consume any CPU
         time. */
      vTaskDelayUntil (&xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS);

      /* Send to the queue - causing the queue receive task to flash its LED.
         0 is used as the block time so the sending operation will not block -
         it shouldn't need to block as the queue should always be empty at this
         point in the code. */
      ulValueToSend = 0;
      xQueueSend (xQueue, &ulValueToSend, 0);
    }
}

/*-----------------------------------------------------------*/

static void
prvQueueReceiveTask (void *pvParameters)
{
  unsigned long ulReceivedValue;

  (void) pvParameters;

  for (;;)
    {
      /* Wait until something arrives in the queue - this task will block
         indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
         FreeRTOSConfig.h. */
      xQueueReceive (xQueue, &ulReceivedValue, portMAX_DELAY);

      /*  To get here something must have been received from the queue, but
         is it the expected value?  If it is, toggle the LED. */
      if (ulReceivedValue == 100UL)
	GPIOSetValue (LED_PORT, LED_BIT, 1);
      else
	GPIOSetValue (LED_PORT, LED_BIT, 0);
    }
}
