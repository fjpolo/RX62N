/*
 * maindefines.h
 *
 *  Created on: 10.07.2018
 *      Author: fpolo
 */

#ifndef MAINDEFINES_H_
#define MAINDEFINES_H_

/* Priorities at which the tasks are created. */
#define	configBLINKY_TASK_PRIORITY			( tskIDLE_PRIORITY + 3 )
#define	configLJULogo_TASK_PRIORITY			( tskIDLE_PRIORITY + 4 )

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
#define mainONE_SHOT_TIMER_PERIOD 			pdMS_TO_TICKS(100)
#define mainONE_SHOT_CLEAR_TIMER_PERIOD 	pdMS_TO_TICKS(1000)
#define mainTIMER_ONE_PERIOD				pdMS_TO_TICKS(50)
#define mainTIMER_TWO_PERIOD				pdMS_TO_TICKS(500)
#define mainTIMER_THREE_PERIOD				pdMS_TO_TICKS(5000)
#define mainSWITCH_READ_PERIOD				pdMS_TO_TICKS(25)

#endif /* MAINDEFINES_H_ */
