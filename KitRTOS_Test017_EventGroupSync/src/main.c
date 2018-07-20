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
 *  Adaptation of Example 23
 *
 *  This example uses xEventGroupSync() to synchronize three
 * instances of a single task implementation. The task parameter
 * is used to pass into each instance the event bit the task
 * will set when it calls xEventGroupSync().
 *
 *  The task toggles a LED before calling xEventGroupSync(),
 * and again after the call to xEventGroupSync() has returned.
 * This allows the sequence of execution to be observed in the
 * output produced. A pseudo random delay is used to prevent
 * all the tasks reaching the synchronization point at the same
 * time.
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

/************************************************************
 *
 * 						MAIN
 *
 * **********************************************************/
/* main */
void main(void){
	/* Before an event group can be used it must first be created. */
	xEventGroup = xEventGroupCreate();
	/* Create three instances of the task. Each task is given a different name,
	which is later printed out to give a visual indication of which task is
	executing. The event bit to use when the task reaches its synchronization point
	is passed into the task using the task parameter. */
	xTaskCreate( vSyncingTask, "Task 1", 1000, mainFIRST_TASK_BIT, 1, NULL );
	xTaskCreate( vSyncingTask, "Task 2", 1000, mainSECOND_TASK_BIT, 1, NULL );
	xTaskCreate( vSyncingTask, "Task 3", 1000, mainTHIRD_TASK_BIT, 1, NULL );
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
