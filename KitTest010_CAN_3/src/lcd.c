/***********************************************************************************
FILE NAME		lcd.c
  
DESCRIPTION		LCD Module utility functions.
				Written for KS0066u compatible LCD Module.
				(8 characters by 2 lines)

Copyright   : 2010 Renesas Electronics Europe Ltd.
Copyright   : 2010 Renesas Electronics Corporation.
All Rights Reserved

***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
20.06.2007 RTE-VNA First Release
*********************************************************************/

/**********************************************************************************
System Includes
***********************************************************************************/
#include <machine.h>

/**********************************************************************************
User Includes
***********************************************************************************/
/* sfr121.h provides a structure to access all of the device registers. */
#include "lcd.h"
#include "iodefine.h"
#include "rskRX62Ndef.h"

/**********************************************************************************
User Program Code
***********************************************************************************/

/*****************************************************************************
Name:          	InitDisplay 
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
Name:        	DisplayString   
Parameters:  	position	Line number of display
			    string		Pointer to data to be written to display.
			    			Last character should be null.
Returns:        none
Description:    This function controls LCD writes to line 1 or 2 of the LCD.
                You need to use the defines LCD_LINE1 and LCD_LINE2 in order
				to specify the starting position.
				For example, to start at the 2nd position on line 1...
				   DisplayString(LCD_LINE1 + 1, "Hello")
*****************************************************************************/
void DisplayString(uint8_t position, uint8_t * string)
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

	/*	Add Spaces and a string terminating character.	*/
	buffer[7] = ' ';
	buffer[8] = '\0';
}/* end Convert_16BitNumber_ToString() */

/*****************************************************************************
Name:          LcdShow2DigHex
Parameters:    data, LCD-display line 1 or 2
Returns:       -
Description:   convert one byte to 2 ascii chars and show them.
*****************************************************************************/
void LcdShow2DigHex( uint8_t  data, uint8_t  position)
{
	uint8_t stringdata[3]; /* One char extra for trailing NULL. */

	/* Convert data to string for LCD. */
	IntToAsciiHex( stringdata, /* Pointer to where to put string */
				   (uint8_t) 2, /* Nr characters output string will have */
				   data ); /* uint32_t value */
	
	DisplayString((unsigned char)position, stringdata);
}/* end LcdShow2DigHex() */

/*****************************************************************************
Name:          LcdShow4DigHex
Parameters:    msbyte, lsbyte, LCD-display line 1 or 2
Returns:       -
Description:   Convert two bytes to 4 ascii characters and show them to LCD 
               at 5th position line 1 or 2.
*****************************************************************************/
void LcdShow4DigHex(	uint8_t  	msbyte,
						uint8_t  	lsbyte,
						uint8_t	    position)
{
	uint32_t intdata;
	uint8_t stringdata[FOUR_DIGITS_PLUS_NULL];

	intdata = ((uint32_t) msbyte)<<8;
	intdata = intdata + (uint32_t)lsbyte;

	/* Convert data to string for LCD. */
	IntToAsciiHex( 	stringdata, /* Pointer to where to put string */
				   	FOUR_DIGITS_PLUS_NULL-1, /* Nr characters output string will have */
					intdata  ); /* Value */
	
	DisplayString(position, stringdata);
}/* end LcdShow4DigHex() */

/*****************************************************************************
Name:		IntToAsciiHex   
Parameters:	dest_string
				Pointer to a buffer will the string will reside
			min_digits
				Specifies the number of characters the output string will
				have. Leading zeros will be written as '0' characters.
Returns:	A pointer to the string's NULL character in the string that was just
				created.
Does:		Converts a passed uint32_t into a ASCII	string represented in 
				Hexidecimal format.
*****************************************************************************/
void IntToAsciiHex( uint8_t*    dest_string,
					uint8_t		min_digits, 
					uint32_t 	value )
{
	uint32_t i, total_digits = 0;
	uint8_t  buff[4];
	
	for(i=0; i<4; i++)
	{
		buff[i] = (uint8_t)(value & 0x0F);
		value = value >> 4;
		if( buff[i] <= 9)
			buff[i] += '0';
		else
			buff[i] = (uint8_t)(buff[i] - 0xA + 'A');

		if(buff[i] != '0')
			total_digits = i+1;
	}

	if( total_digits < 	min_digits)
		total_digits = min_digits;

	i = total_digits;
	while(i)
	{
		*dest_string++ = buff[i-1];
		i--;
	}

	*dest_string = 0;
}/* end IntToAsciiHex() */
/* eof */