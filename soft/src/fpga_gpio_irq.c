/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : fpga_gpio_irq.c
 * Author               : Aurélien Siu
 * Date                 : 18.03.2019
 *
 * Context              : SOCF lab
 *
 *****************************************************************************************
 * Brief: access input/outputs via FPGA on DE1-SOC with interrupts
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 *
 *
*****************************************************************************************/

#include "exceptions.h"
#include "address_map_arm.h"
#include "defines.h"
#include "fpga_gpio_irq.h"

/* Array filled with all printable numbers on 7 segments display */
static short values_7seg[NB_VAL_7SEG] = {
    VAL_7SEG_0, VAL_7SEG_1, VAL_7SEG_2, VAL_7SEG_3,
    VAL_7SEG_4, VAL_7SEG_5, VAL_7SEG_6, VAL_7SEG_7,
    VAL_7SEG_8, VAL_7SEG_9, VAL_7SEG_A, VAL_7SEG_B,
    VAL_7SEG_C, VAL_7SEG_D, VAL_7SEG_E, VAL_7SEG_F,
};


int main(void)
{
    disable_A9_interrupts(); /* disable interrupts in the A9 processor */
    set_A9_IRQ_stack(); 	 /* initialize the stack pointer for IRQ mode */
    config_GIC(); 			 /* configure the general interrupt controller */
    config_KEYs(); 			 /* configure KEYs to generate interrupts */
    enable_A9_interrupts();  /* enable interrupts in the A9 processor */
    
    
    volatile int *fpga_sw   = (volatile int *)(LW_BRIDGE_BASE + FPGA_SW);
    volatile int *fpga_ledr = (volatile int *)(LW_BRIDGE_BASE + FPGA_LEDR);
    volatile int *fpga_keys = (volatile int *)(LW_BRIDGE_BASE + FPGA_KEYS);
    
    volatile int *fpga_hex0 = (volatile int *)(LW_BRIDGE_BASE + FPGA_HEX0);
    volatile int *fpga_hex1 = (volatile int *)(LW_BRIDGE_BASE + FPGA_HEX1);
    volatile int *fpga_hex2 = (volatile int *)(LW_BRIDGE_BASE + FPGA_HEX2);
    volatile int *fpga_hex3 = (volatile int *)(LW_BRIDGE_BASE + FPGA_HEX3);
    
    int switchs_value;

    while(1) {
		if(!(*fpga_keys & KEY0_MASK)) {
			switchs_value = *fpga_sw;
			*fpga_ledr = switchs_value;
			*fpga_hex0 = HEX_MASK & ~values_7seg[switchs_value &  SW3_0_MASK];
			*fpga_hex1 = HEX_MASK & ~values_7seg[(switchs_value &  SW7_4_MASK) >> 4];
			*fpga_hex2 = HEX_MASK & ~values_7seg[(switchs_value &  SW8_MASK) >> 8];
			*fpga_hex3 = HEX_MASK & ~values_7seg[(switchs_value &  SW9_MASK) >> 9];
			
		} else if (!(*fpga_keys & KEY1_MASK)) {
			switchs_value = *fpga_sw;
			*fpga_hex0 = HEX_MASK & ~values_7seg[reverse_n_bits(switchs_value & SW3_0_MASK, 4)];
			*fpga_hex1 = HEX_MASK & ~values_7seg[reverse_n_bits((switchs_value &  SW7_4_MASK) >> 4 , 4)];
			*fpga_hex2 = HEX_MASK & ~values_7seg[(switchs_value &  SW8_MASK) >> 8];
			*fpga_hex3 = HEX_MASK & ~values_7seg[(switchs_value &  SW9_MASK) >> 9];
		}
    }
}

/* setup the KEY interrupts in the FPGA */
void config_KEYs()
{
    volatile int * KEY_ptr = (int *) (LW_BRIDGE_BASE | FPGA_KEYS);
    /* KEY base address */
    *(KEY_ptr + 2) = 0xF; /* enable interrupts for all  KEY3 and KEY2 */
}

/*
* Configure the Generic Interrupt Controller (GIC)
*/
void config_GIC(void)
{
	config_interrupt (72, CPU0); /* configure the FPGA KEYs interrupt (72) */
	
	/* Set Interrupt Priority Mask Register (ICCPMR). Enable all priorities */
	*((int *) 0xFFFEC104) = 0xFFFF;
	
	/* Set the enable in the CPU Interface Control Register (ICCICR) */
	*((int *) 0xFFFEC100) = ENABLE; 
	
	/* Set the enable in the Distributor Control Register (ICDDCR) */
	*((int *) 0xFFFED000) = ENABLE;
}

/*
* Configure registers in the GIC for an individual Interrupt ID. We
* configure only the Interrupt Set Enable Registers (ICDISERn) and
* Interrupt Processor Target Registers (ICDIPTRn). The default (reset)
* values are used for other registers in the GIC
*/
void config_interrupt (int N, int CPU_target)
{
	int reg_offset, index, value, address;
	/* Configure the Interrupt Set-Enable Registers (ICDISERn).
	 * reg_offset = (integer_div(N / 32) * 4; value = 1 << (N mod 32) */
	 reg_offset = (N >> 3) & 0xFFFFFFFC;
	 index = N & 0x1F;
	 value = 0x1 << index;
	 address = 0xFFFED100 + reg_offset;
	 /* Using the address and value, set the appropriate bit */
	 *(int *)address |= value;
	 /* Configure the Interrupt Processor Targets Register (ICDIPTRn)
	  * reg_offset = integer_div(N / 4) * 4; index = N mod 4 */
	 reg_offset = (N & 0xFFFFFFFC);
	 index = N & 0x3;
	 address = 0xFFFED800 + reg_offset + index;
	  
	 /* Using the address and value, write to (only) the appropriate byte */
	 *(char *)address = (char) CPU_target;
}


/* This function a new value with reversed bits from 0 to n */
int reverse_n_bits(int value, int n) {
    int i;
    int new_value = 0;
    if (n > 0 && n < 31) {
        for(i = n - 1; i >= 0; --i) {
            new_value |= (value & 0x1) << i;
            value = value >> 0x1;
        }
    }
    return new_value;
}
