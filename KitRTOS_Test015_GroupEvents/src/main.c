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
 *  Adaptation of Example 22
 *
 *  This example demonstrates how to:
 *  	- Create an event group.
 *  	- Set bits in an event group from an interrupt service
 *  	routine.
 *  	- Set bits in an event group from a task.
 *  	- Block on an event group.
 *
 *  The effect of the xEventGroupWaitBits() xWaitForAllBits
 * parameter is demonstrated by first executing the example
 * with xWaitForAllBits set to pdFALSE, and then executing
 * the example with xWaitForAllBits set to pdTRUE.
 *
 *  Event bit 0 and event bit 1 are set from a task. Event
 * bit 2 is set from an interrupt service routine.
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

/* Event Group handles */
EventGroupHandle_t xEventGroup;

/************************************************************
 *
 * 						MAIN
 *
 * **********************************************************/
/* main */
void main(void)
{
	/* Before an event group can be used it must first be created. */
	xEventGroup = xEventGroupCreate();

	/* Start int timer */
		prvInitTimer();

	/* Create the task that sets event bits in the event group. */
	xTaskCreate( vEventBitSettingTask, "Bit Setter", 1000, NULL, 1, NULL );
	/* Create the task that waits for event bits to get set in the event group. */
	xTaskCreate( vEventBitReadingTask, "Bit Reader", 1000, NULL, 2, NULL );

	/* Install the handler for the software interrupt. The syntax necessary to do
	this is dependent on the FreeRTOS port being used. The syntax shown here can
	only be used with the FreeRTOS Windows port, where such interrupts are only
	simulated. */
	//vPortSetInterruptHandler( mainINTERRUPT_NUMBER, ulEventBitSettingISR );
	/* Start the scheduler so the created tasks start executing. */
	vTaskStartScheduler();
	/* The following line should never be reached. */
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
