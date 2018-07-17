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
extern xSemaphoreHandle xBinarySemaphore;


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

/* Applciation setup timer interrupt */
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

/* Application Malloc failed Hook */
void vApplicationMallocFailedHook( void )
{
	for( ;; );
}

/* Application stack overflow hook */
void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pxTask;
	( void ) pcTaskName;
	for( ;; );
}

/* Application Idle Hook */
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

/* Blink */
void pubBlink( void *pvParameters ){

	portTickType xTimeNow;

	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();
	/* Output a string to show the time at which the callback was executed. */
	//vPrintStringAndNumber( "Auto-reload timer callback executing", xTimeNow );
	led__toggle(2);
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

//
// End: Public tasks
//







