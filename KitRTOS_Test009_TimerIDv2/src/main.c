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

/* FreeRTOS Tasks to be executed */
#include "maintasks.h"


/************************************************************
 * Project global variables
 * **********************************************************/

/* This variable is not used by this simple Blinky example.  It is defined
purely to allow the project to link as it is used by the full project. */
volatile unsigned long ulHighFrequencyTickCount = 0UL;

/* Counts timers calls */
uint32_t ulIdleCycleCount = 0;

/* Timer handles */
xTimerHandle xAutoReloadTimer, xOneShotTimer, xBlinkAutoReloadTimer;

/*-----------------------------------------------------------*/



/************************************************************
 *
 * 						MAIN
 *
 * **********************************************************/
/* main */
void main(void)
{
	portBASE_TYPE xTimer1Started, xTimer2Started, xTimer3Started;

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

	/* Create the auto-reload timer, storing the handle to the created timer in xAutoReloadTimer. */
	xBlinkAutoReloadTimer = xTimerCreate(
				/* Text name for the software timer - not used by FreeRTOS. */
				"BlinkAutoReload",
				/* The software timer's period in ticks. */
				mainBLINK_AUTO_RELOAD_PERIOD,
				/* Setting uxAutoRealod to pdTRUE creates an auto-reload timer. */
				pdTRUE,
				/* This example does not use the timer id. */
				0,
				/* The callback function to be used by the software timer being created. */
				pubBlink );



	if( ( xOneShotTimer != NULL ) && ( xAutoReloadTimer != NULL ) && ( xBlinkAutoReloadTimer != NULL ))
	{
		/* Start the software timers, using a block time of 0 (no block time). The scheduler has
		not been started yet so any block time specified here would be ignored anyway. */
		xTimer1Started = xTimerStart( xOneShotTimer, 0 );
		xTimer2Started = xTimerStart( xAutoReloadTimer, 0 );
		xTimer3Started = xTimerStart( xBlinkAutoReloadTimer, 0 );

		/* The implementation of xTimerStart() uses the timer command queue, and xTimerStart()
		will fail if the timer command queue gets full. The timer service task does not get
		created until the scheduler is started, so all commands sent to the command queue will
		stay in the queue until after the scheduler has been started. Check both calls to
		xTimerStart() passed. */
		if( ( xTimer1Started == pdPASS ) && ( xTimer2Started == pdPASS ) && ( xTimer3Started == pdPASS ) )
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

