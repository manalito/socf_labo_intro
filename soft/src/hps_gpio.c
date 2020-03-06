/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : hps_gpio.c
 * Author               : Aurélien Siu
 * Date                 : 26.02.2019
 *
 * Context              : SOCF tutorial lab
 *
 *****************************************************************************************
 * Brief: light HPS user LED up when HPS user button pressed, for DE1-SoC board
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 *
 *
*****************************************************************************************/

#include "address_map_arm.h"

#define HPS_GPIO1 (HPS_BRIDGE_BASE + HPS_GPIO1_BASE)

#define HPS_GPIO1_54 (0x1 << 25)
#define HPS_GPIO1_53 (0x1 << 24)
#define GPIO_SWPORTA_DR  0x00
#define GPIO_SWPORTA_DDR 0x04
#define GPIO_EXT_PORTA   0X50


int main(void){

    volatile int *gpio_swporta_dr  = (volatile int *)(HPS_GPIO1 + GPIO_SWPORTA_DR);
    volatile int *gpio_swporta_ddr = (volatile int *)(HPS_GPIO1 + GPIO_SWPORTA_DDR);
    volatile int *gpio_ext_swporta = (volatile int *)(HPS_GPIO1 + GPIO_EXT_PORTA);

    /* Set Directions */
    /* set GPIO 53 as output */
    *gpio_swporta_ddr |= HPS_GPIO1_53;
    /* set GPIO 54 as input */
    *gpio_swporta_ddr &= ~HPS_GPIO1_54;

    while(1) {
      /* read GPIO 54 input value and update GPIO 53 output (LED)  */
      if((*gpio_ext_swporta) & HPS_GPIO1_54) {
         *gpio_swporta_dr &= ~HPS_GPIO1_53;
      } else {
          *gpio_swporta_dr |= HPS_GPIO1_53;
      }
    }

    return 0;


}
