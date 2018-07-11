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
 *  This example demonstrates the steps needed to create two simple
 * tasks, then start the tasks executing. The tasks simply blinks
 * an LED, using a crude null loop to create the period delay. Both
 * tasks are created at the same priority, and are identical except
 * for the LED they blink.
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
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );
static void prvBlink0( void *pvParameters );
static void prvBlink1( void *pvParameters );
static void prvBlink2( void *pvParameters );
static void prvBlink3( void *pvParameters );




/************************************************************
 *
 * 						MAIN
 *
 * **********************************************************/
/* main */
void main(void)
{
	/* Create LED tasks */
	xTaskCreate( prvBlink0, "Blink LED0", configMINIMAL_STACK_SIZE, NULL, configBLINKY_TASK_PRIORITY, NULL );
	xTaskCreate( prvBlink1, "Blink LED1", configMINIMAL_STACK_SIZE, NULL, configBLINKY_TASK_PRIORITY, NULL );
	//xTaskCreate( prvBlink2, "Blink LED2", configMINIMAL_STACK_SIZE, NULL, configBLINKY_TASK_PRIORITY, NULL );
	//xTaskCreate( prvBlink3, "Blink LED3", configMINIMAL_STACK_SIZE, NULL, configBLINKY_TASK_PRIORITY, NULL );

	/* Start the tasks running. */
	vTaskStartScheduler();

	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
}// END OF MAIN

/*-----------------------------------------------------------*/

static void prvBlink0( void *pvParameters ){

	volatile uint32_t u1;
	//
	for( ;; ){		//
		// Blink LED here
		PORT0.DR.BIT.B2 = !PORT0.DR.BIT.B2;
		// Delay
		for(u1 = 0; u1< 1000000; u1++);
	}
}

/*-----------------------------------------------------------*/

static void prvBlink1( void *pvParameters ){

	volatile uint32_t u1;
	//
	for( ;; ){		//
		// Blink LED here
		PORT0.DR.BIT.B3 = !PORT0.DR.BIT.B3;
		// Delay
		for(u1 = 0; u1< 1000000; u1++);
	}
}

/*-----------------------------------------------------------*/

static void prvBlink2( void *pvParameters ){

	volatile uint32_t u1;
	//
	for( ;; ){		//
		// Blink LED here
		PORT0.DR.BIT.B5 = !PORT0.DR.BIT.B5;
		// Delay
		for(u1 = 0; u1< 1000000; u1++);
	}
}

/*-----------------------------------------------------------*/

static void prvBlink3( void *pvParameters ){

	volatile uint32_t u1;
	//
	for( ;; ){		//
		// Blink LED here
		PORT3.DR.BIT.B4 = !PORT3.DR.BIT.B4;
		// Delay
		for(u1 = 0; u1< 1000000; u1++);
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
