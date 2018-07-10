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
* File Name    : resetprg.c
* Version      : 1.00
* Description  : Power on reset 
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 15.02.2010 1.00    First Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include	<machine.h>
#include	<_h_c_lib.h>
//#include	<stddef.h>					// Remove the comment when you use errno
//#include 	<stdlib.h>					// Remove the comment when you use rand()
//#include	"typedefine.h"
#include	"stacksct.h"
#include	<stdint.h>


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define PSW_init  0x00010000
#define FPSW_init 0x00000100

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
//#ifdef __cplusplus				// Use SIM I/O
//extern "C" {
//#endif
//extern void _INIT_IOLIB(void);
//extern void _CLOSEALL(void);
//#ifdef __cplusplus
//}
//#endif

//extern void srand(_UINT);		// Remove the comment when you use rand()
//extern _SBYTE *_s1ptr;				// Remove the comment when you use strtok()
		
#ifdef __cplusplus				// Use Hardware Setup
extern "C" {
#endif
extern void main(void);
extern void HardwareSetup(void);
#ifdef __cplusplus
}
#endif
	
//#ifdef __cplusplus			// Remove the comment when you use global class object
//extern "C" {					// Sections C$INIT and C$END will be generated
//#endif
//extern void _CALL_INIT(void);
//extern void _CALL_END(void);
//#ifdef __cplusplus
//}
//#endif

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
void PowerON_Reset_PC(void);
#ifdef __cplusplus
}
#endif

/******************************************************************************
Private global variables and functions
******************************************************************************/
#pragma inline_asm Change_PSW_PM_to_UserMode
static void Change_PSW_PM_to_UserMode(void);

/* ==== Section name changed to ResetPRG ==== */
#pragma section ResetPRG

/* ==== Entry function specified ==== */
#pragma entry PowerON_Reset_PC

/******************************************************************************
* ID           : 
* Outline      : CPU initialization function
*------------------------------------------------------------------------------
* Include      : <machine.h>, <_h_c_lib.h>, "stacksct.h", and "iodefine.h"
*------------------------------------------------------------------------------
* Declaration  : void PowerON_Reset_PC(void);
*------------------------------------------------------------------------------
* Description  : Executes the CPU initialization processing to register 
*              : the power-on reset vector to the exception vector table.
*              : This function is executed first after power-on reset.
*------------------------------------------------------------------------------
* Argument     : void
*------------------------------------------------------------------------------
* Return Value : void
*------------------------------------------------------------------------------
* Note         : The process commented out with "//" should be removed as needed.
******************************************************************************/
void PowerON_Reset_PC(void)
{ 
	set_intb((uint32_t)__sectop("C$VECT"));
	set_fpsw(FPSW_init);

	_INITSCT();

//	_INIT_IOLIB();					// Use SIM I/O

//	errno=0;						// Remove the comment when you use errno
//	srand((_UINT)1);					// Remove the comment when you use rand()
//	_s1ptr=NULL;					// Remove the comment when you use strtok()
		
	HardwareSetup();				// Use Hardware Setup
    nop();

//	_CALL_INIT();					// Remove the comment when you use global class object

	set_psw(PSW_init);				// Set Ubit & Ibit for PSW
	Change_PSW_PM_to_UserMode();	// Change PSW PMbit (SuperVisor->User)

	main();

//	_CLOSEALL();					// Use SIM I/O
	
//	_CALL_END();					// Remove the comment when you use global class object

	brk();
}

/******************************************************************************
* Function Name: Change_PSW_PM_to_UserMode
* Description  : Change PSW PMbit (SuperVisor->User)
* Arguments    : none
* Return Value : none
******************************************************************************/
static void Change_PSW_PM_to_UserMode(void)
{
	MVFC   PSW,R1
	OR     #00100000h,R1
	PUSH.L R1
	MVFC   PC,R1
	ADD    #10,R1
	PUSH.L R1
	RTE
	NOP
	NOP
}

/* END of File */
