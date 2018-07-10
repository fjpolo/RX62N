/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2017. Renesas Electronics Corp., All Rights Reserved.
******************************************************************************
* File Name        R_CAN_API.C
* Version          2.05
* Platform         ROK5562N
* Description      CAN API function definitions
* Operation        See example usage API_DEMO.C
* Limitations      :
******************************************************************************
* History       Ver.    Description
* Mar 22 '10    1.00    For RX62N with new CAN API.
* Apr 15 '10            Remote frame handling added.
* Apr 22 '10            Port configuration moved to config.h. User just 
                            sets port and pin number for the transceiver control 
                            ports. No need to change driver.
                        R_CAN_Control: Enter Sleep added.
                        R_CAN_PortSet: Modes automatically now enter and exit
                             Halt mode, so user need just one call to change
                             mode.
                        R_CAN_TxStopMsg(): TRMREQ to 0, then TRMABT clear.
                        R_CAN_RxSetMask(): Halt CAN before mask change, resume 
                            after.
                        R_CAN_RxPoll(): Function rewritten to use INVALDATA flag.
                            R_CAN_NOT_OK added; "No message waiting or currently 
                            being written".
                        All RETURN values spelled out in all function headers.
                        Return values added and changed for some APIs.
* Jun 7 2010             Changed TRM_ACTIVE -> SENTDATA in R_CAN_WaitTxRx() since 
                            TRM_ACTIVE is low for a while after TRMREQ is set high.                
                        Increased MAX_CAN_SW_DELAY from 0x1000 to 0x2000. If the
                            TxCheck function is not used, the timer could time out
                             and the mailbox will not send properly if the user 
                            ignores the api_status warning that WaitTxRx timed out.
* Jan 10 2011   2.00    Updated to support CAN extended ID mode. Added functions: 
                        R_CAN_TxSetXid, R_CAN_RxSetXid, R_CAN_TxSetFifoXid. 
                        Added limited support for FIFO transmit mode.
                        In R_CAN_WaitTxRx changed to while (...SENTDATA == 0)...
* Mar 22 2012           Removed RESET_CAN_SW_TMR. Counter reset at function start.     
* Oct 12 2012           Changed R_CAN_Control() sleep mode, to ensure that Halt mode
                            is entered before exiting sleep. 
                        Got rid of can_frame_t. can_frame_t to cover both Standard 
                            and Ext frames.                    
* Jun 7 2016    2.04    All API calls' first argument now have 'ch_nr' instead of '0'.
                        When writing 0 to bits NEWDATA, SENTDATA, MSGLOST, TRMABT, 
                            RECREQ, and TRMREQ by a program, do not use the logic 
                            operation instruction (AND). Write 0 to only the specified 
                            bit and write 1 to the other bits before using the transfer 
                            (MOV) instruction. See HW manual footnote.
                        R_CAN_Control() case OPERATE_CANMODE; check also that Halt 
                            Status bit is 0.
* Dec 8 2017    2.05    Fixed bug in extended ID mode and Mixed ID mode handling.
                        Some code cleanup.
                        Removed unused FIFO mode experimental code
                        Moved port configuration to config_r_can_rapi.h
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>
#include <machine.h>
#include "config_r_can_rapi.h"
#if (USE_FIT_BSP == 1)
#include "platform.h"
#else
#include "iodefine.h"
#endif

#include "r_can_api.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Macro definitions
******************************************************************************/
/* These macros are for determining if some can while loop times out. If they do,
the canx_sw_err variable will be non zero. This is to prevent an error in the 
can peripheral and driver from blocking the CPU indefinatly. */
#define DEC_CHK_CAN_SW_TMR      (--can_tmo_cnt != 0)
/* You may change MAX_CAN_SW_DELAY, but you risk the function timing out even 
more if set lower. As long as you check the return value and are aware that a 
timeout occurred, you can set the wait time as you wish, the shorter the time, 
the more likely you are to time out. There is no "correct" value, but do not 
set very low without a good reason. */
#define MAX_CAN_SW_DELAY        (0x2000)
#define CHECK_MBX_NR            {if (mbox_nr > 31) return R_CAN_SW_BAD_MBX;}
/* Board specific port defines. */
#define CAN_TRX_DDR(x, y)               CAN_TRX_DDR_PREPROC(x, y)
#define CAN_TRX_DDR_PREPROC(x, y)       (PORT ## x .DDR.BIT.B ## y)
#define CAN_TRX_DR(x, y)                CAN_TRX_DR_PREPROC(x, y)
#define CAN_TRX_DR_PREPROC(x, y)        (PORT ## x .DR.BIT.B ## y)
/* Define a mask for MSB of a long to serve as an extended ID mode bit flag. */
/* Extended ID occupies lower 29 bits, so use this to mask off upper 3 bits. */ 
#define XID_MASK    0xE0000000
/* Define a mask for the 11 bits that make up a standard ID. */ 
#define SID_MASK    0x000007FF  

/******************************************************************************
Global variables and functions imported (externs)
******************************************************************************/
/******************************************************************************
Constant definitions
*****************************************************************************/
/******************************************************************************
Global variables and functions private to the file
******************************************************************************/
/* Data */
/* Functions */
static void     CanClearSentData(const uint32_t ch_nr, const uint32_t mbox_nr);
static uint32_t R_CAN_WaitTxRx(const uint32_t ch_nr, const uint32_t mbox_nr);
static void     R_CAN_ConfigCANinterrupts(const uint32_t ch_nr);

/******************************************************************************

                        C A N 0   F U N C T I O N S

******************************************************************************/
/*******************************************************************************
Function Name:  R_CAN_Create
Description:    Configure the CAN peripheral.
Parameters:     ch_nr
Return value:   R_CAN_OK            Action completed successfully.
                R_CAN_SW_BAD_MBX    Bad mailbox number.
                R_CAN_BAD_CH_NR     The channel number does not exist.
                R_CAN_SW_RST_ERR    The CAN peripheral did not enter Reset mode.
                See also R_CAN_Control return values.
*******************************************************************************/
uint32_t R_CAN_Create(const uint32_t ch_nr)
{
    uint32_t api_status = R_CAN_OK;
    uint32_t i, j;
    /* A faulty CAN peripheral block, due to HW, FW could potentially block (hang) 
    the program at a while-loop. To prevent this, a sw timer in the while-loops
    will time out enabling the CPU to continue. */
    uint32_t can_tmo_cnt = MAX_CAN_SW_DELAY;   

    volatile struct st_can __evenaccess * can_block_p;
    
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;
    }
    else
    {
        return R_CAN_BAD_CH_NR;
    }

    /* Exit Sleep mode. */
    api_status |= R_CAN_Control(ch_nr, EXITSLEEP_CANMODE);
    
    /* Sleep -> RESET mode. ****************************************************/
    api_status |= R_CAN_Control(ch_nr, RESET_CANMODE);
    
    /*** Setting of CAN0 Control register.***/
    /* BOM:    Bus Off recovery mode acc. to IEC11898-1 */
    can_block_p->CTLR.BIT.BOM = 0;

	/* MBM: Select normal mailbox mode. */
	can_block_p->CTLR.BIT.MBM = 0;
    
    /* IDFM: Select ID mode. */
    can_block_p->CTLR.BIT.IDFM = FRAME_ID_MODE;
    
    /*     :    0 = Overwrite mode: Latest message overwrites old.
            1 = Overrun mode: Latest message discarded. */
    can_block_p->CTLR.BIT.MLM = 0;
    /* TPM: ID priority mode. */
    can_block_p->CTLR.BIT.TPM = 0;
    /* TSRC: Only to be set to 1 in operation mode */
    can_block_p->CTLR.BIT.TSRC = 0;
    /* TSPS: Update every 8 bit times */
    can_block_p->CTLR.BIT.TSPS = 3;

    /* Set BAUDRATE */
    R_CAN_SetBitrate(ch_nr);

    /* Reset -> HALT mode ******************************************************/
    api_status |= R_CAN_Control(ch_nr, HALT_CANMODE);

    /* Configure CAN interrupts. */ 
    R_CAN_ConfigCANinterrupts(ch_nr);

    /* Configure mailboxes in Halt mode. */
    for (i = 0; i < 32; i++)
    {
        can_block_p->MB[i].ID.LONG = 0x00;
        can_block_p->MB[i].DLC.WORD = 0x00;
        for (j = 0; j < 8; j++)
        {
            can_block_p->MB[i].DATA[j] = 0x00;
        }
        for (j = 0; j < 2; j++)
        {
            can_block_p->MB[i].TS.WORD = 0x00;
        }
    }    

	/* Mask invalid for all mailboxes by default. */
	can_block_p->MKIVLR = 0xFFFFFFFF;

    /* Halt -> OPERATION mode **************************************************/
    /* Note: EST and BLIF flag go high here when stepping code in debugger. */
    api_status |= R_CAN_Control(ch_nr, OPERATE_CANMODE);

    /* Time Stamp Counter reset. Set the TSRC bit to 1 in CAN Operation mode. */
    can_block_p->CTLR.BIT.TSRC = 1;

    while ((can_block_p->CTLR.BIT.TSRC) && DEC_CHK_CAN_SW_TMR)
    {
    }

    if (can_tmo_cnt == 0)
    {
        api_status |= R_CAN_SW_TSRC_ERR;
    }
    
    /* Check for errors so far, report, and clear. */
    if (can_block_p->STR.BIT.EST)
    {
        api_status |= R_CAN_SW_RST_ERR;
    }

    /* Clear Error Interrupt Factor Judge Register. */
    if (can_block_p->EIFR.BYTE)
    {
        api_status |= R_CAN_SW_RST_ERR;
    }

    can_block_p->EIFR.BYTE = 0x00;

    /* Clear Error Code Store Register. */
    if (can_block_p->ECSR.BYTE)
    {
        api_status |= R_CAN_SW_RST_ERR;
    }
    can_block_p->ECSR.BYTE = 0x00;

    return api_status;
}/* end R_CAN_Create() */

/***********************************************************************************
Function Name:  R_CAN_PortSet
Description:    Configures the MCU and transceiver port pins. This function is 
                responsible for configuring the MCU and transceiver port pins. 
                Transceiver port pins such as Enable will vary depending on design, 
                and this fucntion must then be modified. The function is also used 
                to enter the CAN port test modes, such as Listen Only.
            
                Typical TJA1041 transceiver voltages with CAN active for RX/62N 
                RSK (ROK5562N0C):
                    PIN:        Voltage
                    1    TXD        3.25
                    2    GND        0.00
                    3    VCC        5.08
                    4    RXD        3.20
                    5    VIO        3.25
                    6    EN         3.25
                    7    INH        5.08
                    8    ERR        0.10
                    9    WAKE       0.00
                    10   VBAT       5.08
                    11   SPLIT      2.57
                    12   CANL       2.56
                    13   CANH       2.56
                    14   STB        3.25
Parameters:     ch_nr 
                action_types: ENABLE, DISABLE, CANPORT_TEST_LISTEN_ONLY, 
                CANPORT_TEST_0_EXT_LOOPBACK, CANPORT_TEST_1_INT_LOOPBACK, and
                CANPORT_RETURN_TO_NORMAL which is the default; no need to call 
                unless another test mode was invoked previously.
Return value:   R_CAN_OK                Action completed successfully.
                R_CAN_SW_BAD_MBX        Bad mailbox number.
                R_CAN_BAD_CH_NR         The channel number does not exist.
                R_CAN_BAD_ACTION_TYPE   No such action type exists for this function.
                R_CAN_SW_HALT_ERR       The CAN peripheral did not enter Halt mode.
                R_CAN_SW_RST_ERR        The CAN peripheral did not enter Reset mode.
                See also R_CAN_Control return values.
***********************************************************************************/
uint32_t R_CAN_PortSet(const uint32_t ch_nr, const uint32_t action_type)
{  
    uint32_t api_status = R_CAN_OK;
    volatile struct st_can __evenaccess * can_block_p;
    
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;        /* Point to CAN0 peripheral block. */
    }
    else
    {
        return R_CAN_BAD_CH_NR;        /* Channel does not exist for this MCU. */
    }
        
    switch (action_type)
    {
		case ENABLE:
			/* Enable the CTX0 and CRX0 pins. */

			/* Port function control register J (PFJCAN) is used to configure I/O pin.
			1: The CTX0 and CRX0 pins are enabled. */
			PFJCAN_REG = 1;

			CTX0_PORT_PIN = 1;	//CTX0 is output
			CRX0_PORT_PIN = 0;	//CRX0 is input

			/* Set Input Buffer Control Register for CRX0 to connect input to CAN peripheral. */
			CRX0_PORT_INP_CTRL = 1;

			/* Configure CAN0 STBn pin. See config.h. */
			CAN_TRX_DDR( CAN0_TRX_STB_PORT, CAN0_TRX_STB_PIN ) = 1;

			CAN_TRX_DR(CAN0_TRX_STB_PORT, CAN0_TRX_STB_PIN) = CAN0_TRX_STB_LVL;

			/* Configure CAN0 EN pin. */
			CAN_TRX_DDR( CAN0_TRX_ENABLE_PORT, CAN0_TRX_ENABLE_PIN ) = 1;
			CAN_TRX_DR(CAN0_TRX_ENABLE_PORT, CAN0_TRX_ENABLE_PIN) = CAN0_TRX_ENABLE_LVL;
		break;

		case DISABLE:
			/* Configure CAN0 TX and RX pins. */

			/* Port function control register J (PFJCAN) is used to configure I/O pin
			0: The CTX0 and CRX0 pins are disabled. */
			PFJCAN_REG = 0;

			/* Configure CAN0 STBn pin. See config.h. */
			CAN_TRX_DDR( CAN0_TRX_STB_PORT, CAN0_TRX_STB_PIN ) = 1;
			CAN_TRX_DR(CAN0_TRX_STB_PORT, CAN0_TRX_STB_PIN) = !CAN0_TRX_STB_LVL; //Negated level

			/* Configure CAN0 EN pin. */
			CAN_TRX_DDR( CAN0_TRX_ENABLE_PORT, CAN0_TRX_ENABLE_PIN ) = 1;
			CAN_TRX_DR(CAN0_TRX_ENABLE_PORT, CAN0_TRX_ENABLE_PIN) = !CAN0_TRX_ENABLE_LVL; //Negated level
		break;

		/* Run in Listen Only test mode. */
		case CANPORT_TEST_LISTEN_ONLY:
			api_status = R_CAN_Control(ch_nr, HALT_CANMODE);
			can_block_p->TCR.BYTE = 0x03;
			api_status |= R_CAN_Control(ch_nr, OPERATE_CANMODE);
			api_status |= R_CAN_PortSet(0, ENABLE);
		break;

		/* Run in External Loopback test mode. */
		case CANPORT_TEST_0_EXT_LOOPBACK:
			api_status = R_CAN_Control(ch_nr, HALT_CANMODE);
			can_block_p->TCR.BYTE = 0x05;
			api_status |= R_CAN_Control(ch_nr, OPERATE_CANMODE);
			api_status |= R_CAN_PortSet(0, ENABLE);
		break;

		/* Run in Internal Loopback test mode. */
		case CANPORT_TEST_1_INT_LOOPBACK:
			api_status = R_CAN_Control(ch_nr, HALT_CANMODE);
			can_block_p->TCR.BYTE = 0x07;
			api_status |= R_CAN_Control(ch_nr, OPERATE_CANMODE);
		break;

		/* Return to default CAN bus mode.
		This is the default setting at CAN reset. */
		case CANPORT_RETURN_TO_NORMAL:
			api_status = R_CAN_Control(ch_nr, HALT_CANMODE);
			can_block_p->TCR.BYTE = 0x00;
			api_status |= R_CAN_Control(ch_nr, OPERATE_CANMODE);
			api_status |= R_CAN_PortSet(0, ENABLE);
		break;

		default:
			/* Bad action type. */
			api_status = R_CAN_BAD_ACTION_TYPE;
		break;
    }
    return api_status;
}/* end R_CAN_PortSet() */

/*******************************************************************************
Function Name:  R_CAN_Control
Description:    Controls transition to CAN operating modes determined by the CAN 
                Control register. For example, the Halt mode should be used to 
                later configure a recieve mailbox. 
Parameters:     ch_nr
                action_type: EXITSLEEP_CANMODE, ENTERSLEEP_CANMODE,
                RESET_CANMODE, HALT_CANMODE, OPERATE_CANMODE.
Return value:   R_CAN_OK                Action completed successfully.
                R_CAN_SW_BAD_MBX        Bad mailbox number.
                R_CAN_BAD_CH_NR         The channel number does not exist.
                R_CAN_BAD_ACTION_TYPE   No such action type exists for this function.
                R_CAN_SW_WAKEUP_ERR     The CAN peripheral did not wake up from Sleep mode.
                R_CAN_SW_SLEEP_ERR      The CAN peripheral did enter Sleep mode.
                R_CAN_SW_RST_ERR        The CAN peripheral did not enter Halt mode.
                R_CAN_SW_HALT_ERR       The CAN peripheral did not enter Halt mode.
                R_CAN_SW_RST_ERR        The CAN peripheral did not enter Reset mode.
                See also R_CAN_PortSet return values.
*******************************************************************************/
uint32_t R_CAN_Control(const uint32_t ch_nr, const uint32_t action_type)
{
    uint32_t api_status = R_CAN_OK;
    uint32_t can_tmo_cnt = MAX_CAN_SW_DELAY;
    volatile struct st_can __evenaccess * can_block_p;
    
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;        /* Point to CAN0 peripheral block. */
    }
    else
    {
        return R_CAN_BAD_CH_NR;        /* Channel does not exist for this MCU. */
    }

    switch (action_type)
    {
		case EXITSLEEP_CANMODE:
			/* Set to, and ensure that RCAN returns in, Sleep mode.
			Write to the SLPM bit (in CAN Reset or) CAN Halt mode,
			but if we are in Sleep mode, we should already also be
			in Halt mode. */
			can_block_p->CTLR.BIT.SLPM = CAN_NOT_SLEEP;
			while ((can_block_p->STR.BIT.SLPST) && DEC_CHK_CAN_SW_TMR)
			{
				nop();
			}
			if (can_tmo_cnt == 0)
			{
				api_status = R_CAN_SW_WAKEUP_ERR;
			}
		break;

		case ENTERSLEEP_CANMODE:
			/* Set to, and ensure that RCAN returns in, the Sleep state.
			Write to the SLPM bit in CAN Reset or CAN Halt modes. */
			api_status = R_CAN_Control(ch_nr, HALT_CANMODE);

			can_block_p->CTLR.BIT.SLPM = CAN_SLEEP;
			while ((!can_block_p->STR.BIT.SLPST) && DEC_CHK_CAN_SW_TMR)
			{
				nop();
			}
			if (can_tmo_cnt == 0)
			{
				api_status = R_CAN_SW_SLEEP_ERR;
			}
		break;

		case RESET_CANMODE:
			/* Set to, and ensure that RCAN is in, the Reset state. */
			can_block_p->CTLR.BIT.CANM = CAN_RESET;

			while ((!can_block_p->STR.BIT.RSTST) && DEC_CHK_CAN_SW_TMR)
			{
			}

			if (can_tmo_cnt == 0)
			{
				api_status = R_CAN_SW_RST_ERR;
			}
		break;

		case HALT_CANMODE:
			/* Set to, and ensure that RCAN is in, the Halt state. */
			/* The CAN module enters CAN Halt mode after waiting for the end of
			message reception or transmission. */
			can_block_p->CTLR.BIT.CANM = CAN_HALT;

			while ((!can_block_p->STR.BIT.HLTST) && DEC_CHK_CAN_SW_TMR)
            {
            }

			if (can_tmo_cnt == 0)
			{
				api_status = R_CAN_SW_HALT_ERR;
			}

		break;

		case OPERATE_CANMODE:
			/* Take CAN out of Stop mode. */
			SYSTEM.MSTPCRB.BIT.MSTPB0  =  0;
			while ((SYSTEM.MSTPCRB.BIT.MSTPB0) && DEC_CHK_CAN_SW_TMR)
			{
				nop();
			}

			/* Set to Operate mode. */
			can_block_p->CTLR.BIT.CANM = CAN_OPERATION;

			/* Ensure that RCAN is in Operation mode. */
			while (((can_block_p->STR.BIT.HLTST) || (can_block_p->STR.BIT.RSTST)) && DEC_CHK_CAN_SW_TMR)
			{
			}

			if (can_tmo_cnt == 0)
			{
				api_status = R_CAN_SW_RST_ERR;
			}
		break;
		default:
			api_status = R_CAN_BAD_ACTION_TYPE;
		break;
    }

    return api_status;
}/* end R_CAN_Control() */

/*******************************************************************************
Function Name:  R_CAN_TxSet
Description:    Set up a CAN mailbox to transmit.
Parameters:     Channel nr.
                Mailbox nr.
                frame_p - pointer to a data frame structure.
                remote - REMOTE_FRAME to send remote request, DATA_FRAME for 
                sending normal dataframe.
Return value:   R_CAN_OK                The mailbox was set up for transmission.
                R_CAN_SW_BAD_MBX        Bad mailbox number.
                R_CAN_BAD_CH_NR         The channel number does not exist.
                R_CAN_BAD_ACTION_TYPE   No such action type exists for this 
                                        function.
*******************************************************************************/
uint32_t R_CAN_TxSet(   const uint32_t             ch_nr,
                        const uint32_t             mbox_nr,
                        const can_frame_t*     frame_p,
                        const uint32_t             frame_type)
{
    uint32_t api_status = R_CAN_OK;
    uint32_t i;
    volatile struct st_can __evenaccess * can_block_p;
    
    CHECK_MBX_NR
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;        /* Point to CAN0 peripheral block. */
    }
    else
    {
        return R_CAN_BAD_CH_NR;        /* Channel does not exist for this MCU. */
    }

    /* Wait for any previous transmission to complete. */
    api_status = R_CAN_WaitTxRx(ch_nr, mbox_nr);

    /* Interrupt disable the mailbox.in case it was a receive mailbox */
    can_block_p->MIER &= ~(bit_set[mbox_nr]);

    /* Clear message mailbox control register. Setting TRMREQ to 0 should abort. Do a byte-write to avoid read-modify-write with HW writing another bit inbetween. See HW manual footnote.
     * Do it twice since "Bits SENTDATA and TRMREQ cannot be set to 0 simultaneously." */
    can_block_p->MCTL[mbox_nr].BYTE = 0;
    can_block_p->MCTL[mbox_nr].BYTE = 0;

    /*** Set Mailbox ID based on ID mode ***/
    if (frame_p->id & XID_MASK)    /* Check for XID flag bit set in ID field */
    {
        /* Set message mailbox buffer Extended ID, masking off temporary XID flag bit. */      
        can_block_p->MB[mbox_nr].ID.LONG = (frame_p->id & (~XID_MASK));
        if (can_block_p->CTLR.BIT.IDFM == MIXED_ID_MODE)
        {
            can_block_p->MB[mbox_nr].ID.BIT.IDE = 1;   /* Mixed mode; select to send extended frame. */
        }
    }
    else
    {
        /* Set message mailbox buffer Standard ID. Put only the lower 11 bit in the SID. */
        can_block_p->MB[mbox_nr].ID.BIT.SID = (frame_p->id & SID_MASK);

        /* Always set IDE to 0 (unless mixed mode and extended frame as above). */
        can_block_p->MB[mbox_nr].ID.BIT.IDE = 0;
    }

    /* Set the Data Length Code */
    can_block_p->MB[mbox_nr].DLC.BIT.DLC = frame_p->dlc;
    
    /* Frame select: Data frame = 0, Remote = 1 */
    if (frame_type == REMOTE_FRAME)
    {
        can_block_p->MB[mbox_nr].ID.BIT.RTR = 1;
    }
    else
    { 
        can_block_p->MB[mbox_nr].ID.BIT.RTR = 0;
    }
      
    /* Copy frame data into mailbox */
    for (i = 0; ((i < frame_p->dlc) && (i<8)); i++)
    {
        can_block_p->MB[mbox_nr].DATA[i] = frame_p->data[i];
    }
    /**********************/

    #ifndef USE_CAN_POLL
        /* Interrupt enable the mailbox */
        can_block_p->MIER |= (bit_set[mbox_nr]);
    #endif

    R_CAN_Tx(ch_nr, mbox_nr);
    
    return api_status;
} /* end R_CAN_TxSet() */

/*******************************************************************************
Function Name:  R_CAN_TxSetXid
Description:    Set up a CAN mailbox to transmit in extended ID mode.
                Uses temporary copy of the can_frame data structure to set the 
                MSB of the frame ID field to serve as a flag to indicate extended 
                ID mode, then calls the regular R_CAN_TxSet() function passing 
                along all the parameters.
Arguments:      Channel nr.
                Mailbox nr.
                frame_p - pointer to a data frame structure.
                remote - REMOTE_FRAME to send remote request, DATA_FRAME for 
                sending normal dataframe.
Return value:   R_CAN_OK                The mailbox was set up for transmission.
                R_CAN_SW_BAD_MBX        Bad mailbox number.
                R_CAN_BAD_CH_NR         The channel number does not exist.
                R_CAN_BAD_ACTION_TYPE   No such action type exists for this 
                                        function.
*******************************************************************************/
uint32_t R_CAN_TxSetXid(const uint32_t     ch_nr,
                        const uint32_t     mbox_nr,
                        can_frame_t*       frame_p,
                        const uint32_t     frame_type)
{
    can_frame_t temp_frame;
    uint32_t    api_status;
    
    /* Copy the user frame to a temporary frame to which we add the Xid bit,
    so that 29-bit ID will be used by R_CAN_TxSet(). The original frame is left
    untouched (in case user later wants to send the same frame with standard ID). */
    memcpy(&temp_frame, frame_p, sizeof(can_frame_t));

    temp_frame.id |= XID_MASK;    /* Set XID flag bit set in ID field */
    api_status = R_CAN_TxSet(ch_nr, mbox_nr, (can_frame_t*)&temp_frame, frame_type);
           
    return api_status; 
}/* end R_CAN_TxSetXid() */
                        

/*******************************************************************************
Function Name:  R_CAN_Tx
Description:    Starts actual message transmission onto the CAN bus.
Parameters:     Channel nr.
                Mailbox nr.
Return value:   R_CAN_OK            The mailbox was set to transmit a previously 
                                    configured mailbox.
                R_CAN_SW_BAD_MBX    Bad mailbox number.
                R_CAN_BAD_CH_NR     The channel number does not exist.
                R_CAN_SW_SET_TX_TMO Waiting for previous transmission to finish 
                                    timed out.
                R_CAN_SW_SET_RX_TMO Waiting for previous reception to complete 
                                    timed out.
*******************************************************************************/
uint32_t R_CAN_Tx(const uint32_t ch_nr, const uint32_t mbox_nr)
{
    uint32_t api_status = R_CAN_OK;
    volatile struct st_can __evenaccess * can_block_p;
    
    CHECK_MBX_NR

    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;        /* Point to CAN0 peripheral block. */
    }
    else
    {
        return R_CAN_BAD_CH_NR;        /* Channel does not exist for this MCU. */
    }

    /* Wait for any previous transmission to complete. */
    api_status = R_CAN_WaitTxRx(ch_nr, mbox_nr);

    /* Clear SentData flag since we are about to send anew. */
    CanClearSentData(ch_nr, mbox_nr);
    
    /* Set TrmReq bit to "1" */
	can_block_p->MCTL[mbox_nr].BIT.TX.TRMREQ = 1;
    
    return api_status;
}/* end R_CAN_Tx() */

/*****************************************************************************
Name:           R_CAN_TxCheck
Parameters:     Channel nr.
                Mailbox nr.
Description:    Use to check a mailbox for a successful data frame transmission.
                Primarily used when polling to check that message was sent, so 
                that the next in series of messages can be sent. To do this when 
                using CAN interrupts, this function can be called to check which 
                mailbox caused the interrupt.
Return value:   R_CAN_OK            Transmission was completed successfully.
                R_CAN_SW_BAD_MBX    Bad mailbox number.
                R_CAN_BAD_CH_NR     The channel number does not exist.
                R_CAN_MSGLOST        Message was overwritten or lost.
                R_CAN_NO_SENTDATA    No message was sent.
*****************************************************************************/
uint32_t R_CAN_TxCheck(const uint32_t ch_nr, const uint32_t mbox_nr)
{
    uint32_t api_status = R_CAN_OK;
    volatile struct st_can __evenaccess * can_block_p;
    
    CHECK_MBX_NR
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;
    }
    else
    {
        return R_CAN_BAD_CH_NR;
    }

    if (can_block_p->MCTL[mbox_nr].BIT.TX.SENTDATA == 0)
    {
        api_status = R_CAN_NO_SENTDATA;
    }
    else
    {
        CanClearSentData(ch_nr, mbox_nr);        /* Clear SentData flag. */
    }

    return api_status;
}/* end R_CAN_TxCheck() */

/*****************************************************************************
Name:            R_CAN_TxStopMsg
Parameters:        Channel nr.
                Mailbox nr.
Description:      Stop a mailbox that has been asked to transmit a frame. If the 
                message was not stopped, R_CAN_SW_ABORT_ERR is returned. Note 
                that the cause of this could be that the message was already sent. 
Return value:     R_CAN_OK            Action completed successfully.
                R_CAN_SW_BAD_MBX    Bad mailbox number.
                R_CAN_BAD_CH_NR     The channel number does not exist.
                R_CAN_SW_ABORT_ERR    Waiting for an abort timed out.
*****************************************************************************/
uint32_t R_CAN_TxStopMsg(const uint32_t ch_nr, const uint32_t mbox_nr)
{
    uint32_t api_status = R_CAN_OK;
    uint32_t can_tmo_cnt = MAX_CAN_SW_DELAY;   
    volatile struct st_can __evenaccess * can_block_p;
    
    CHECK_MBX_NR
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;        /* Point to CAN0 peripheral block. */
    }
    else
    {
        return R_CAN_BAD_CH_NR;        /* Channel does not exist for this MCU. */
    }
    
    /* Clear message mailbox control register. Setting TRMREQ to 0 should abort. Do a byte-write to avoid read-modify-write with HW writing another bit inbetween. See HW manual footnote.
     * Do it twice since "Bits SENTDATA and TRMREQ cannot be set to 0 simultaneously." */
    can_block_p->MCTL[mbox_nr].BYTE = 0;
    can_block_p->MCTL[mbox_nr].BYTE = 0;

    /* Wait for abort. */
    while ((can_block_p->MCTL[mbox_nr].BIT.TX.TRMABT) && DEC_CHK_CAN_SW_TMR)
    {
    }

    if (can_tmo_cnt == 0)
    {
        api_status = R_CAN_SW_ABORT_ERR;
    }
    
    /* Clear abort flag. Do a byte-write to avoid read-modify-write with HW writing another bit inbetween. */
    can_block_p->MCTL[mbox_nr].BYTE = 0;
    
    return api_status;
}/* end R_CAN_TxStopMsg() */

/*****************************************************************************
Name:            CanClearSentData
Parameters:        Channel nr.
                Mailbox nr.
Description:      Use in poll mode for checking successful data frame transmission.
Return value:     CAN API code (CAN_R_CAN_OK if mailbox has sent.)
*****************************************************************************/
#pragma inline(CanClearSentData)
static void CanClearSentData(const uint32_t ch_nr, const uint32_t mbox_nr)
{ 
    volatile struct st_can __evenaccess * can_block_p;
    
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;        /* Point to CAN0 peripheral block. */
    }
    else
    {
        return;        /* Channel does not exist for this MCU. */
    }

    /* Clear SENTDATA.
     * Do a byte-write to avoid read-modify-write with HW writing another bit inbetween. See HW manual footnote.
     * TRMREQ must be set to 0 (or will send again).
     * Do it twice since "Bits SENTDATA and TRMREQ cannot be set to 0 simultaneously." */
     can_block_p->MCTL[mbox_nr].BYTE = 0;
     can_block_p->MCTL[mbox_nr].BYTE = 0;
    
}/* end CanClearSentData() */   

/*******************************************************************************
Function Name:  R_CAN_RxSet
Description:    Set up a mailbox to receive. The API sets up a given mailbox to 
                receive dataframes with the given CAN ID. Incoming data frames 
                with the same ID will be stored in the mailbox. 
Parameters:     ch_nr
                Mailbox nr.
                Frame ID value
                remote - REMOTE_FRAME to listen for remote requests, DATA_FRAME
                 for receiving normal dataframes.
Return value:     R_CAN_OK            Action completed successfully.
                R_CAN_SW_BAD_MBX    Bad mailbox number.
                R_CAN_BAD_CH_NR     The channel number does not exist.
                R_CAN_SW_SET_TX_TMO    Waiting for previous transmission to finish 
                                    timed out.
                R_CAN_SW_SET_RX_TMO    Waiting for previous reception to complete 
                                    timed out.
*******************************************************************************/
uint32_t R_CAN_RxSet(const uint32_t     ch_nr, 
                     const uint32_t     mbox_nr, 
                     const uint32_t     id,
                     const uint32_t     frame_type)
{
    uint32_t api_status = R_CAN_OK;
    volatile struct st_can __evenaccess * can_block_p;
    
    CHECK_MBX_NR
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;        /* Point to CAN0 peripheral block. */
    }
    else
    {
        return R_CAN_BAD_CH_NR;        /* Channel does not exist for this MCU. */
    }

    /* Wait for any previous transmission/reception to complete. */
    api_status = R_CAN_WaitTxRx(ch_nr, mbox_nr);

    /* Interrupt disable the mailbox. */
    can_block_p->MIER &= ~(bit_set[mbox_nr]);

    /* Clear message mailbox control register. */
    can_block_p->MCTL[mbox_nr].BYTE = 0;

    /*** Set Mailbox ID based on ID mode ***/
    /* Check for XID flag bit set in ID argument */
    if (id & XID_MASK)
    {
        /* Set message mailbox buffer Extended ID, masking off temporary XID flag bit. */
        can_block_p->MB[mbox_nr].ID.LONG = (id & (~XID_MASK));
    }     
    else    
    {
        /* Set message mailbox buffer Standard ID */ 
        can_block_p->MB[mbox_nr].ID.BIT.SID = (id & SID_MASK); /* Now put the lower 11 bit in the SID. */
    }

    /* Set IDE bit depending on if want to receive SID or XID frame. Only for mixed mode. */
    if (can_block_p->CTLR.BIT.IDFM == MIXED_ID_MODE)
    {
        if (id & XID_MASK)
        {
            can_block_p->MB[mbox_nr].ID.BIT.IDE = 1;
        }
        else
        {
            can_block_p->MB[mbox_nr].ID.BIT.IDE = 0;
        }
    }
    else
    {
        /* When not MIXED mode, IDE should be written with 0. */
        can_block_p->MB[mbox_nr].ID.BIT.IDE = 0;
    }
    
    /* Dataframe = 0, Remote frame = 1    */
    if (frame_type == REMOTE_FRAME)
    {
        can_block_p->MB[mbox_nr].ID.BIT.RTR = 1;
    }
    else 
    {    
        can_block_p->MB[mbox_nr].ID.BIT.RTR = 0;
    }
 
    #if (USE_CAN_POLL == 0)
        /* Interrupt enable the mailbox */
        can_block_p->MIER |= (bit_set[mbox_nr]);
    #endif

    /* Request to receive the frame with RecReq bit. */
    can_block_p->MCTL[mbox_nr].BYTE = 0x40;

    return api_status;
} /* end R_CAN_RxSet() */


/*******************************************************************************
Function Name:  R_CAN_RxSetXid
Description:    Calls R_CAN_RxSet() after setting a bit in the id parameter to
                serve as an extended ID mode flag.          
Parameters:     ch_nr
                Mailbox nr.
                Frame ID value
                remote - REMOTE_FRAME to listen for remote requests, DATA_FRAME
                 for receiving normal dataframes.
Return value:     value returned by R_CAN_RxSet is passed on.
*******************************************************************************/
uint32_t R_CAN_RxSetXid(const uint32_t     ch_nr, 
                        const uint32_t     mbox_nr, 
                        uint32_t           xid,
                        const uint32_t     frame_type)
{   
    /* Add the Xid bit so that 29-bit ID will be used by R_CAN_RxSet(). */
    return R_CAN_RxSet(ch_nr, mbox_nr, (xid | XID_MASK) , frame_type);
} /* end R_CAN_RxSetXid() */

/*******************************************************************************
Function Name:    R_CAN_RxPoll
Description:    Checks for received message in mailbox.
Parameters:        Channel nr.
                Mailbox nr.
Return value:    R_CAN_OK            There is a message waiting.
                R_CAN_NOT_OK        No message waiting.
                R_CAN_RXPOLL_TMO    Message pending but timed out.
                R_CAN_SW_BAD_MBX    Bad mailbox number.
                R_CAN_BAD_CH_NR     The channel number does not exist.
*******************************************************************************/
uint32_t R_CAN_RxPoll(const uint32_t ch_nr, const uint32_t mbox_nr)
{
    uint32_t api_status = R_CAN_NOT_OK;
    uint32_t poll_delay = MAX_CAN_REG_POLLTIME;
    volatile struct st_can __evenaccess * can_block_p;
    
    CHECK_MBX_NR
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;         /* Point to CAN0 peripheral block. */
    }
    else
    {
        return R_CAN_BAD_CH_NR;        /* Channel does not exist for this MCU. */
    }
    
    /* Wait if new data is currently being received. */
    while ((can_block_p->MCTL[mbox_nr].BIT.RX.INVALDATA) && poll_delay)
    {
        poll_delay--;
    }

    if (poll_delay == 0)
    {
        api_status = R_CAN_RXPOLL_TMO;     /* Still updating mailbox. Come back later. */
    }
    else /* Message received? */
    {
    	/* If message received, tell user. */
        if (can_block_p->MCTL[mbox_nr].BIT.RX.NEWDATA == 1)
        {
            api_status = R_CAN_OK;
        }
    }
    return api_status;
}/* end R_CAN_RxPoll() */

/*******************************************************************************
Function Name:  R_CAN_RxRead
Parameters:        Mailbox nr.
                frame_p: Data frame structure
Description:    Call from CAN receive interrupt. Copies received data from 
                message mailbox to memory.
Return value:    R_CAN_OK            There is a message waiting.
                R_CAN_SW_BAD_MBX    Bad mailbox number.
                R_CAN_BAD_CH_NR     The channel number does not exist.
                R_CAN_MSGLOST      Message was overwritten or lost.
*******************************************************************************/
uint32_t R_CAN_RxRead( const uint32_t           ch_nr,
                       const uint32_t           mbox_nr,
                       can_frame_t * const  frame_p )
{
    uint32_t i;
    uint32_t api_status = R_CAN_OK;
    volatile struct st_can __evenaccess * can_block_p;
    
    CHECK_MBX_NR
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200; /* Point to CAN0 peripheral block. */
    }
    else
    {
        return R_CAN_BAD_CH_NR;    /* Channel does not exist for this MCU. */
    }
    
    /* EXT_ID_MODE */
    /* Copy received data from message mailbox to memory. The IDE bit is only valid in mixed mode. */
    if (can_block_p->CTLR.BIT.IDFM == EXT_ID_MODE)
    {
        /* Get mailbox Extended ID, keeping only lower 29 bits. */
        frame_p->id = (can_block_p->MB[mbox_nr].ID.LONG & (~XID_MASK));
    }
    /* MIXED_ID_MODE */
    else if (can_block_p->CTLR.BIT.IDFM == MIXED_ID_MODE)
    {
        if (can_block_p->MB[mbox_nr].ID.BIT.IDE == 1) /* Check for XID control bit set. */
        {
            /* Get mailbox Extended ID, keeping only lower 29 bits. */
            frame_p->id = (can_block_p->MB[mbox_nr].ID.LONG & (~XID_MASK));
        }
        else
        {
            /* Get only the lower 11 bits from the SID. */
            frame_p->id = can_block_p->MB[mbox_nr].ID.BIT.SID;
        }
    }
    /* STD_ID_MODE */
    else
    {
        /* Get lower 11 bits from SID. */
        frame_p->id = can_block_p->MB[mbox_nr].ID.BIT.SID;
    }

    frame_p->dlc = (uint8_t)can_block_p->MB[mbox_nr].DLC.BIT.DLC;

    for (i = 0; i < can_block_p->MB[mbox_nr].DLC.BIT.DLC; i++)
    {
        frame_p->data[i] = can_block_p->MB[mbox_nr].DATA[i];
    }

    /* Check if message was lost/overwritten. */
    if (can_block_p->MCTL[mbox_nr].BIT.RX.MSGLOST)
    {
        api_status = R_CAN_MSGLOST;
        /* MSGLOST bit will be cleared below. */
    }

    /* Clear MSGLOST.
     * Also set NEWDATA to 0 since the mailbox was just emptied and start over with new RxPolls.
     * Do a byte-write to avoid read-modify-write with HW writing another bit inbetween. See HW manual footnote.*/
    can_block_p->MCTL[mbox_nr].BYTE = 0x40;
    
    return api_status;
}/* end R_CAN_RxRead() */

/*******************************************************************************
Function Name:  R_CAN_RxSetMask
Description:    Set a CAN bus mask for specified mask register. Note that the 
                MKIVLR register is used to disable the acceptance filtering 
                function individually for each mailbox.
Parameters:     ch_nr
                mask_value. For each bit that is 1; corresponding SID bit
                                is compared.
                mbox_nr            0-31. The mailbox nr translates to mask_reg_nr:
                                0 for mailboxes 0-3
                                1 for mailboxes 4-7
                                2 for mailboxes 8-11
                                3 for mailboxes 12-15
                                4 for mailboxes 16-19
                                5 for mailboxes 20-23
                                6 for mailboxes 24-27
                                7 for mailboxes 28-31
Return value:     -
*******************************************************************************/
void R_CAN_RxSetMask( const uint32_t ch_nr,
                      const uint32_t mbox_nr,
                      const uint32_t mask_value)
{
    volatile struct st_can __evenaccess * can_block_p;
    
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;/* Point to CAN0 peripheral block. */
    }
    else
    {
        return; /* Channel does not exist for this MCU. */
    }

    /* Write to MKR0 to MKR7 in CAN reset mode or CAN halt mode. */
    R_CAN_Control(ch_nr, HALT_CANMODE);
    
    /* Set mask for the group of mailboxes. */
    if ((can_block_p->CTLR.BIT.IDFM == EXT_ID_MODE) || (can_block_p->CTLR.BIT.IDFM == MIXED_ID_MODE))
    {
        /* Set XID 29-bit mask value in mask register. */
        can_block_p->MKR[mbox_nr/4].LONG = (mask_value & (~XID_MASK));
    }
    else
    {
        /* Set SID 11-bit mask value in mask register. */
        can_block_p->MKR[mbox_nr/4].BIT.SID = mask_value;
    }

    /* Set mailbox mask to be used. (0 = mask VALID.) */
    can_block_p->MKIVLR &= ~(bit_set[mbox_nr]);
    
    R_CAN_Control(ch_nr, OPERATE_CANMODE);
            
}/* end R_CAN_RxSetMask() */

/*****************************************************************************
Name:           R_CAN_WaitTxRx

Parameters:     Channel nr.
                Mailbox nr.

Description:    Wait for communicating mailbox to complete action. This is 
                appropriate if a mailbox needs to be reconfigured to do some-
                thing else, but the user wants any pending receive or transmit 
                to finish.
                    Note that the function does not always wait. If the mailbox
                is idle, it will not wait at all.
                    If you want to send in a hurry, use another mailbox, alter-
                nate mailboxes, or even use the FIFO that can transmit/receive 
                from four mailboxes. 
                    The max wait MAX_CAN_SW_DELAY is kind of superficial. "Ideal-
                ly" you would wait as long as it takes for the previous frame to 
                finish its transmit, but that may could cause the program to 
                block a long time if the bus is busy. This would be bad espec-
                ially if the system is not using an RTOS.

Return value:   R_CAN_OK            There is a message waiting.
                R_CAN_SW_BAD_MBX    Bad mailbox number.
                R_CAN_BAD_CH_NR     The channel number does not exist.
                R_CAN_SW_SET_TX_TMO    Waiting for previous transmission to finish 
                                    timed out.
                R_CAN_SW_SET_RX_TMO    Waiting for previous reception to complete 
                                    timed out.
*****************************************************************************/
#pragma inline(R_CAN_WaitTxRx)
static uint32_t R_CAN_WaitTxRx(const uint32_t ch_nr, const uint32_t mbox_nr)
{
    uint32_t api_status = R_CAN_OK;
    uint32_t can_tmo_cnt = MAX_CAN_SW_DELAY;
    volatile struct st_can __evenaccess * can_block_p;
    
    CHECK_MBX_NR
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;        /* Point to CAN0 peripheral block. */
    }
    else
    {
        return R_CAN_BAD_CH_NR;        /* Channel does not exist for this MCU. */
    }

    /* Wait for any previous transmission to complete. */
    if (can_block_p->MCTL[mbox_nr].BIT.TX.TRMREQ)
    {
        while ((can_block_p->MCTL[mbox_nr].BIT.TX.SENTDATA == 0) && DEC_CHK_CAN_SW_TMR) 
        {
        }

        if (can_tmo_cnt == 0) 
        {
            api_status = R_CAN_SW_SET_TX_TMO;
        }
    }
    /* Wait for any previous reception to complete. */
    else if (can_block_p->MCTL[mbox_nr].BIT.TX.RECREQ)    //Strange; but iodefine has TX here..
    {
        while ((can_block_p->MCTL[mbox_nr].BIT.RX.INVALDATA == 1) && DEC_CHK_CAN_SW_TMR) 
        {
        }

        if (can_tmo_cnt == 0) 
        {
            api_status = R_CAN_SW_SET_RX_TMO;
        }
    }
    return api_status;
}/* end R_CAN_WaitTxRx() */

/*******************************************************************************
Function Name:  R_CAN_CheckErr
Description:    Checks CAN peripheraol error state.
Parameters:        -
Return value:     0 = No error
                1 = CAN is in error active state
                2 = CAN is in error passive state
                4 = CAN is in bus-off state
*******************************************************************************/
uint32_t R_CAN_CheckErr(const uint32_t    ch_nr)
{
    uint32_t api_status = R_CAN_STATUS_ERROR_ACTIVE; /* Store return value */
    volatile struct st_can __evenaccess * can_block_p;
    
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;        /* Point to CAN0 peripheral block. */
    }
    else
    {
        return R_CAN_BAD_CH_NR;        /* Channel does not exist for this MCU. */
    }

    /* Check CAN error state */
    if (can_block_p->STR.BIT.EST)
    {
        /* Check error-passive state */
        if (can_block_p->STR.BIT.EPST)
        {
            api_status = R_CAN_STATUS_ERROR_PASSIVE;
        }

        /* Check bus-off state */
        else if (can_block_p->STR.BIT.BOST)
        {
            api_status = R_CAN_STATUS_BUSOFF;
        }
    }
    
    return api_status;
} /* end R_CAN_CheckErr() */

/*******************************************************************************
Function Name:  R_CAN_SetBitrate
Description:    Sets clock speed and bit rate for CAN as defined in 
                config.h.
Parameters:     -
Return value:     -
*******************************************************************************/
void R_CAN_SetBitrate(const uint32_t ch_nr)
{    
    volatile struct st_can __evenaccess * can_block_p;
    
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;        /* Point to CAN0 peripheral block. */
    }
    else
    {
        return;        /* Channel does not exist for this MCU. */
    }

    /* Set TSEG1, TSEG2 and SJW. */
    can_block_p->BCR.BIT.BRP   = CAN_BRP - 1;
    can_block_p->BCR.BIT.TSEG1 = CAN_TSEG1 - 1;
    can_block_p->BCR.BIT.TSEG2 = CAN_TSEG2 - 1;
    can_block_p->BCR.BIT.SJW   = CAN_SJW - 1;
}/* end R_CAN_SetBitrate() */

/**********************************************************************************
Function Name:  R_CAN_ConfigCANinterrupts
Description  :  Configuration of CAN interrupts.
                
                CAN0 interrupts:
                Source    Name    Vector    Address        IER, BIT        IPR
                                nr.        offset
                ============================================================
                CAN0     ERS0     56         00E0         IER07, IEN0        IPR18
                "        RXF0     57         00E4         IER07, IEN1        "
                "        TXM0     58         00E8         IER07, IEN2        "
                "        RXM0     59         00EC         IER07, IEN3        "
                "        TXM0     60         00F0         IER07, IEN4        "
Parameters   :     -
Return value :     -
***********************************************************************************/
void R_CAN_ConfigCANinterrupts(const uint32_t ch_nr)
{
    volatile struct st_can __evenaccess * can_block_p;
    
    if (ch_nr == 0)
    {
        can_block_p = (struct st_can __evenaccess *) 0x90200;        /* Point to CAN0 peripheral block. */
    }
    else
    {
        return;        /* Channel does not exist for this MCU. */
    }
    
    if (ch_nr == 0)
    {
        /* Mailbox interrupt enable registers. Disable interrupts for all slots. 
        They will be enabled individually by the API. */
        can_block_p->MIER = 0x00000000;

#if (USE_CAN_POLL == 0)
            /* Configure CAN Tx interrupt. */
            //ICU.IER[IER_CAN0_TXM0].BIT.IEN4 = 1;
            //ICU.IPR[IPR_CAN0_TXM0].BIT.IPR = 3;
            /* ..or use the macros! */
            IEN(CAN0, TXM0) = 1;    //1 = interrupt enabled.
            IPR(CAN0, TXM0) = CAN0_INT_LVL;    //priority
    
            /* Configure CAN Rx interrupt. */
            IEN(CAN0, RXM0) = 1;
            IPR(CAN0, RXM0) = CAN0_INT_LVL;
    
            /* Configure CAN Error interrupt. */
            IEN(CAN0, ERS0) = 1;
            IPR(CAN0, ERS0) = CAN0_INT_LVL;
            CAN0.EIER.BYTE = 0xFF;

            /* RX CAN0 uses:
            - Interrupt Priority Register 18, IPR18. 
            - Interrupt Request Enable Register 7, IER07.
            - Vector 57 RXF0, and 58 TXM0. */
            ICU.IPR[18].BIT.IPR = CAN0_INT_LVL;
            /* Interrupt enable bit per mailbox (can_block_p->MIER) set by 
            R_CAN_TxSet() and R_CAN_RxSet() */        
#endif
    }
    else
    {
        return;
    }
}/* end R_CAN_ConfigCANinterrupts() */


/*********************************************************************************

        CAN interrupts are in application.

**********************************************************************************/
/* eof */
