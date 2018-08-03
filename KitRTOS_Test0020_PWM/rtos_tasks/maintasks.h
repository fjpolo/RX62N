/*
 * maintasks.h
 *
 *  Created on: 12.07.2018
 *      Author: fpolo
 */

#ifndef MAINTASKS_H_
#define MAINTASKS_H_

// Public tasks
void vApplicationSetupTimerInterrupt( void );
void vApplicationMallocFailedHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationIdleHook( void );

/**/
void pubBlink( void *pvParameters );
/* Display LJU Logo */
void pubLJULogoON( void *pvParameters );
/* Clear LJU Logo */
void pubLJULogoOFF( void *pvParameters );
/* CounterOne */
void pubCounterOne( void *pvParameters );
/* CounterTwo */
void pubCounterTwo( void *pvParameters );
/* CounterThree */
void pubCounterThree( void *pvParameters );
/* Read Switches */
void pubReadSwitches( void *pvParameters );

#endif /* MAINTASKS_H_ */
