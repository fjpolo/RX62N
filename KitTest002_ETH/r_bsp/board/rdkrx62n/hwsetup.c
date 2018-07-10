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
 * Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : hwsetup.c
 * Device(s)    : RX
 * H/W Platform : YRDKRX62N
 * Description  : Defines the initialization routines used each time the MCU is restarted.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 26.10.2011 1.00     First Release
 *         : 07.05.2013 1.10     Added a call to bsp_non_existent_port_init() which initializes non-bonded out GPIO pins.
 *         : 25.06.2013 1.20     Moved call to bsp_non_existent_port_init() in hardware_setup() to last so that user
 *                               does not have to worry about writing 'missing' bits in PDR registers.
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
/* I/O Register and board definitions */
#include "platform.h"
#include "r_ether.h"


/***********************************************************************************************************************
Private global variables and functions
 ***********************************************************************************************************************/
/* MCU I/O port configuration function declaration */
static void output_ports_configure(void);

/* Interrupt configuration function declaration */
static void interrupts_configure(void);

/* MCU peripheral module configuration function declaration */
static void peripheral_modules_enable(void);


/***********************************************************************************************************************
 * Function name: hardware_setup
 * Description  : Contains setup functions called at device restart
 * Arguments    : none
 * Return value : none
 ***********************************************************************************************************************/
void hardware_setup(void)
{
	output_ports_configure();
	interrupts_configure();
	peripheral_modules_enable();
	bsp_non_existent_port_init();
}

/***********************************************************************************************************************
 * Function name: output_ports_configure
 * Description  : Configures the port and pin direction settings, and sets the pin outputs to a safe level.
 * Arguments    : none
 * Return value : none
 ***********************************************************************************************************************/
static void output_ports_configure(void)
{
	/* Port 0 - all inputs (IRQ's from ethernet & WiFi) */
	PORT0.DDR.BYTE = 0x00;

	/* Port 1 - all inputs (IIC and USB settings will override these later */
	PORT1.DDR.BYTE = 0x00;

	/* Port 2 - USB */
	PORT2.DR.BYTE = 0x00;   /* All outputs low to start */
	PORT2.DDR.BIT.B3 = 1;   /* USB0 DPUPEA is an output */
	PORT2.DDR.BIT.B4 = 1;   /* USB VBUSEN is an output */

	/* Port 3 - JTAG (P30, P31, P34), CAN (P32=Tx, P33=Rx), NMI (P35) */
	PORT3.DR.BIT.B2 = 0;    /* Tx low to start */
	PORT3.DDR.BIT.B2 = 1;   /* Tx is an output */

	/* Port 4 - Switches (P40-P42), AIN (P43-P47)  */
	PORT4.DDR.BYTE = 0x00;  /* Port 4 is all inputs */
	PORT4.ICR.BYTE = 0xFF;  /* Turn on input buffers */

	/* Port 5 - LCD control signals, some audio  */
	PORT5.DR.BYTE = 0x00;   /* All outputs off to start */
	PORT5.DDR.BIT.B0 = 1;   /* TxD2 is output. */
	PORT5.DDR.BIT.B1 = 1;   /* LCD_RS is an output */
	PORT5.DDR.BIT.B2 = 0;   /* RxD2 is input */
	PORT5.ICR.BIT.B2 = 1;   /* Enable input buffer on RxD2. */
	PORT5.DDR.BIT.B3 = 1;   /* BCLK is an output */
	PORT5.DDR.BIT.B4 = 1;   /* PWMLP_OUT is an output */
	PORT5.DDR.BIT.B5 = 1;   /* AMP_SHDN is an output */

	/* SCI2 B pins are used. */
	IOPORT.PFFSCI.BYTE = 0x04;

	/* Port A outputs all LOW to start  */
	PORTA.DR.BYTE = 0x00;

	/* Port 0 */
	PORT0.DDR.BIT.B2 = 1;
	PORT0.DDR.BIT.B3 = 1;
	PORT0.DDR.BIT.B5 = 1;
	PORT3.DDR.BIT.B4 = 1;

	PORT0.DR.BIT.B2 = 1;
	PORT0.DR.BIT.B3 = 1;
	PORT0.DR.BIT.B5 = 1;
	PORT3.DR.BIT.B4 = 1;

	PORT0.DR.BIT.B2 = 0;
	PORT0.DR.BIT.B3 = 0;
	PORT0.DR.BIT.B5 = 0;
	PORT3.DR.BIT.B4 = 0;


	/* Port A - Expansion (PA0-PA2), Ether (PA3-PA5), Audio (PA6-PA7) */
	PORTA.DDR.BYTE = 0x00;  /* All pins are inputs for now */

	/* Port B outputs all LOW to start  */
	PORTB.DR.BYTE = 0x00;

	/* Port B - Ether */
	PORTB.DDR.BIT.B4 = 1;   /* RMII_TX_EN is an output */
	PORTB.DDR.BIT.B5 = 1;   /* RMII_TXD0 is an output */
	PORTB.DDR.BIT.B6 = 1;   /* RMII_TXD6 is an output */

	/* Choose A pin group and enable RSPIA pins */
	IOPORT.PFGSPI.BYTE = 0x0E;

	/* Port C - Chip selects, clock = high; IO reset = low */
	PORTC.DR.BYTE = 0x17;

	/* Port C - SPI (PC0-2, PC4-7), IO reset (PC3) */
	PORTC.DDR.BIT.B0 = 1;   /* FLASH_CS  is an output */
	PORTC.DDR.BIT.B1 = 1;   /* WIFI_CS is an output */
	PORTC.DDR.BIT.B2 = 1;   /* LCD_CS is an output */
	PORTC.DDR.BIT.B3 = 1;   /* LCD_RESET is an output */
	PORTC.DDR.BIT.B4 = 1;   /* SDMICRO_CS is an output */
	PORTC.DDR.BIT.B5 = 1;   /* RSPI_SCLOCK is an output */
	PORTC.DDR.BIT.B6 = 1;   /* RSPI_MOSI is an output */
	PORTC.DDR.BIT.B7 = 0;   /* MTCLKB-B/RSPI_MISO is an input */
	PORTC.ICR.BIT.B7 = 1;

	/* Move MTCLKB */
	IOPORT.PFCMTU.BYTE = 0x80;

	/* Port D - All LED's off */
	PORTD.DR.BYTE = 0xFF;

	/* Port D - LED's */
	PORTD.DDR.BYTE = 0xFF;  /* All are outputs */

	/* Port E - All LED's off */
	PORTE.DR.BYTE = 0x0F;

	/* Port E - LED's */
	PORTE.DDR.BYTE = 0x0F;




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
#if ETH_MODE_SEL == ETH_MII_MODE

	/*	P54=1 Set LINKSTA input	*/
	PORT5.ICR.BIT.B4 = 1;
	/*	P71=1 Set MDIO input	*/
	PORT7.ICR.BIT.B1 = 1;
	/*	P72=1 Set MDC input	*/
	//PORT7.ICR.BIT.B2 = 1;
	/*	PC4=1 Set EX_TX_CLK input	*/
	PORTC.ICR.BIT.B4 = 1;
	/*	P80=1 Set TX_EN input	*/
	//PORT8.ICR.BIT.B0 = 1;
	/*	PC3=1 Set TX_ERR input	*/
	PORTC.ICR.BIT.B3 = 1;
	/*	P81=1 Set TXD0 input	*/
	//PORT8.ICR.BIT.B1 = 1;
	/*	P82=1 Set TXD1 input	*/
	//PORT8.ICR.BIT.B2 = 1;
	/*	PC5=1 Set TXD2 input	*/
	//PORTC.ICR.BIT.B5 = 1;
	/*	PC6=1 Set TXD3 input	*/
	//PORTC.ICR.BIT.B6 = 1;
	/*	PC2=1 Set ET_RX_DV input	*/
	PORTC.ICR.BIT.B2 = 1;
	/*	P77=1 Set ET_RX_ER input	*/
	PORT7.ICR.BIT.B7 = 1;
	/*	P74=1 Set RXD1 input	*/
	PORT7.ICR.BIT.B4 = 1;
	/*	P75=1 Set RXD0 input	*/
	PORT7.ICR.BIT.B5 = 1;
	/*	PC1=1 Set ET_ERXD2 input	*/
	PORTC.ICR.BIT.B1 = 1;
	/*	PC0=1 Set ET_ERXD3 input	*/
	PORTC.ICR.BIT.B0 = 1;
	/*	PC7=1 Set COL input	*/
	PORTC.ICR.BIT.B7 = 1;
	/*	P83=1 Set CRS input	*/
	PORT8.ICR.BIT.B3 = 1;
	/*	P76=1 Set CLK input	*/
	PORT7.ICR.BIT.B6 = 1;
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


}

/***********************************************************************************************************************
 * Function name: interrupts_configure
 * Description  : Configures interrupts used
 * Arguments    : none
 * Return value : none
 ***********************************************************************************************************************/
static void interrupts_configure(void)
{
	/* Add code here to setup additional interrupts */
}

/***********************************************************************************************************************
 * Function name: peripheral_modules_enable
 * Description  : Enables and configures peripheral devices on the MCU
 * Arguments    : none
 * Return value : none
 ***********************************************************************************************************************/
static void peripheral_modules_enable(void)
{
	/* Add code here to enable peripherals used by the application */
	SYSTEM.MSTPCRB.BIT.MSTPB15 = 0;				/* EtherC, EDMAC */
}
