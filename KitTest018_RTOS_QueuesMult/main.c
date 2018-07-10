/*
    FreeRTOS V7.3.0 - Copyright (C) 2012 Real Time Engineers Ltd.

    FEATURES AND PORTS ARE ADDED TO FREERTOS ALL THE TIME.  PLEASE VISIT 
    http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

 ***************************************************************************
 *                                                                       *
 *    FreeRTOS tutorial books are available in pdf and paperback.        *
 *    Complete, revised, and edited pdf reference manuals are also       *
 *    available.                                                         *
 *                                                                       *
 *    Purchasing FreeRTOS documentation will not only help you, by       *
 *    ensuring you get running as quickly as possible and with an        *
 *    in-depth knowledge of how to use FreeRTOS, it will also help       *
 *    the FreeRTOS project to continue with its mission of providing     *
 *    professional grade, cross platform, de facto standard solutions    *
 *    for microcontrollers - completely free of charge!                  *
 *                                                                       *
 *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
 *                                                                       *
 *    Thank you for using FreeRTOS, and thank you for your support!      *
 *                                                                       *
 ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

 ***************************************************************************
 *                                                                       *
 *    Having a problem?  Start by reading the FAQ "My application does   *
 *    not run, what could be wrong?"                                     *
 *                                                                       *
 *    http://www.FreeRTOS.org/FAQHelp.html                               *
 *                                                                       *
 ***************************************************************************


    http://www.FreeRTOS.org - Documentation, training, latest versions, license 
    and contact details.  

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool.

    Real Time Engineers ltd license FreeRTOS to High Integrity Systems, who sell 
    the code with commercial support, indemnification, and middleware, under 
    the OpenRTOS brand: http://www.OpenRTOS.com.  High Integrity Systems also
    provide a safety engineered and independently SIL3 certified version under 
    the SafeRTOS brand: http://www.SafeRTOS.com.
 */

/* 
 *  This example is similar to _Queues, but the task priorities are
 * reversed, so the receiving task has a lower priority than the sending
 * tasks. Also, the queue is used to pass structures, rather than
 * integers.
 *
 *    In _Queues, the receiving task has the highest priority, so the queue
 *  never contains more than one item. This results from the receiving task
 *  pre-empting the sending tasks as soon as data is placed into the queue.
 *  In _QueuesMult, the sending tasks have the higher priority, so the queue
 *  will normally be full. This is because, as soon as the receiving task
 *  removes an item from the queue, it is pre-empted by one of the sending
 *  tasks which then immediately re-fills the queue. The sending task then
 *  re-enters the Blocked state to wait for space to become available on the
 *  queue again.
 */

#warning LEDS and Switches have been configured for RDK RX62N Development Board under YRDKRX62N.h
#warning Configuration done in hardware_setup() (hwsetup.c) by PowerON_Reset() (reset_program.c)

/* Standard C includes. */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Hardware specific includes. */
#include "iodefine.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Demo includes. */
#include "partest.h"

/* Defines used in MAIN  */
#include "maindefines.h"


/*
 * The tasks as defined at the top of this file.
 */
static void prvBlink( void *pvParameters );
static void prvPeriodicBlink( void *pvParameters );
static void prvSenderTask( void *pvParameters );
static void prvReceiverTask( void *pvParameters );

/* This variable is not used by this simple Blinky example.  It is defined 
purely to allow the project to link as it is used by the full project. */
volatile unsigned long ulHighFrequencyTickCount = 0UL;

/* Just to see if the send and receive tasks are running if there are no LEDs. */
volatile unsigned long ulSends = 0, ulReceives = 0;
volatile uint32_t ulIdleCycleCount = 0;

/* Declare a variable of type QueueHandle_t. This is used to store the handle
to the queue that is accessed by all three tasks. */
static xQueueHandle xQueue = NULL;


/*-----------------------------------------------------------*/

void main(void)
{
	/* Turn all LEDs off. */
	vParTestInitialise();

	/* The queue is created to hold a maximum of 3 structures of type Data_t. */
	xQueue = xQueueCreate( 10, sizeof( Data_t ) );

	if( xQueue != NULL )
	{

		// Blocking tasks using relative delay
		xTaskCreate( prvBlink, "Blink LED0", configMINIMAL_STACK_SIZE, (void *)0, configBLINKY_TASK_PRIORITY, NULL );			// LED0
		xTaskCreate( prvBlink, "Blink LED1", configMINIMAL_STACK_SIZE, (void *)1, configBLINKY_TASK_PRIORITY, NULL );			// LED1
		// Blocking tasks using absolute delay
		xTaskCreate( prvPeriodicBlink, "Blink LED2", configMINIMAL_STACK_SIZE, (void *)2, configBLINKY_TASK_PRIORITY, NULL );	// LED2
		// LED3 toggles every time ulIdleCycleCount overflows at 20bits

		/* Create two instances of the task that will write to the queue. The
		parameter is used to pass the structure that the task will write to the
		queue, so one task will continuously send xStructsToSend[ 0 ] to the queue
		while the other task will continuously send xStructsToSend[ 1 ]. Both
		tasks are created at priority 2, which is above the priority of the receiver. */
		xTaskCreate( prvSenderTask, "Sender1", 1000, &( xStructsToSend[ 0 ] ), 2, NULL );
		xTaskCreate( prvSenderTask, "Sender2", 1000, &( xStructsToSend[ 1 ] ), 2, NULL );

		/* Create the task that will read from the queue. The task is created with
		priority 1, so below the priority of the sender tasks. */
		xTaskCreate( prvReceiverTask, "Receiver", 1000, NULL, 1, NULL );

		/* Start the tasks running. */
		vTaskStartScheduler();
	}

	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
}

/*-----------------------------------------------------------*/

static void prvBlink( void *pvParameters ){
	const portTickType xDelay250mS = 500 / portTICK_RATE_MS;
	//volatile uint32_t u1;
	//
	for( ;; ){		//
		// Blink LED here
		led__toggle((int)pvParameters);
		// Delay
		vTaskDelay(xDelay250mS);
	}
}

/*-----------------------------------------------------------*/

static void prvPeriodicBlink( void *pvParameters ){
	//
	const portTickType xDelay50mS = 50 / portTICK_RATE_MS;
	portTickType xLastWakeTime;
	//
	xLastWakeTime = xTaskGetTickCount();
	//
	for( ;; ){		//
		// Blink LED here
		led__toggle((int)pvParameters);
		// Delay
		vTaskDelayUntil(&xLastWakeTime, xDelay50mS);
	}
}

/*-----------------------------------------------------------*/

static void prvSenderTask( void *pvParameters ){
	//
	portBASE_TYPE xStatus;
	const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; ){

		/* Send to the queue.
		The second parameter is the address of the structure being sent. The
		address is passed in as the task parameter so pvParameters is used
		directly.
		The third parameter is the Block time - the time the task should be kept
		in the Blocked state to wait for space to become available on the queue
		if the queue is already full. A block time is specified because the
		sending tasks have a higher priority than the receiving task so the queue
		is expected to become full. The receiving task will remove items from
		the queue when both sending tasks are in the Blocked state. */
		xStatus = xQueueSendToBack( xQueue, pvParameters, xTicksToWait );

		if( xStatus != pdPASS )
		{
			/* The send operation could not complete, even after waiting for 100ms.
			This must be an error as the receiving task should make space in the
			queue as soon as both sending tasks are in the Blocked state. */
			//vPrintString( "Could not send to the queue.\r\n" );
			nop();
		}
	}
}

/*-----------------------------------------------------------*/

static void prvReceiverTask( void *pvParameters ){
	/* Declare the structure that will hold the values received from the queue. */
	Data_t xReceivedStructure;
	portBASE_TYPE xStatus;

	/* This task is also defined within an infinite loop. */
	for( ;; )
	{
		/* Because it has the lowest priority this task will only run when the
		sending tasks are in the Blocked state. The sending tasks will only enter
		the Blocked state when the queue is full so this task always expects the
		number of items in the queue to be equal to the queue length, which is 3 in
		this case. */
		if( uxQueueMessagesWaiting( xQueue ) != 3 )
		{
			//vPrintString( "Queue should have been full!\r\n" );
			nop();
		}
		/* Receive from the queue.
		The second parameter is the buffer into which the received data will be
		placed. In this case the buffer is simply the address of a variable that
		has the required size to hold the received structure.
		The last parameter is the block time - the maximum amount of time that the
		task will remain in the Blocked state to wait for data to be available
		if the queue is already empty. In this case a block time is not necessary
		because this task will only run when the queue is full. */
		xStatus = xQueueReceive( xQueue, &xReceivedStructure, 0 );

		if( xStatus == pdPASS )
		{
			/* Data was successfully received from the queue, print out the received
		value and the source of the value. */
			if( xReceivedStructure.eDataSource == eSender1 )
			{
				//vPrintStringAndNumber( "From Sender 1 = ", xReceivedStructure.ucValue );
				nop();
			}
			else
			{
				//vPrintStringAndNumber( "From Sender 2 = ", xReceivedStructure.ucValue );
				nop();
			}
		}
		else
		{
			/* Nothing was received from the queue. This must be an error as this
			task should only run when the queue is full. */
			//vPrintString( "Could not receive from the queue.\r\n" );
			nop();
		}
	}
}

/*-----------------------------------------------------------*/

void vApplicationSetupTimerInterrupt( void )
{
	/* Enable compare match timer 0. */
	MSTP( CMT0 ) = 0;

	/* Interrupt on compare match. */
	CMT0.CMCR.BIT.CMIE = 1;

	/* Set the compare match value. */
	CMT0.CMCOR = ( unsigned short ) ( ( ( configPERIPHERAL_CLOCK_HZ / configTICK_RATE_HZ ) -1 ) / 8 );

	/* Divide the PCLK by 8. */
	CMT0.CMCR.BIT.CKS = 0;

	/* Enable the interrupt... */
	_IEN( _CMT0_CMI0 ) = 1;

	/* ...and set its priority to the application defined kernel priority. */
	_IPR( _CMT0_CMI0 ) = configKERNEL_INTERRUPT_PRIORITY;

	/* Start the timer. */
	CMT.CMSTR0.BIT.STR0 = 1;
}
/*-----------------------------------------------------------*/

/* This function is explained by the comments above its prototype at the top
of this file. */
void vApplicationMallocFailedHook( void )
{
	for( ;; );
}
/*-----------------------------------------------------------*/

/* This function is explained by the comments above its prototype at the top
of this file. */
void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pxTask;
	( void ) pcTaskName;
	for( ;; );
}
/*-----------------------------------------------------------*/

/* This function is explained by the comments above its prototype at the top
of this file. */
void vApplicationIdleHook( void )
{
	/* Just to prevent the variable getting optimised away. */
	( void ) ulHighFrequencyTickCount;
	ulIdleCycleCount++;
	if( ulIdleCycleCount == 1048576){		// Overfolw at 20-bit
		led__toggle(3);
		ulIdleCycleCount = 0UL;
	}

}
/*-----------------------------------------------------------*/
