/*
 * file: exceptions.h
 * author: Aurélien SIU

*/

void __attribute__ ((interrupt)) __cs3_isr_irq(void);

// Define the remaining exception handlers
void __attribute__ ((interrupt)) __cs3_reset (void);

void __attribute__ ((interrupt)) __cs3_isr_undef (void);

void __attribute__ ((interrupt)) __cs3_isr_swi (void);

void __attribute__ ((interrupt)) __cs3_isr_pabort (void);

void __attribute__ ((interrupt)) __cs3_isr_dabort (void);

void __attribute__ ((interrupt)) __cs3_isr_fiq (void);

/* 
 * Initialize the banked stack pointer register for IRQ mode
*/
void set_A9_IRQ_stack(void);

/* 
 * Turn on interrupts in the ARM processor
*/
void enable_A9_interrupts(void);

/* 
 * Turn off interrupts in the ARM processor
*/
void disable_A9_interrupts (void);

void pushbutton_ISR( void );

void hex_3_0_rotate_right(void);
void hex_3_0_rotate_left(void);
