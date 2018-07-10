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
* File Name    : sbrk.c
* Version      : 1.00
* Description  : 
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 15.02.2010 1.00    First Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stddef.h>
#include <stdio.h>
#include "typedefine.h"
#include "sbrk.h"

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

_SBYTE  *sbrk(size_t size);

/* Specifies the minimum unit of */
//const size_t _sbrk_size=	

/* the defined heap area */
extern _SBYTE *_s1ptr;

union HEAP_TYPE {
	/* Dummy for 4-byte boundary */
    _SDWORD  dummy ;
	/* Declaration of the area managed by sbrk*/
    _SBYTE heap[HEAPSIZE];
};

static union HEAP_TYPE heap_area ;

/* End address allocated by sbrk	*/
static _SBYTE *brk=(_SBYTE *)&heap_area;

/**************************************************************************/
/*     sbrk:Memory area allocation                                        */
/*          Return value:Start address of allocated area (Pass)           */
/*                       -1                              (Failure)        */
/**************************************************************************/

/* Assigned area size   */
_SBYTE  *sbrk(size_t size)
{
    _SBYTE  *p;
	/* Empty area size      */
    if(brk+size > heap_area.heap+HEAPSIZE)
	{
        p = (_SBYTE *)-1;
    }
    else
	{
		/* Area assignment      */
        p = brk;
		/* End address update   */
        brk += size;
    }
    return p;
}

