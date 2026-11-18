// kpm.c - Contains functions to interface the 4x4 matrix keypad with LPC2148

#include <LPC21xx.h>       // Includes LPC2148 hardware register definitions

#include "defines.h"       // Includes bit manipulation macros
#include "types.h"         // Includes user-defined data types such as u32
#include "kpm_defines.h"   // Includes keypad row and column pin definitions
#include "delay.h"         // Includes delay function declarations
#include "interrupt.h"    // Includes RTC interrupt checking function declaration

/* Keypad lookup table */

// Stores the corresponding key values for each row and column
u32 kpmLUT[4][4] =
{
    {1, 2, 3, 10},         // Key values in the first row
    {4, 5, 6, 11},         // Key values in the second row
    {7, 8, 9, 12},         // Key values in the third row
    {14, 0, 15, 13}        // Key values in the fourth row
};

/* Check whether any keypad key is pressed */

// Function to check whether any keypad key is pressed
u32 ColScan(void)
{
    // Read the four column pins and store their status
    u32 status = READNIBBLE(IOPIN1, COL0);

    // Return 0 if a key is pressed; otherwise return 1
    return (status < 15U) ? 0U : 1U;
}

/* Identify the pressed row */

// Function to identify which keypad row contains the pressed key
u32 RowCheck(void)
{
    // Variable used to scan the four keypad rows
    u32 r;

    // Repeat the scanning process for all four rows
    for (r = 0; r < 4U; r++)
    {
        // Drive one row LOW and keep the remaining rows HIGH
        WRITENIBBLE(IOPIN1, ROW0, ((~(1U << r)) & 0x0FU));

        // Wait for 10 microseconds to allow the signal to stabilize
        delay_us(10);

        // Check whether a key is detected in the current row
        if (ColScan() == 0U)
            break;         // Stop scanning when the pressed row is identified
    }

    // Make all four rows LOW after scanning
    WRITENIBBLE(IOPIN1, ROW0, 0U);

    // Return the identified row number
    return r;
}

/* Identify the pressed column */

// Function to identify which keypad column contains the pressed key
u32 ColCheck(void)
{
    // Variable used to scan the four keypad columns
    u32 c;

    // Repeat the scanning process for all four columns
    for (c = 0; c < 4U; c++)
    {
        // Check whether the current column pin is LOW
        if (READBIT(IOPIN1, (COL0+c))==0U)
            break;         // Stop scanning when the pressed column is identified
    }

    // Return the identified column number
    return c;
}

/* Read keypad and check RTC interrupt */

// Function to read keypad input and check for RTC interrupts
u32 KeyScan(void)
{
    // Declare variables to store row, column and pressed key values
    u32 row, col, keyV;

    // Run continuously until a valid key is detected
    while (1)
    {
        /* Wait for keypad press */

        // Continue waiting while no key is pressed
        while (ColScan() != 0U)
        {
            // Check whether an RTC interrupt has occurred
            CheckRTCInterrupt();

            // Wait for 10 milliseconds before checking again
            delay_ms(10);
        }

        /* Debounce key press */

        // Wait for 15 milliseconds to avoid detecting switch bouncing
        delay_ms(15);

        // Check again to confirm that the key is still pressed
        if (ColScan() != 0U)
            continue;      // Restart the loop if the key press was not confirmed

        /* Identify row and column */

        // Identify the row in which the key is pressed
        row = RowCheck();

        // Identify the column in which the key is pressed
        col = ColCheck();

        /* Get key value */

        // Check whether the detected row and column are valid
        if (row < 4U && col < 4U)
            keyV = kpmLUT[row][col];  // Get the key value from the lookup table
        else
            keyV = 16U;               // Store an invalid key value

        /* Wait until the pressed key is released */

        // Continue waiting while the key remains pressed
        while (ColScan() == 0U)
        {
            // Wait for 10 milliseconds before checking again
            delay_ms(10);
        }

        /* Debounce key release */

        // Wait for 15 milliseconds to confirm that the key is released
        delay_ms(15);

        /*
         * If RTC interrupt occurred while a key
         * was pressed, process it and discard
         * that old key press.
         */

        /* Return valid keypad value */

        // Check whether the detected key value is valid
        if (keyV < 16U)
            return keyV;   // Return the detected key value to the calling function
    }
}

/* Initialize keypad */

// Function to initialize the keypad
void InitKPM(void)
{
    // Configure the four keypad row pins as output pins
    IODIR1 |= (15U << ROW0);

    // Make all four keypad rows LOW initially
    WRITENIBBLE(IOPIN1, ROW0, 0U);
}
