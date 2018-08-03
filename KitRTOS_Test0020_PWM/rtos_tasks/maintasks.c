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
#include "led.h"

/* Defines used in MAIN  */
#include "maindefines.h"

/* FreeRTOS Tasks to be executed */
#include "maintasks.h"
/* string */
#include <string.h>

/* CMT RPDL definitions */
#include <r_pdl_cmt.h>
/* RPDL IO definitions */
#include <r_pdl_io_port.h>
/* General RPDL function definitions */
#include <r_pdl_definitions.h>

/* PWM */
#include "pwm.h"


/* Imported variables */
extern uint32_t CounterOne, CounterTwo, CounterThree;
extern uint8_t switchRead;
extern uint32_t ulIdleCycleCount;
extern uint8_t pwmDC;


//
// Begin: Private functions/tasks
//

/**/
//static void thisisaprivatefunction (void){}

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
}

/* Display LJU Logo */
void pubLJULogoON( void *pvParameters ){
}

/* Clear LJU Logo */
void pubLJULogoOFF( void *pvParameters ){
}

/* CounterOne */
void pubCounterOne( void *pvParameters ){
	uint16_t pwmDCvalue;
	//
	if ( pwmDC == 100  ) pwmDC = 0;
	else pwmDC++;
	pwmDCvalue = pwm__percentage_to_value( pwmDC );
	pwm__set_DC( pwmDCvalue, pwmDCvalue  );
}

/* CounterTwo */
void pubCounterTwo( void *pvParameters ){
}

/* CounterThree */
void pubCounterThree( void *pvParameters ){
}

/* Read Switches */
void pubReadSwitches( void *pvParameters ){
}

//
// End: Public tasks
//







