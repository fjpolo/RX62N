
/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
  Copyright (C) 2008. Renesas Technology Corp., All Rights Reserved.
*******************************************************************************
* File Name    : rsksh7216.h
* Version      : 1.00
* Description  : RSK 7216 board specific settings
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 06.10.2009 1.00    First Release 
*         : 23.08.2010 1.01    Updated for RSK+RX62N 
******************************************************************************/

#ifndef RSKRX62N_H
#define RSKRX62N_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/* System Clock Settings */
#define     INPUTCLK        (12000000)
#define     ICLK            (INPUTCLK * 8)
#define     PCLK            (INPUTCLK * 4)
#define     BCLK            (INPUTCLK * 2)

#define     CMT0_CLK_SELECT (512)

/* General Values */
#define     LED_ON          (0)
#define     LED_OFF         (1)
#define     SET_BIT_HIGH    (1)
#define     SET_BIT_LOW     (0)
#define     SET_BYTE_HIGH   (0xFF)
#define     SET_BYTE_LOW    (0x00)
#define     SET_OUTPUT      (1)
#define     SET_INPUT       (0)

/* Switches 1 to 3 */
#define     SW_ACTIVE       FALSE
#define     SW1             PORT0.DR.BIT.B0
#define     SW2             PORT0.DR.BIT.B1
#define     SW3             PORT0.DR.BIT.B7
#define     SW1_DDR         PORT0.DDR.BIT.B0
#define     SW2_DDR         PORT0.DDR.BIT.B1
#define     SW3_DDR         PORT0.DDR.BIT.B7
#define     SW1_ICR         PORT0.ICR.BIT.B0
#define     SW2_ICR         PORT0.ICR.BIT.B1
#define     SW3_ICR         PORT0.ICR.BIT.B7


/* LEDs 0 to 3*/
#define     LED0            PORT0.DR.BIT.B2
#define     LED1            PORT0.DR.BIT.B3
#define     LED2            PORT0.DR.BIT.B5
#define     LED3            PORT3.DR.BIT.B4
#define     LED0_DDR        PORT0.DDR.BIT.B2
#define     LED1_DDR        PORT0.DDR.BIT.B3
#define     LED2_DDR        PORT0.DDR.BIT.B5
#define     LED3_DDR        PORT3.DDR.BIT.B4

/* LCD */
#define     LCD_RS          PORT8.DR.BIT.B4
#define     LCD_RS_DDR      PORT8.DDR.BIT.B4
#define     LCD_EN          PORT8.DR.BIT.B5
#define     LCD_EN_DDR      PORT8.DDR.BIT.B5
#define     LCD_DATA        PORT9.DR.BYTE
#define     LCD_DATA_DDR    PORT9.DDR.BYTE

/*  Ethernet */
#define     ETH_RMII_MODE   (0)
#define     ETH_MII_MODE    (1)
#define     ETH_MODE_SEL    ETH_MII_MODE

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/



/* RSKRX62N_H */
#endif      

