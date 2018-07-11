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
 *  This example demonstrates a queue being created, data being
 * sent to the queue from multiple tasks, and data being received
 * from the queue. The queue is created to hold data items of type
 * int32_t. The tasks that send to the queue do not specify a block
 * time, whereas the task that receives from the queue does.
 *
 *  The priority of the tasks that send to the queue are lower than
 * the priority of the task that receives from the queue. This means
 * the queue should never contain more than one item because, as soon
 * as data is sent to the queue the receiving task will unblock,
 * pre-empt the sending task, and remove the data—leaving the queue
 * empty once again.
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

		/* Create two instances of the task that will send to the queue. The task
		parameter is used to pass the value that the task will write to the queue,
		so one task will continuously write 100 to the queue while the other task
		will continuously write 200 to the queue. Both tasks are created at
		priority 1. */
		xTaskCreate( prvSenderTask, "Sender1", 1000, ( void * ) 100, 1, NULL );
		xTaskCreate( prvSenderTask, "Sender2", 1000, ( void * ) 200, 1, NULL );

		/* Create the task that will read from the queue. The task is created with
		priority 2, so above the priority of the sender tasks. */
		xTaskCreate( prvReceiverTask, "Receiver", 1000, NULL, 2, NULL );

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
	int32_t lValueToSend;
	portBASE_TYPE xStatus;

	/* Two instances of this task are created so the value that is sent to the
	queue is passed in via the task parameter - this way each instance can use
	a different value. The queue was created to hold values of type int32_t,
	so cast the parameter to the required type. */
	lValueToSend = (int32_t) pvParameters;

	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; ){

		/* Send the value to the queue.
		The first parameter is the queue to which data is being sent. The
		queue was created before the scheduler was started, so before this task
		started to execute.
		The second parameter is the address of the data to be sent, in this case
		the address of lValueToSend.
		The third parameter is the Block time – the time the task should be kept
		in the Blocked state to wait for space to become available on the queue
		should the queue already be full. In this case a block time is not
		specified because the queue should never contain more than one item, and
		therefore never be full. */
		xStatus = xQueueSendToBack( xQueue, &lValueToSend, 0 );

		if( xStatus != pdPASS ){

			/* The send operation could not complete because the queue was full -
			this must be an error as the queue should never contain more than
			one item! */
			//vPrintString( "Could not send to the queue.\r\n" );
			nop();
		}
	}
}

/*-----------------------------------------------------------*/

static void prvReceiverTask( void *pvParameters ){
	/* Declare the variable that will hold the values received from the queue. */
	int32_t lReceivedValue;
	portBASE_TYPE xStatus;
	const portTickType xTicksToWait = 100 / portTICK_RATE_MS;;
	/* This task is also defined within an infinite loop. */
	for( ;; )
	{
		/* This call should always find the queue empty because this task will
		immediately remove any data that is written to the queue. */
		if( uxQueueMessagesWaiting( xQueue ) != 0 )
		{
			//vPrintString( "Queue should have been empty!\r\n" );
			nop();
		}
		/* Receive data from the queue.
		The first parameter is the queue from which data is to be received. The
		queue is created before the scheduler is started, and therefore before this
		task runs for the first time.
		The second parameter is the buffer into which the received data will be
		placed. In this case the buffer is simply the address of a variable that
		has the required size to hold the received data.
		The last parameter is the block time – the maximum amount of time that the
		task will remain in the Blocked state to wait for data to be available
		should the queue already be empty. */
		// TODO: Add lReceivedValue to Watch Expression and debug
		xStatus = xQueueReceive( xQueue, &lReceivedValue, xTicksToWait );

		if( xStatus == pdPASS )
		{
			/* Data was successfully received from the queue, print out the received
			value. */
			//vPrintStringAndNumber( "Received = ", lReceivedValue );
			nop();
		}
		else
		{
			/* Data was not received from the queue even after waiting for 100ms.
			This must be an error as the sending tasks are free running and will be
			continuously writing to the queue. */
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
