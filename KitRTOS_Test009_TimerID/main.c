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
 *  The same callback function can be assigned to more than one
 * software timer. When that is done, the callback function
 * parameter is used to determine which software timer expired.
 *
 *  Example 13 used two separate callback functions; one callback
 * function was used by the one-shot timer, and the other callback
 * function was used by the auto-reload timer. Example 14 creates
 * similar functionality to that created by Example 13, but assigns a
 * single callback function to both software timers.
 *
 *  The main() function used by Example 14 is almost identical to the
 * main() function used in Example 13. The only difference is where
 * the software timers are created. This difference is shown in Listing
 * 80, where prvTimerCallback() is used as the callback function for
 * both timers.
 *
 *
 *prvTimerCallback() will execute when either timer expires. The
 *prvTimerCallback() implementation of prvTimerCallback() uses the
 *prvTimerCallback() function’s parameter to determine if it was called
 * because the one-shot timer expired, or because the auto-reload timer
 * expired.
 *
 *
 *prvTimerCallback() also demonstrates how to use the software timer ID
 *prvTimerCallback() as timer specific storage; each software timer keeps
 * a count of the number of times it has expired in its own ID, and the
 * auto-reload timer uses the count to stop itself the fifth time it executes.
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
#include "timers.h"

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

/* Counts timers calls */
uint32_t ulIdleCycleCount = 0;
uint32_t ulCallCount1 = 0;
uint32_t ulCallCount2 = 0;

/* Declare a variable of type QueueHandle_t. This is used to store the handle
to the queue that is accessed by all three tasks. */
static xQueueHandle xQueue = NULL;

/* Timer handles */
xTimerHandle xAutoReloadTimer, xOneShotTimer;

/*-----------------------------------------------------------*/

/************************************************************
 * Project tasks
 * **********************************************************/
static void prvBlink( void *pvParameters );
static void prvPeriodicBlink( void *pvParameters );
static void prvSenderTask( void *pvParameters );
static void prvReceiverTask( void *pvParameters );
static void prvOneShotTimerCallback( xTimerHandle xTimer );
static void prvAutoReloadTimerCallback( xTimerHandle xTimer );
static void prvTimerCallback( xTimerHandle xTimer );



/************************************************************
 *
 * 						MAIN
 *
 * **********************************************************/
/* main */
void main(void)
{
	portBASE_TYPE xTimer1Started, xTimer2Started;

	/* Create the one shot timer software timer, storing the handle in xOneShotTimer. */
	xOneShotTimer = xTimerCreate( "OneShot",
			mainONE_SHOT_TIMER_PERIOD,
			pdFALSE,
			/* The timer’s ID is initialized to 0. */
			0,
			/* prvTimerCallback() is used by both timers. */
			prvTimerCallback );

	/* Create the auto-reload software timer, storing the handle in xAutoReloadTimer */
	xAutoReloadTimer = xTimerCreate( "AutoReload",
			mainAUTO_RELOAD_TIMER_PERIOD,
			pdTRUE,
			/* The timer’s ID is initialized to 0. */
			0,
			/* prvTimerCallback() is used by both timers. */
			prvTimerCallback );



	if( ( xOneShotTimer != NULL ) && ( xAutoReloadTimer != NULL ) )
	{
		/* Start the software timers, using a block time of 0 (no block time). The scheduler has
		not been started yet so any block time specified here would be ignored anyway. */
		xTimer1Started = xTimerStart( xOneShotTimer, 0 );
		xTimer2Started = xTimerStart( xAutoReloadTimer, 0 );

		/* The implementation of xTimerStart() uses the timer command queue, and xTimerStart()
		will fail if the timer command queue gets full. The timer service task does not get
		created until the scheduler is started, so all commands sent to the command queue will
		stay in the queue until after the scheduler has been started. Check both calls to
		xTimerStart() passed. */
		if( ( xTimer1Started == pdPASS ) && ( xTimer2Started == pdPASS ) )
		{
			/* Start the scheduler. */
			vTaskStartScheduler();
		}
	}

	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
}
// END OF MAIN



/*-----------------------------------------------------------*/

static void prvTimerCallback( xTimerHandle xTimer )
{
	portTickType xTimeNow;
	uint32_t ulExecutionCount;
	/* A count of the number of times this software timer has expired is stored in the timer's
	ID. Obtain the ID, increment it, then save it as the new ID value. The ID is a void
	pointer, so is cast to a uint32_t. */
	ulExecutionCount = ( uint32_t ) pvTimerGetTimerID( xTimer );
	ulExecutionCount++;
	vTimerSetTimerID( xTimer, ( void * ) ulExecutionCount );
	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();
	/* The handle of the one-shot timer was stored in xOneShotTimer when the timer was created.
	Compare the handle passed into this function with xOneShotTimer to determine if it was the
	one-shot or auto-reload timer that expired, then output a string to show the time at which
	the callback was executed. */
	if( xTimer == xOneShotTimer )
	{
		//vPrintStringAndNumber( "One-shot timer callback executing", xTimeNow );
		nop();	// TODO DO SOMETHING!!!
		led__toggle(0);
	}
	else
	{
		/* xTimer did not equal xOneShotTimer, so it must have been the auto-reload timer that
		expired. */
		//vPrintStringAndNumber( "Auto-reload timer callback executing", xTimeNow );
		nop();	// TODO DO SOMETHING!!!
		led__toggle(1);

		if( ulExecutionCount == 50 )
		{
			/* Stop the auto-reload timer after it has executed 5 times. This callback function
	executes in the context of the RTOS daemon task so must not call any functions that
	might place the daemon task into the Blocked state. Therefore a block time of 0 is
	used. */
			xTimerStop( xTimer, 0 );
		}
	}
}

/*-----------------------------------------------------------*/

static void prvOneShotTimerCallback( xTimerHandle xTimer ){

	portTickType xTimeNow;

	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();
	/* Output a string to show the time at which the callback was executed. */
	//vPrintStringAndNumber( "One-shot timer callback executing", xTimeNow );
	led__toggle(0);
	/* File scope variable. */
	ulCallCount1++;
}

/*-----------------------------------------------------------*/

static void prvAutoReloadTimerCallback( xTimerHandle xTimer ){

	portTickType xTimeNow;

	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();
	/* Output a string to show the time at which the callback was executed. */
	//vPrintStringAndNumber( "Auto-reload timer callback executing", xTimeNow );
	led__toggle(1);
	/* File scope variable. */
	ulCallCount2++;
}

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
