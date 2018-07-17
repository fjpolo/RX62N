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
 *   This example improves on _Interrupt's implementation by
 *  using a counting semaphore in place of the binary semaphore.
 *  main() is changed to include a call to xSemaphoreCreateCounting()
 *  in place of the call to xSemaphoreCreateBinary().
 *
 *   To simulate multiple events occurring at high frequency, the
 *  interrupt service routine is changed to ‘give’ the semaphore more
 *  than once per interrupt. Each event is latched in the semaphore’s
 *  count value
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


/************************************************************
 * Project global function definition
 * **********************************************************/
void prvInitTimer(void);

/************************************************************
 * Project global variables
 * **********************************************************/

/* This variable is not used by this simple Blinky example.  It is defined
purely to allow the project to link as it is used by the full project. */
volatile unsigned long ulHighFrequencyTickCount = 0UL;

/* Counts timers calls */
uint32_t ulIdleCycleCount = 0;

/* Timer handles */
xTimerHandle xAutoReloadTimer, xOneShotTimer;

/* Semaphore handles */
xSemaphoreHandle xBinarySemaphore, xCountingSemaphore;


/************************************************************
 *
 * 						MAIN
 *
 * **********************************************************/
/* main */
void main(void)
{
	portBASE_TYPE xTimer1Started, xTimer2Started;

	/* Before a semaphore is used it must be explicitly created. In this example
	a counting semaphore is created. */
	vSemaphoreCreateBinary(xBinarySemaphore);
	xCountingSemaphore = xSemaphoreCreateCounting( 10, 0 );

	/* Create the one shot timer software timer, storing the handle in xOneShotTimer. */
	xOneShotTimer = xTimerCreate( "OneShot",
			mainONE_SHOT_TIMER_PERIOD,
			pdFALSE,
			/* The timer’s ID is initialized to 0. */
			0,
			/* prvTimerCallback() is used by both timers. */
			pubTimerCallback );

	/* Create the auto-reload software timer, storing the handle in xAutoReloadTimer */
	xAutoReloadTimer = xTimerCreate( "AutoReload",
			mainAUTO_RELOAD_TIMER_PERIOD,
			pdTRUE,
			/* The timer’s ID is initialized to 0. */
			0,
			/* prvTimerCallback() is used by both timers. */
			pubTimerCallback );

	prvInitTimer();



	if( ( xOneShotTimer != NULL ) && ( xAutoReloadTimer != NULL ) && ( xBinarySemaphore != NULL ) )
	{
		/* Start the software timers, using a block time of 0 (no block time). The scheduler has
		not been started yet so any block time specified here would be ignored anyway. */
		xTimer1Started = xTimerStart( xOneShotTimer, 0 );
		xTimer2Started = xTimerStart( xAutoReloadTimer, 0 );
		/* Create the 'handler' task, which is the task to which interrupt
		processing is deferred. This is the task that will be synchronized with
		the interrupt. The handler task is created with a high priority to ensure
		it runs immediately after the interrupt exits. In this case a priority of
		3 is chosen. */
		xTaskCreate( vHandlerTask, "INTHandler", 1000, NULL, 3, NULL );

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



/*------------------------------------------------------------------------------------------------------------*/

void prvInitTimer(void){
	IEN(TMR0,CMIA0) = 1;
	IPR(TMR0,CMIA0) = 3;
	MSTP(TMR0) = 0; 		//Activate TMR0 unit
	MSTP(TMR1) = 0; 		//Activate TMR1 unit
	TMR1.TCCR.BIT.CSS = 1;
	TMR1.TCCR.BIT.CKS = 5; 	//Count source is PCLK/1024 for TMR1
	TMR0.TCCR.BIT.CSS = 3;
	TMR0.TCR.BIT.CCLR = 1; 	//Count source is TMR1 for TMR0
	TMR0.TCORA = 0x5B; 		//Set TCORA to 23405
	TMR1.TCORA = 0x6D; 		//(0x5B6D hex)
	TMR0.TCR.BIT.CMIEA = 1;
}

/*------------------------------------------------------------------------------------------------------------*/
