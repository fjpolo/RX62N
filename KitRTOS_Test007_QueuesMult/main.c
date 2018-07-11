/***********************************************************************/
/*                                                                     */
/*  FILE        : Main.c                                   */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"

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
 *
 */

/************************************************************
 * Project warnings
 * **********************************************************/
#warning PLEASE CONFIGURE YOUR BOARD USING HARDWARE_INIT()

/************************************************************
 * Project includes
 * **********************************************************/

/* Standard C */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Hardware specific includes. */
#include "iodefine.h"

/* Demo includes. */
#include "partest.h"
#include "led.h"

/* Defines used in MAIN  */
#include "maindefines.h"


/************************************************************
 * Project defines
 * **********************************************************/

/* Priorities at which the tasks are created. */
#define	configBLINKY_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )


/************************************************************
 * Project global variables
 * **********************************************************/


/* This variable is not used by this simple Blinky example.  It is defined
purely to allow the project to link as it is used by the full project. */
volatile unsigned long ulHighFrequencyTickCount = 0UL;

/* Count Idle task calls */
volatile uint32_t ulIdleCycleCount = 0;

/* Declare a variable of type QueueHandle_t. This is used to store the handle
to the queue that is accessed by all three tasks. */
static xQueueHandle xQueue = NULL;

/*-----------------------------------------------------------*/

/************************************************************
 * Project tasks
 * **********************************************************/
static void prvBlink( void *pvParameters );
static void prvPeriodicBlink( void *pvParameters );
static void prvSenderTask( void *pvParameters );
static void prvReceiverTask( void *pvParameters );

/************************************************************
 *
 * 						MAIN
 *
 * **********************************************************/
/* main */
void main(void)
{
	/* The queue is created to hold a maximum of 5 values, each of which is
	large enough to hold a variable of type int32_t. */
	xQueue = xQueueCreate( 5, sizeof( int32_t ) );

	if( xQueue != NULL )
	{

		/* Create LED tasks */
		xTaskCreate( prvBlink, "Blink LED0", configMINIMAL_STACK_SIZE, (void *)0, configBLINKY_TASK_PRIORITY, NULL );
		xTaskCreate( prvBlink, "Blink LED1", configMINIMAL_STACK_SIZE, (void *)1, configBLINKY_TASK_PRIORITY, NULL );
		// Blocking tasks using absolute delay
		xTaskCreate( prvPeriodicBlink, "Blink LED2", configMINIMAL_STACK_SIZE, (void *)2, configBLINKY_TASK_PRIORITY, NULL );
		// LED3 toggles using Idle Task

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
}// END OF MAIN

/*-----------------------------------------------------------*/

static void prvBlink( void *pvParameters ){
	const portTickType xDelay500mS = 500 / portTICK_RATE_MS;
	volatile uint32_t u1;
	//
	for( ;; ){		//
		// Blink LED here
		//LED0 = !LED0;
		led__toggle(( uint8_t )pvParameters);
		// Delay
		vTaskDelay( xDelay500mS );
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
		led__toggle((int *)pvParameters);
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
