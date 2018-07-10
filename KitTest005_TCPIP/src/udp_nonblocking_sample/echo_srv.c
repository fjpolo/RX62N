/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011(2016) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
* File Name     : echo_srv_udp_blocking.c
* Version       : 2.00
* Device(s)     : Renesas microcomputer
* Tool-Chain    : Renesas compilers
* OS            :
* H/W Platform  :
* Description   : T4 sample program.
* Limitations   :
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.06.2010 1.00 First Release
*               : 30.08.2011 1.01 Clean up source code
*               : 01.04.2014 2.00 Release for V.2.00
******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "r_t4_itcpip.h"
#include "r_ether.h"
#include "echo_srv_sample.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct _CEP
{
    uint32_t    status;
    T_IPV4EP    dst_addr;
    T_IPV4EP    src_addr;
    int32_t     current_rcv_data_len;
    int32_t     total_rcv_len;
    UB          rcv_buf[1460];
    UB          snd_buf[1460];
    int32_t     _1sec_counter;
    int32_t     _1sec_timer;
    int32_t     pre_1sec_timer;
} CEP;


/******************************************************************************
Macro definitions
******************************************************************************/
#if(APPLICATION_T4_CHANNEL_NUM == 2)
#define CEP_NUM          (4)
#elif(APPLICATION_T4_CHANNEL_NUM == 1)
#define CEP_NUM          (2)
#endif

#define T4_UDP_RECV    0
#define T4_UDP_SEND    1
#define T4_RESET_WAIT  255

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void echo_srv(void);
ER    t4_udp_callback(ID cepid, FN fncd , VP p_parblk);

/******************************************************************************
Private global variables and functions
******************************************************************************/
static CEP cep[CEP_NUM];
static UB guc_event[APPLICATION_T4_CHANNEL_NUM];
static DHCP* gpt_dhcp[APPLICATION_T4_CHANNEL_NUM];

/******************************************************************************
* ID                : 1.0
* Outline           : echo_srv
* Include           : r_t4_itcpip.h
* Declaration       : void echo_srv(void)
* Function Name     : echo_srv
* Description       : echo server main function.
* Argument          : none
* Return Value      : none
******************************************************************************/
void echo_srv(void)
{
    UB interface;
    uint32_t i;
    uint32_t k;

    /* initialize cep status */
    for (i = 0; i < CEP_NUM; i++)
    {
        cep[i].status = T4_UDP_RECV;
    }

    while ((bool)1)
    {
        R_ETHER_LinkProcess();
        /* check all connections */
        for (i = 0; i < CEP_NUM; i++)
        {
            interface = tcp_ccep[i].cepatr;

            if(1 == _t4_dhcp_enable)                                                                                /*dhcp use*/
            {
                if (DHCP_EV_PLEASE_RESET == guc_event[interface])                                                   /*t4 reset request*/
                {
                    guc_event[interface] = 0;                                                                       /*reset callback event */
#if defined(DEBUG_PRINT)
                    printf("main:call tcpudp_reset(channel:%d)\n", interface);
#endif/*#if defined(DEBUG_PRINT)*/
                    tcpudp_reset(interface);                                                                        /*reset DHCP use interface*/

                    /* reset application data same as interface. */
                    for (k = 0; k < CEP_NUM; k++)
                    {
                        if (interface == udp_ccep[k].cepatr)
                        {
                            memset(&cep[k], 0, sizeof(CEP));
                            cep[k].status = T4_UDP_RECV;
                        }
                    }
                }
            }
        }

        /* udp action does nothing, all process will be executed in the callback routine */

    }
}


/******************************************************************************
* ID                : 1.0
* Outline           : t4_udp_callback
* Include           : r_t4_itcpip.h
* Declaration       : ER t4_udp_callback(ID cepid, FN fncd , VP p_parblk)
* Function Name     : t4_udp_callback
* Description       : Callback function.
* Argument          : ID        cepid   ;    Communication endpoint ID
*                   : FN        fncd    ;    Event code
*                   : VP        p_parblk;    Parameter block
* Return Value      : ER                ;    always 0 (not in use)
******************************************************************************/
ER    t4_udp_callback(ID cepid, FN fncd , VP p_parblk)
{
    uint32_t i;
    ER ercd;

    i = cepid - 1;
    ercd = *(ER *)p_parblk;

    switch (fncd)
    {
        case TEV_UDP_RCV_DAT:
            ercd = udp_rcv_dat(cepid, &cep[i].dst_addr, cep[i].rcv_buf, sizeof(cep[i].rcv_buf), TMO_POL);
            udp_snd_dat(cepid, &cep[i].dst_addr, cep[i].rcv_buf, ercd, TMO_NBLK);
            break;
        case TFN_UDP_RCV_DAT:
            udp_snd_dat(cepid, &cep[i].dst_addr, cep[i].rcv_buf, ercd, TMO_NBLK);
            break;
        case TFN_UDP_SND_DAT:
            udp_rcv_dat(cepid, &cep[i].dst_addr, cep[i].rcv_buf, sizeof(cep[i].rcv_buf), TMO_NBLK);
            break;
        default:
            break;
    }
    return 0;
}


/******************************************************************************
* ID                : 1.0
* Outline           : user_cb
* Include           : r_t4_itcpip.h
* Declaration       : ER user_cb(UB channel, FN eventid , VP param)
* Function Name     : user_cb
* Description       : Callback function.
* Argument          : UB        channel ;    RJ45 interface
*                   : FN        eventid ;    Event code
*                   : VP        param   ;    Parameter block
* Return Value      : ER                ;    always 0 (not in use)
******************************************************************************/
ER user_cb(UB channel, UW eventid, VP param)
{
#if defined(DEBUG_PRINT)
    uint8_t*    ev_tbl[] =
    {
        "ETHER_EV_LINK_OFF",
        "ETHER_EV_LINK_ON",
        "ETHER_EV_COLLISION_IP",
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
        "DHCP_EV_LEASE_IP",
        "DHCP_EV_LEASE_OVER",
        "DHCP_EV_INIT",
        "DHCP_EV_INIT_REBOOT",
        "DHCP_EV_APIPA",
        "DHCP_EV_NAK",
        "DHCP_EV_FATAL_ERROR",
        "DHCP_EV_PLEASE_RESET"
    };
    printf("^^>>>user_cb<<< ch:%d,eventID = %s\n", channel, ev_tbl[eventid]);
#endif /*#if defined(DEBUG_PRINT)*/
    guc_event[channel] = eventid;
    switch(eventid)
    {
        case ETHER_EV_LINK_OFF:
            break;

        case ETHER_EV_LINK_ON:
            break;

        case ETHER_EV_COLLISION_IP:
            break;

        case DHCP_EV_LEASE_IP:
            gpt_dhcp[channel] = (DHCP*)param;
#if defined(DEBUG_PRINT)
            printf("DHCP.ipaddr[4]   %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->ipaddr[0], gpt_dhcp[channel]->ipaddr[1],
                   gpt_dhcp[channel]->ipaddr[2], gpt_dhcp[channel]->ipaddr[3]);
            printf("DHCP.maskaddr[4] %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->maskaddr[0], gpt_dhcp[channel]->maskaddr[1],
                   gpt_dhcp[channel]->maskaddr[2], gpt_dhcp[channel]->maskaddr[3]);
            printf("DHCP.gwaddr[4]   %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->gwaddr[0], gpt_dhcp[channel]->gwaddr[1],
                   gpt_dhcp[channel]->gwaddr[2], gpt_dhcp[channel]->gwaddr[3]);
            printf("DHCP.dnsaddr[4]  %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->dnsaddr[0], gpt_dhcp[channel]->dnsaddr[1],
                   gpt_dhcp[channel]->dnsaddr[2], gpt_dhcp[channel]->dnsaddr[3]);
            printf("DHCP.dnsaddr2[4] %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->dnsaddr2[0], gpt_dhcp[channel]->dnsaddr2[1],
                   gpt_dhcp[channel]->dnsaddr2[2], gpt_dhcp[channel]->dnsaddr2[3]);
            printf("DHCP.macaddr[6]  %02X:%02X:%02X:%02X:%02X:%02X\n",
                   gpt_dhcp[channel]->macaddr[0],  gpt_dhcp[channel]->macaddr[1],  gpt_dhcp[channel]->macaddr[2],
                   gpt_dhcp[channel]->macaddr[3],  gpt_dhcp[channel]->macaddr[4],  gpt_dhcp[channel]->macaddr[5]);
            printf("DHCP.domain[%d] %s\n", strlen(gpt_dhcp[channel]->domain), gpt_dhcp[channel]->domain);
            printf("\n");
#endif /*#if defined(DEBUG_PRINT)*/
            break;
        case DHCP_EV_LEASE_OVER:
            break;

        case DHCP_EV_INIT:
            break;

        case DHCP_EV_INIT_REBOOT:
            break;

        case DHCP_EV_APIPA:
            gpt_dhcp[channel] = (DHCP*)param;
#if defined(DEBUG_PRINT)
            printf("DHCP.ipaddr[4]   %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->ipaddr[0], gpt_dhcp[channel]->ipaddr[1],
                   gpt_dhcp[channel]->ipaddr[2], gpt_dhcp[channel]->ipaddr[3]);
            printf("DHCP.maskaddr[4] %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->maskaddr[0], gpt_dhcp[channel]->maskaddr[1],
                   gpt_dhcp[channel]->maskaddr[2], gpt_dhcp[channel]->maskaddr[3]);
            printf("DHCP.gwaddr[4]   %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->gwaddr[0], gpt_dhcp[channel]->gwaddr[1],
                   gpt_dhcp[channel]->gwaddr[2], gpt_dhcp[channel]->gwaddr[3]);
            printf("DHCP.dnsaddr[4]  %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->dnsaddr[0], gpt_dhcp[channel]->dnsaddr[1],
                   gpt_dhcp[channel]->dnsaddr[2], gpt_dhcp[channel]->dnsaddr[3]);
            printf("DHCP.dnsaddr2[4] %d.%d.%d.%d\n",
                   gpt_dhcp[channel]->dnsaddr2[0], gpt_dhcp[channel]->dnsaddr2[1],
                   gpt_dhcp[channel]->dnsaddr2[2], gpt_dhcp[channel]->dnsaddr2[3]);
            printf("DHCP.macaddr[6]  %02X:%02X:%02X:%02X:%02X:%02X\n",
                   gpt_dhcp[channel]->macaddr[0],  gpt_dhcp[channel]->macaddr[1],  gpt_dhcp[channel]->macaddr[2],
                   gpt_dhcp[channel]->macaddr[3],  gpt_dhcp[channel]->macaddr[4],  gpt_dhcp[channel]->macaddr[5]);
            printf("DHCP.domain[%d] %s\n", strlen(gpt_dhcp[channel]->domain), gpt_dhcp[channel]->domain);
            printf("\n");
#endif /*#if defined(DEBUG_PRINT)*/
            break;

        case DHCP_EV_NAK:
            break;

        case DHCP_EV_FATAL_ERROR:
            break;

        case DHCP_EV_PLEASE_RESET:
            break;

        default:
            break;
    }
    return 0;
}
