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
 *  The two tasks created in Test0034 are periodic tasks, but
 * using vTaskDelay() does not guarantee that the frequency at
 * which they run is fixed, as the time at which the tasks leave
 * the Blocked state is relative to when they call vTaskDelay().
 * Converting the tasks to use vTaskDelayUntil() instead of
 * vTaskDelay() solves this potential problem.
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

/*-----------------------------------------------------------*/

/************************************************************
 * Project tasks
 * **********************************************************/
static void prvBlink( void *pvParameters );
static void prvPeriodicBlink( void *pvParameters );



/************************************************************
 *
 * 						MAIN
 *
 * **********************************************************/
/* main */
void main(void)
{
	/* Create LED tasks */
	xTaskCreate( prvBlink, "Blink LED0", configMINIMAL_STACK_SIZE, (void *)0, configBLINKY_TASK_PRIORITY, NULL );
	xTaskCreate( prvBlink, "Blink LED1", configMINIMAL_STACK_SIZE, (void *)1, configBLINKY_TASK_PRIORITY, NULL );
	// Blocking tasks using absolute delay
	xTaskCreate( prvPeriodicBlink, "Blink LED2", configMINIMAL_STACK_SIZE, (void *)2, configBLINKY_TASK_PRIORITY, NULL );
	xTaskCreate( prvPeriodicBlink, "Blink LED3", configMINIMAL_STACK_SIZE, (void *)3, configBLINKY_TASK_PRIORITY, NULL );

	/* Start the tasks running. */
	vTaskStartScheduler();

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
}
/*-----------------------------------------------------------*/
