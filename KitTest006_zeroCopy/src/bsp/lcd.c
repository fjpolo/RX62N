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
* File Name    : ldc.c
* Version      : 1.00
* Description  : LCD Module utility functions.             
*              : Written for KS0066u compatible LCD Module.
*              : (8 characters by 2 lines)                 
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 20.06.2007 1.00    First Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <machine.h>
#include <stdio.h>
#include "iodefine.h"
#include "rskrx62n.h"
#include "lcd.h"

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

/*****************************************************************************
Name:           InitDisplay 
Parameters:     none                
Returns:        none
Description:    Intializes the LCD display. 
*****************************************************************************/
void InitialiseDisplay( void )
{
    /* Power Up Delay for LCD Module */
    EN_PIN = SET_BIT_HIGH;
    DisplayDelay(7000);
    EN_PIN = SET_BIT_LOW;

    /* Display initialises in 8 bit mode - so send one write (seen as 8 bit)
    to set to 4 bit mode. */
    /* Function Set */
    LCD_nibble_write(CTRL_WR,0x03);
    LCD_nibble_write(CTRL_WR,0x03);
    DisplayDelay(39);
 
    /* Configure display */
    LCD_nibble_write(CTRL_WR,0x03);
    LCD_nibble_write(CTRL_WR,0x02);
    LCD_nibble_write(CTRL_WR,(LCD_DISPLAY_ON | LCD_TWO_LINE ));
    LCD_nibble_write(CTRL_WR,(LCD_DISPLAY_ON | LCD_TWO_LINE ));
    DisplayDelay(39);

    /* Display ON/OFF control */
    LCD_write(CTRL_WR,LCD_CURSOR_OFF);
    DisplayDelay(39);

    /* Display Clear */
    LCD_write(CTRL_WR,LCD_CLEAR);
    DisplayDelay(1530);

    /* Entry Mode Set */
    LCD_write(CTRL_WR,0x06);
    LCD_write(CTRL_WR,LCD_HOME_L1);
}

/*****************************************************************************
Name:           DisplayString   
Parameters:     position    Line number of display
                string      Pointer to data to be written to display.
                            Last character should be null.
Returns:        none
Description:    This function controls LCD writes to line 1 or 2 of the LCD.
                You need to use the defines LCD_LINE1 and LCD_LINE2 in order
                to specify the starting position.
                For example, to start at the 2nd position on line 1...
                   DisplayString(LCD_LINE1 + 1, "Hello")
*****************************************************************************/
void DisplayString(unsigned char position, _far char * string)
{
    static unsigned char next_pos = 0xFF;

    /* Set line position if needed. We don't want to if we don't need 
       to because LCD control operations take longer than LCD data
       operations. */
    if( next_pos != position)
    {
        if(position < LCD_LINE2)
        {
            /* Display on Line 1 */
            LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L1 + position) );
        }
        else
        {
            /* Display on Line 2 */
            LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L2 + position - LCD_LINE2) );
        }
        /* set position index to known value */
        next_pos = position;        
    }

    do
    {
        LCD_write(DATA_WR,*string++);
        /* increment position index */
        next_pos++;             
    }
    while(*string);


}

/*****************************************************************************
Name:           LCD_write
Parameters:     value - the value to write
                data_or_ctrl - To write value as DATA or CONTROL
                                1 = DATA
                                0 = CONTROL
Returns:        none
Description:    Writes data to display. Sends command to display.  
*****************************************************************************/
void LCD_write(unsigned char data_or_ctrl, unsigned char value)
{
    /* Write upper nibble first */
    LCD_nibble_write(data_or_ctrl, (value & 0xF0) >> 4);
    /* Write lower nibble second */
    LCD_nibble_write(data_or_ctrl, (value & 0x0F));
}

/*****************************************************************************
Name:           LCD_nibble_write
Parameters:     value - the value to write
                data_or_ctrl - To write value as DATA or CONTROL
                                1 = DATA
                                0 = CONTROL
Returns:        none
Description:    Writes data to display. Sends command to display.  
*****************************************************************************/
void LCD_nibble_write(unsigned char data_or_ctrl, unsigned char value)
{
    unsigned char ucStore;
    /* Set Register Select pin high for Data */
    if (data_or_ctrl == DATA_WR)
    {
        RS_PIN = SET_BIT_HIGH;
    }
    else
    {
        RS_PIN = SET_BIT_LOW;
    }
    /* There must be 40ns between RS write and EN write */
    DisplayDelay(5);                    
    /* EN enable chip (HIGH) */
    EN_PIN = SET_BIT_HIGH;
    /* Tiny delay */            
    DisplayDelay(5);
    /* Clear port bits used */  
    ucStore = DATA_PORT;
    ucStore &= ~DATA_PORT_MASK;
    /* OR in data */    
    ucStore |= ((value << DATA_PORT_SHIFT) & DATA_PORT_MASK );
    /* Write data to port */    
    DATA_PORT = ucStore;
    /* write delay while En High */             
    DisplayDelay(80);
    /* Latch data by dropping EN */                 
    EN_PIN = SET_BIT_LOW;
    /* Data hold delay */               
    DisplayDelay(80);                   
    if (data_or_ctrl == CTRL_WR)
    {
        /* Extra delay needed for control writes */
        DisplayDelay(0x7FF);
    }               
}

/*****************************************************************************
Name:          DisplayDelay 
Parameters:    units - Approximately in microseconds                   
Returns:       none 
Description:   Delay routine for LCD display.   
*****************************************************************************/
void DisplayDelay(unsigned long int units)
{
    unsigned long counter = units * DELAY_TIMING;
    while(counter--)
    {
        nop();
    }
}


/*****************************************************************************
Name:           Convert_16bitNumber_ToString
Description:    To convert a char number to hex ascii string of length of
                one line of display hence suitable for use in a call to DisplayString.
                Buffer supplied must be at least 9 chars long.
Parameters:     char, buffer
Return Value:   none
*****************************************************************************/
void Convert_16BitNumber_ToString(unsigned int c, char* buffer)
{
    char a;

    buffer[0] = '=';
    buffer[1] = 'H';
    buffer[2] = '\'';
    a = (char)((c & 0xF000)>> 12);
    buffer[3] = (a < 0x0A) ? (a+0x30):(a+0x37);
    a = (char)((c & 0x0F00)>> 8);
    buffer[4] = (a < 0x0A) ? (a+0x30):(a+0x37);
    a = (char)((c & 0x00F0)>> 4);
    buffer[5] = (a < 0x0A) ? (a+0x30):(a+0x37);
    a = (char)(c & 0x000F);
    buffer[6] = (a < 0x0A) ? (a+0x30):(a+0x37);

    /*  Add Spaces and a string terminating character.  */
    buffer[7] = ' ';
    buffer[8] = '\0';
}

/**********************************************************************************
Name:           DisplayuIPDemo
Description:    Clears the LCD and displays "uIP Demo" on line 1.    
Parameters:     None
Returns:        None
**********************************************************************************/
void DisplayuIPDemo(void)
{
    DisplayClear();
    DisplayString(LCD_LINE1, "uIP Demo");
}

/**********************************************************************************
Name:           DisplayLinkMissing
Description:    Clears the LCD and displays a message instructing the user to
                check the Ethernet link (cable).    
Parameters:     None
Returns:        None
**********************************************************************************/
void DisplayLinkMissing(void)
{
    DisplayClear();
    DisplayString(LCD_LINE1, "  Link  ");
    DisplayString(LCD_LINE2, "  down  ");
}


/**********************************************************************************
Name:           DisplayIPAddress
Description:    Clears the LCD and displays IP address in dotted decimal format. 
                First two decimals are displyed on line 1 and the others on
                line 2.    
Parameters:     (unsigned short []) ipaddr - IP address to display
Returns:        None
**********************************************************************************/
void DisplayIPAddress(const unsigned short ipaddr[])
{
    char dis_buf[9];
    char *p = (char *)ipaddr;
    DisplayClear();
    sprintf(dis_buf, "%d.%d.", *p, *(p+1));
    DisplayString(LCD_LINE1, dis_buf);
    sprintf(dis_buf, "%d.%d.", *(p+2), *(p+3));
    DisplayString(LCD_LINE2, dis_buf);
}

/**********************************************************************************
Name:           DisplayClear
Description:    Clears the LCD.      
Parameters:     None
Returns:        None
**********************************************************************************/
void DisplayClear(void)
{
    DisplayString(LCD_LINE1, "        ");
    DisplayString(LCD_LINE2, "        ");
}


