/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : defines.h
 * Author               : Sébastien Masle
 * Date                 : 16.02.2018
 *
 * Context              : SOCF class
 *
 *****************************************************************************************
 * Brief: some definitions
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Engineer      Comments
 * 0.0    16.02.2018  SMS           Initial version.
 *
*****************************************************************************************/
#define		EDGE_TRIGGERED			0x1
#define		LEVEL_SENSITIVE			0x0
#define		CPU0					0x01	// bit-mask; bit 0 represents cpu0
#define		ENABLE					0x1

#define		USER_MODE				0b10000
#define		FIQ_MODE				0b10001
#define		IRQ_MODE				0b10010
#define		SVC_MODE				0b10011
#define		ABORT_MODE				0b10111
#define		UNDEF_MODE				0b11011
#define		SYS_MODE				0b11111

#define		INT_ENABLE				0b01000000
#define		INT_DISABLE				0b11000000

/* ********************** New defines added """""""""""""""""""""""""*****************"*/ 
#define 	HPS_GPIO1 			   (HPS_BRIDGE_BASE + HPS_GPIO1_BASE)

#define 	HPS_GPIO1_54 		   (0x1 << 25)
#define 	HPS_GPIO1_53 		   (0x1 << 24)
#define 	FPGA_SW   				0x00000000
#define 	FPGA_LEDR 				0x00000010
#define 	FPGA_KEYS 				0x00000020
#define 	FPGA_HEX0 				0x00000030
#define 	FPGA_HEX1 				0x00000040
#define 	FPGA_HEX2				0x00000050
#define 	FPGA_HEX3   			0x00000060

#define 	KEY0_MASK 				0x1
#define 	KEY1_MASK 			   (0x1 << 1)
#define 	KEY2_MASK 			   (0x1 << 2)
#define 	KEY3_MASK			   (0x1 << 3)

/* 7-segments display masks*/
#define 	HEX_MASK  				0x7F

/* Switches masks*/
#define 	SW3_0_MASK  			0xF
#define 	SW7_4_MASK 			   (0xF << 4)
#define 	SW8_MASK   		       (0x1 << 8)
#define 	SW9_MASK   			   (0x1 << 9)

/* nb values defined in array to show on 7 seg display */
#define 	VAL_7SEG_0 				0x3f
#define 	VAL_7SEG_1 				0x06
#define 	VAL_7SEG_2				0x5b
#define 	VAL_7SEG_3 				0x4f
#define 	VAL_7SEG_4 				0x66
#define 	VAL_7SEG_5 				0x6d
#define 	VAL_7SEG_6				0x7d
#define 	VAL_7SEG_7 				0x07
#define 	VAL_7SEG_8 				0x7f
#define 	VAL_7SEG_9 				0x6f
#define 	VAL_7SEG_A 				0x77
#define 	VAL_7SEG_B 				0x7C
#define 	VAL_7SEG_C 				0x6f
#define 	VAL_7SEG_D 				0x5e
#define 	VAL_7SEG_E 				0x79
#define 	VAL_7SEG_F 				0x71

/* nb values defined in array to show on 7 seg display */
#define NB_VAL_7SEG 				16
