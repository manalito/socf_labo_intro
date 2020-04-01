/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : fpga_gpio.c
 * Author               : Aurélien Siu
 * Date                 : 18.03.2019
 *
 * Context              : SOCF lab
 *
 *****************************************************************************************
 * Brief: access input/outputs via FPGA on DE1-SOC
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 *
 *
*****************************************************************************************/

#include "address_map_arm.h"

#define FPGA_SW   	0x00000000
#define FPGA_LEDR 	0x00000010
#define FPGA_KEYS 	0x00000020
#define FPGA_HEX0 	0x00000030
#define FPGA_HEX1 	0x00000040
#define FPGA_HEX2	0x00000050
#define FPGA_HEX3   0x00000060

#define KEY0_MASK 0x1
#define KEY1_MASK (0x1 << 1)
#define KEY2_MASK (0x1 << 2)
#define KEY3_MASK (0x1 << 3)

#define HEX_MASK  0x7F

#define SW3_0_MASK  0xF
#define SW7_4_MASK (0xF << 4)
#define SW8_MASK   (0x1 << 8)
#define SW9_MASK   (0x1 << 9)

#define VAL_7SEG_0 0x3f
#define VAL_7SEG_1 0x06
#define VAL_7SEG_2 0x5b
#define VAL_7SEG_3 0x4f
#define VAL_7SEG_4 0x66
#define VAL_7SEG_5 0x6d
#define VAL_7SEG_6 0x7d
#define VAL_7SEG_7 0x07
#define VAL_7SEG_8 0x7f
#define VAL_7SEG_9 0x6f
#define VAL_7SEG_A 0x77
#define VAL_7SEG_B 0x7C
#define VAL_7SEG_C 0x6f
#define VAL_7SEG_D 0x5e
#define VAL_7SEG_E 0x79
#define VAL_7SEG_F 0x71

/* nb values defined in array to show on 7 seg display */
#define NB_VAL_7SEG 16

/* Array filled with all printable numbers on 7 segments display */
static short values_7seg[NB_VAL_7SEG] = {
    VAL_7SEG_0, VAL_7SEG_1, VAL_7SEG_2, VAL_7SEG_3,
    VAL_7SEG_4, VAL_7SEG_5, VAL_7SEG_6, VAL_7SEG_7,
    VAL_7SEG_8, VAL_7SEG_9, VAL_7SEG_A, VAL_7SEG_B,
    VAL_7SEG_C, VAL_7SEG_D, VAL_7SEG_E, VAL_7SEG_F,
};

/* This function a new value with reversed bits from 0 to n */
int reverse_n_bits(int value, int n){
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


int main(void){

    volatile int *gpio_swporta_dr  = (volatile int *)(HPS_GPIO1 + GPIO_SWPORTA_DR);
    volatile int *gpio_swporta_ddr = (volatile int *)(HPS_GPIO1 + GPIO_SWPORTA_DDR);
    volatile int *gpio_ext_swporta = (volatile int *)(HPS_GPIO1 + GPIO_EXT_PORTA);
    
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

    return 0;


}
