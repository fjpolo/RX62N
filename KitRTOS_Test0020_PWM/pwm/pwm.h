/*
 * pwm.h
 *
 *  Created on: 03.08.2018
 *      Author: FPolo
 */

#ifndef PWM_H_
#define PWM_H_

/*** Defines ***/
#define PWM_HANDLER_OK			0
#define PWM_HANDLER_NOK			1



/*** Functions ***/


/* Creation */
uint8_t pwm__create( void );

/* Start */
uint8_t pwm__start( void );

/* Set Duty Cycle */
uint8_t pwm__set_DC( uint16_t counter_val1, uint16_t counter_val2  );

/* Percentage to value */
uint16_t pwm__percentage_to_value( uint8_t percentage_val );

#endif /* PWM_H_ */
