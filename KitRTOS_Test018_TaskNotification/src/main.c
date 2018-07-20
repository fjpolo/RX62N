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
 *  Adaptation of Example 24
 *
 *  _Interrupt used a binary semaphore to unblock a task from
 * within an interrupt service routine—effectively synchronizing
 * the task with the interrupt. This example replicates the
 * functionality of Example 16, but uses a direct to task
 * notification in place of the binary semaphore.
 *
 *  The ulTaskNotifyTake() xClearCountOnExit parameter is set to
 * pdTRUE, which results in the receiving task’s notification
 * value being cleared to zero before ulTaskNotifyTake() returns.
 * It is therefore necessary to process all the events that are
 * already available between each call to ulTaskNotifyTake().
 * In _Interrupt, because a binary semaphore was used, the number
 * of pending events had to be determined from the hardware, which
 * is not always practical. In this example, the number of pending
 * events is returned from ulTaskNotifyTake().
 *
 *  Interrupt events that occur between calls to ulTaskNotifyTake
 * are latched in the task’s notification value, and calls to
 * ulTaskNotifyTake() will return immediately if the calling task
 * already has notifications pending.
 *
 *  The periodic task used to generate software interrupts toggles
 * LED1 ehen the interrupt is generated. This allows the sequence
 * of execution to be observed in the output produced.
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

/* Declare the event group used to synchronize the three tasks. */
EventGroupHandle_t xEventGroup;

/* Task handles */
TaskHandle_t xHandlerTask;

/************************************************************
 *
 * 						MAIN
 *
 * **********************************************************/
/* main */
void main(void){

	/* init timer interrupt */
	prvInitTimer();

	/* Create the 'handler' task, which is the task to which interrupt
	processing is deferred. This is the task that will be synchronized with
	the interrupt. The handler task is created with a high priority to ensure
	it runs immediately after the interrupt exits. In this case a priority of
	3 is chosen. */
	xTaskCreate( vHandlerTask, "INTHandler", 1000, NULL, 3, &xHandlerTask );

	/* Start the scheduler so the created tasks start executing. */
	vTaskStartScheduler();
	/* As always, the following line should never be reached. */
	for( ;; );
	return 0;
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
