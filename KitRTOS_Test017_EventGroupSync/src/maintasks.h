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
void pubBlink_blocking( void *pvParameters );
void pubBlinkingLED ( void *pvParameters );
void pubTimerCallback( xTimerHandle xTimer );
void vHandlerTask( void *pvParameters );
void vDeferredHandlingFunction( void *pvParameter1, uint32_t ulParameter2 );
void vEventBitSettingTask( void *pvParameters );
void vEventBitReadingTask( void *pvParameters );
void vSyncingTask( void *pvParameters );

#endif /* MAINTASKS_H_ */
