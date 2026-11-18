#include <LPC21xx.h>       // Includes LPC2148 microcontroller register definitions
#include "types.h"         // Includes user-defined data types
#include "delay.h"         // Includes delay function declarations
#include "lcd.h"           // Includes LCD function declarations
#include "lcd_defines.h"   // Includes LCD command definitions
#include "kpm.h"           // Includes keypad function declarations
#include "uart.h"           // Includes UART communication functions
#include "rtc.h"            // Includes RTC function declarations
#include "interrupt.h"     // Includes external interrupt function declarations
#include "atm.h"            // Includes ATM function declarations

/* Set by CheckRTCInterrupt() after RTC settings are completed. */
volatile int rtcDisplayRestore = 0; // Global flag used to track RTC display restoration

/*
 * This function is also called by the keypad scanning routine while it
 * waits for a key. Therefore, after RTC settings are completed, restore
 * the active ATM screen here instead of waiting for KeyScan() to return.
 */

void CheckRTCInterrupt(void) // Function to check and process the RTC-setting interrupt
{
    static int rtcSettingInProgress = 0; // Remembers whether RTC settings are already being processed

    if ((interruptFlag == 1) && (rtcSettingInProgress == 0)) // Checks whether an interrupt occurred and settings are not already active
    {
        /* Prevent this interrupt from being processed again. */
        interruptFlag = 0;             // Clears the interrupt software flag
        rtcSettingInProgress = 1;      // Marks RTC settings as currently in progress

        /* Open the RTC date/time settings menu. */
        SetTimeFromKeypad();           // Opens the RTC date and time editing menu using the keypad

        /* Display the updated RTC for one second. */
        CmdLCD(CLEAR_LCD);             // Clears the LCD display
        delay_ms(5);                   // Waits for 5 milliseconds

        DisplayRTC();                  // Displays the updated RTC time and date
        delay_ms(1000);                // Displays the RTC values for one second

        /* Immediately redraw the screen that was active before the interrupt. */
        RestoreATMDisplay();           // Restores the previous ATM screen on the LCD

        rtcDisplayRestore = 0;         // Clears the RTC display restoration flag
        interruptFlag = 0;             // Clears any pending software interrupt flag
        rtcSettingInProgress = 0;      // Marks RTC settings as completed
    }
}

int main(void) // Main function: program execution starts here
{
    /* Configure buzzer pin as GPIO output. */
    PINSEL1 &= ~(3U << 6);             // Configures P0.19 as GPIO by clearing its pin-selection bits
    IODIR0 |= (1U << BUZZER_PIN);      // Configures the buzzer pin as an output

    /* Preserve the original buzzer output initialization. */
    IOCLR0 = (1U << BUZZER_PIN);       // Makes the buzzer output LOW, keeping the buzzer OFF

    InitLCD();                         // Initializes the LCD
    InitKPM();                         // Initializes the keypad
    UART0_Init();                      // Initializes UART0 communication

    initrtc();                         // Initializes the Real-Time Clock
    Init_interrupt();                  // Initializes the external interrupt EINT1

    Display_welcome();                 // Displays the welcome message on the LCD

    CmdLCD(CLEAR_LCD);                 // Clears the LCD display
    delay_ms(5);                       // Waits for 5 milliseconds
    //setrtc();                        // Commented function call that would set default RTC date and time

    /* Display the running RTC for approximately five seconds. */
    timedisplay();                     // Displays the current RTC time and date for approximately five seconds

    CmdLCD(CLEAR_LCD);                 // Clears the LCD display
    delay_ms(5);                       // Waits for 5 milliseconds

    /* Do not carry a startup RTC display flag into the login screen. */
    rtcDisplayRestore = 0;             // Clears the RTC display restoration flag before login

    while (1)                          // Runs the ATM program continuously
    {
        CheckRTCInterrupt();           // Checks whether the RTC-setting interrupt has occurred

        if (ATM_Login())               // Checks whether ATM login is successful
        {
            CheckRTCInterrupt();       // Checks for an RTC interrupt after successful login
            ATM_Menu();                // Opens the ATM main menu
        }
    }
}

