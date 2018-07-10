/*********************************************************************************                                                                                   *
FILE NAME  	:	rskrx600def.h
DESCRIPTION	:	Defines for RSKRX600

Copyright   : 	2010 Renesas Electronics Corporation.
All Rights Reserved
***********************************************************************************/

#ifndef RSKRX600DEF_H_INCLUDED
#define RSKRX600DEF_H_INCLUDED

#include <machine.h>
#include <builtin.h>

/***********************************************************************************
Defines
***********************************************************************************/
/* Peripheral Clock Speed */
#define	f1_CLK_SPEED	(50000000)
#define XIN_FREQ 		(12500000)

/* Switches */
#define	SW1 			PORT0.PORT.BIT.B0
#define SW2 			PORT0.PORT.BIT.B1
#define SW3 			PORT0.PORT.BIT.B7

#define SW1_DDR			PORT0.DDR.BIT.B0
#define SW2_DDR			PORT0.DDR.BIT.B1
#define SW3_DDR			PORT0.DDR.BIT.B7

/* LEDs */
#define	GRN_LED0			PORT0.DR.BIT.B2
#define	YLW_LED1			PORT0.DR.BIT.B3
#define	ORG_LED2			PORT0.DR.BIT.B5
#define	RED_LED3			PORT3.DR.BIT.B4
#define	RED_LED4			PORT6.DR.BIT.B0
#define	RED_LED5			PORT7.DR.BIT.B3

#define	GRN_LED0_DDR		PORT0.DDR.BIT.B2
#define	YLW_LED1_DDR		PORT0.DDR.BIT.B3
#define	ORG_LED2_DDR		PORT0.DDR.BIT.B5
#define	RED_LED3_DDR		PORT3.DDR.BIT.B4
#define	RED_LED4_DDR		PORT6.DDR.BIT.B0
#define	RED_LED5_DDR		PORT7.DDR.BIT.B3

#define LED_ON      	0
#define LED_OFF     	1
#define ALL_LEDS_ON		{GRN_LED0 = YLW_LED1 = ORG_LED2 = RED_LED3 = RED_LED4 = RED_LED5 = LED_ON;}
#define ALL_LEDS_OFF	{GRN_LED0 = YLW_LED1 = ORG_LED2 = RED_LED3 = RED_LED4 = RED_LED5 = LED_OFF;}

/* Common Defines */
#ifndef TRUE
#define TRUE    		1
#endif
#ifndef FALSE
#define FALSE   		0
#endif

//#define ENABLE_IRQ   	{ SETPSW I }
/*#define ENABLE_IRQ   	{ uint32_t psw = _builtin_get_psw(); \
						 _builtin_set_psw(psw | 0x00010000); \
						} 
Use ", enable", as in 
#pragma interrupt CAN0_TXM0_ISR(vect=VECT_CAN0_TXM0, enable) */
#define DISABLE_IRQ		{ CLRPSW I }

#define ENABLE_LEDS		{GRN_LED0_DDR = YLW_LED1_DDR = ORG_LED2_DDR = RED_LED3_DDR = RED_LED4_DDR = RED_LED5_DDR = 1;}

#endif 		/* RSKRX600DEF_H_INCLUDED */
