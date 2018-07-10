/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Technology Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Technology Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* TECHNOLOGY CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2008. Renesas Technology Corp., All Rights Reserved.
******************************************************************************* 
* File Name    : hwsetup.c
* Version      : 1.00
* Description  : Power up hardware initializations
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 15.02.2010 1.00    First Release 
*         : 23.08.2010 1.01    Updated for RSK+RX62N 
*****************************************************************************/ 


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include "iodefine.h"
#include "rskrx62n.h"
#include "hwsetup.h"
#include "lcd.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
void io_set_cpg(void);
void ConfigurePortPins(void);
void EnablePeripheralModules(void);

/******************************************************************************
* Function Name: HardwareSetup
* Description  : This function does initial setting for CPG port pins used in
*              : the Demo including the MII pins of the Ethernet PHY connection.
* Arguments    : none
* Return Value : none
******************************************************************************/
void HardwareSetup(void)
{
    /* CPG setting */
    io_set_cpg();

    /* Setup the port pins */
    ConfigurePortPins();

    /* Enables peripherals */
    EnablePeripheralModules();

    /* Initialize display */
    InitialiseDisplay();
}

/******************************************************************************
* Function Name: EnablePeripheralModules
* Description  : Enables Peripheral Modules before use
* Arguments    : none
* Return Value : none
******************************************************************************/
void EnablePeripheralModules(void)
{
    /*  Module standby clear */
    SYSTEM.MSTPCRB.BIT.MSTPB15 = 0;             /* EtherC, EDMAC */
    SYSTEM.MSTPCRA.BIT.MSTPA15 = 0;             /* CMT0 */
}

/******************************************************************************
* Function Name: ConfigurePortPins
* Description  : Configures port pins.
* Arguments    : none
* Return Value : none
******************************************************************************/
void ConfigurePortPins(void)
{
/* Configure Ethernet pins */
#if ETH_MODE_SEL == ETH_MII_MODE
    /*  EE=1, PHYMODE=1, ENETE3=1, ENETE2=0, ENETE1=1, ENETE0=0 (Ethernet)  */
    IOPORT.PFENET.BYTE = 0x9A;
    /*  P54=1 Set ET_LINKSTA input  */
    PORT5.ICR.BIT.B4 = 1;
    /*  P71=1 Set ET_MDIO input     */
    PORT7.ICR.BIT.B1 = 1;
    /*  P74=1 Set ET_ERXD1 input    */
    PORT7.ICR.BIT.B4 = 1;
    /*  P75=1 Set ET_ERXD0 input    */
    PORT7.ICR.BIT.B5 = 1;
    /*  P76=1 Set ET_RX_CLK input   */
    PORT7.ICR.BIT.B6 = 1;
    /*  P77=1 Set ET_RX_ER input    */
    PORT7.ICR.BIT.B7 = 1;
    /*  P83=1 Set ET_CRS input      */
    PORT8.ICR.BIT.B3 = 1;
    /*  PC0=1 Set ET_ERXD3 input    */
    PORTC.ICR.BIT.B0 = 1;
    /*  PC1=1 Set ET_ERXD2 input    */
    PORTC.ICR.BIT.B1 = 1;
    /*  PC2=1 Set ET_RX_DV input    */
    PORTC.ICR.BIT.B2 = 1;
    /*  PC4=1 Set EX_TX_CLK input   */
    PORTC.ICR.BIT.B4 = 1;
    /*  PC7=1 Set ET_COL input      */
    PORTC.ICR.BIT.B7 = 1;
#endif  
#if ETH_MODE_SEL == ETH_RMII_MODE
    /*  EE=1, PHYMODE=0, ENETE3=0, ENETE2=0, ENETE1=1, ENETE0=0 (Ethernet)  */
    IOPORT.PFENET.BYTE = 0x82;
    /*  P54=1 Set ET_LINKSTA input  */
    PORT5.ICR.BIT.B4 = 1;
    /*  P71=1 Set ET_MDIO input     */
    PORT7.ICR.BIT.B1 = 1;
    /* P74=1 Set RMII_RXD1 input    */
    PORT7.ICR.BIT.B4 = 1;
    /* P75=1 Set RMII_RXD0 input    */
    PORT7.ICR.BIT.B5 = 1;
    /* P76=1 Set REF50CLK input     */
    PORT7.ICR.BIT.B6 = 1;
    /* P77=1 Set RMII_RX_ER input   */
    PORT7.ICR.BIT.B7 = 1;
    /* P83=1 Set RMII_CRS_DV input  */
    PORT8.ICR.BIT.B3 = 1;
#endif  

    /* Configure LED 0-3 pins */
    LED0 = LED_OFF;
    LED1 = LED_OFF;    
    LED2 = LED_OFF;    
    LED3 = LED_OFF;    
    LED0_DDR = SET_OUTPUT;
    LED1_DDR = SET_OUTPUT;
    LED2_DDR = SET_OUTPUT;
    LED3_DDR = SET_OUTPUT;

    /* Configure SW 1-3 pins */
    SW1_DDR = SET_INPUT;
    SW2_DDR = SET_INPUT;
    SW3_DDR = SET_INPUT;
    SW1_ICR = 1;
    SW2_ICR = 1;
    SW3_ICR = 1;

    /* Configure LCD pins */
    LCD_RS_DDR = SET_OUTPUT;
    LCD_EN_DDR = SET_OUTPUT;
    LCD_DATA_DDR = 0xF0;
}

/******************************************************************************
* Function Name: io_set_cpg
* Description  : Sets up operating speed
* Arguments    : none
* Return Value : none
******************************************************************************/
void io_set_cpg(void)
{
/* Set CPU PLL operating frequencies. Changes to the peripheral clock will require
changes to the debugger and flash kernel BRR settings. */

    /* ==== CPG setting ==== */
    SYSTEM.SCKCR.LONG = 0x00020100; /* Clockin = 12MHz */
                                    /* I Clock = 96MHz, B Clock = 24MHz, */
                                    /* P Clock = 48MHz */

}

