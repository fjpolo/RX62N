/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
  Copyright (C) 2008. Renesas Technology Corp., All Rights Reserved.
*******************************************************************************
* File Name    : lcd.h
* Version      : 1.00
* Description  : LCD Module utility functions.             
*              : Written for KS0066u compatible LCD Module.
*              : (8 characters by 2 lines)                 
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 20.06.2007 1.00    First Release 
*         : 23.08.2010 1.01    Updated for RSK+RX62N 
******************************************************************************/

#ifndef LCD_H
#define LCD_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/* RS Register Select pin */
#define RS_PIN              PORT8.DR.BIT.B4
/* Display Enable pin */    
#define EN_PIN              PORT8.DR.BIT.B5

/* Data bus port */
#define DATA_PORT           PORT9.DR.BYTE
/* Bit mask from entire port */     
#define DATA_PORT_MASK      0xF0
/* Number of bits data needs to shift */    
#define DATA_PORT_SHIFT     4       

#define DATA_WR 1
#define CTRL_WR 0

/* Set to ensure base delay of 1mS minimum */
#define DELAY_TIMING        0x0F
/* number of lines on the LCD display */
#define NUMB_CHARS_PER_LINE 8
/* Maximum charactors per line of LCD display. */   
#define MAXIMUM_LINES       2       

#define LCD_LINE1 0
#define LCD_LINE2 16

/* Command definitions */

/* Clear LCD display and home cursor */
#define LCD_CLEAR        0x01
/* move cursor to line 1 */
#define LCD_HOME_L1      0x80
/* move cursor to line 2 */      
#define LCD_HOME_L2      0xC0
/* Cursor auto decrement after R/W */  
#define CURSOR_MODE_DEC  0x04
/* Cursor auto increment after R/W */
#define CURSOR_MODE_INC  0x06
/* Setup, 4 bits,2 lines, 5X7 */
#define FUNCTION_SET     0x28
/* Display ON with Cursor */
#define LCD_CURSOR_ON    0x0E
/* Display ON with Cursor off */
#define LCD_CURSOR_OFF   0x0C
/* Display on with blinking cursor */
#define LCD_CURSOR_BLINK 0x0D
/*Move Cursor Left One Position */
#define LCD_CURSOR_LEFT  0x10
/* Move Cursor Right One Position */
#define LCD_CURSOR_RIGHT 0x14     

#define LCD_DISPLAY_ON   0x04
#define LCD_TWO_LINE     0x08

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
void InitialiseDisplay( void );
void DisplayString(unsigned char position, _far char * string);
void LCD_write(unsigned char data_or_ctrl, unsigned char value);
void LCD_nibble_write(unsigned char data_or_ctrl, unsigned char value);
void DisplayDelay(unsigned long int units);
void Convert_16BitNumber_ToString(unsigned int c, char* buffer);
void DisplayClear(void);
void DisplayuIPDemo(void);
void DisplayLinkMissing(void);
void DisplayIPAddress(const unsigned short ipaddr[]);

#endif
