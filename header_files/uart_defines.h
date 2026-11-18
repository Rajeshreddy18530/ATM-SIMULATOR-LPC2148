// uart_defines.h

#define TXD0_PIN_FUNC 0x00000001  // Defines the pin function value to configure P0.0 as UART0 Transmitter (TXD0).

#define RXD0_PIN_FUNC 0x00000004  // Defines the pin function value to configure P0.1 as UART0 Receiver (RXD0).

#define BAUD 9600                 // Defines the UART communication speed as 9600 bits per second.

#define FOSC 12000000             // Defines the frequency of the external crystal oscillator as 12 MHz (12,000,000 Hz).

#define CCLK (FOSC*5)             // Defines the CPU clock frequency as 5 times the oscillator frequency (60 MHz).

#define PCLK (CCLK/4)             // Defines the peripheral clock frequency as one-fourth of the CPU clock (15 MHz).

#define DIVISOR (PCLK/(16 * BAUD)) // Calculates the UART baud-rate divisor using the peripheral clock and baud rate.

// Defines for UxLCR SFR (UART Line Control Register)

#define _8BIT             3       // Selects an 8-bit data word length for UART communication.

#define WORD_LEN_SEL_BITS 0       // Defines the starting bit position of the word length selection bits in UxLCR.

#define DLAB_BIT          7       // Defines bit 7 of UxLCR, used to enable or disable access to the divisor latch registers.

// Defines for UxLSR SFR (UART Line Status Register)

#define TEMT_BIT          6       // Defines bit 6 of UxLSR, indicating that the Transmitter Holding Register and Transmitter Shift Register are empty.

#define DR_BIT            0       // Defines bit 0 of UxLSR, indicating that received data is available in the Receiver Buffer Register.
