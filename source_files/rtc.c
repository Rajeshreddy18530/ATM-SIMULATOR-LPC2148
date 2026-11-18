#include <LPC21xx.h>              // Includes LPC2148 microcontroller register definitions
#include "rtc.h"                  // Includes RTC macros and function declarations
#include "lcd.h"                  // Includes LCD function declarations
#include "lcd_defines.h"          // Includes LCD command definitions
#include "delay.h"                // Includes delay function declarations

/* RTC time variables */
unsigned int seconds, minutes, hours; // Stores seconds, minutes and hours read from RTC
unsigned int day, month, year;        // Stores day, month and year read from RTC
int count;                            // Loop counter used in timedisplay()
int tick;                             // Loop counter used for generating delay

/* Initialize RTC */
//void initrtc(void)                 // Previously used RTC initialization function
//{                                   // Beginning of old function
    /* Stop RTC counter during initialization */
//  CCR = 0x00;                       // Previously used to disable the RTC

    /* Configure RTC prescaler for PCLK = 15 MHz */
//  PREINT = 456;                     // Previously used to configure integer prescaler
//  PREFRAC = 25024;                  // Previously used to configure fractional prescaler

    /* Enable RTC and select PCLK */
//  CCR = 0x11;                       // Previously used to enable RTC
//}                                   // End of old function

void initrtc(void)                  // Function used to initialize the RTC
{                                    // Beginning of initrtc()

    // Disable and reset the RTC
    CCR = 1<<1;                     // Sets bit 1 of CCR to reset the RTC clock divider
    //CCR=(1<<0);                    // Commented statement that would enable the RTC

    /*#ifndef _LPC2148                 // Commented preprocessor condition for other controllers

    // Set prescaler integer and fractional parts
    PREINT = PREINT_VAL;            // Sets the integer prescaler value
    PREFRAC = PREFRAC_VAL;          // Sets the fractional prescaler value

    // Enable the RTC
    CCR = RTC_ENABLE;               // Enables the RTC using the selected clock source

    #else */                        // End of the commented conditional compilation section

    // Enable the RTC with external clock source
    CCR = (1<<0) | (1<<4);          // Sets bit 0 to enable RTC and bit 4 to select external clock

    //#endif                         // Commented end of conditional compilation

}                                    // End of initrtc()

/* Set default RTC date and time */
/*void setrtc(void)                  // Previously used function to set default date and time
{                                    // Beginning of setrtc()
    // Stop RTC counter
    CCR = 0x00;                     // Disables the RTC counter

    SEC   = 0;                      // Sets seconds to 0
    MIN   = 30;                     // Sets minutes to 30
    HOUR  = 11;                     // Sets hours to 11

    DOM   = 12;                     // Sets day of month to 12
    MONTH = 11;                     // Sets month to 11
    YEAR  = 2026;                   // Sets year to 2026

    // Restart RTC using PCLK
    //CCR = 0x11;                    // Previously used to restart RTC
        CCR = RTC_ENABLE | RTC_CLKSRC; // Enables RTC with external clock source
} */                                // End of commented setrtc() function

/* Read current RTC date and time */
void readrtc(void)                 // Function to read the current RTC values
{                                    // Beginning of readrtc()
    seconds = SEC;                  // Reads seconds from RTC SEC register
    minutes = MIN;                  // Reads minutes from RTC MIN register
    hours   = HOUR;                 // Reads hours from RTC HOUR register

    day   = DOM;                    // Reads day from RTC DOM register
    month = MONTH;                  // Reads month from RTC MONTH register
    year  = YEAR;                   // Reads year from RTC YEAR register
}                                    // End of readrtc()

/* Update RTC with values entered through keypad */
void UpdateRTC(unsigned int h,    // Receives the hours entered through keypad
               unsigned int m,    // Receives the minutes entered through keypad
               unsigned int s,    // Receives the seconds entered through keypad
               unsigned int d,    // Receives the day entered through keypad
               unsigned int mon,  // Receives the month entered through keypad
               unsigned int y)   // Receives the year entered through keypad
{                                    // Beginning of UpdateRTC()

    /* Stop RTC counter while updating registers */
    //CCR = 0x00;                   // Commented statement that would stop the RTC
    //CCR=RTC_CLKSRC;               // Commented statement to select external clock while stopping RTC

    HOUR  = h;                      // Writes the entered hours into RTC
    MIN   = m;                      // Writes the entered minutes into RTC
    SEC   = s;                      // Writes the entered seconds into RTC

    DOM   = d;                      // Writes the entered day into RTC
    MONTH = mon;                    // Writes the entered month into RTC
    YEAR  = y;                      // Writes the entered year into RTC

    /* Restart RTC using PCLK */
    //CCR = RTC_ENABLE | RTC_CLKSRC; // Commented statement that would enable RTC with external clock
}                                    // End of UpdateRTC()

/* Display RTC */
void DisplayRTC(void)               // Function to display RTC time and date on LCD
{                                    // Beginning of DisplayRTC()
    readrtc();                      // Reads the latest time and date from RTC

    CmdLCD(GOTO_LINE1_POS0);        // Moves LCD cursor to first row, first column

    if(hours < 10)                  // Checks whether hours contain only one digit
        CharLCD('0');               // Displays zero before a single-digit hour

    U32LCD(hours);                  // Displays the hours value on LCD
    CharLCD(':');                   // Displays colon between hours and minutes

    if(minutes < 10)                // Checks whether minutes contain only one digit
        CharLCD('0');               // Displays zero before a single-digit minute

    U32LCD(minutes);                // Displays the minutes value on LCD
    CharLCD(':');                   // Displays colon between minutes and seconds

    if(seconds < 10)                // Checks whether seconds contain only one digit
        CharLCD('0');               // Displays zero before a single-digit second

    U32LCD(seconds);                // Displays the seconds value on LCD

    CmdLCD(GOTO_LINE2_POS0);        // Moves LCD cursor to second row, first column

    if(day < 10)                    // Checks whether day contains only one digit
        CharLCD('0');               // Displays zero before a single-digit day

    U32LCD(day);                    // Displays the day on LCD
    CharLCD('/');                   // Displays slash between day and month

    if(month < 10)                  // Checks whether month contains only one digit
        CharLCD('0');               // Displays zero before a single-digit month

    U32LCD(month);                  // Displays the month on LCD
    CharLCD('/');                   // Displays slash between month and year

    U32LCD(year);                   // Displays the year on LCD
}                                    // End of DisplayRTC()

void timedisplay(void)              // Function to display RTC for approximately five seconds
{                                    // Beginning of timedisplay()
    for(count = 0; count < 5; count++) // Repeats the display operation five times
    {                                // Beginning of outer loop
        DisplayRTC();               // Displays current RTC time and date

        for(tick = 0; tick < 100; tick++) // Repeats the delay operation 100 times
        {                            // Beginning of inner loop
            // CheckRTCInterrupt();  // Commented function call to check RTC interrupt
            delay_ms(10);           // Delays execution for 10 milliseconds
        }                            // End of inner loop
    }                                // End of outer loop
}                                    // End of timedisplay()
