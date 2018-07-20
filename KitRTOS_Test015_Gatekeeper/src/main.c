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
 *  This example provides another alternative implementation
 * for pubBlink_blocking(). This time, a gatekeeper task is
 * used to manage access to register bits. When a task wants
 * to write a register bit, it does not call led__toggle
 * function directly but, instead, sends the message with
 * the LED number to be toggled to the gatekeeper.
 *
 * The gatekeeper task uses a FreeRTOS queue to serialize
 * access to the register. The internal implementation of the
 * task does not have to consider mutual exclusion because it
 * is the only task permitted to access registers directly.
 *
 * The gatekeeper task spends most of its time in the Blocked
 * state, waiting for messages to arrive on the queue. When a
 * message arrives, the gatekeeper simply writes the bit to
 * the register, before returning to the Blocked state to wait
 * for the next message.
 *
 * Interrupts can send to queues, so interrupt service routines
 * can also safely use the services of the gatekeeper to write
 * messages to the terminal. In this example, a tick hook
 * function is used to write out a message every 200 ticks.
 *
 * A tick hook (or tick callback) is a function that is called
 * by the kernel during each tick interrupt. Set
 * configUSE_TICK_HOOK to 1 in FreeRTOSConfig.h. Provide the
 * implementation of the hook function.
 *
 * Tick hook functions execute within the context of the tick interrupt, and so must be kept very short, must use only a moderate amount of stack space, and must not call any FreeRTOS API functions that do not end with ‘FromISR()’
 *
 * The scheduler will always execute immediately after the tick hook function, so interrupt safe FreeRTOS API functions called from the tick hook do not need to use their pxHigherPriorityTaskWoken parameter, and the parameter can be set to NULL.
 *
 *
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
xSemaphoreHandle xBinarySemaphore, xMutex;

/* Declare a variable of type QueueHandle_t. This is used to store the handle
to the queue that is accessed by all three tasks. */
xQueueHandle xBlinkQueue = NULL;

/************************************************************
 *
 * 						MAIN
 *
 * **********************************************************/
/* main */
void main(void)
{
	/* Before a queue is used it must be explicitly created. The queue is created
	to hold a maximum of 2 int pointers. */
	xBlinkQueue = xQueueCreate( 2, sizeof( int * ) );

	//prvInitTimer();



	if( xBlinkQueue != NULL )
	{
		/* Create two instances of the tasks that toggle LED1 and LED2.
		The LED number theyay toggle is passed into the task as the task’s
		parameter. The tasks are created at different priorities so some
		pre-emption will occur. */
		xTaskCreate( pubBlinkingLED, "BlinkLED2", 1000, ( void * ) 1, 1, NULL );
		xTaskCreate( pubBlinkingLED, "BlinkLED3", 1000, ( void * ) 2, 2, NULL );

		/* Create the gatekeeper task. This is the only task that is permitted
		to directly access registers. */
		xTaskCreate( prvStdioGatekeeperTask, "Gatekeeper", 1000, NULL, 0, NULL );

		/* Start the scheduler. */
		vTaskStartScheduler();
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
