// uart.h

#include "types.h"       // Includes user-defined data types such as u8, s8, u32, s32 and f32.

// Primary drivers

void UART0_Init(void);   // Initializes UART0 by configuring baud rate, pins and communication settings.

void UART0_Tx(u8 sDat);  // Transmits a single 8-bit character through UART0.

u8 UART0_Rx(void);       // Receives a single 8-bit character through UART0 and returns the received data.

// Secondary drivers

void U0_TxStr(s8 *);     // Transmits a string through UART0.

void U0_TxU32(u32 );     // Transmits an unsigned 32-bit integer through UART0.

void U0_TxS32(s32 );     // Transmits a signed 32-bit integer through UART0.

void U0_TxF32(f32,u8);   // Transmits a floating-point number through UART0.
                         // First argument: Floating-point value.
                         // Second argument: Number of decimal places to display.

s8 * U0_RxStr(void);     // Receives a string through UART0 and returns a pointer to the received string.
