/***********************************************************************/
/*                                                                     */
/*  FILE        :hwsetup.c                                             */
/*  DATE        :Wed, Jan 28, 2009                                     */
/*  DESCRIPTION :Hardware Setup file                                   */
/*  CPU TYPE    :Other                                                 */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.13).    */
/*                                                                     */
/***********************************************************************/
/* Mar 22 '10	RTA-CSS		For RX62N with new CAN API.
/* Sep 1 '10	REA			Hwsetup for YRDKRX62N.	*/

#include "iodefine.h"
#include "rdkrx62n.h"
#include "stdint.h"
#include "r_can_api.h"
	
#ifdef __cplusplus
extern "C" {
#endif
extern void HardwareSetup(void);
#ifdef __cplusplus
}
#endif

static void PeripheralStandbyControl(void);

/******************************************************************************
Function Name:       HardwareSetup
Parameters:          -
Return value:        -
Description:         IO and peripheral control.
******************************************************************************/
void HardwareSetup(void)
{
	/* ICLK = EXTAL*8 = 12M*8 = 96 MHz. */
	SYSTEM.SCKCR.BIT.ICK  =  0;
	/* PCLK = EXTAL*4 = 12M*4 = 48 MHz. */
	SYSTEM.SCKCR.BIT.PCK  =  1;
	/* BCLK = EXTAL*2 = 12M*2 = 24 MHz. */
	SYSTEM.SCKCR.BIT.BCK  =  2;
	/* Enable BCLK output (high level). */
	SYSTEM.SCKCR.BIT.PSTOP1  =  0;	
	
	/* BCLK output (24MHz). */
	/* P5.DDR.BIT.B3  =  1;
    
		
	/*** DEFAULT port settings. Configure as output low. ***/
	/* Port0. */
	PORT1.DDR.BYTE = 0xFF;
	PORT1.DR.BYTE = 0x00;
	/* Port1. */
	PORT1.DDR.BYTE = 0xFF;
	PORT1.DR.BYTE = 0x00;
	/* Port2. */
	PORT2.DDR.BYTE = 0xFF;
	PORT2.DR.BYTE = 0x00;
	/* Port3. */
	PORT3.DDR.BYTE = 0xFF;
	PORT3.DR.BYTE = 0x00;
	/* Port4. */
	PORT4.DDR.BYTE = 0xFF;
	PORT4.DR.BYTE = 0x00;
	/* Port5. */
	PORT5.DDR.BYTE = 0xFF;
	PORT5.DR.BYTE = 0x00;
	/* Port6. */
	PORT6.DDR.BYTE = 0xFF;
	PORT6.DR.BYTE = 0x00;
	/* Port7. */
	PORT7.DDR.BYTE = 0xFF;
	PORT7.DR.BYTE = 0x00;
	/* Port8 */
	PORT8.DDR.BYTE = 0xFF;
	PORT8.DR.BYTE = 0x00;
	/* Port9. */
	PORT7.DDR.BYTE = 0xFF;
	PORT7.DR.BYTE = 0x00;
	
	
	/*** SPECIFIC port settings for RDK. ***/
    PORT0.DDR.BYTE = 0x00 ;     // Port 0: inputs (IRQ's from ethernet & WiFi)
    PORT1.DDR.BYTE = 0x00 ;     // Port 1: inputs (IIC and USB settings will override these later)
    PORT2.DDR.BYTE = 0x1A ;     // Port 2: USB signals
    PORT3.DDR.BYTE = 0x04 ;     // Port 3: JTAG (P30, P31, P34), CAN (P32=Tx, P33=Rx), NMI (P35)
    PORT4.DDR.BYTE = 0x00 ;     // Port 4: Switches (P40-P42), AIN (P43-P47)
    PORT5.DDR.BYTE = 0x3B ;     // Port 5: Audio (P55,P54), BCLK (P53), SCI (P52=Rx, P50=Tx), LCD-RS (P51)

    PORTA.DR.BYTE = 0x00 ;      // Port A outputs all LOW to start
    PORTA.DDR.BYTE = 0xFF ;     // Port A: Expansion (PA0-PA2), Ether (PA3-PA5), Audio (PA6-PA7)
    
    PORTB.DR.BYTE = 0x00 ;
    PORTB.DDR.BYTE = 0x70 ;     // Port B: Ether

    PORTC.DR.BYTE = 0xF7 ;      // Port C: Chip selects, clock = high; IO reset = low
    PORTC.DDR.BYTE = 0x7F ;     // Port C: SPI (PC0-2, PC4-7), IO reset (PC3)

    PORTD.DR.BYTE = 0x00;       // All LED's off
    PORTD.DDR.BYTE = 0xFF ;     // Port D: LED's

    PORTE.DR.BYTE = 0x00  ;     // All LED's off
    PORTE.DDR.BYTE = 0xFF ;     // Port E: LED's (PE0-PE3), LCD data D0-D3) (PE4-PE7)

	/* Switches; set ICR for P0_0, 1, and 7. */
	PORT4.ICR.BIT.B0 = 1;
	PORT4.ICR.BIT.B1 = 1;
	PORT4.ICR.BIT.B2 = 1;
	
	/* For CAN see API. Set default port for CAN so an output high on a 
    CAN pin or low doesn't affect the bus. */
	R_CAN_PortSet(0, ENABLE);
    
	PeripheralStandbyControl();
}/* HardwareSetup() */

/******************************************************************************
Function Name:       PeripheralStandbyControl
Parameters:          -
Return value:        -
Description:         Control whether peripherals are in standby or is on.
******************************************************************************/
static void PeripheralStandbyControl(void)
{
	/* CAN: Module Stop Control Register B (MSTPCRB) bit0 is for CAN Module.
	0: The module stop state is canceled.
	1: Transition to the module stop state is made. */
	SYSTEM.MSTPCRB.BIT.MSTPB0  =  0;
	
}/* end PeripheralStandbyControl() */
/* EOF */