#include <LPC21xx.h>       // Includes LPC21xx microcontroller register definitions.
#include "types.h"         // Includes user-defined data types such as u8, s8, u32, s32 and f32.
#include "uart_defines.h"  // Includes UART pin configuration, baud rate and register bit definitions.
void UART0_Init(void)       // Function to initialize UART0.
{
    // Configure P0.0 and P0.1 pins as TXD0 and RXD0 respectively.

    PINSEL0 &= ~((3<<2)|(3<<0));  
    // Clears bits 0, 1, 2 and 3 of PINSEL0.
    // These bits control the functions of P0.0 and P0.1.

    PINSEL0 |= TXD0_PIN_FUNC | RXD0_PIN_FUNC;
    // Configures P0.0 as TXD0 (UART transmit).
    // Configures P0.1 as RXD0 (UART receive).

    // Configure U0LCR for 8N1 communication and enable DLAB.

    U0LCR = ((1<<DLAB_BIT) |
             (_8BIT<<WORD_LEN_SEL_BITS));
    // Sets DLAB bit to 1 to access baud-rate divisor registers.
    // Configures 8-bit data length.
    // 8N1 means 8 data bits, no parity and 1 stop bit.

    // Configure baud rate.

    U0DLL = DIVISOR;
    // Loads the lower 8 bits of the baud-rate divisor into U0DLL.

    U0DLM = DIVISOR >> 8;
    // Loads the upper 8 bits of the baud-rate divisor into U0DLM.

    // Reset or clear DLAB bit.

    U0LCR &= ~(1<<DLAB_BIT);
    // Clears DLAB bit to 0.
    // Enables access to U0RBR and U0THR registers.
}
void UART0_Tx(u8 sDat)       // Function to transmit one byte of data.
{
    // Write data into U0THR; serialization begins.

    U0THR = sDat;
    // Stores the given data byte in the UART0 Transmit Holding Register.
    // UART hardware starts transmitting the data serially.

    // Wait until transmission is completely finished.

    while(((U0LSR>>TEMT_BIT)&1)==0);
    // Checks bit 6 (TEMT) of the UART0 Line Status Register.
    // TEMT = 0: Transmitter is not completely empty.
    // TEMT = 1: Transmitter holding and shift registers are empty.
    // Waits until the complete transmission is finished.
}
u8 UART0_Rx(void)             // Function to receive one byte of data.
{
    // Wait until reception status is observed.

    while(((U0LSR>>DR_BIT)&1)==0);
    // Checks bit 0 (DR) of U0LSR.
    // DR = 0: No data available.
    // DR = 1: Received data is available.
    // Waits until data is received.

    // Read and return the received byte.

    return U0RBR;
    // Reads the received byte from UART0 Receiver Buffer Register.
    // Returns the received data to the calling function.
}
void U0_TxStr(s8 *s)          // Function to transmit a string.
{
    while(*s)                 // Checks whether the current character is not NULL.
        UART0_Tx(*s++);       // Transmits the current character and moves to the next character.
}
void U0_TxU32(u32 n)          // Function to transmit an unsigned 32-bit integer.
{
    s32 i=0;                  // Declares an index variable and initializes it to zero.
    u8 a[10];                 // Creates an array to store the extracted digits.

    if(n==0)                  // Checks whether the number is zero.
    {
        UART0_Tx('0');        // Transmits character '0'.
    }
    else
    {
        while(n>0)            // Repeats until all digits are extracted.
        {
            a[i++]=(n%10)+48; // Extracts the last digit and converts it into ASCII.
            n/=10;            // Removes the last digit from the number.
        }

        for(--i;i>=0;i--)     // Starts from the last stored digit and moves backwards.
        {
            UART0_Tx(a[i]);   // Transmits each digit in the correct order.
        }
    }
}
void U0_TxS32(s32 n)         // Function to transmit a signed 32-bit integer.
{
    if(n<0)                  // Checks whether the number is negative.
    {
        UART0_Tx('-');       // Transmits the minus sign.
        n=-n;                // Converts the number into a positive value.
    }

    U0_TxU32(n);             // Calls unsigned integer function to transmit the digits.
}
void U0_TxF32(f32 fn,u8 nDP)  // Function to transmit a floating-point number.
                              // fn = Floating-point number.
                              // nDP = Number of decimal places.
{
    u32 n,i;                  // Declares variables for integer part and loop counter.

    if(fn<0.0)                // Checks whether the floating-point number is negative.
    {
        UART0_Tx('-');        // Transmits the minus sign.
        fn=-fn;               // Converts the number into a positive value.
    }

    n=fn;                     // Extracts the integer part of the floating-point number.

    U0_TxU32(n);              // Transmits the integer part.

    UART0_Tx('.');            // Transmits the decimal point.

    for(i=0;i<nDP;i++)        // Repeats according to the required decimal places.
    {
        fn=(fn-n)*10;         // Extracts the next decimal digit by multiplying by 10.

        n=fn;                 // Stores the extracted integer digit.

        UART0_Tx(n+48);       // Converts the digit into ASCII and transmits it.
    }
}
s8 * U0_RxStr(void)            // Function to receive a string and return its address.
{
    static s8 rBuff[100];      // Creates a static character array to store received data.

    s8 t,i=0;                  // t stores received character; i is the array index.

    while(1)                   // Infinite loop to continuously receive characters.
    {
        t=UART0_Rx();          // Receives one character from UART0.

        rBuff[i]=t;            // Stores the received character in the array.

        UART0_Tx(t);           // Transmits the same character back (echo).

        if((t=='\n')||(t=='\r')) // Checks whether Enter key is pressed.
        {
            rBuff[i]='\0';     // Replaces the newline or carriage return with NULL.
            break;             // Terminates the receiving loop.
        }

        i++;                   // Increments the array index for the next character.
    }

    U0_TxStr("\n\r");          // Transmits newline and carriage return.

    return rBuff;              // Returns the address of the received string.
}
