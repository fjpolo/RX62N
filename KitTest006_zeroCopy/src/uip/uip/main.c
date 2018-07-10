/*
 * Copyright (c) 2001, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Adam Dunkels.
 * 4. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: main.c,v 1.16 2006/06/11 21:55:03 adam Exp $
 *
 */
#include <stddef.h>
#include <string.h>
#include "iodefine.h"
#include "typedefine.h"


#include "uip.h"
#include "uip_arp.h"
#include "r_ether.h"
#include "httpd.h"
#include "timer.h"
#include "user-app.h"
#include "lcd.h"

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

extern const struct uip_eth_addr my_mac;
uint8_t destin[6];

/*----------------------------------------------------------------------------*/ 
int
main(void)
{
    int i;
    struct timer periodic_timer, arp_timer, init_timer;
    uint32_t ch = 0;

    /* Display "uIP Demo" on LCD. */
    DisplayuIPDemo();

    timer_init();
    timer_set(&periodic_timer, CLOCK_SECOND / 2);
    timer_set(&arp_timer, CLOCK_SECOND * 10);
    timer_set(&init_timer, CLOCK_SECOND * 10);

    /* Set Ethernet address. */
    uip_setethaddr(my_mac);
    uip_init();

    /* If links is down, wait until initialization timer to expire
       before displaying link is down.  This is for keeping "uIP
       Demo" on the LCD a bit longer if the Ethernet cable is not
       plugged in. */
    if (R_Ether_CheckLink_ZC(ch) != R_ETHER_OK)
    {
        while (!timer_expired(&init_timer));
        DisplayLinkMissing();
    }

    /* Initialize the MAC. */
    R_Ether_Open_ZC(ch, (uint8_t*)&my_mac.addr[0], (void **)&uip_buf);

    /* R_Ether_WaitLink must be called at least once after
       R_Ether_Open to complete link and Ethernet
       initializations.*/
    while (R_Ether_WaitLink_ZC(ch) != R_ETHER_OK);

    /* Initialize DHCP and HTTP. */
    dhcpc_init(&my_mac.addr[0], 6);
    httpd_init();

    while (1)
    {
        /* Check if link is down. If so, wait until is back up and
           restart DHCP. */
        if (R_Ether_CheckLink_ZC(ch) != R_ETHER_OK)
        {
            DisplayLinkMissing();
            while (R_Ether_WaitLink_ZC(ch) != R_ETHER_OK);
            dhcpc_init(&my_mac.addr[0], 6);
        }

        /* Check global receive flag. */
        if (g_ether_rx)
        {
            uip_len = (uint16_t)R_Ether_Read_ZC(ch, (void **)&uip_buf);
            g_ether_rx--;
        }

        if( ( uip_len > 0 ) && ( uip_buf != NULL ) )
        {
            /* Standard uIP loop taken from the uIP manual. */
            if(BUF->type == htons(UIP_ETHTYPE_IP))
            {
                uip_arp_ipin();
                uip_input();

                /* If the above function invocation resulted in data that should
                   be sent out on the network, the global variable uip_len is
                   set to a value > 0. */
                if(uip_len > 0)
                {
                    uip_arp_out();
                    R_Ether_Write_ZC(ch, (void **)&uip_buf, uip_len);
                }               
            }
            else if(BUF->type == htons(UIP_ETHTYPE_ARP))
            {
                uip_arp_arpin();

                /* If the above function invocation resulted in data that should
                   be sent out on the network, the global variable uip_len is
                   set to a value > 0. */
                if(uip_len > 0)
                {
                    R_Ether_Write_ZC(ch, (void **)&uip_buf, uip_len);
                }
            }
        }
        
        if(timer_expired(&periodic_timer))
        {
            timer_reset(&periodic_timer);
            for(i = 0; i < UIP_CONNS; i++)
            {
                uip_periodic(i);

                /* If the above function invocation resulted in data that should
                   be sent out on the network, the global variable uip_len is
                   set to a value > 0. */
                if(uip_len > 0)
                {
                    uip_arp_out();
                    R_Ether_Write_ZC(ch, (void **)&uip_buf, uip_len);
                }
            }

#if UIP_UDP
            for (i = 0; i < UIP_UDP_CONNS; i++)
            {
                uip_udp_periodic(i);
                /* If the above function invocation resulted in data that should
                   be sent out on the network, the global variable uip_len is
                   set to a value > 0.*/
                if (uip_len > 0)
                {
                    uip_arp_out();
                    R_Ether_Write_ZC(ch, (void **)&uip_buf, uip_len);
                }
            }
#endif /* UIP_UDP */

            /* Call the ARP timer function every 10 seconds. */
            if( timer_expired(&arp_timer))
            {
                timer_reset(&arp_timer);
                uip_arp_timer();
            }
        }

        /* Insert user applications here.  Call WEB application that
           controls the LEDs on the target board. */
        user_app();
    
    }   /* End of while(1) */
    return 0;
}
/*---------------------------------------------------------------------------*/
