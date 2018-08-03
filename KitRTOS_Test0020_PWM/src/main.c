/***********************************************************************/
/*                                                                     */
/*  FILE        : Main.c                        			           */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION : Main Program                                         */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE: THIS IS A FreeRTOS EXAMPLE.                                  */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"

/*
 * Testing FreeRTOS. Blinking LCD
 */

/************************************************************
 * Project warnings
 * **********************************************************/
#warning PLEASE CONFIGURE THE BOARD USING HARDWARE_INIT()

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
#include "queue.h",
#include "timers.h"

/* Hardware specific includes. */
#include "iodefine.h"

/* Demo includes. */
#include "led.h"
#include "partest.h"


/* Tasks */
#include "maintasks.h"
#include "maindefines.h"


/* CMT RPDL definitions */
#include <r_pdl_cmt.h>
/* MTU2 */
#include <r_pdl_mtu2.h>
/* General RPDL function definitions */
#include <r_pdl_definitions.h>

/* PWM */
#include "pwm.h"




/************************************************************
 * Project global variables
 * **********************************************************/

/* This variable is not used by this simple Blinky example.  It is defined
purely to allow the project to link as it is used by the full project. */
volatile unsigned long ulHighFrequencyTickCount = 0UL;

/* Variable to count idle Hook calls */
uint32_t ulIdleCycleCount = 0UL;

/* Timer handles */
xTimerHandle xLJULogoOneShotTimer;
xTimerHandle xCounterOneAutoReloadTimer;
xTimerHandle xCounterTwoAutoReloadTimer;
xTimerHandle xCounterThreeAutoReloadTimer;
xTimerHandle xSwitchReadAutoReloadTimer;

/* Counters */
uint32_t CounterOne = 0;
uint32_t CounterTwo = 0;
uint32_t CounterThree = 0;
uint32_t ulIdleCycleCount;

/* Switches */
uint8_t switchRead = 0x00;

/* SPI */
uint32_t wert_spi[1];

/* PWM */
uint8_t pwmDC = 0;

/************************************************************
 *
 * 						MAIN
 *
 * **********************************************************/
/* main */
void main(void)
{
	portBASE_TYPE xTimerLJULogoStarted, xTimerOneStarted, xTimerTwoStarted, xTimerThreeStarted, xSwitchReadStarted;
	int i = 0;

	/* PWM */
	pwm__create();

	/* Start Count */
	pwm__start();

	/* Set DC */
	pwm__set_DC( 23999 , 11999 );

	/* Create the one shot timer software timer, storing the handle in xLJULogoOneShotTimer. */
	xLJULogoOneShotTimer = xTimerCreate(
			"LJULogoOneTime",
			mainONE_SHOT_TIMER_PERIOD,
			pdFALSE,
			0,
			pubLJULogoON
	);
	/* Create the auto-reload timer, storing the handle to the created timer in xCounterOneAutoReloadTimer. */
	xCounterOneAutoReloadTimer = xTimerCreate(
			"CounterOneAutoReloadTimer",
			mainTIMER_ONE_PERIOD,
			pdTRUE,
			0,
			pubCounterOne
	);
	/* Create the auto-reload timer, storing the handle to the created timer in xCounterTwoAutoReloadTimer. */
	xCounterTwoAutoReloadTimer = xTimerCreate(
			"CounterTwoAutoReloadTimer",
			mainTIMER_TWO_PERIOD,
			pdTRUE,
			0,
			pubCounterTwo
	);
	/* Create the auto-reload timer, storing the handle to the created timer in xCounterTwoAutoReloadTimer. */
	xCounterThreeAutoReloadTimer = xTimerCreate(
			"CounterThreeAutoReloadTimer",
			mainTIMER_THREE_PERIOD,
			pdTRUE,
			0,
			pubCounterThree
	);
	/* Create the auto-reload timer, storing the handle to the created timer in xSwitchReadAutoReloadTimer. */
	xSwitchReadAutoReloadTimer = xTimerCreate(
			"SwitchReadAutoReloadTimer",
			mainSWITCH_READ_PERIOD,
			pdTRUE,
			1,
			pubReadSwitches
	);
	//
	if( ( xLJULogoOneShotTimer != NULL ) && ( xCounterOneAutoReloadTimer != NULL ) && ( xCounterTwoAutoReloadTimer != NULL ) && ( xCounterThreeAutoReloadTimer != NULL ) && ( xSwitchReadAutoReloadTimer != NULL ) ){
		/* Start the software timers, using a block time of 0 (no block time). The scheduler has
 			not been started yet so any block time specified here would be ignored anyway. */
		xTimerLJULogoStarted = xTimerStart( xLJULogoOneShotTimer, 0 );
		xTimerOneStarted = xTimerStart( xCounterOneAutoReloadTimer, 0 );
		xTimerTwoStarted = xTimerStart( xCounterTwoAutoReloadTimer, 0 );
		xTimerThreeStarted = xTimerStart( xCounterThreeAutoReloadTimer, 0 );
		xSwitchReadStarted = xTimerStart( xSwitchReadAutoReloadTimer, 0 );
		//
		if( ( xTimerLJULogoStarted == pdPASS ) && ( xTimerOneStarted == pdPASS ) && ( xTimerTwoStarted == pdPASS ) && ( xTimerThreeStarted == pdPASS ) && ( xSwitchReadStarted == pdPASS ) ){

			/* Start the tasks running. */
			vTaskStartScheduler();
		}
	}

	/* If all is well we will never reach here as the scheduler will now be
		running.  If we do reach here then it is likely that there was insufficient
		heap available for the idle task to be created. */
	for( ;; );
}// END OF MAIN


