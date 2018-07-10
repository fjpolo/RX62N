/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Technology Corp. and is only
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Technology Corp. and is protected under
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.	ALL SUCH WARRANTIES ARE EXPRESSLY
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
* File Name	   : main.c
* Version	   : 1.00
* Device(s)	   : RX62N
* Tool-Chain   : RX Standard Toolchain (V.1.0.0.0)
* OS		   :
* H/W Platform : RSK2+RX62N
* Description  : This is the main tutorial code.
* Operation	   :
* Limitations  :
******************************************************************************
* History : DD.MM.YYYY Version Description
*		  : 15.02.2010 1.00	   First Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"
#include "r_ether.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
#pragma section ETH_BUFF
typedef struct{
	uint8_t frame[BUFSIZE];
	int32_t len;
	uint8_t wk[12];
}USER_BUFFER;
USER_BUFFER recv[10];
#pragma section

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
extern void main(void);

/******************************************************************************
Private global variables and functions
******************************************************************************/
static uint8_t s_frame[] = {
	0xff,0xff,0xff,0xff,0xff,0xff,	/* Destination MAC address				*/
	0x74,0x90,0x50,0x00,0xCC,0xF9,	/* Source MAC address (00:01:02:03:04:05)*/
	0x08,0x06,						/* Type (ARP)							*/
	0x00,0x01,						/*	+--H/W type= Ethernet				*/
	0x08,0x00,						/*	+--Protocol type= IP				*/
	0x06,0x04,						/*	+--HW/protocol address length		*/
	0x00,0x01,						/*	+--OPCODE= request					*/
	0x74,0x90,0x50,0x00,0xCC,0xF9,	/*	+--Source MAC address (00:01:02:03:04:05) */
	0xC0,0xA8,0x00,0x21,			/*	+--Source IP address (192.168.0.3)	*/
	0x00,0x00,0x00,0x00,0x00,0x00,	/*	+--Inquiry MAC address				*/
	0xc0,0xa8,0x00,0x37,			/*	+--Inquiry IP address (192.168.0.5)	*/
};

static uint8_t mac_addr[6] = {0x74, 0x90, 0x50, 0x00, 0xCC, 0xF9};

void SampleEthernetTransmission(void);
void SampleEthernetReception(void);



void main(void)
{
	int8_t i;

	/* Select test type */
	i = 1;

	switch(i)
	{
		case 1:	/* Transmission test */
			SampleEthernetTransmission();
			break;
		case 2:	/* Reception test */
			SampleEthernetReception();
			break;
		default:
			break;
	}
	while(1);
}

/******************************************************************************
* Function Name: SampleEthernetTransmission
* Description : Ethernet transmission sample program function
* Argument : none
* Return Value : none
******************************************************************************/
void SampleEthernetTransmission(void)
{
	int32_t i;
	int32_t ret;

	/* ==== Ethernet initial setting ==== */
	ret = R_Ether_Open(0, mac_addr);
	if( ret == R_ETHER_OK )
	{
		/* ==== 10-frame transmission start ==== */
		for( i=0; i<10; i++ )
		{
			/* ----Transmission ---- */
			ret = R_Ether_Write( 0, s_frame, sizeof(s_frame) );
			if( ret != R_ETHER_OK )
			{
				break;
			}
		}
	}
	/* Check transmission completion */
	while(EDMAC.EDTRR.BIT.TR != 0);
	/* ==== Ethernet transmission and reception stop ==== */
	R_Ether_Close(0);
}

/******************************************************************************
* Function Name: SampleEthernetReception
* Description : Ethernet reception sample program function
* Argument : none
* Return Value : none
******************************************************************************/
void SampleEthernetReception(void)
{
	int32_t i;
	int32_t ret;

	/* ==== Ethernet initial setting ==== */
	ret = R_Ether_Open(0, mac_addr);
	if( ret == R_ETHER_OK )
	{
		/* ==== Start reception of 10 frames ==== */
		for( i=0; i<10; i++ )
		{
			/* ---- Reception ---- */
			recv[i].len = R_Ether_Read(0, recv[i].frame);
			if( recv[i].len == 0 )
			{
				i--;
			}
		}
	}
	/* ==== Ethernet transmission/reception halted ==== */
	R_Ether_Close(0);
}

/* End of file */

