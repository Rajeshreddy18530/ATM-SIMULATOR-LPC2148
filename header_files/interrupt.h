#ifndef INTERRUPT_H
// Checks whether INTERRUPT_H has not been defined.
// Prevents multiple inclusion of this header file.

#define INTERRUPT_H
// Defines INTERRUPT_H to prevent this header from being included again.

extern volatile int interruptFlag;
// Declares a global variable defined in another source file.
// extern: Indicates that the variable is defined elsewhere.
// volatile: Prevents the compiler from optimizing away accesses to this variable.
// int: Stores the interrupt flag status.

void Init_interrupt(void);
// Function declaration to initialize the external interrupt.
// Configures the required interrupt pin and interrupt controller.

void SetTimeFromKeypad(void);
// Function declaration to set the RTC time using keypad input.

void eint1_irq(void) __irq;
// Declares the EINT1 interrupt service routine (ISR).
// __irq: Compiler-specific keyword indicating an IRQ interrupt handler.
// This function executes when External Interrupt 1 occurs.

void CheckRTCInterrupt(void);
// Function declaration to check whether an RTC interrupt has occurred.
// Used to handle or process RTC interrupt conditions.

#endif
// Ends the conditional compilation block.
// Prevents multiple inclusion of interrupt.h.
