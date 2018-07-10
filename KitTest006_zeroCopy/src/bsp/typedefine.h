
/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
  Copyright (C) 2008. Renesas Technology Corp., All Rights Reserved.
*******************************************************************************
* File Name    : typedefine.h
* Version      : 1.00
* Description  : Common type definitions and defines
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 15.02.2010 1.00    First Release
******************************************************************************/

#ifndef	TYPEDEFINE_H
#define	TYPEDEFINE_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef signed char _SBYTE;
typedef unsigned char _UBYTE;
typedef signed short _SWORD;
typedef unsigned short _UWORD;
typedef signed int _SINT;
typedef unsigned int _UINT;
typedef signed long _SDWORD;
typedef unsigned long _UDWORD;

typedef unsigned char Bool;

/**
 * The standard typedefs for RX compiler are defined in stdint.h
 * system header file. Standard types are: int8_t, unit8_t 
 * int16_t, unit16_t int32_t, unit32_t int64_t, unit64_t 
 */

/******************************************************************************
Macro definitions
******************************************************************************/
#define TRUE    (Bool)1
#define FALSE   (Bool)0
#define	OFF		FALSE
#define	ON		TRUE

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/




#endif /* TYPEDEFINE_H */ 
