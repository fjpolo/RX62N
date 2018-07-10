/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
  Copyright (C) 2017. Renesas Electronics Corp., All Rights Reserved.
*******************************************************************************
* File Name    : config_r_can_rapi.h
* Version      : 2.05
* Description  : Edit this file to configure the CAN API.
******************************************************************************
* History       Ver.	Description
* Mar 22 '10	1.00	For RX62N with new CAN API.
* Dec 15 2011	2.00   	Updated to support CAN extended ID mode.
* Dec 08 2017   2.05    Removed unsupported FIFO mode option
*                       Moved port configuration here out of r_can_api.c
******************************************************************************/
#ifndef R_CAN_API_CFG_H
#define R_CAN_API_CFG_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Macro definitions
******************************************************************************/
/* Include this line if you want to poll can mailboxes for messages received 
and sent. COMMENT to use the CAN interrupts. */
//#define USE_CAN_POLL			1

#define USE_FIT_BSP (0) // 1 for FIT BSP included in project, 0 for no BSP

/* Select MCU to be used. */
//#define CAN_MCU_RX62G
//#define CAN_MCU_RX62T
#define CAN_MCU_RX62N
//#define CAN_MCU_RX621

/*** CAN interrupt ************************************************************/
/* Level */
#define CAN0_INT_LVL			2

/* Frame ID mode: Standard ID, Extended ID, or Mixed ID mode */
/* Must choose 1 and only 1 from: */
#define FRAME_ID_MODE   STD_ID_MODE
//#define FRAME_ID_MODE   EXT_ID_MODE
//#define FRAME_ID_MODE   MIXED_ID_MODE
 

/*** Board specific ports ******************************************************
Tell me where you want to map the transceiver control pins.
Some transceivers may have other control pins. You would have to configure this
yourself. */
/* define CTX0 port pin. */
#define CTX0_PORT_PIN (PORT3.DDR.BIT.B2)
/* define CTX0 port pin. */
#define CRX0_PORT_PIN (PORT3.DDR.BIT.B3)
/* define Input Buffer Control Register for CRX0 port */
#define CRX0_PORT_INP_CTRL (PORT3.ICR.BIT.B3)

/* Configure CAN0 STBn pin. For the RDK this is the P41 pin.
Output. High = not standby. */
#define CAN0_TRX_STB_PORT 	  	4
#define CAN0_TRX_STB_PIN		1
#define CAN0_TRX_STB_LVL		1   //High = Not standby.

/* Configure CAN0 EN pin. For the RDK this is the P42 pin.
Output. High to enable CAN transceiver. */
#define CAN0_TRX_ENABLE_PORT    4
#define CAN0_TRX_ENABLE_PIN     2
#define CAN0_TRX_ENABLE_LVL     1   //High = Enable.

/*** Baudrate settings ********************************************************
	Calculation of baudrate:
	*********************************
	*	PCLK = 48 MHz = fcan.		*
	*	fcanclk = fcan/prescale		*
	*********************************
	
	Example 1) 
	Desired baudrate 500 kbps.
	Selecting prescale to 4.
    fcanclk = 48000000/4
	fcanclk = 12000000 Hz
	Bitrate = fcanclk/Tqtot
	 or,
	Tqtot = fcanclk/bitrate
	Tqtot = 12000000/500000
	Tqtot = 120/5 = 24.
    Tqtot = TSEG1 + TSEG2 + SS
	Using TSEG1 = 15 Tq
		  TSEG2 = 8 Tq
		  SS = 1 Tq always
		  Re-synchronization Control (SJW) should be 1-4 Tq (must be <=TSEG2). */
	#define CAN_BRP		4
	#define CAN_TSEG1	15
	#define CAN_TSEG2	8
	#define CAN_SJW		2
	
	/* 
	Example 2) Selecting prescale to 8.
    Desired baudrate 500 kbps.
	fcanclk = 48000000/8
	fcanclk = 6000000 Hz
	Tqtot = fcanclk/bitrate
	Tqtot = 6000000/500000
	Tqtot = 60/5 = 12.
	Tqtot = TSEG1 + TSEG2 + SS
	Using 	TSEG1 = 8 Tq
			TSEG2 = 3 Tq
			SS = 1 Tq always
			SJW should be 1-4 Tq (<=TSEG2). *
	#define CAN_BRP		8
	#define CAN_TSEG1	8
	#define CAN_TSEG2	3
	#define CAN_SJW		1	*/
	
/*** Other settings  *********************************************************/
/* Max time to poll a CAN register bit for expected value. Don't set to zero. */
#define MAX_CAN_REG_POLLTIME	4

/******************************************************************************
Constant definitions
*****************************************************************************/
/* define port function control register J (PFJCAN) depending on target MCU*/
/* Do not edit this section */
#if defined CAN_MCU_RX62N
#define PFJCAN_REG (IOPORT.PFJCAN.BIT.CAN0E)
#elif defined CAN_MCU_RX621
#define PFJCAN_REG (IOPORT.PFJCAN.BIT.CAN0E)
#elif defined CAN_MCU_RX62G
#define PFJCAN_REG (IOPORT.PFJCAN.BIT.CANE)
#elif defined CAN_MCU_RX62T
#define PFJCAN_REG (IOPORT.PFJCAN.BIT.CANE)
#else
#error  "Error - Must define a target MCU."
#endif

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Function prototypes
******************************************************************************/
#endif	/* R_CAN_API_CFG_H */
/* eof */
