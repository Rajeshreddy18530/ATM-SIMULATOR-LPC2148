// lcd.c - Contains functions to control the LCD display

#include <LPC21xx.h>       // Includes LPC2148 register definitions
#include "lcd_defines.h"   // Includes LCD pin numbers and command definitions
#include "defines.h"       // Includes bit manipulation macros
#include "types.h"         // Includes user-defined data types such as u8, u32 and s32
#include "delay.h"        // Includes delay function declarations

// Function to send a byte to the LCD
void WriteLCD(u8 byte)
{
    // Select write operation by making RW pin LOW
    IOCLR0=1<<LCD_RW;

    // Place the given byte on LCD data pins D0 to D7
    WRITEBYTE(IOPIN0,LCD_DATA,byte);

    // Make EN pin HIGH to start the latching pulse
    IOSET0=1<<LCD_EN;

    // Wait for 1 microsecond
    delay_us(1);

    // Make EN pin LOW to complete the latching pulse
    IOCLR0=1<<LCD_EN;

    // Wait for 2 milliseconds for LCD processing
    delay_ms(2);
}

// Function to send a command to the LCD
void CmdLCD(u8 cmd)
{
    // Select command register by making RS pin LOW
    IOCLR0=1<<LCD_RS;

    // Send the command byte to the LCD
    WriteLCD(cmd);
}

// Function to initialize the LCD
void InitLCD(void)
{
    // Configure LCD connection pins as GPIO output pins

    // Configure LCD data pins D0-D7 as output (commented original code)
    // WRITEBYTE(IODIR0,LCD_DATA,0xFF);

    // Configure RS pin as output (commented original code)
    // SETBIT(IODIR0,LCD_RS);

    // Configure RW pin as output (commented original code)
    // SETBIT(IODIR0,LCD_RW);

    // Configure EN pin as output (commented original code)
    // SETBIT(IODIR0,LCD_EN);

    // Configure all LCD data and control pins as outputs
    IODIR0|=(0xFF<<LCD_DATA)|(1<<LCD_RS)|
            (1<<LCD_RW)|(1<<LCD_EN);

    // Wait for 15 milliseconds after powering ON the LCD
    delay_ms(15);

    // Send the first initialization command
    CmdLCD(0x30);

    // Wait for 4 milliseconds
    delay_ms(4);

    // Wait for 100 microseconds
    delay_us(100);

    // Send the second initialization command
    CmdLCD(0x30);

    // Wait for 100 microseconds
    delay_us(100);

    // Send the third initialization command
    CmdLCD(0x30);

    // Configure LCD for 8-bit mode and 2-line display
    CmdLCD(MODE_8BIT_2LINE);

    // Turn ON display and turn OFF cursor
    CmdLCD(DSP_ON_CUR_OFF);

    // Clear all characters from the LCD
    CmdLCD(CLEAR_LCD);

    // Set cursor movement direction to right
    CmdLCD(SHIFT_CUR_RIGHT);
}

// Function to display a single character on the LCD
void CharLCD(u8 asciiVal)
{
    // Select data register by making RS pin HIGH
    IOSET0=1<<LCD_RS;

    // Send the character to the LCD
    WriteLCD(asciiVal);
}

// Function to display a string on the LCD
void StrLCD(const char *s)
{
    // Continue until the null character '\0' is reached
    while(*s)

        // Display the current character and move to the next character
        CharLCD(*s++);
}

// Function to display an unsigned 32-bit integer on the LCD
void U32LCD(u32 n)
{
    // Declare a signed index variable and an array to store digits
    s32 i=0;
    u8 a[10];

    // Check whether the number is zero
    if(n==0)
    {
        // Display character '0' on the LCD
        CharLCD('0');
    }
    else
    {
        // Repeat until all digits of the number are extracted
        while(n>0)
        {
            // Extract the last digit and convert it to its ASCII value
            a[i++]=(n%10)+48;

            // Remove the last digit from the number
            n/=10;
        }

        // Start from the last stored digit and display digits in reverse order
        for(--i;i>=0;i--)
        {
            // Display each digit on the LCD
            CharLCD(a[i]);
        }
    }
}

// Function to display a signed 32-bit integer on the LCD
void S32LCD(s32 n)
{
    // Check whether the number is negative
    if(n<0)
    {
        // Display the negative sign
        CharLCD('-');

        // Convert the negative number into a positive number
        n=-n;
    }

    // Display the number using the unsigned integer display function
    U32LCD(n);
}

// Function to display a floating-point number with the specified decimal places
void F32LCD(f32 fn,u8 nDP)
{
    // Declare variables for integer and decimal digit processing
    u32 n,i;

    // Check whether the floating-point number is negative
    if(fn<0.0)
    {
        // Display the negative sign
        CharLCD('-');

        // Convert the number into a positive value
        fn=-fn;
    }

    // Extract the integer part of the floating-point number
    n=fn;

    // Display the integer part on the LCD
    U32LCD(n);

    // Display the decimal point
    CharLCD('.');

    // Repeat according to the required number of decimal places
    for(i=0;i<nDP;i++)
    {
        // Extract the next decimal digit by multiplying the fractional part by 10
        fn=(fn-n) * 10;

        // Store the integer part of the result as the next decimal digit
        n=fn;

        // Convert the digit into ASCII and display it
        CharLCD(n+48);
    }
}

// Function to create custom characters in LCD CG
