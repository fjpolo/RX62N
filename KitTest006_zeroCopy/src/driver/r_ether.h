/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
  Copyright (C) 2008. Renesas Technology Corp., All Rights Reserved.
*******************************************************************************
* File Name    : r_ether.h
* Version      : 1.02
* Description  : Ethernet module device driver
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 15.02.2010 1.00    First Release
*         : 03.03.2010 1.01    Buffer size is aligned on the 32-byte boundary.
*         : 06.04.2010 1.02    RX62N changes
*         : 28.02.2011 1.03    RX62N enhancements for PAUSE frame generation 
*         : 08.07.2011 2.00    Second release 
*****************************************************************************/ 

#ifndef R_ETHER_H
#define R_ETHER_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
/* r_ether return definitions */
#define R_ETHER_OK      (0)
#define R_ETHER_ERROR   (-1)

/******************************************************************************
Variable Externs
******************************************************************************/
extern uint32_t g_ether_rx;
extern uint32_t g_ether_tx;

/******************************************************************************
Functions Prototypes
******************************************************************************/
int32_t R_Ether_Open_ZC(uint32_t ch, uint8_t mac_addr[], void **buf);
int32_t R_Ether_Close_ZC(uint32_t ch);
int32_t R_Ether_Read_ZC(uint32_t ch, void **buf);
int32_t R_Ether_Write_ZC(uint32_t ch, void **buf, uint32_t len);
int32_t R_Ether_CheckLink_ZC(uint32_t ch);
int32_t R_Ether_WaitLink_ZC(uint32_t ch);

#endif /* R_ETHER_H */
