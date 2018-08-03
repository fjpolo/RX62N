/*
 * pwm.c
 *
 *  Created on: 03.08.2018
 *      Author: FPolo
 */

/* Standard C */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* CMT RPDL definitions */
#include <r_pdl_cmt.h>
/* MTU2 */
#include <r_pdl_mtu2.h>
/* General RPDL function definitions */
#include <r_pdl_definitions.h>

/* PWM */
#include "pwm.h"

/************************************************************
 * Library functions
 * **********************************************************/

//
// Begin: Private funtions
//

//
// End: Private functions
//
/******************************************************************************/
//
// Begin: Public funtions
//

/* Creation */
uint8_t pwm__create( void ){
	uint8_t pwmHandler = PWM_HANDLER_OK;
	R_MTU2_Create_structure parameters;
	//
	R_MTU2_Create_load_defaults( &parameters );
	//
	parameters.data2 = PDL_MTU2_MODE_PWM1 | PDL_MTU2_SYNC_DISABLE;
	parameters.data3 = PDL_MTU2_CLK_PCLK_DIV_1 | PDL_MTU2_CLK_RISING | PDL_MTU2_CLEAR_TGRC;
	parameters.data4 = PDL_MTU2_ADC_TRIG_TGRA_DISABLE;
	parameters.data5 = PDL_MTU2_BUFFER_AC_DISABLE | PDL_MTU2_BUFFER_BD_DISABLE;
	parameters.data6 = PDL_MTU2_A_OC_HIGH | PDL_MTU2_B_OC_LOW;
	parameters.data7 = PDL_MTU2_C_OC_HIGH | PDL_MTU2_D_OC_LOW;
	parameters.data9 = 0;
	parameters.data10 = 47999;
	parameters.data11 = 23999;
	parameters.data12 = 47999;
	parameters.data13 = 23999;
	parameters.data14 = 0;
	parameters.data15 = 0;
	parameters.data16 = 0;
	parameters.data17 = 0;
	parameters.data18 = 0;
	parameters.func1 = PDL_NO_FUNC;
	parameters.func2 = PDL_NO_FUNC;
	parameters.func3 = PDL_NO_FUNC;
	parameters.func4 = PDL_NO_FUNC;
	parameters.data19 = 0;
	parameters.func5 = PDL_NO_FUNC;
	parameters.func6 = PDL_NO_FUNC;
	parameters.func7 = PDL_NO_FUNC;
	parameters.func8 = PDL_NO_FUNC;
	parameters.data20 = 0;
	//
	R_MTU2_Create( 3, &parameters );
	//
	//R_MTU2_Set( PDL_MTU2_PIN_3C_A );
	R_MTU2_Set( PDL_MTU2_PIN_3C_B );
	//
	return pwmHandler;
}

/* Start */
uint8_t pwm__start( void ){
	uint8_t pwmHandler = PWM_HANDLER_OK;
	R_MTU2_ControlChannel_structure parameters;
	//
	parameters.data2 = PDL_MTU2_START;
	parameters.data3 = PDL_NO_DATA;
	parameters.data4 = PDL_NO_DATA;
	parameters.data5 = PDL_NO_DATA;
	parameters.data6 = PDL_NO_DATA ;
	parameters.data7 = PDL_NO_DATA;
	parameters.data8 = PDL_NO_DATA;
	parameters.data9 = PDL_NO_DATA;
	parameters.data10 = PDL_NO_DATA;
	parameters.data11 = PDL_NO_DATA;
	parameters.data12 = PDL_NO_DATA;
	//
	R_MTU2_ControlChannel( 3, &parameters );
	//
	return pwmHandler;
}

/* Set Duty Cycle */
uint8_t pwm__set_DC( uint16_t counter_val1, uint16_t counter_val2  ){
	uint8_t pwmHandler = PWM_HANDLER_OK;
	R_MTU2_ControlChannel_structure parameters;
	//
	parameters.data2 = PDL_NO_DATA;
	parameters.data3 = PDL_MTU2_REGISTER_TGRB | PDL_MTU2_REGISTER_TGRD;
	parameters.data4 = PDL_NO_DATA;
	parameters.data5 = PDL_NO_DATA;
	parameters.data6 = counter_val1;
	parameters.data7 = PDL_NO_DATA;
	parameters.data8 = counter_val2;
	parameters.data9 = PDL_NO_DATA;
	parameters.data10 = PDL_NO_DATA;
	parameters.data11 = PDL_NO_DATA;
	parameters.data12 = PDL_NO_DATA;

	return R_MTU2_ControlChannel( 3, &parameters );
	//
	return pwmHandler;
}

/* Percentage to value */
uint16_t pwm__percentage_to_value( uint8_t percentage_val ){
	uint16_t value;
	//
	value = (uint16_t) (percentage_val * 47999/100);
	//
	return value;
}

//
// End: Public functions
//
