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
void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName );
void vApplicationIdleHook( void );

void pubBlink( void *pvParameters );
void pubTimerCallback( xTimerHandle xTimer );
void vHandlerTask( void *pvParameters );

#endif /* MAINTASKS_H_ */
