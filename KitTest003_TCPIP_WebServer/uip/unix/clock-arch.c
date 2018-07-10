/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
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
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack
 *
 * $Id: clock-arch.c,v 1.2 2006/06/12 08:00:31 adam Exp $
 */

/**
 * \file
 *         Implementation of architecture-specific clock functionality
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "iodefine.h"
#include "rskrx62n.h"
#include "clock-arch.h"

// Renesas -- #include <sys/time.h>

clock_time_t timer_clock_ticks = 0;


/**
 * System timer tick initialization
 *
 * This function initializes a timer interrupt with 100ms tick. 
 *
 */
void  timer_init(void)
{

    /** 
    * Compare Match Timer Unit 0 is used
    * generate 100 ms (10 Hz) system tick.  
    */
    
    /* Count stop */
    CMT.CMSTR0.BIT.STR0 = 0x0;           
    /* Pclock/512 */
    CMT0.CMCR.WORD = 0x0043;             
    /* Timer counter clear */
    CMT0.CMCNT = 0x0000;				 
    /* 100ms */
    CMT0.CMCOR = (PCLK/CMT0_CLK_SELECT) / CLOCK_CONF_SECOND;				 
    /* Count start */
    CMT.CMSTR0.BIT.STR0 = 0x1;           

    /* Interrupt priority level = 1 */  
    /* Enable ICU for CMT0 interrupts */
    IPR(CMT0, CMI0) = 1;
    IEN(CMT0, CMI0) = 1;
}

/**
 * System timer tick 
 *
 * This function is called from timer interrupt to increment a 
 * system timer tick.  Also an LED on the target board is 
 * toggled to show activity. 
 *
 */
void int_cmt0_isr(void)
{
    static unsigned char g_led_onoff = 0;

	/* Activity LED blinking */
	g_led_onoff ^= 1;

    if(g_led_onoff == 0)
	{
		LED0 = LED_OFF;
	}
	else
	{
		LED0 = LED_ON;
	}

    timer_clock_ticks++;
	
}


/*---------------------------------------------------------------------------*/
clock_time_t
clock_time(void)
{
  return timer_clock_ticks;
}
/*---------------------------------------------------------------------------*/

