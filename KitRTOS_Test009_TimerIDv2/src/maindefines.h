/*
 * maindefines.h
 *
 *  Created on: 10.07.2018
 *      Author: fpolo
 */

#ifndef MAINDEFINES_H_
#define MAINDEFINES_H_

/* Priorities at which the tasks are created. */
//#define configQUEUE_RECEIVE_TASK_PRIORITY	( tskIDLE_PRIORITY + 1 )
//#define	configQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	configBLINKY_TASK_PRIORITY			( tskIDLE_PRIORITY + 3 )

/* The rate at which data is sent to the queue, specified in milliseconds. */
#define mainQUEUE_SEND_FREQUENCY_MS			( 500 / portTICK_RATE_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added so the send task should always find the
queue empty. */
#define mainQUEUE_LENGTH					( 1 )

/* Define an enumerated type used to identify the source of the data. */
typedef enum
{
	eSender1,
	eSender2
} DataSource_t;
/* Define the structure type that will be passed on the queue. */
typedef struct
{
	uint8_t ucValue;
	DataSource_t eDataSource;
} Data_t;
/* Declare two variables of type Data_t that will be passed on the queue. */
static const Data_t xStructsToSend[ 2 ] =
{
	{ 100, eSender1 }, /* Used by Sender1. */
	{ 200, eSender2 } /* Used by Sender2. */
};


/* Timers */
/* The periods assigned to the one-shot and auto-reload timers are 3.333 second and half a second respectively. */
#define mainONE_SHOT_TIMER_PERIOD 			10000 / portTICK_RATE_MS
#define mainAUTO_RELOAD_TIMER_PERIOD 		333 / portTICK_RATE_MS
#define mainBLINK_AUTO_RELOAD_PERIOD		250 / portTICK_RATE_MS

#endif /* MAINDEFINES_H_ */
