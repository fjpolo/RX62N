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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : ether_callback.c
* Version      : ----
* Description  : This module solves all the world's problems
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 05.01.2015 ----     Clean up source code.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_ether.h"
#include "r_t4_itcpip.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define ETHER_CHANNEL_MAX 1

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
void callback_wakeon_lan(uint32_t channel);
void callback_link_on(uint32_t channel);
void callback_link_off(uint32_t channel);
static callback_from_system_t g_fp_callback;

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
volatile uint8_t  pause_enable = ETHER_FLAG_OFF;
volatile uint8_t  magic_packet_detect[ETHER_CHANNEL_MAX];
volatile uint8_t  link_detect[ETHER_CHANNEL_MAX];


/***********************************************************************************************************************
* Function Name: callback_wakeon_lan
* Description  :
* Arguments    : channel -
*                    Ethernet channel number
* Return Value : none
***********************************************************************************************************************/
void callback_wakeon_lan(uint32_t channel)
{
    if (ETHER_CHANNEL_MAX > channel)
    {
        magic_packet_detect[channel] = 1;

        /* Please add necessary processing when magic packet is detected.  */
    }
} /* End of function callback_wakeon_lan() */

/***********************************************************************************************************************
* Function Name: callback_link_on
* Description  :
* Arguments    : channel -
*                    Ethernet channel number
* Return Value : none
***********************************************************************************************************************/
void callback_link_on(uint32_t channel)
{
    if (ETHER_CHANNEL_MAX > channel)
    {
        if((link_detect[channel] != ETHER_FLAG_ON_LINK_ON) && (g_fp_callback != 0))
        {
            g_fp_callback(channel, ETHER_EV_LINK_ON, 0);
        }
        link_detect[channel] = ETHER_FLAG_ON_LINK_ON;

        /* Please add necessary processing when becoming Link up. */
    }
} /* End of function callback_link_on() */

/***********************************************************************************************************************
* Function Name: callback_link_off
* Description  :
* Arguments    : channel -
*                    Ethernet channel number
* Return Value : none
***********************************************************************************************************************/
void callback_link_off(uint32_t channel)
{
    if (ETHER_CHANNEL_MAX > channel)
    {
        if((link_detect[channel] != ETHER_FLAG_ON_LINK_OFF) && (g_fp_callback != 0))
        {
            g_fp_callback(channel, ETHER_EV_LINK_OFF, 0);
        }
        link_detect[channel] = ETHER_FLAG_ON_LINK_OFF;

        /* Please add necessary processing when becoming Link down. */
    }
} /* End of function ether_cb_link_off() */

/***********************************************************************************************************************
* Function Name: register_callback_linklayer
* Description  :
* Arguments    : fp -
*                    callback function
* Return Value : none
***********************************************************************************************************************/
void register_callback_linklayer(callback_from_system_t fp)
{
    g_fp_callback = fp;

    return;
}

