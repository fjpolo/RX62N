/*
 * maintasks.c
 *
 *  Created on: 12.07.2018
 *      Author: fpolo
 */

/* Standard C */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "event_groups.h"

/* Hardware specific includes. */
#include "iodefine.h"

/* Demo includes. */
#include "partest.h"
#include "led.h"

/* Defines used in MAIN  */
#include "maindefines.h"

/* FreeRTOS Tasks to be executed */
#include "maintasks.h"

/* Imported variables */
extern xTimerHandle xAutoReloadTimer, xOneShotTimer;
extern uint32_t ulIdleCycleCount;
extern xSemaphoreHandle xBinarySemaphore, xMutex;
extern EventGroupHandle_t xEventGroup;


//
// Begin: Private functions/tasks
//

static void thisisaprivatefunction (void){}

//
// End: Private functions/tasks
//



//
// Begin: Public tasks
//

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
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
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

/* Blink */
void pubBlink( void *pvParameters ){

	portTickType xTimeNow;

	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();
	/* Output a string to show the time at which the callback was executed. */
	//led__toggle(2);
}

void pubBlink_blocking( void *pvParameters ){
	/* The mutex is created before the scheduler is started, so already exists by the
	time this task executes.
	Attempt to take the mutex, blocking indefinitely to wait for the mutex if it is
	not available straight away. The call to xSemaphoreTake() will only return when
	the mutex has been successfully obtained, so there is no need to check the
	function return value. If any other delay period was used then the code must
	check that xSemaphoreTake() returns pdTRUE before accessing the shared resource
	(which in this case is standard out). As noted earlier in this book, indefinite
	time outs are not recommended for production code. */
	xSemaphoreTake( xMutex, portMAX_DELAY );
	{
		/* The following line will only execute once the mutex has been successfully
	obtained. Standard out can be accessed freely now as only one task can have
	the mutex at any one time. */
		led__toggle(2);
		/* The mutex MUST be given back! */
	}
	xSemaphoreGive( xMutex );

}

void pubBlinkingLED ( void *pvParameters ){
	const TickType_t xMaxBlockTimeTicks = 0x20;
	/* Two instances of this task are created. The string printed by the task is
	passed into the task using the task’s parameter. The parameter is cast to the
	required type. */
	//pcStringToPrint = ( char * ) pvParameters;
	for( ;; )
	{
		/* Print out the string using the newly defined function. */
		//prvNewPrintString( pcStringToPrint );
		pubBlink_blocking(NULL);	// NULL to keep compiler happy
		/* Wait a pseudo random time. Note that rand() is not necessarily reentrant,
	but in this case it does not really matter as the code does not care what
	value is returned. In a more secure application a version of rand() that is
	known to be reentrant should be used - or calls to rand() should be protected
	using a critical section. */
		vTaskDelay( ( 50000*rand() % xMaxBlockTimeTicks ) );
	}
}

/* Timer callback */
void pubTimerCallback( xTimerHandle xTimer ){
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
	else if( xTimer == xAutoReloadTimer )
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

/* INT callback */
void vHandlerTask( void *pvParameters ){
	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		/* Use the semaphore to wait for the event. The semaphore was created
	before the scheduler was started, so before this task ran for the first
	time. The task blocks indefinitely, meaning this function call will only
	return once the semaphore has been successfully obtained - so there is
	no need to check the value returned by xSemaphoreTake(). */
		xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );
		/* To get here the event must have occurred. Process the event (in this
	Case, just print out a message). */
		led__toggle(2);
	}
}

/* Centralized Deferred Interrupt Control */
void vDeferredHandlingFunction( void *pvParameter1, uint32_t ulParameter2 ){
	/* Process the event. pvParameter1 and pvParameter2 are not used in this example. */
	//vPrintStringAndNumber( "Handler function - Processing event ", ulParameter2 );
	nop();
	led__toggle(2);
}

/* The task that sets two bits in the event group */
void vEventBitSettingTask( void *pvParameters ){
	const TickType_t xDelay200ms = pdMS_TO_TICKS( 200UL ), xDontBlock = 0;
	for( ;; )
	{
		/* Delay for a short while before starting the next loop. */
		vTaskDelay( xDelay200ms );
		/* Print out a message to say event bit 0 is about to be set by the task,
then set event bit 0. */
		//vPrintString( "Bit setting task -\t about to set bit 0.\r\n" );
		//led__toggle(0);
		xEventGroupSetBits( xEventGroup, mainFIRST_TASK_BIT );
		/* Delay for a short while before setting the other bit. */
		vTaskDelay( xDelay200ms );
		/* Print out a message to say event bit 1 is about to be set by the task,
then set event bit 1. */
		//vPrintString( "Bit setting task -\t about to set bit 1.\r\n" );
		//led__toggle(2);
		xEventGroupSetBits( xEventGroup, mainSECOND_TASK_BIT );
	}
}


void vEventBitReadingTask( void *pvParameters ){
	EventBits_t xEventGroupValue;
	const EventBits_t xBitsToWaitFor = ( mainFIRST_TASK_BIT |
			mainSECOND_TASK_BIT |
			mainISR_BIT );
	for( ;; )
	{
		/* Block to wait for event bits to become set within the event group. */
		xEventGroupValue = xEventGroupWaitBits( /* The event group to read. */
				xEventGroup,
				/* Bits to test. */
				xBitsToWaitFor,
				/* Clear bits on exit if the
unblock condition is met. */
				pdTRUE,
				/* Don't wait for all bits. This
parameter is set to pdTRUE for the
second execution. */
				pdFALSE,
				/* Don't time out. */
				portMAX_DELAY );
		/* Print a message for each bit that was set. */
		if( ( xEventGroupValue & mainFIRST_TASK_BIT ) != 0 )
		{
			//vPrintString( "Bit reading task -\t Event bit 0 was set\r\n" );
			led__toggle(0);
		}
		if( ( xEventGroupValue & mainSECOND_TASK_BIT ) != 0 )
		{
			//vPrintString( "Bit reading task -\t Event bit 1 was set\r\n" );
			led__toggle(1);
		}
		if( ( xEventGroupValue & mainISR_BIT ) != 0 )
		{
			//vPrintString( "Bit reading task -\t Event bit 2 was set\r\n" );
			led__toggle(2);
		}
	}
}

/* The implementation of the task used in Example 23 */
void vSyncingTask( void *pvParameters ){
	const TickType_t xMaxDelay = pdMS_TO_TICKS( 4000UL );
	const TickType_t xMinDelay = pdMS_TO_TICKS( 200UL );
	TickType_t xDelayTime;
	EventBits_t uxThisTasksSyncBit;
	const EventBits_t uxAllSyncBits = ( mainFIRST_TASK_BIT |
			mainSECOND_TASK_BIT |
			mainTHIRD_TASK_BIT );
	/* Three instances of this task are created - each task uses a different event
	bit in the synchronization. The event bit to use is passed into each task
	instance using the task parameter. Store it in the uxThisTasksSyncBit
	variable. */
	uxThisTasksSyncBit = ( EventBits_t ) pvParameters;

	for( ;; ){
		/* Simulate this task taking some time to perform an action by delaying for a
		pseudo random time. This prevents all three instances of this task reaching
		the synchronization point at the same time, and so allows the example’s
		behavior to be observed more easily. */
		xDelayTime = ( rand() % xMaxDelay ) + xMinDelay;
		vTaskDelay( xDelayTime );
		/* Print out a message to show this task has reached its synchronization
		point. pcTaskGetTaskName() is an API function that returns the name assigned
		to the task when the task was created. */
		//vPrintTwoStrings( pcTaskGetTaskName( NULL ), "reached sync point" );
		led__toggle(0);
		/* Wait for all the tasks to have reached their respective synchronization
		points. */
		xEventGroupSync( 	/* The event group used to synchronize. */
							xEventGroup,
							/* The bit set by this task to indicate it has reached the
							synchronization point. */
							uxThisTasksSyncBit,
							/* The bits to wait for, one bit for each task taking part
							in the synchronization. */
							uxAllSyncBits,
							/* Wait indefinitely for all three tasks to reach the
							synchronization point. */
							portMAX_DELAY );
		/* Print out a message to show this task has passed its synchronization
		point. As an indefinite delay was used the following line will only be
		executed after all the tasks reached their respective synchronization
		points. */
		//vPrintTwoStrings( pcTaskGetTaskName( NULL ), "exited sync point" );
		led__toggle(1);
	}
}


//
// End: Public tasks
//







