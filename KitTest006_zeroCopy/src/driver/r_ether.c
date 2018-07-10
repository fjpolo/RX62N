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
* File Name    : r_ether.c
* Version      : 1.03
* Description  : Ethernet module device driver
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 15.02.2010 1.00    First Release
*         : 03.03.2010 1.01    Buffer size is aligned on the 32-byte boundary.
*         : 08.03.2010 1.02    Modification of receiving method
*         : 06.04.2010 1.03    RX62N changes
*         : 29.03.2011 1.04    Enhancements for better link detection,
*                              autonegotiation, and more. 
*         : 08.07.2011 2.00    Second release 
******************************************************************************/ 


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stddef.h>
#include <machine.h>
#include "iodefine.h"
#include "r_ether.h"
#include "phy.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/**
 * When using this driver with a TCPIP stack, please make sure
 * the EMAC_BUFSIZE is equal to the buffer size in bytes the 
 * stack is using. In case of Renesas uIP demo, this is set to 
 * 1516 bytes.  
 */
#define EMAC_BUFSIZE                (1516)
/* The number of Rx descriptors. */
#define EMAC_NUM_RX_DESCRIPTORS     (8)
/* The number of Tx descriptors. */
#define EMAC_NUM_TX_DESCRIPTORS     (4)

/** 
 * The total number of EMAC buffers to allocate. The number of 
 * total buffers is simply the sum of the number of transmit and
 * receive buffers. 
 */
#define EMAC_NUM_BUFFERS            (EMAC_NUM_RX_DESCRIPTORS + (EMAC_NUM_TX_DESCRIPTORS))

/** 
 * Only transmit end and receive end interrupts are used by this
 * driver.
 */
#define EMAC_TC_INT        (1UL << 21)
#define EMAC_FR_INT        (1UL << 18)

/* Bit definitions of status member of DescriptorS */
#define  ACT                (0x80000000)
#define  DL                 (0x40000000)    
#define  FP1                (0x20000000)    
#define  FP0                (0x10000000)    
#define  FE                 (0x08000000)

#define  RFOVER             (0x00000200)
#define  RAD                (0x00000100)
#define  RMAF               (0x00000080)
#define  RRF                (0x00000010)
#define  RTLF               (0x00000008)
#define  RTSF               (0x00000004)
#define  PRE                (0x00000002)
#define  CERF               (0x00000001)

#define  CLR                (0x3800039F)
#define  ERR                (0x0800039F)
                                       
#define  TWBI               (0x04000000)    
#define  TAD                (0x00000100)
#define  CND                (0x00000008)
#define  DLC                (0x00000004)
#define  CD                 (0x00000002)
#define  TRO                (0x00000001)
#define  OK                 (0x00010000)

/* Number of entries in PAUSE resolution table */
#define PAUSE_TABLE_ENTRIES (8)
/* Local device and link partner PAUSE settings */
#define XMIT_PAUSE_OFF      (0)
#define RECV_PAUSE_OFF      (0)
#define XMIT_PAUSE_ON       (1)
#define RECV_PAUSE_ON       (1)
/* PAUSE link mask and shift values */
#define LINK_RES_ABILITY_MASK           (3)
#define LINK_RES_LOCAL_ABILITY_BITSHIFT (2)

/**
 * Link status read from LMON bit of ETHERC PSR register.  The 
 * state is hardware dependent.   
 */
#define LINK_PRESENT        (0)
#define LINK_NOTPRESENT     (1)

/* Used for single buffer applications e.g uIP stack */
#define SINGLE_BUFFER_APP

/******************************************************************************
Typedef definitions
******************************************************************************/

/**
 * EDMAC descriptor as defined in the hardware manual. It is 
 * modifed to support little endian CPU mode. 
 */
struct DescriptorS
{
    __evenaccess uint32_t   status;
#if __LIT                               
/* Little endian */
    __evenaccess uint16_t   size;
    __evenaccess uint16_t   bufsize;
#else                                   
/* Big endian */
    __evenaccess uint16_t   bufsize;
    __evenaccess uint16_t   size;

#endif
    uint8_t                 *buf_p;
    struct DescriptorS      *next;
};
typedef struct DescriptorS   descriptor_s;

/**
 * Ethernet buffer type definition.  Member temp is used to 
 * align the buffer on 32-bit address.
 */ 
struct EtherBufferS
{
    uint32_t temp;
    uint8_t  buffer[EMAC_NUM_BUFFERS][EMAC_BUFSIZE];
};
typedef struct EtherBufferS etherbuffer_s;

/**
 * PauseMaskE, PauseValE and pause_resolutionS are use to create 
 * PAUSE resolution Table 28B-3 in IEEE 802.3-2008 standard. 
 */
enum PauseMaskE
{ 
    PAUSE_MASK0, PAUSE_MASK1, PAUSE_MASK2, PAUSE_MASK3, PAUSE_MASK4, PAUSE_MASK5, PAUSE_MASK6, PAUSE_MASK7,
    PAUSE_MASK8, PAUSE_MASK9, PAUSE_MASKA, PAUSE_MASKB, PAUSE_MASKC, PAUSE_MASKD, PAUSE_MASKE, PAUSE_MASKF
};
typedef enum PauseMaskE  pausemask_e;

enum PauseValE
{
    PAUSE_VAL0, PAUSE_VAL1, PAUSE_VAL2, PAUSE_VAL3, PAUSE_VAL4, PAUSE_VAL5, PAUSE_VAL6, PAUSE_VAL7,
    PAUSE_VAL8, PAUSE_VAL9, PAUSE_VALA, PAUSE_VALB, PAUSE_VALC, PAUSE_VALD, PAUSE_VALE, PAUSE_VALF
};
typedef enum PauseValE   pauseval_e;

struct pause_resolutionS
{                            
    pausemask_e mask;
    pauseval_e  value;
    uint8_t     transmit;
    uint8_t     receive;
};
typedef struct pause_resolutionS  pauseresolution_s;

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
uint32_t g_ether_rx = 0;
uint32_t g_ether_tx = 0;

/******************************************************************************
Private global variables and functions
******************************************************************************/

/**
 * Private global function prototyes
 */
void _R_Ether_ResetMAC(void);
void _R_Ether_InitDescriptors(void);
void _R_Ether_ConfigEthernet(void);
void _R_Ether_Swap_Buffers(void **buf);
void _R_Ether_PauseResolution(uint8_t local_ability, uint8_t partner_ability,
                              uint16_t *tx_pause, uint16_t *rx_pause);
/**
 * Private global variables
 */

/* Pointer to the receive descriptors */
descriptor_s *app_rx_desc = NULL;
descriptor_s *emac_rx_desc = NULL;

/* Pointer to the transmit descriptors */
descriptor_s *app_tx_desc = NULL;
descriptor_s *emac_tx_desc = NULL;

/** 
 * PAUSE Resolution as documented in IEEE 802.3-2008_section2 Annex         
 * 28B, Table 28B-3. The following table codify logic that 
 * determines how the PAUSE is configured for local transmitter 
 * and receiver and partner transmitter and receiver. 
 */
const pauseresolution_s pause_resolution[PAUSE_TABLE_ENTRIES] =
{
    { PAUSE_MASKC, PAUSE_VAL0, XMIT_PAUSE_OFF, RECV_PAUSE_OFF },
    { PAUSE_MASKE, PAUSE_VAL4, XMIT_PAUSE_OFF, RECV_PAUSE_OFF },
    { PAUSE_MASKF, PAUSE_VAL6, XMIT_PAUSE_OFF, RECV_PAUSE_OFF },
    { PAUSE_MASKF, PAUSE_VAL7, XMIT_PAUSE_ON,  RECV_PAUSE_OFF },
    { PAUSE_MASKE, PAUSE_VAL8, XMIT_PAUSE_OFF, RECV_PAUSE_OFF },
    { PAUSE_MASKA, PAUSE_VALA, XMIT_PAUSE_ON,  RECV_PAUSE_ON  },
    { PAUSE_MASKF, PAUSE_VALC, XMIT_PAUSE_OFF, RECV_PAUSE_OFF },
    { PAUSE_MASKF, PAUSE_VALD, XMIT_PAUSE_OFF, RECV_PAUSE_ON  }
};

/**
 * Receive, transmit descriptors and their buffer.  They are 
 * defined with section pragma directives to easly locate them 
 * on the memory map.  
 */
#pragma section _RX_DESC
descriptor_s    rx_descriptors[EMAC_NUM_RX_DESCRIPTORS];
#pragma section _TX_DESC
descriptor_s    tx_descriptors[EMAC_NUM_TX_DESCRIPTORS];
#pragma section _ETHERNET_BUFFERS
etherbuffer_s   ether_buffers;
#pragma section

/**
 * Renesas Ethernet API functions
 */

/******************************************************************************
* Function Name: R_Ether_Open
* Description  : Initializes and enables Ethernet peripheral
* Arguments    : ch - Ethernet channel number 
*              : mac_addr - MAC address
*              : buf - The buffer pointer to initialize
* Return Value : R_ETHER_OK
*              : R_ETHER_ERROR
******************************************************************************/
int32_t R_Ether_Open_ZC(uint32_t ch, uint8_t mac_addr[], void **buf)
{
    uint32_t    mac_h, mac_l;
  
    ch = ch;                                
    
    /* Software reset */
    _R_Ether_ResetMAC();

    mac_h = ((uint32_t)mac_addr[0] << 24) | \
            ((uint32_t)mac_addr[1] << 16) | \
            ((uint32_t)mac_addr[2] << 8 ) | \
            (uint32_t)mac_addr[3];

    mac_l = ((uint32_t)mac_addr[4] << 8 ) | \
             (uint32_t)mac_addr[5];

    ETHERC.MAHR = mac_h;  
    ETHERC.MALR.LONG = mac_l;
    
    /* Initialize receive and transmit descriptors */
    _R_Ether_InitDescriptors();

    /* Perform rest of hardware interface configuration */
    _R_Ether_ConfigEthernet();

    /* Initialize the PHY */
    Phy_Init();

    /* Initialize application buffer */
    *buf = app_tx_desc->buf_p;

    return R_ETHER_OK;
}

/******************************************************************************
* Function Name: R_Ether_Close
* Description  : Disables Ethernet peripheral
* Arguments    : ch - Ethernet channel number
* Return Value : R_ETHER_OK
*              : R_ETHER_ERROR
******************************************************************************/
int32_t R_Ether_Close_ZC(uint32_t ch)
{
    ch = ch;                                

    /* Disable TE and RE  */
    ETHERC.ECMR.LONG = 0x00000000;          

    return R_ETHER_OK;
}

/******************************************************************************
* Function Name: R_Ether_Read
* Description  : Receives an Ethernet frame.  Sets the passed 
*              : buffer pointer to the Ethernet frame buffer 
*              : from the driver. This makes the data available to
*              : the caller as a zero-copy operation.
* Arguments    : ch - Ethernet channel number buf
*              : buf - pointer to the Ethernet driver buffer
* Return Value : Number of bytes received (Value greater than zero)
******************************************************************************/
int32_t R_Ether_Read_ZC(uint32_t ch, void **buf)
{
    int32_t num_recvd;
  
    ch = ch;                                

    if (app_rx_desc->status & OK)
    {
        /**
         * Pass the pointer to received data to application.  This is 
         * zero-copy operation.    
         */
        *buf = (void *)app_rx_desc->buf_p;

        /* Get bytes received */
        num_recvd = app_rx_desc->size;
    }
    else
    {
        /** 
         * The status bits contain the error information.  This can be 
         * passed to the stack or the application if further analysis is 
         * needed.  This API simply clears the error information.   
         * 
         */

        /* Tell no data bytes received */
        num_recvd = 0;
    }

    /* Move to next descriptor */
    app_rx_desc->status |= ACT;
    app_rx_desc->status &= ~(CLR | OK);
    app_rx_desc = app_rx_desc->next;

    return(num_recvd);
}

/******************************************************************************
* Function Name: R_Ether_Write
* Description  : Transmits an Ethernet frame. The transmit 
*              : descriptor points to the data to transmit. Data
*              : is sent directly from memory as a "zero copy"
*              : operation.  
* Arguments    : ch - Ethernet channel number
*              : buf - buffer pointer
*              : len - length of data, in bytes, to transmit
* Return Value : R_ETHER_OK
*              : R_ETHER_ERROR
******************************************************************************/
int32_t R_Ether_Write_ZC(uint32_t ch, void **buf, uint32_t len)
{
    ch = ch;

    if ((app_tx_desc->status & ACT) == ACT)
    {
        /* All transmit buffers are full */
        return R_ETHER_ERROR;
    }
    else
    {

#ifdef SINGLE_BUFFER_APP
        /* Swap buffers for single buffer applications */
        _R_Ether_Swap_Buffers(buf);
#endif
        /** 
        * Prepare transmit descriptor for transmission.  Because the
        * size of buffers, the frame will fit into one buffer. 
        * Therefore, the driver runs in single frame single buffer mode.
        */
        app_tx_desc->buf_p = *buf;
        app_tx_desc->bufsize = len;
        app_tx_desc->status &= ~(FP1 | FP0);
        app_tx_desc->status |= (FP1 | FP0 | ACT);
        app_tx_desc = app_tx_desc->next;

        /* Give application another buffer to work with */
        *buf = app_tx_desc->buf_p;

        if (EDMAC.EDTRR.LONG == 0x00000000L)
        {
            /* Restart if stopped */
            EDMAC.EDTRR.LONG = 0x00000001L;
        }

        return R_ETHER_OK;
    }
}

/******************************************************************************
* Function Name: R_Ether_CheckLink
* Description  : Verifies the Etherent link is up or not.
* Arguments    : ch - Ethernet channel number
* Return Value : R_ETHER_OK (Link is up)
*              : R_ETHER_ERROR (Link is down)
******************************************************************************/
int32_t R_Ether_CheckLink_ZC(uint32_t ch)
{
    int16_t status;

    ch = ch; 

    status = Phy_GetLinkStatus();

    if (status == R_PHY_ERROR)
    {
        /* Link is down */
        return R_ETHER_ERROR;
    }
    else
    {
        /* Link is up */
        return R_ETHER_OK;
    }
}

/******************************************************************************
* Function Name: R_Ether_WaitLink
* Description  : Determines the partner PHY capability through 
*              : auto-negotiation process. The link abilities 
*              : are handled to determine duplex, speed and flow
*              : control (PAUSE frames).
*              : This API must be called at least once after R_Ether_Open.
* Arguments    : ch - Ethernet channel number
* Return Value : R_ETHER_OK
*                R_ETHER_ERROR
******************************************************************************/
int32_t R_Ether_WaitLink_ZC(uint32_t ch)
{
    int32_t ret;
    uint16_t link_speed_duplex;
    uint16_t local_pause_bits;
    uint16_t partner_pause_bits;
    uint16_t transmit_pause_set;
    uint16_t receive_pause_set;
    uint16_t full_duplex = 0;
    
    ch = ch;        

    /* Set the link status */
    while (R_PHY_ERROR == Phy_SetAutonegotiate(&link_speed_duplex, 
                                               &local_pause_bits,
                                               &partner_pause_bits));
 
    switch(link_speed_duplex)
    {
        /* Half duplex link */
        case PHY_LINK_100H:
            ETHERC.ECMR.BIT.DM = 0;
            ETHERC.ECMR.BIT.RTM = 1;
            ret = R_ETHER_OK;
            break;

        case PHY_LINK_10H:
            ETHERC.ECMR.BIT.DM = 0;
            ETHERC.ECMR.BIT.RTM = 0;
            ret = R_ETHER_OK;
            break;

        /* Full duplex link */
        case PHY_LINK_100F:
            ETHERC.ECMR.BIT.DM = 1;
            ETHERC.ECMR.BIT.RTM = 1;
            full_duplex = 1;
            ret = R_ETHER_OK;
            break;

        case PHY_LINK_10F:
            ETHERC.ECMR.BIT.DM = 1;
            ETHERC.ECMR.BIT.RTM = 0;
            full_duplex = 1;
            ret = R_ETHER_OK;
            break;

        default:
            ret = R_ETHER_ERROR;
            break;
    }

    if(ret == R_ETHER_OK)
    {
        if (full_duplex)
        {
            /**
             * Enable PAUSE for full duplex link depending on 
             * the pause resolution results 
             */
            _R_Ether_PauseResolution(local_pause_bits, partner_pause_bits,
                                     &transmit_pause_set, &receive_pause_set);

            /* Set automatic PAUSE for 512 bit-time */
            ETHERC.APR.LONG = 1;
            /* Set unlimited retransmit of PAUSE frames */
            ETHERC.TPAUSER.LONG = 0;

            if (transmit_pause_set == XMIT_PAUSE_ON)
            {
                /* Enable automatic PAUSE frame transmission */
                ETHERC.ECMR.BIT.TXF = 1;
            }
            else
            {
                /* Disable automatic PAUSE frame transmission */
                ETHERC.ECMR.BIT.TXF = 0;
            }

            if (receive_pause_set == RECV_PAUSE_ON)
            {
                /* Enable reception of PAUSE frames */
                ETHERC.ECMR.BIT.RXF = 1;
            }
            else
            {
                /* Disable reception of PAUSE frames */
                ETHERC.ECMR.BIT.RXF = 0;
            }
        }
        else
        {
            /* Disable PAUSE for half duplex link */
            ETHERC.ECMR.BIT.TXF = 0;    
            ETHERC.ECMR.BIT.RXF = 0;
        }

        /* Enable receive and transmit. */
        ETHERC.ECMR.BIT.RE = 1;
        ETHERC.ECMR.BIT.TE = 1;

        /* Enable EDMAC receive */
        EDMAC.EDRRR.LONG = 0x1;
    }

    return ret;
}

/**
 * Private functions
 */

/******************************************************************************
* Function Name: _R_Ether_ResetMAC
* Description  : The EDMAC and EtherC are reset through the software reset.
* Arguments    : None
* Return Value : None 
******************************************************************************/
void _R_Ether_ResetMAC(void)
{
    uint32_t i;

    /* Ensure the EtherC and EDMAC are enabled. */
    SYSTEM.MSTPCRB.BIT.MSTPB15 = 0;

    /* Delay */
    for (i = 0; i < 10000; i++)
    {
    }

    EDMAC.EDMR.BIT.SWR = 1; 

    /* Delay */
    for (i = 0; i < 10000; i++)
    {
    }
}

/******************************************************************************
* Function Name: _R_Ether_InitDescriptors
* Description  : The E-DMAC descriptors and the driver buffers are
*                initialized.
* Arguments    : None
* Return Value : None 
******************************************************************************/
void _R_Ether_InitDescriptors(void)
{
    descriptor_s *descriptor;
    uint32_t i;

    /* Initialize the receive descriptors */
    for(i = 0; i < EMAC_NUM_RX_DESCRIPTORS; i++)
    {
        descriptor = &(rx_descriptors[i]);
        descriptor->buf_p = &(ether_buffers.buffer[i][0]);
        descriptor->bufsize = EMAC_BUFSIZE;
        descriptor->size = 0;
        descriptor->status = ACT;
        descriptor->next = (descriptor_s *) &rx_descriptors[i + 1];
    }

    /* The last descriptor points back to the start */
    descriptor->status |= DL;
    descriptor->next = (descriptor_s *) &rx_descriptors[0];

    /* Initialize application and emac receive descriptor pointers*/
    app_rx_desc  = &(rx_descriptors[0]);
    emac_rx_desc = &(rx_descriptors[0]);


    /* Initialize the transmit descriptors */
    for (i = 0; i < EMAC_NUM_TX_DESCRIPTORS; i++)
    {
        descriptor = &(tx_descriptors[i]);
        descriptor->buf_p = &(ether_buffers.buffer[EMAC_NUM_RX_DESCRIPTORS + i][0]);;
        descriptor->bufsize = EMAC_BUFSIZE;
        descriptor->size = 0;
        descriptor->status = 0;
        descriptor->next = (descriptor_s *) &(tx_descriptors[i + 1]);
    }

    /* The last descriptor points back to the start */
    descriptor->status |= DL;
    descriptor->next = (descriptor_s *) &(tx_descriptors[0]);

    /* Initialize application and emac receive descriptor pointers*/
    app_tx_desc  = &(tx_descriptors[0]);
    emac_tx_desc = &(tx_descriptors[0]);
}

/******************************************************************************
* Function Name: _R_Ether_ConfigEthernet
* Description  : Configure the Ethernet Controller (EtherC) and the Ethernet
*                Direct Memory Access controller (EDMAC).
* Arguments    : None
* Return Value : None
******************************************************************************/
void _R_Ether_ConfigEthernet(void)
{   
    /* Clear all ETHERC status BFR, PSRTO, LCHNG, MPD, ICD */
    ETHERC.ECSR.LONG = 0x00000037;              

    /* Enable interrupts of interest only. */
    EDMAC.EESIPR.LONG = (EMAC_TC_INT | EMAC_FR_INT);

    /* Ethernet lenght 1500 + CRC and intergap is 96-bit time */
    ETHERC.RFLR.LONG = 1518;                 
    ETHERC.IPGR.LONG = 0x00000014;              

    /* Clear all ETHERC and EDMAC status bits */
    EDMAC.EESR.LONG = 0x47FF0F9F;               

#ifdef __LIT
    /* Set little endian mode */
    EDMAC.EDMR.BIT.DE = 1;
#endif

    /* Initialize Rx descriptor list address */
    EDMAC.RDLAR = (void *) app_rx_desc;   
    /* Initialize Tx descriptor list address */
    EDMAC.TDLAR = (void *) app_tx_desc;
    /* Copy-back status is RFE & TFE only */
    EDMAC.TRSCER.LONG = 0x00000000;
    /* Threshold of Tx_FIFO */
    EDMAC.TFTR.LONG = 0x00000000;
    /* Transmit fifo is 256 bytes and receive fifo is 1536 bytes */
    EDMAC.FDR.LONG = 0x00000005;
    /* Allow automatic RR reset.  Need for PAUSE flow control. */
    EDMAC.RMCR.LONG = 0x00000000;   
    /* PAUSE flow control FIFO settings. */
    EDMAC.FCFTR.LONG = 0x00070005;  

    /* Set Ethernet interrupt level and enable */
    IPR(ETHER, EINT) = 2;
    IEN(ETHER, EINT) = 1;
}

/******************************************************************************
* Function Name: _R_Ether_Swap_Buffers
* Description  : Swaps the receive buffer that used to transmist
*              : data with a transmit buffer.
* Arguments    : buf - pointer to pointer of buffer used to 
*              : transmit.
* Return Value : None
******************************************************************************/
void _R_Ether_Swap_Buffers(void **buf)
{
    uint32_t i;
    volatile descriptor_s *descriptor;


    for (i = 0; i < EMAC_NUM_RX_DESCRIPTORS; i++)
    {
         descriptor = &(rx_descriptors[i]);

         if (descriptor->buf_p == *buf)
         {
             /** 
             * This is to suport uIP since it is single buffer application 
             * for both transmit and receive.  The buffer passed up to the 
             * application in read operation can be used to transmit data. 
             * If that is the case, buffers are swapped. 
             */
             descriptor->buf_p = app_tx_desc->buf_p;
             app_tx_desc->buf_p = *buf;
             break;
         }
    }
}

/******************************************************************************
* Function Name: _R_Ether_PauseResolution
* Description  : Determines PAUSE frame generation and handling. Uses 
*              : the resolution Table 28B-3 of IEEE 802.3-2008. 
* Arguments    : local_ability - local PAUSE capability (2 least significant bits)
*              : partner_ability - link partner PAUSE capability (2 least significant bits)
*              : *tx_pause - pointer to location to store the
*              :             result of the table lookup for transmit
*              :             PAUSE. 1 is enable, 0 is disable.
*              : *rx_pause - pointer to location to store the
*              :             result of the table lookup for receive
*              :             PAUSE. 1 is enable, 0 is disable.
* Return Value : None
******************************************************************************/
void _R_Ether_PauseResolution(uint8_t local_ability, uint8_t partner_ability,
                              uint16_t *tx_pause, uint16_t *rx_pause)
{
    uint32_t i;
    uint8_t  ability_compare;

    /**
     * Arrange the bits so that they correspond to the Table 28B-3 
     * of the IEEE 802.3 values.
     */
    ability_compare = ((local_ability & LINK_RES_ABILITY_MASK) << LINK_RES_LOCAL_ABILITY_BITSHIFT) | 
                       (partner_ability & LINK_RES_ABILITY_MASK);

    /* Walk through the look up table */
    for (i = 0; i < PAUSE_TABLE_ENTRIES; i++)
    {
        if ( (ability_compare & pause_resolution[i].mask) == pause_resolution[i].value )
        {
            *tx_pause = pause_resolution[i].transmit;
            *rx_pause = pause_resolution[i].receive;
            return;
        }
   }
}

/******************************************************************************
* Function Name: Excep_Ether
* Description  : Interrupt handler for Ethernet receive and 
*              : transmit interrupts. 
* Arguments    : None Return Value : None
******************************************************************************/
#pragma interrupt (Excep_Ether(vect = VECT_ETHER_EINT, enable))
void Excep_Ether(void)
{
    uint32_t status = EDMAC.EESR.LONG;

    if(status & EMAC_FR_INT)
    {
        /**
         * Because of the size of the buffers, the frame will always be 
         * complete.  The driver is designed to run in single frame 
         * single buffer mode for better efficiency. Both good and 
         * erroneous frames move the descriptor. So, clear all status 
         * bits and move to the new one.  Actual read of the recieved 
         * frame is done in R_Ether_Read API. 
         */
        if ((emac_rx_desc->status & ERR) != 0)
        {
            /* Error in frame */
            if (emac_rx_desc->status & RMAF)
            {
                /* Multicast is OK */
                emac_rx_desc->status |= OK; 
            }
            else
            {
                /* Error is not OK */
                emac_rx_desc->status &= ~OK;
            }
        }
        else
        {
            /* Good frame is OK */
            emac_rx_desc->status |= OK;
        }

        /* Process the descriptor */ 
        emac_rx_desc = emac_rx_desc->next;

        /* Set global flag to indicate frame reception */
        g_ether_rx++;



        if (EDMAC.EDRRR.LONG == 0x00000000L)
        {
            /* Restart if stopped */
            EDMAC.EDRRR.LONG = 0x00000001L;
        }

        /* Clear frame reception interrupt bit */
        EDMAC.EESR.BIT.FR = 1;
    }

    /* Transmit handler */
    if(status & EMAC_TC_INT)
    {
        /* Clear frame transmission bit */
        EDMAC.EESR.BIT.TC = 1;
    }
}





