/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_ether_setting_rx.c
* Version      : 
* Device       : 
* Description  : Ethernet module device driver
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : --.--.---- ----     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"

#include "r_ether.h"
#include "r_ether_local.h"

/***********************************************************************************************************************
* Function Name: ether_port_enable
* Description  :
* Arguments    : channel -
*                    Ethernet channel number
* Return Value : none
***********************************************************************************************************************/
void ether_port_enable(uint8_t connect)
{
	/*  Module standby clear */
	SYSTEM.MSTPCRB.BIT.MSTPB15 = 0;				/* EtherC, EDMAC */

/* Port pins default to inputs. To ensure safe initialisation set the pin states
before changing the data direction registers. This will avoid any unintentional
state changes on the external ports.
Many peripheral modules will override the setting of the port registers. Ensure
that the state is safe for external devices if the internal peripheral module is
disabled or powered down. */

	/* ==== MII/RMII Pins setting ==== */
	/*--------------------------------------*/
	/*    Port Function Control Register    */
	/*--------------------------------------*/
#if ETH_MODE_SEL == ETH_MII_MODE
	/*	EE=1, PHYMODE=1, ENETE3=1, ENETE2=0, ENETE1=1, ENETE0=0 (Ethernet)	*/
	IOPORT.PFENET.BYTE = 0x9A;
#endif	/*	ETH_MODE_SEL	*/
#if ETH_MODE_SEL == ETH_RMII_MODE
	/*	EE=1, PHYMODE=0, ENETE3=0, ENETE2=0, ENETE1=1, ENETE0=0 (Ethernet)	*/
	IOPORT.PFENET.BYTE = 0x82;
#endif	/*	ETH_MODE_SEL	*/
	/*-------------------------------------------*/
	/*    Input Buffer Control Register (ICR)    */
	/*-------------------------------------------*/
#if (BSP_CFG_MCU_PART_PACKAGE == 0)
#if ETH_MODE_SEL == ETH_MII_MODE
	/*	P54=1 Set ET_LINKSTA input	*/
	PORT5.ICR.BIT.B4 = 1;
	/*	P71=1 Set ET_MDIO input	*/
	PORT7.ICR.BIT.B1 = 1;
	/*	P74=1 Set ET_ERXD1 input	*/
	PORT7.ICR.BIT.B4 = 1;
	/*	P75=1 Set ET_ERXD0 input	*/
	PORT7.ICR.BIT.B5 = 1;
	/*	P76=1 Set ET_RX_CLK input	*/
	PORT7.ICR.BIT.B6 = 1;
	/*	P77=1 Set ET_RX_ER input	*/
	PORT7.ICR.BIT.B7 = 1;
	/*	P83=1 Set ET_CRS input	*/
	PORT8.ICR.BIT.B3 = 1;
	/*	PC0=1 Set ET_ERXD3 input	*/
	PORTC.ICR.BIT.B0 = 1;
	/*	PC1=1 Set ET_ERXD2 input	*/
	PORTC.ICR.BIT.B1 = 1;
	/*	PC2=1 Set ET_RX_DV input	*/
	PORTC.ICR.BIT.B2 = 1;
	/*	PC4=1 Set EX_TX_CLK input	*/
	PORTC.ICR.BIT.B4 = 1;
	/*	PC7=1 Set ET_COL input	*/
	PORTC.ICR.BIT.B7 = 1;
#endif	/*	ETH_MODE_SEL	*/
#if ETH_MODE_SEL == ETH_RMII_MODE
	/*	P54=1 Set ET_LINKSTA input	*/
	PORT5.ICR.BIT.B4 = 1;
	/*	P71=1 Set ET_MDIO input	*/
	PORT7.ICR.BIT.B1 = 1;
	/* P74=1 Set RMII_RXD1 input	*/
	PORT7.ICR.BIT.B4 = 1;
	/* P75=1 Set RMII_RXD0 input	*/
	PORT7.ICR.BIT.B5 = 1;
	/* P76=1 Set REF50CLK input	*/
	PORT7.ICR.BIT.B6 = 1;
	/* P77=1 Set RMII_RX_ER input	*/
	PORT7.ICR.BIT.B7 = 1;
	/* P83=1 Set RMII_CRS_DV input	*/
	PORT8.ICR.BIT.B3 = 1;
#endif	/*	ETH_MODE_SEL	*/
#endif	/*	LFBGA176	*/

#if (BSP_CFG_MCU_PART_PACKAGE == 0x2)
#if ETH_MODE_SEL == ETH_MII_MODE
	/*	P54=1 Set ET_LINKSTA input	*/
	PORT5.ICR.BIT.B4 = 1;	
	/*	P71=1 Set ET_MDIO input	*/
	PORT7.ICR.BIT.B1 = 1;
	/*	P74=1 Set ET_ERXD1 input	*/
	PORT7.ICR.BIT.B4 = 1;
	/*	P75=1 Set ET_ERXD0 input	*/
	PORT7.ICR.BIT.B5 = 1;
	/*	P76=1 Set ET_RX_CLK input	*/
	PORT7.ICR.BIT.B6 = 1;
	/*	P77=1 Set ET_RX_ER input	*/
	PORT7.ICR.BIT.B7 = 1;
	/*	P83=1 Set ET_CRS input	*/
	PORT8.ICR.BIT.B3 = 1;
	/*	PC0=1 Set ET_ERXD3 input	*/
	PORTC.ICR.BIT.B0 = 1;
	/*	PC1=1 Set ET_ERXD2 input	*/
	PORTC.ICR.BIT.B1 = 1;
	/*	PC2=1 Set ET_RX_DV input	*/
	PORTC.ICR.BIT.B2 = 1;
	/*	PC4=1 Set EX_TX_CLK input	*/
	PORTC.ICR.BIT.B4 = 1;
	/*	PC7=1 Set ET_COL input	*/
	PORTC.ICR.BIT.B7 = 1;
#endif	/*	ETH_MODE_SEL	*/
#if ETH_MODE_SEL == ETH_RMII_MODE
	/*	P54=1 Set ET_LINKSTA input	*/
	PORT5.ICR.BIT.B4 = 1;	
	/*	P71=1 Set ET_MDIO input	*/
	PORT7.ICR.BIT.B1 = 1;
	/* P74=1 Set RMII_RXD1 input	*/
	PORT7.ICR.BIT.B4 = 1;
	/* P75=1 Set RMII_RXD0 input	*/
	PORT7.ICR.BIT.B5 = 1;
	/* P76=1 Set REF50CLK input	*/
	PORT7.ICR.BIT.B6 = 1;
	/* P77=1 Set RMII_RX_ER input	*/
	PORT7.ICR.BIT.B7 = 1;
	/* P83=1 Set RMII_CRS_DV input	*/
	PORT8.ICR.BIT.B3 = 1;
#endif	/*	ETH_MODE_SEL	*/
#endif	/*	LQFP144	*/

#if (BSP_CFG_MCU_PART_PACKAGE == 0x3)
#if ETH_MODE_SEL == ETH_MII_MODE
	/*	PA5=1 Set ET_LINKSTA input	*/
	PORTA.ICR.BIT.B5 = 1;	
	/*	PA3=1 Set ET_MDIO input	*/
	PORTA.ICR.BIT.B3 = 1;
	/*	PB0=1 Set ET_ERXD1 input	*/
	PORTB.ICR.BIT.B0 = 1;
	/*	PB1=1 Set ET_ERXD0 input	*/
	PORTB.ICR.BIT.B1 = 1;
	/*	PB2=1 Set ET_RX_CLK input	*/
	PORTB.ICR.BIT.B2 = 1;
	/*	PB3=1 Set ET_RX_ER input	*/
	PORTB.ICR.BIT.B3 = 1;
	/*	PB7=1 Set ET_CRS input	*/
	PORTB.ICR.BIT.B7 = 1;
	/*	PC0=1 Set ET_ERXD3 input	*/
	PORTC.ICR.BIT.B0 = 1;
	/*	PC1=1 Set ET_ERXD2 input	*/
	PORTC.ICR.BIT.B1 = 1;
	/*	PC2=1 Set ET_RX_DV input	*/
	PORTC.ICR.BIT.B2 = 1;
	/*	PC4=1 Set EX_TX_CLK input	*/
	PORTC.ICR.BIT.B4 = 1;
	/*	PC7=1 Set ET_COL input	*/
	PORTC.ICR.BIT.B7 = 1;
#endif	/*	ETH_MODE_SEL	*/
#if ETH_MODE_SEL == ETH_RMII_MODE
	/*	PA5=1 Set ET_LINKSTA input	*/
	PORTA.ICR.BIT.B5 = 1;
	/*	PA3=1 Set ET_MDIO input	*/	
	PORTA.ICR.BIT.B3 = 1;
	/* PB0=1 Set RMII_RXD1 input	*/
	PORTB.ICR.BIT.B0 = 1;
	/* PB1=1 Set RMII_RXD0 input	*/
	PORTB.ICR.BIT.B1 = 1;
	/* PB2=1 Set REF50CLK input	*/
	PORTB.ICR.BIT.B2 = 1;
	/* PB3=1 Set RMII_RX_ER input	*/
	PORTB.ICR.BIT.B3 = 1;
	/* PB7=1 Set RMII_CRS_DV input	*/
	PORTB.ICR.BIT.B7 = 1;
#endif	/*	ETH_MODE_SEL	*/
#endif	/*	LQFP100	*/

} /* End of function ether_port_enable() */
/* End of File */
