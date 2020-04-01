/*
 * file: fpga_gpio_irq.h
 * author: Aurélien SIU

*/

#include "address_map_arm.h"

int reverse_n_bits(int value, int n);

void config_GIC (void);

/*
 interrupts
* provides code that initializes the generic interrupt controller
*/
void config_interrupt (int, int);

void config_KEYs (void);

