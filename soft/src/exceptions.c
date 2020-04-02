/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : execptions.c
 * Author               : Sébastien Masle
 * Date                 : 16.02.2018
 *
 * Context              : SOCF class
 *
 *****************************************************************************************
 * Brief: defines exception vectors for the A9 processor
 *        provides code that sets the IRQ mode stack, and that dis/enables interrupts
 *        provides code that initializes the generic interrupt controller
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Engineer      Comments
 * 0.0    16.02.2018  SMS           Initial version.
 *
*****************************************************************************************/
#include <stdint.h>

#include "address_map_arm.h"
#include "defines.h"
#include "exceptions.h"

int *hex_3_0_array[] = {(int *)(LW_BRIDGE_BASE|FPGA_HEX0),
                        (int *)(LW_BRIDGE_BASE|FPGA_HEX1),
                        (int *)(LW_BRIDGE_BASE|FPGA_HEX2),
                        (int *)(LW_BRIDGE_BASE|FPGA_HEX3)};

// Define the IRQ exception handler
void __attribute__ ((interrupt)) __cs3_isr_irq(void)
{

	/* Read CPU Interface registers to determine which peripheral has caused an interrupt
	 * Read the ICCIAR from the CPU Interface in the GIC */
	int interrupt_ID =*((int*) (MPCORE_GIC_CPUIF|ICCIAR));
    
    // Handle the interrupt if it comes from the KEYs
    if (interrupt_ID == 72) // check if interrupt is from the KEYs
        pushbutton_ISR();
    else
        while (1); // if unexpected, then stay here


	/* Clear interrupt from the CPU Interface
     * Write to the End of Interrupt Register (ICCEOIR) */
    *((int*) (MPCORE_GIC_CPUIF|ICCEOIR)) = interrupt_ID;

	return;
} 

// Define the remaining exception handlers
void __attribute__ ((interrupt)) __cs3_reset (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_undef (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_swi (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_pabort (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_dabort (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_fiq (void)
{
    while(1);
}

/*
* Initialize the banked stack pointer register for IRQ mode
*/
void set_A9_IRQ_stack(void)
{
	uint32_t stack, mode;
	stack = A9_ONCHIP_END - 7;		// top of A9 onchip memory, aligned to 8 bytes
	/* change processor to IRQ mode with interrupts disabled */
	mode = INT_DISABLE | IRQ_MODE;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
	/* set banked stack pointer */
	asm("mov sp, %[ps]" : : [ps] "r" (stack));

	/* go back to SVC mode before executing subroutine return! */
	mode = INT_DISABLE | SVC_MODE;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
}
/*
* Turn on interrupts in the ARM processor
*/
void enable_A9_interrupts(void)
{
    uint32_t status = SVC_MODE | INT_ENABLE;
    asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}

/*
* Turn off interrupts in the ARM processor
*/
void disable_A9_interrupts(void)
{
    uint32_t status = SVC_MODE | INT_DISABLE;
    asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}


/********************************************************************
* Pushbutton - Interrupt Service Routine
*
* This routine checks which KEY has been pressed. It writes from HEX3 to HEX0
*******************************************************************/
void pushbutton_ISR( void )
{
    /* KEY base address */
    volatile int *KEY_ptr = (int *) (LW_BRIDGE_BASE | FPGA_KEYS);
    
    int press = *(KEY_ptr + 3); // read the pushbutton interrupt register
    
    *(KEY_ptr + 3) = press; // Clear the interrupt
    
    if (press & KEY2_MASK) // KEY2
        hex_3_0_rotate_right();
    else if (press & KEY3_MASK) // KEY3
        hex_3_0_rotate_left();
    
    return;
}

/* This function rotates to right characters displayed from HEX3 to HEX0 */
void hex_3_0_rotate_right(void) {
	int i;
    int temp = *hex_3_0_array[0];
    
    for(i = 0; i < 3; ++i) {
        *hex_3_0_array[i] = *hex_3_0_array[i+1];
    }
    
    *hex_3_0_array[3] = temp;
}

/* This function rotates to left characters displayed from HEX3 to HEX0 */
void hex_3_0_rotate_left(void){
    
	int i;
    int temp = *hex_3_0_array[3];
    
    for(i = 3; i > 0; --i) {
        *hex_3_0_array[i] = *hex_3_0_array[i-1];
    }
    
    *hex_3_0_array[0] = temp;
    
}
