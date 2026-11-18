#include <LPC21xx.h>  // Includes the LPC21xx.h declarations or definitions.
#include "rtc.h"  // Includes the rtc.h declarations or definitions.
#include "kpm.h"  // Includes the kpm.h declarations or definitions.
#include "lcd.h"  // Includes the lcd.h declarations or definitions.
#include "lcd_defines.h"  // Includes the lcd_defines.h declarations or definitions.
#include "delay.h"  // Includes the delay.h declarations or definitions.
#include "interrupt.h"  // Includes the interrupt.h declarations or definitions.

volatile int interruptFlag = 0;  // Declares a flag that can be changed unexpectedly by the interrupt service routine.


/*------------------------------------------------
              INTERRUPT INITIALIZATION  // Continues the current operation in this function.
------------------------------------------------*/  // Continues the current operation in this function.

void Init_interrupt(void)  // Begins the Init_interrupt function.
{  // Begins this function, loop, conditional block, or switch block.
    /* Configure P0.3 as EINT1 */
    PINSEL0 &= ~(3U << 6);  // Clears the pin-function selection bits for P0.3 before configuring its alternate function.
    PINSEL0 |=  (3U << 6);  // Selects the EINT1 alternate function for pin P0.3.

    /* Configure EINT1 as IRQ */
    VICIntSelect &= ~(1U << 15);  // Selects IRQ mode for EINT1 in the interrupt controller.

    /* Configure falling-edge-sensitive interrupt */
    EXTMODE |= (1U << 1);  // Configures EINT1 to trigger on an edge rather than a level.
    EXTPOLAR &= ~(1U << 1);  // Selects a falling edge as the EINT1 trigger.

    /* Clear pending interrupt */
    EXTINT = (1U << 1);  // Clears the pending EINT1 interrupt flag by writing a one to its bit.

    /* Configure vectored interrupt slot 0 */
    VICVectAddr0 = (unsigned long)eint1_irq;  // Stores the address of the EINT1 interrupt service routine in vector slot 0.
    VICVectCntl0 = (1U << 5) | 15;  // Enables vectored interrupt slot 0 and assigns it to interrupt source 15 (EINT1).

    EXTINT = (1U << 1);  // Clears the pending EINT1 interrupt flag by writing a one to its bit.

    /* Enable EINT1 */
    VICIntEnable |= (1U << 15);  // Enables EINT1 in the vectored interrupt controller.
}  // Ends the current function, loop, conditional block, or switch block.


/*------------------------------------------------
                    ISR  // Continues the current operation in this function.
------------------------------------------------*/  // Continues the current operation in this function.

void eint1_irq(void) __irq  // Begins the eint1_irq function.
{  // Begins this function, loop, conditional block, or switch block.
    interruptFlag = 1;  // Sets the software flag to tell the main program that EINT1 occurred.

    EXTINT = (1U << 1);  // Clears the pending EINT1 interrupt flag by writing a one to its bit.

    VICVectAddr = 0;  // Signals to the interrupt controller that the interrupt service routine has finished.
}  // Ends the current function, loop, conditional block, or switch block.


/*------------------------------------------------
                 EDIT DATE MENU  // Continues the current operation in this function.
------------------------------------------------*/  // Continues the current operation in this function.

/* Returns 1 for BACK, 0 for CANCEL */

static int Edit_Date(void)  // Begins the Edit_Date function.
{  // Begins this function, loop, conditional block, or switch block.
    unsigned int d, mon, y;  // Declares a local variable used by this function.
    int key;  // Declares a local variable used by this function.
    int i;  // Declares a local variable used by this function.
    int choice;  // Declares a local variable used by this function.

    while (1)  // Repeats this block while the specified condition remains true.
    {  // Begins this function, loop, conditional block, or switch block.
        readrtc();  // Reads the current time and date from the RTC registers into the shared variables.

        CmdLCD(0x01);  // Clears the LCD display.
        delay_ms(5);  // Waits for 5 milliseconds.

        CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
        StrLCD("1.DATE  2.MONTH");  // Displays "1.DATE  2.MONTH" on the LCD.

        CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
        StrLCD("3.YEAR  4.BACK");  // Displays "3.YEAR  4.BACK" on the LCD.

        choice = KeyScan();  // Scans the keypad and stores the pressed key in choice.
        delay_ms(150);  // Waits for 150 milliseconds.

        switch (choice)  // Chooses which menu action to execute based on the selected key.
        {  // Begins this function, loop, conditional block, or switch block.

            /*--------------------------------
                     ENTER DATE  // Continues the current operation in this function.
            --------------------------------*/  // Continues the current operation in this function.

            case 1:  // Selects the code for this keypad/menu choice.

                CmdLCD(0x01);  // Clears the LCD display.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                StrLCD("Enter Day:");  // Displays "Enter Day:" on the LCD.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

                i = 0;  // Executes this statement as part of the current operation.
                d = 0;  // Executes this statement as part of the current operation.

                while (i < 2)  // Repeats this block while the specified condition remains true.
                {  // Begins this function, loop, conditional block, or switch block.
                    key = KeyScan();  // Scans the keypad and stores the pressed key in key.
                    delay_ms(500);  // Waits for 500 milliseconds.

                    if (key >= 0 && key <= 9)  // Checks the condition before executing the following block.
                    {  // Begins this function, loop, conditional block, or switch block.
                        d = d * 10 + key;  // Appends the newly pressed digit to the number being entered.

                        CharLCD(key + '0');  // Displays the entered character or digit on the LCD.

                        i++;  // Increases the digit counter after accepting a digit.
                        delay_ms(200);  // Waits for 200 milliseconds.
                    }  // Ends the current function, loop, conditional block, or switch block.

                    else if (key == 11)  // Checks this alternative condition when the previous condition was false.
                    {  // Begins this function, loop, conditional block, or switch block.
                        if (i > 0)  // Checks the condition before executing the following block.
                        {  // Begins this function, loop, conditional block, or switch block.
                            d /= 10;  // Removes the last entered decimal digit by dividing the value by 10.
                            i--;  // Decreases the digit counter after deleting a digit.

                            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                            delay_ms(5);  // Waits for 5 milliseconds.

                            if (i == 1)  // Checks the condition before executing the following block.
                            {  // Begins this function, loop, conditional block, or switch block.
                                CharLCD(d + '0');  // Displays the entered character or digit on the LCD.
                                CharLCD(' ');  // Displays the entered character or digit on the LCD.
                            }  // Ends the current function, loop, conditional block, or switch block.
                            else  // Executes this alternative block when the preceding condition is false.
                            {  // Begins this function, loop, conditional block, or switch block.
                                CharLCD(' ');  // Displays the entered character or digit on the LCD.
                            }  // Ends the current function, loop, conditional block, or switch block.

                            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                            delay_ms(5);  // Waits for 5 milliseconds.
                        }  // Ends the current function, loop, conditional block, or switch block.
                    }  // Ends the current function, loop, conditional block, or switch block.

                    else if (key == 13)  // Checks this alternative condition when the previous condition was false.
                    {  // Begins this function, loop, conditional block, or switch block.
                        return 0;  // Returns the specified value to the calling function, or exits the function.
                    }  // Ends the current function, loop, conditional block, or switch block.
                }  // Ends the current function, loop, conditional block, or switch block.

                if (d < 1 || d > 31)  // Checks the condition before executing the following block.
                {  // Begins this function, loop, conditional block, or switch block.
                    CmdLCD(0x01);  // Clears the LCD display.
                    delay_ms(5);  // Waits for 5 milliseconds.

                    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                    StrLCD("Invalid Day");  // Displays "Invalid Day" on the LCD.

                    delay_ms(1500);  // Waits for 1500 milliseconds.
                }  // Ends the current function, loop, conditional block, or switch block.
                else  // Executes this alternative block when the preceding condition is false.
                {  // Begins this function, loop, conditional block, or switch block.
                    day = d;  // Stores the newly entered value in the shared RTC variable.

                    UpdateRTC(hours, minutes, seconds,  // Writes the edited date/time values into the RTC registers.
                              day, month, year);  // Executes this statement as part of the current operation.

                    readrtc();  // Reads the current time and date from the RTC registers into the shared variables.
                }  // Ends the current function, loop, conditional block, or switch block.

                break;  // Exits the current switch case or loop.


            /*--------------------------------
                     ENTER MONTH  // Continues the current operation in this function.
            --------------------------------*/  // Continues the current operation in this function.

            case 2:  // Selects the code for this keypad/menu choice.

                CmdLCD(0x01);  // Clears the LCD display.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                StrLCD("Enter Month:");  // Displays "Enter Month:" on the LCD.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

                i = 0;  // Executes this statement as part of the current operation.
                mon = 0;  // Executes this statement as part of the current operation.

                while (i < 2)  // Repeats this block while the specified condition remains true.
                {  // Begins this function, loop, conditional block, or switch block.
                    key = KeyScan();  // Scans the keypad and stores the pressed key in key.
                    delay_ms(500);  // Waits for 500 milliseconds.

                    if (key >= 0 && key <= 9)  // Checks the condition before executing the following block.
                    {  // Begins this function, loop, conditional block, or switch block.
                        mon = mon * 10 + key;  // Appends the newly pressed digit to the number being entered.

                        CharLCD(key + '0');  // Displays the entered character or digit on the LCD.

                        i++;  // Increases the digit counter after accepting a digit.
                        delay_ms(200);  // Waits for 200 milliseconds.
                    }  // Ends the current function, loop, conditional block, or switch block.

                    else if (key == 11)  // Checks this alternative condition when the previous condition was false.
                    {  // Begins this function, loop, conditional block, or switch block.
                        if (i > 0)  // Checks the condition before executing the following block.
                        {  // Begins this function, loop, conditional block, or switch block.
                            mon /= 10;  // Removes the last entered decimal digit by dividing the value by 10.
                            i--;  // Decreases the digit counter after deleting a digit.

                            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                            delay_ms(5);  // Waits for 5 milliseconds.

                            if (i == 1)  // Checks the condition before executing the following block.
                            {  // Begins this function, loop, conditional block, or switch block.
                                CharLCD(mon + '0');  // Displays the entered character or digit on the LCD.
                                CharLCD(' ');  // Displays the entered character or digit on the LCD.
                            }  // Ends the current function, loop, conditional block, or switch block.
                            else  // Executes this alternative block when the preceding condition is false.
                            {  // Begins this function, loop, conditional block, or switch block.
                                CharLCD(' ');  // Displays the entered character or digit on the LCD.
                            }  // Ends the current function, loop, conditional block, or switch block.

                            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                            delay_ms(5);  // Waits for 5 milliseconds.
                        }  // Ends the current function, loop, conditional block, or switch block.
                    }  // Ends the current function, loop, conditional block, or switch block.

                    else if (key == 13)  // Checks this alternative condition when the previous condition was false.
                    {  // Begins this function, loop, conditional block, or switch block.
                        return 0;  // Returns the specified value to the calling function, or exits the function.
                    }  // Ends the current function, loop, conditional block, or switch block.
                }  // Ends the current function, loop, conditional block, or switch block.

                if (mon < 1 || mon > 12)  // Checks the condition before executing the following block.
                {  // Begins this function, loop, conditional block, or switch block.
                    CmdLCD(0x01);  // Clears the LCD display.
                    delay_ms(5);  // Waits for 5 milliseconds.

                    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                    StrLCD("Invalid Month");  // Displays "Invalid Month" on the LCD.

                    delay_ms(1500);  // Waits for 1500 milliseconds.
                }  // Ends the current function, loop, conditional block, or switch block.
                else  // Executes this alternative block when the preceding condition is false.
                {  // Begins this function, loop, conditional block, or switch block.
                    month = mon;  // Stores the newly entered value in the shared RTC variable.

                    UpdateRTC(hours, minutes, seconds,  // Writes the edited date/time values into the RTC registers.
                              day, month, year);  // Executes this statement as part of the current operation.

                    readrtc();  // Reads the current time and date from the RTC registers into the shared variables.
                }  // Ends the current function, loop, conditional block, or switch block.

                break;  // Exits the current switch case or loop.


            /*--------------------------------
                      ENTER YEAR  // Continues the current operation in this function.
            --------------------------------*/  // Continues the current operation in this function.

            case 3:  // Selects the code for this keypad/menu choice.

                CmdLCD(0x01);  // Clears the LCD display.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                StrLCD("Enter Year:");  // Displays "Enter Year:" on the LCD.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

                i = 0;  // Executes this statement as part of the current operation.
                y = 0;  // Executes this statement as part of the current operation.

                while (i < 4)  // Repeats this block while the specified condition remains true.
                {  // Begins this function, loop, conditional block, or switch block.
                    key = KeyScan();  // Scans the keypad and stores the pressed key in key.
                    delay_ms(500);  // Waits for 500 milliseconds.

                    if (key >= 0 && key <= 9)  // Checks the condition before executing the following block.
                    {  // Begins this function, loop, conditional block, or switch block.
                        y = y * 10 + key;  // Appends the newly pressed digit to the number being entered.

                        CharLCD(key + '0');  // Displays the entered character or digit on the LCD.

                        i++;  // Increases the digit counter after accepting a digit.
                        delay_ms(200);  // Waits for 200 milliseconds.
                    }  // Ends the current function, loop, conditional block, or switch block.

                    else if (key == 11)  /* B = Backspace */  // Checks this alternative condition when the previous condition was false.
                    {  // Begins this function, loop, conditional block, or switch block.
                        if (i > 0)  // Checks the condition before executing the following block.
                        {  // Begins this function, loop, conditional block, or switch block.
                            y /= 10;  // Removes the last entered decimal digit by dividing the value by 10.
                            i--;  // Decreases the digit counter after deleting a digit.

                            /* Clear previous year digits */
                            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                            delay_ms(5);  // Waits for 5 milliseconds.

                            StrLCD("    ");  // Displays "    " on the LCD.

                            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                            delay_ms(5);  // Waits for 5 milliseconds.

                            /* Redraw remaining year digits */

                            if (i == 1)  // Checks the condition before executing the following block.
                            {  // Begins this function, loop, conditional block, or switch block.
                                CharLCD((y % 10) + '0');  // Displays the entered character or digit on the LCD.
                            }  // Ends the current function, loop, conditional block, or switch block.

                            else if (i == 2)  // Checks this alternative condition when the previous condition was false.
                            {  // Begins this function, loop, conditional block, or switch block.
                                CharLCD((y / 10) + '0');  // Displays the entered character or digit on the LCD.
                                CharLCD((y % 10) + '0');  // Displays the entered character or digit on the LCD.
                            }  // Ends the current function, loop, conditional block, or switch block.

                            else if (i == 3)  // Checks this alternative condition when the previous condition was false.
                            {  // Begins this function, loop, conditional block, or switch block.
                                CharLCD((y / 100) + '0');  // Displays the entered character or digit on the LCD.

                                CharLCD(((y / 10) % 10) + '0');  // Displays the entered character or digit on the LCD.

                                CharLCD((y % 10) + '0');  // Displays the entered character or digit on the LCD.
                            }  // Ends the current function, loop, conditional block, or switch block.

                            delay_ms(100);  // Waits for 100 milliseconds.
                        }  // Ends the current function, loop, conditional block, or switch block.
                    }  // Ends the current function, loop, conditional block, or switch block.

                    else if (key == 13)  // Checks this alternative condition when the previous condition was false.
                    {  // Begins this function, loop, conditional block, or switch block.
                        return 0;  // Returns the specified value to the calling function, or exits the function.
                    }  // Ends the current function, loop, conditional block, or switch block.
                }  // Ends the current function, loop, conditional block, or switch block.

                if (y != 2026)  // Checks the condition before executing the following block.
                {  // Begins this function, loop, conditional block, or switch block.
                    CmdLCD(0x01);  // Clears the LCD display.
                    delay_ms(5);  // Waits for 5 milliseconds.

                    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                    StrLCD("Invalid Year");  // Displays "Invalid Year" on the LCD.

                    delay_ms(1500);  // Waits for 1500 milliseconds.
                }  // Ends the current function, loop, conditional block, or switch block.
                else  // Executes this alternative block when the preceding condition is false.
                {  // Begins this function, loop, conditional block, or switch block.
                    year = y;  // Stores the newly entered value in the shared RTC variable.

                    UpdateRTC(hours, minutes, seconds,  // Writes the edited date/time values into the RTC registers.
                              day, month, year);  // Executes this statement as part of the current operation.

                    readrtc();  // Reads the current time and date from the RTC registers into the shared variables.
                }  // Ends the current function, loop, conditional block, or switch block.

                break;  // Exits the current switch case or loop.


            /*--------------------------------
                        BACK  // Continues the current operation in this function.
            --------------------------------*/  // Continues the current operation in this function.

            case 4:  // Selects the code for this keypad/menu choice.

                return 1;  // Returns the specified value to the calling function, or exits the function.


            default:  // Handles any menu choice not covered by the listed cases.

                CmdLCD(0x01);  // Clears the LCD display.
                delay_ms(5);  // Waits for 5 milliseconds.

                StrLCD("INVALID OPTION");  // Displays "INVALID OPTION" on the LCD.

                delay_ms(1000);  // Waits for 1000 milliseconds.

                break;  // Exits the current switch case or loop.
        }  // Ends the current function, loop, conditional block, or switch block.
    }  // Ends the current function, loop, conditional block, or switch block.
}  // Ends the current function, loop, conditional block, or switch block.


/*------------------------------------------------
                  EDIT TIME MENU  // Continues the current operation in this function.
------------------------------------------------*/  // Continues the current operation in this function.

/* Returns 1 for BACK, 0 for CANCEL */

static int Edit_Time(void)  // Begins the Edit_Time function.
{  // Begins this function, loop, conditional block, or switch block.
    unsigned int h, m, s;  // Declares a local variable used by this function.
    int key;  // Declares a local variable used by this function.
    int i;  // Declares a local variable used by this function.
    int choice;  // Declares a local variable used by this function.

    while (1)  // Repeats this block while the specified condition remains true.
    {  // Begins this function, loop, conditional block, or switch block.
        readrtc();  // Reads the current time and date from the RTC registers into the shared variables.

        CmdLCD(0x01);  // Clears the LCD display.
        delay_ms(5);  // Waits for 5 milliseconds.

        CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
        StrLCD("1.HOUR  2.MIN");  // Displays "1.HOUR  2.MIN" on the LCD.

        CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
        StrLCD("3.SEC   4.BACK");  // Displays "3.SEC   4.BACK" on the LCD.

        choice = KeyScan();  // Scans the keypad and stores the pressed key in choice.
        delay_ms(150);  // Waits for 150 milliseconds.

        switch (choice)  // Chooses which menu action to execute based on the selected key.
        {  // Begins this function, loop, conditional block, or switch block.

            /*--------------------------------
                     ENTER HOURS  // Continues the current operation in this function.
            --------------------------------*/  // Continues the current operation in this function.

            case 1:  // Selects the code for this keypad/menu choice.

                CmdLCD(0x01);  // Clears the LCD display.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                StrLCD("Enter Hours:");  // Displays "Enter Hours:" on the LCD.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

                i = 0;  // Executes this statement as part of the current operation.
                h = 0;  // Executes this statement as part of the current operation.

                while (i < 2)  // Repeats this block while the specified condition remains true.
                {  // Begins this function, loop, conditional block, or switch block.
                    key = KeyScan();  // Scans the keypad and stores the pressed key in key.
                    delay_ms(500);  // Waits for 500 milliseconds.

                    if (key >= 0 && key <= 9)  // Checks the condition before executing the following block.
                    {  // Begins this function, loop, conditional block, or switch block.
                        h = h * 10 + key;  // Appends the newly pressed digit to the number being entered.

                        CharLCD(key + '0');  // Displays the entered character or digit on the LCD.

                        i++;  // Increases the digit counter after accepting a digit.
                        delay_ms(200);  // Waits for 200 milliseconds.
                    }  // Ends the current function, loop, conditional block, or switch block.

                    else if (key == 11)  // Checks this alternative condition when the previous condition was false.
                    {  // Begins this function, loop, conditional block, or switch block.
                        if (i > 0)  // Checks the condition before executing the following block.
                        {  // Begins this function, loop, conditional block, or switch block.
                            h /= 10;  // Removes the last entered decimal digit by dividing the value by 10.
                            i--;  // Decreases the digit counter after deleting a digit.

                            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                            delay_ms(5);  // Waits for 5 milliseconds.

                            if (i == 1)  // Checks the condition before executing the following block.
                            {  // Begins this function, loop, conditional block, or switch block.
                                CharLCD(h + '0');  // Displays the entered character or digit on the LCD.
                                CharLCD(' ');  // Displays the entered character or digit on the LCD.
                            }  // Ends the current function, loop, conditional block, or switch block.
                            else  // Executes this alternative block when the preceding condition is false.
                            {  // Begins this function, loop, conditional block, or switch block.
                                CharLCD(' ');  // Displays the entered character or digit on the LCD.
                            }  // Ends the current function, loop, conditional block, or switch block.

                            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                            delay_ms(5);  // Waits for 5 milliseconds.
                        }  // Ends the current function, loop, conditional block, or switch block.
                    }  // Ends the current function, loop, conditional block, or switch block.

                    else if (key == 13)  // Checks this alternative condition when the previous condition was false.
                    {  // Begins this function, loop, conditional block, or switch block.
                        return 0;  // Returns the specified value to the calling function, or exits the function.
                    }  // Ends the current function, loop, conditional block, or switch block.
                }  // Ends the current function, loop, conditional block, or switch block.

                if (h >= 24)  // Checks the condition before executing the following block.
                {  // Begins this function, loop, conditional block, or switch block.
                    CmdLCD(0x01);  // Clears the LCD display.
                    delay_ms(5);  // Waits for 5 milliseconds.

                    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                    StrLCD("Invalid Hours");  // Displays "Invalid Hours" on the LCD.

                    delay_ms(1500);  // Waits for 1500 milliseconds.
                }  // Ends the current function, loop, conditional block, or switch block.
                else  // Executes this alternative block when the preceding condition is false.
                {  // Begins this function, loop, conditional block, or switch block.
                    hours = h;  // Stores the newly entered value in the shared RTC variable.

                    UpdateRTC(hours, minutes, seconds,  // Writes the edited date/time values into the RTC registers.
                              day, month, year);  // Executes this statement as part of the current operation.

                    readrtc();  // Reads the current time and date from the RTC registers into the shared variables.
                }  // Ends the current function, loop, conditional block, or switch block.

                break;  // Exits the current switch case or loop.


            /*--------------------------------
                    ENTER MINUTES  // Continues the current operation in this function.
            --------------------------------*/  // Continues the current operation in this function.

            case 2:  // Selects the code for this keypad/menu choice.

                CmdLCD(0x01);  // Clears the LCD display.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                StrLCD("Enter Minutes:");  // Displays "Enter Minutes:" on the LCD.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

                i = 0;  // Executes this statement as part of the current operation.
                m = 0;  // Executes this statement as part of the current operation.

                while (i < 2)  // Repeats this block while the specified condition remains true.
                {  // Begins this function, loop, conditional block, or switch block.
                    key = KeyScan();  // Scans the keypad and stores the pressed key in key.
                    delay_ms(500);  // Waits for 500 milliseconds.

                    if (key >= 0 && key <= 9)  // Checks the condition before executing the following block.
                    {  // Begins this function, loop, conditional block, or switch block.
                        m = m * 10 + key;  // Appends the newly pressed digit to the number being entered.

                        CharLCD(key + '0');  // Displays the entered character or digit on the LCD.

                        i++;  // Increases the digit counter after accepting a digit.
                        delay_ms(200);  // Waits for 200 milliseconds.
                    }  // Ends the current function, loop, conditional block, or switch block.

                    else if (key == 11)  // Checks this alternative condition when the previous condition was false.
                    {  // Begins this function, loop, conditional block, or switch block.
                        if (i > 0)  // Checks the condition before executing the following block.
                        {  // Begins this function, loop, conditional block, or switch block.
                            m /= 10;  // Removes the last entered decimal digit by dividing the value by 10.
                            i--;  // Decreases the digit counter after deleting a digit.

                            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                            delay_ms(5);  // Waits for 5 milliseconds.

                            if (i == 1)  // Checks the condition before executing the following block.
                            {  // Begins this function, loop, conditional block, or switch block.
                                CharLCD(m + '0');  // Displays the entered character or digit on the LCD.
                                CharLCD(' ');  // Displays the entered character or digit on the LCD.
                            }  // Ends the current function, loop, conditional block, or switch block.
                            else  // Executes this alternative block when the preceding condition is false.
                            {  // Begins this function, loop, conditional block, or switch block.
                                CharLCD(' ');  // Displays the entered character or digit on the LCD.
                            }  // Ends the current function, loop, conditional block, or switch block.

                            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                            delay_ms(5);  // Waits for 5 milliseconds.
                        }  // Ends the current function, loop, conditional block, or switch block.
                    }  // Ends the current function, loop, conditional block, or switch block.

                    else if (key == 13)  // Checks this alternative condition when the previous condition was false.
                    {  // Begins this function, loop, conditional block, or switch block.
                        return 0;  // Returns the specified value to the calling function, or exits the function.
                    }  // Ends the current function, loop, conditional block, or switch block.
                }  // Ends the current function, loop, conditional block, or switch block.

                if (m >= 60)  // Checks the condition before executing the following block.
                {  // Begins this function, loop, conditional block, or switch block.
                    CmdLCD(0x01);  // Clears the LCD display.
                    delay_ms(5);  // Waits for 5 milliseconds.

                    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                    StrLCD("Invalid Minutes");  // Displays "Invalid Minutes" on the LCD.

                    delay_ms(1500);  // Waits for 1500 milliseconds.
                }  // Ends the current function, loop, conditional block, or switch block.
                else  // Executes this alternative block when the preceding condition is false.
                {  // Begins this function, loop, conditional block, or switch block.
                    minutes = m;  // Stores the newly entered value in the shared RTC variable.

                    UpdateRTC(hours, minutes, seconds,  // Writes the edited date/time values into the RTC registers.
                              day, month, year);  // Executes this statement as part of the current operation.

                    readrtc();  // Reads the current time and date from the RTC registers into the shared variables.
                }  // Ends the current function, loop, conditional block, or switch block.

                break;  // Exits the current switch case or loop.


            /*--------------------------------
                    ENTER SECONDS  // Continues the current operation in this function.
            --------------------------------*/  // Continues the current operation in this function.

            case 3:  // Selects the code for this keypad/menu choice.

                CmdLCD(0x01);  // Clears the LCD display.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                StrLCD("Enter Seconds:");  // Displays "Enter Seconds:" on the LCD.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

                i = 0;  // Executes this statement as part of the current operation.
                s = 0;  // Executes this statement as part of the current operation.

                while (i < 2)  // Repeats this block while the specified condition remains true.
                {  // Begins this function, loop, conditional block, or switch block.
                    key = KeyScan();  // Scans the keypad and stores the pressed key in key.
                    delay_ms(500);  // Waits for 500 milliseconds.

                    if (key >= 0 && key <= 9)  // Checks the condition before executing the following block.
                    {  // Begins this function, loop, conditional block, or switch block.
                        s = s * 10 + key;  // Appends the newly pressed digit to the number being entered.

                        CharLCD(key + '0');  // Displays the entered character or digit on the LCD.

                        i++;  // Increases the digit counter after accepting a digit.
                        delay_ms(200);  // Waits for 200 milliseconds.
                    }  // Ends the current function, loop, conditional block, or switch block.

                    else if (key == 11)  // Checks this alternative condition when the previous condition was false.
                    {  // Begins this function, loop, conditional block, or switch block.
                        if (i > 0)  // Checks the condition before executing the following block.
                        {  // Begins this function, loop, conditional block, or switch block.
                            s /= 10;  // Removes the last entered decimal digit by dividing the value by 10.
                            i--;  // Decreases the digit counter after deleting a digit.

                            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                            delay_ms(5);  // Waits for 5 milliseconds.

                            if (i == 1)  // Checks the condition before executing the following block.
                            {  // Begins this function, loop, conditional block, or switch block.
                                CharLCD(s + '0');  // Displays the entered character or digit on the LCD.
                                CharLCD(' ');  // Displays the entered character or digit on the LCD.
                            }  // Ends the current function, loop, conditional block, or switch block.
                            else  // Executes this alternative block when the preceding condition is false.
                            {  // Begins this function, loop, conditional block, or switch block.
                                CharLCD(' ');  // Displays the entered character or digit on the LCD.
                            }  // Ends the current function, loop, conditional block, or switch block.

                            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                            delay_ms(5);  // Waits for 5 milliseconds.
                        }  // Ends the current function, loop, conditional block, or switch block.
                    }  // Ends the current function, loop, conditional block, or switch block.

                    else if (key == 13)  // Checks this alternative condition when the previous condition was false.
                    {  // Begins this function, loop, conditional block, or switch block.
                        return 0;  // Returns the specified value to the calling function, or exits the function.
                    }  // Ends the current function, loop, conditional block, or switch block.
                }  // Ends the current function, loop, conditional block, or switch block.

                if (s >= 60)  // Checks the condition before executing the following block.
                {  // Begins this function, loop, conditional block, or switch block.
                    CmdLCD(0x01);  // Clears the LCD display.
                    delay_ms(5);  // Waits for 5 milliseconds.

                    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                    StrLCD("Invalid Seconds");  // Displays "Invalid Seconds" on the LCD.

                    delay_ms(1500);  // Waits for 1500 milliseconds.
                }  // Ends the current function, loop, conditional block, or switch block.
                else  // Executes this alternative block when the preceding condition is false.
                {  // Begins this function, loop, conditional block, or switch block.
                    seconds = s;  // Stores the newly entered value in the shared RTC variable.

                    UpdateRTC(hours, minutes, seconds,  // Writes the edited date/time values into the RTC registers.
                              day, month, year);  // Executes this statement as part of the current operation.

                    readrtc();  // Reads the current time and date from the RTC registers into the shared variables.
                }  // Ends the current function, loop, conditional block, or switch block.

                break;  // Exits the current switch case or loop.


            /*--------------------------------
                         BACK  // Continues the current operation in this function.
            --------------------------------*/  // Continues the current operation in this function.

            case 4:  // Selects the code for this keypad/menu choice.

                return 1;  // Returns the specified value to the calling function, or exits the function.


            default:  // Handles any menu choice not covered by the listed cases.

                CmdLCD(0x01);  // Clears the LCD display.
                delay_ms(5);  // Waits for 5 milliseconds.

                StrLCD("INVALID OPTION");  // Displays "INVALID OPTION" on the LCD.

                delay_ms(1000);  // Waits for 1000 milliseconds.

                break;  // Exits the current switch case or loop.
        }  // Ends the current function, loop, conditional block, or switch block.
    }  // Ends the current function, loop, conditional block, or switch block.
}  // Ends the current function, loop, conditional block, or switch block.


/*------------------------------------------------
                  MAIN RTC MENU  // Continues the current operation in this function.
------------------------------------------------*/  // Continues the current operation in this function.

void SetTimeFromKeypad(void)  // Begins the SetTimeFromKeypad function.
{  // Begins this function, loop, conditional block, or switch block.
    int choice;  // Declares a local variable used by this function.

    /* Read existing RTC values */
    readrtc();  // Reads the current time and date from the RTC registers into the shared variables.

    while (1)  // Repeats this block while the specified condition remains true.
    {  // Begins this function, loop, conditional block, or switch block.
        CmdLCD(0x01);  // Clears the LCD display.
        delay_ms(5);  // Waits for 5 milliseconds.

        CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
        StrLCD("1.DATE  2.TIME");  // Displays "1.DATE  2.TIME" on the LCD.

        CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
        StrLCD("3.EXIT");  // Displays "3.EXIT" on the LCD.

        choice = KeyScan();  // Scans the keypad and stores the pressed key in choice.
        delay_ms(150);  // Waits for 150 milliseconds.

        switch (choice)  // Chooses which menu action to execute based on the selected key.
        {  // Begins this function, loop, conditional block, or switch block.
            /* DATE MENU */

            case 1:  // Selects the code for this keypad/menu choice.

                if (Edit_Date() == 0)  // Checks the condition before executing the following block.
                    return;  // Returns the specified value to the calling function, or exits the function.

                break;  // Exits the current switch case or loop.


            /* TIME MENU */

            case 2:  // Selects the code for this keypad/menu choice.

                if (Edit_Time() == 0)  // Checks the condition before executing the following block.
                    return;  // Returns the specified value to the calling function, or exits the function.

                break;  // Exits the current switch case or loop.


            /* EXIT RTC SETTINGS */

            case 3:  // Selects the code for this keypad/menu choice.
                return;  // Returns the specified value to the calling function, or exits the function.


            default:  // Handles any menu choice not covered by the listed cases.

                CmdLCD(0x01);  // Clears the LCD display.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                StrLCD("INVALID OPTION");  // Displays "INVALID OPTION" on the LCD.

                delay_ms(1000);  // Waits for 1000 milliseconds.

                break;  // Exits the current switch case or loop.
        }  // Ends the current function, loop, conditional block, or switch block.
    }  // Ends the current function, loop, conditional block, or switch block.
}  // Ends the current function, loop, conditional block, or switch block.
