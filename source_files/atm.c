#include <LPC21xx.h>  // Provides LPC2148 hardware register definitions.
#include <string.h>  // Provides string-handling functions such as strcmp() and strcpy().
#include "types.h"  // Provides project-specific data type definitions.
#include "delay.h"  // Declares delay functions.
#include "lcd.h"  // Declares LCD functions.
#include "kpm.h"  // Declares keypad functions.
#include "uart.h"  // Declares UART communication functions.
#include "rtc.h"  // Declares RTC functions and shared RTC variables.
#include "interrupt.h"  // Declares interrupt-related functions.
#include "atm.h"  // Declares ATM functions and related definitions.
#include "lcd_defines.h"  // Provides LCD command and pin definitions.

/* Global variable definitions */

char ACCOUNT_PIN[5] = "1234";  // Stores the default four-digit account PIN as a null-terminated string.

int balance = 5000;  // Initializes the ATM account balance.

/* Login variables */

char account[5];  // Declares a variable used by the ATM program.
char pin[5];  // Declares a variable used by the ATM program.

int keyv;  // Declares a variable used by the ATM program.
int attempts = 0;  // Declares a variable used by the ATM program.
int i;  // Declares a variable used by the ATM program.

/* Transaction history */

int transactionAmount[MAX_TRANSACTIONS];  // Declares an array to store transaction amounts.
int transactionType[MAX_TRANSACTIONS];  // Declares an array to store transaction types.
int transactionBalance[MAX_TRANSACTIONS];  // Declares an array to store balances after transactions.

int transactionCount = 0;  // Initializes the number of transactions currently stored.

/* Display context saved so RTC settings can return to any active ATM screen. */
#define ATM_DISPLAY_NONE              0  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_ACCOUNT           1  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_PIN               2  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_MENU              3  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_WITHDRAW          4  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_DEPOSIT           5  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_CHANGE_NEW_PIN    6  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_CHANGE_CONFIRM_PIN 7  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_BALANCE           8  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_NEW_BALANCE       9  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_HISTORY_AMOUNT    10  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_HISTORY_BALANCE   11  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_PIN_CHANGED       12  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_PIN_MISMATCH      13  // Defines a named constant or macro for use in this program.
#define ATM_DISPLAY_NO_HISTORY        14  // Defines a named constant or macro for use in this program.

static volatile int atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.
static volatile int atmDisplayCount = 0;  // Saves the current number of entered characters.
static volatile u32 atmDisplayAmount = 0;  // Saves the amount currently being entered.
static volatile int atmHistoryIndex = 0;  // Records which transaction-history entry is being shown.

static void ATM_DelayWithRTC(unsigned int milliseconds);  // Begins the ATM_DelayWithRTC function definition.


/*--------------------------------------------------
  UART TRANSACTION LOGGING  // Continues the current ATM operation.
--------------------------------------------------*/  // Continues the current ATM operation.

void UART_LogTransaction(char *type,  // Begins the UART_LogTransaction function definition.
                         u32 amount,  // Continues the current ATM operation.
                         u32 remainingBalance)  // Continues the current ATM operation.
{  // Begins the current function or control block.
    readrtc();  // Reads the current date and time from the RTC before logging.

    /* Timestamp: [DD/MM/YYYY HH:MM:SS] */

    U0_TxStr("\r\n[");  // Sends the text "\r\n[" through UART0.

    if (day < 10U)  // Checks this condition before executing the following statement or block.
        U0_TxStr("0");  // Sends the text "0" through UART0.

    U0_TxU32(day);  // Sends the value day through UART0.
    U0_TxStr("/");  // Sends the text "/" through UART0.

    if (month < 10U)  // Checks this condition before executing the following statement or block.
        U0_TxStr("0");  // Sends the text "0" through UART0.

    U0_TxU32(month);  // Sends the value month through UART0.
    U0_TxStr("/");  // Sends the text "/" through UART0.

    U0_TxU32(year);  // Sends the value year through UART0.
    U0_TxStr(" ");  // Sends the text " " through UART0.

    if (hours < 10U)  // Checks this condition before executing the following statement or block.
        U0_TxStr("0");  // Sends the text "0" through UART0.

    U0_TxU32(hours);  // Sends the value hours through UART0.
    U0_TxStr(":");  // Sends the text ":" through UART0.

    if (minutes < 10U)  // Checks this condition before executing the following statement or block.
        U0_TxStr("0");  // Sends the text "0" through UART0.

    U0_TxU32(minutes);  // Sends the value minutes through UART0.
    U0_TxStr(":");  // Sends the text ":" through UART0.

    if (seconds < 10U)  // Checks this condition before executing the following statement or block.
        U0_TxStr("0");  // Sends the text "0" through UART0.

    U0_TxU32(seconds);  // Sends the value seconds through UART0.

    U0_TxStr("] ACC:");  // Sends the text "] ACC:" through UART0.
    U0_TxStr(account);  // Sends a string through UART0.
    U0_TxStr(" ");  // Sends the text " " through UART0.

    if (strcmp(type, "LOGIN_SUCCESS") == 0)  // Checks this condition before executing the following statement or block.
    {  // Begins the current function or control block.
        U0_TxStr("LOGIN SUCCESS");  // Sends the text "LOGIN SUCCESS" through UART0.
    }  // Ends the current function or control block.
    else if (strcmp(type, "LOGIN_FAILED") == 0)  // Checks this alternative condition if the previous condition was false.
    {  // Begins the current function or control block.
        U0_TxStr("LOGIN FAILED");  // Sends the text "LOGIN FAILED" through UART0.
    }  // Ends the current function or control block.
    else if (strcmp(type, "BALANCE_ENQUIRY") == 0)  // Checks this alternative condition if the previous condition was false.
    {  // Begins the current function or control block.
        U0_TxStr("BALANCE Rs.");  // Sends the text "BALANCE Rs." through UART0.
        U0_TxU32(remainingBalance);  // Sends the value remainingBalance through UART0.
    }  // Ends the current function or control block.
    else if (strcmp(type, "WITHDRAW") == 0)  // Checks this alternative condition if the previous condition was false.
    {  // Begins the current function or control block.
        U0_TxStr("WITHDRAW Rs.");  // Sends the text "WITHDRAW Rs." through UART0.
        U0_TxU32(amount);  // Sends the value amount through UART0.

        U0_TxStr(" BAL: Rs.");  // Sends the text " BAL: Rs." through UART0.
        U0_TxU32(remainingBalance);  // Sends the value remainingBalance through UART0.
    }  // Ends the current function or control block.
    else if (strcmp(type, "DEPOSIT") == 0)  // Checks this alternative condition if the previous condition was false.
    {  // Begins the current function or control block.
        U0_TxStr("DEPOSIT Rs.");  // Sends the text "DEPOSIT Rs." through UART0.
        U0_TxU32(amount);  // Sends the value amount through UART0.

        U0_TxStr(" BAL: Rs.");  // Sends the text " BAL: Rs." through UART0.
        U0_TxU32(remainingBalance);  // Sends the value remainingBalance through UART0.
    }  // Ends the current function or control block.
    else if (strcmp(type, "PIN_CHANGED") == 0)  // Checks this alternative condition if the previous condition was false.
    {  // Begins the current function or control block.
        U0_TxStr("PIN CHANGED");  // Sends the text "PIN CHANGED" through UART0.
    }  // Ends the current function or control block.
    else if (strcmp(type, "PIN_CHANGE_FAILED") == 0)  // Checks this alternative condition if the previous condition was false.
    {  // Begins the current function or control block.
        U0_TxStr("PIN CHANGE FAILED");  // Sends the text "PIN CHANGE FAILED" through UART0.
    }  // Ends the current function or control block.
    else if (strcmp(type, "LOGOUT") == 0)  // Checks this alternative condition if the previous condition was false.
    {  // Begins the current function or control block.
        U0_TxStr("LOGOUT");  // Sends the text "LOGOUT" through UART0.
    }  // Ends the current function or control block.
    else if (strcmp(type, "ATM_LOCKED") == 0)  // Checks this alternative condition if the previous condition was false.
    {  // Begins the current function or control block.
        U0_TxStr("ATM LOCKED");  // Sends the text "ATM LOCKED" through UART0.
    }  // Ends the current function or control block.
    else if (strcmp(type, "WITHDRAW_REJECTED") == 0)  // Checks this alternative condition if the previous condition was false.
    {  // Begins the current function or control block.
        U0_TxStr("WITHDRAW REJECTED Rs.");  // Sends the text "WITHDRAW REJECTED Rs." through UART0.
        U0_TxU32(amount);  // Sends the value amount through UART0.

        U0_TxStr(" BAL: Rs.");  // Sends the text " BAL: Rs." through UART0.
        U0_TxU32(remainingBalance);  // Sends the value remainingBalance through UART0.
    }  // Ends the current function or control block.
    else if (strcmp(type, "DEPOSIT_REJECTED") == 0)  // Checks this alternative condition if the previous condition was false.
    {  // Begins the current function or control block.
        U0_TxStr("DEPOSIT REJECTED Rs.");  // Sends the text "DEPOSIT REJECTED Rs." through UART0.
        U0_TxU32(amount);  // Sends the value amount through UART0.

        U0_TxStr(" BAL: Rs.");  // Sends the text " BAL: Rs." through UART0.
        U0_TxU32(remainingBalance);  // Sends the value remainingBalance through UART0.
    }  // Ends the current function or control block.
    else  // Executes this alternative block when the preceding condition was false.
    {  // Begins the current function or control block.
        U0_TxStr(type);  // Sends a string through UART0.
    }  // Ends the current function or control block.

    U0_TxStr("\r\n");  // Sends the text "\r\n" through UART0.
}  // Ends the current function or control block.


/*--------------------------------------------------
  SAVE TRANSACTION  // Continues the current ATM operation.
--------------------------------------------------*/  // Continues the current ATM operation.

void SaveTransaction(u32 amount, int type)  // Begins the SaveTransaction function definition.
{  // Begins the current function or control block.
    if (transactionCount < MAX_TRANSACTIONS)  // Checks this condition before executing the following statement or block.
    {  // Begins the current function or control block.
        transactionAmount[transactionCount] = (int)amount;  // Stores or retrieves the amount for this transaction-history entry.

        transactionType[transactionCount] = type;  // Stores or checks the type of this transaction-history entry.

        transactionBalance[transactionCount] = balance;  // Stores or retrieves the balance associated with this transaction.

        transactionCount++;  // Increases the number of stored transactions by one.
    }  // Ends the current function or control block.
    else  // Executes this alternative block when the preceding condition was false.
    {  // Begins the current function or control block.
        /* Discard oldest entry and shift history */

        for (i = 0; i < MAX_TRANSACTIONS - 1; i++)  // Repeats the block using the specified loop counter and condition.
        {  // Begins the current function or control block.
            transactionAmount[i] = transactionAmount[i + 1];  // Stores or retrieves the amount for this transaction-history entry.

            transactionType[i] = transactionType[i + 1];  // Stores or checks the type of this transaction-history entry.

            transactionBalance[i] = transactionBalance[i + 1];  // Stores or retrieves the balance associated with this transaction.
        }  // Ends the current function or control block.

        transactionAmount[MAX_TRANSACTIONS - 1] = (int)amount;  // Stores or retrieves the amount for this transaction-history entry.

        transactionType[MAX_TRANSACTIONS - 1] = type;  // Stores or checks the type of this transaction-history entry.

        transactionBalance[MAX_TRANSACTIONS - 1] = balance;  // Stores or retrieves the balance associated with this transaction.
    }  // Ends the current function or control block.
}  // Ends the current function or control block.
 void Display_welcome(void)  // Begins the Display_welcome function definition.
 {  // Begins the current function or control block.
         CmdLCD(0X01);  // Sends the clear-display command to the LCD.
         delay_ms(5);  // Waits for 5 milliseconds.
         CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
         StrLCD("..WELCOME..");  // Displays "..WELCOME.." on the LCD.
         CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
         StrLCD("..ATM SIMULATOR..");  // Displays "..ATM SIMULATOR.." on the LCD.
         delay_ms(2000);  // Waits for 2000 milliseconds.
 }  // Ends the current function or control block.

/*--------------------------------------------------
   RESTORE ACTIVE ATM SCREEN AFTER RTC SETTINGS  // Continues the current ATM operation.
--------------------------------------------------*/  // Continues the current ATM operation.

void RestoreATMDisplay(void)  // Begins the RestoreATMDisplay function definition.
{  // Begins the current function or control block.
    int j;  // Declares a variable used by the ATM program.

    CmdLCD(0x01);  // Sends the clear-display command to the LCD.
    delay_ms(5);  // Waits for 5 milliseconds.

    switch (atmDisplayState)  // Selects which operation to perform based on the given value.
    {  // Begins the current function or control block.
        case ATM_DISPLAY_ACCOUNT:  // Selects the operation for this menu/keypad choice.
            CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
            StrLCD("Enter Account:");  // Displays "Enter Account:" on the LCD.
            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
            for (j = 0; j < atmDisplayCount; j++)  // Repeats the block using the specified loop counter and condition.
                CharLCD(account[j]);  // Displays the specified character on the LCD.
            break;  // Exits the current switch case or loop.

        case ATM_DISPLAY_PIN:  // Selects the operation for this menu/keypad choice.
            CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
            StrLCD("Enter PIN:");  // Displays "Enter PIN:" on the LCD.
            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
            for (j = 0; j < atmDisplayCount; j++)  // Repeats the block using the specified loop counter and condition.
                CharLCD('*');  // Displays the specified character on the LCD.
            break;  // Exits the current switch case or loop.

        case ATM_DISPLAY_MENU:  // Selects the operation for this menu/keypad choice.
            CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
            StrLCD("1.B 2.W/D 3.DEP");  // Displays "1.B 2.W/D 3.DEP" on the LCD.
            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
            StrLCD("4.P 5.H D.LOGOUT");  // Displays "4.P 5.H D.LOGOUT" on the LCD.
            break;  // Exits the current switch case or loop.

        case ATM_DISPLAY_WITHDRAW:  // Selects the operation for this menu/keypad choice.
        case ATM_DISPLAY_DEPOSIT:  // Selects the operation for this menu/keypad choice.
            CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
            StrLCD("Enter Amount:");  // Displays "Enter Amount:" on the LCD.
            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
            if (atmDisplayCount > 0)  // Checks this condition before executing the following statement or block.
                U32LCD(atmDisplayAmount);  // Displays the numeric value atmDisplayAmount on the LCD.
            break;  // Exits the current switch case or loop.

        case ATM_DISPLAY_CHANGE_NEW_PIN:  // Selects the operation for this menu/keypad choice.
            CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
            StrLCD("Enter New PIN:");  // Displays "Enter New PIN:" on the LCD.
            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
            for (j = 0; j < atmDisplayCount; j++)  // Repeats the block using the specified loop counter and condition.
                CharLCD('*');  // Displays the specified character on the LCD.
            break;  // Exits the current switch case or loop.

        case ATM_DISPLAY_CHANGE_CONFIRM_PIN:  // Selects the operation for this menu/keypad choice.
            CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
            StrLCD("Confirm PIN:");  // Displays "Confirm PIN:" on the LCD.
            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
            for (j = 0; j < atmDisplayCount; j++)  // Repeats the block using the specified loop counter and condition.
                CharLCD('*');  // Displays the specified character on the LCD.
            break;  // Exits the current switch case or loop.

        case ATM_DISPLAY_BALANCE:  // Selects the operation for this menu/keypad choice.
            CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
            StrLCD("Balance:");  // Displays "Balance:" on the LCD.
            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
            U32LCD((u32)balance);  // Displays the numeric value (u32 on the LCD.
            break;  // Exits the current switch case or loop.

        case ATM_DISPLAY_NEW_BALANCE:  // Selects the operation for this menu/keypad choice.
            CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
            StrLCD("New Balance:");  // Displays "New Balance:" on the LCD.
            CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
            U32LCD((u32)balance);  // Displays the numeric value (u32 on the LCD.
            break;  // Exits the current switch case or loop.

        case ATM_DISPLAY_HISTORY_AMOUNT:  // Selects the operation for this menu/keypad choice.
            if (atmHistoryIndex < transactionCount)  // Checks this condition before executing the following statement or block.
            {  // Begins the current function or control block.
                CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                if (transactionType[atmHistoryIndex] == 1)  // Checks this condition before executing the following statement or block.
                    StrLCD("DEPOSIT");  // Displays "DEPOSIT" on the LCD.
                else  // Executes this alternative block when the preceding condition was false.
                    StrLCD("WITHDRAW");  // Displays "WITHDRAW" on the LCD.
                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                U32LCD((u32)transactionAmount[atmHistoryIndex]);  // Displays the numeric value (u32 on the LCD.
            }  // Ends the current function or control block.
            break;  // Exits the current switch case or loop.

        case ATM_DISPLAY_HISTORY_BALANCE:  // Selects the operation for this menu/keypad choice.
            if (atmHistoryIndex < transactionCount)  // Checks this condition before executing the following statement or block.
            {  // Begins the current function or control block.
                CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                StrLCD("Balance:");  // Displays "Balance:" on the LCD.
                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                U32LCD((u32)transactionBalance[atmHistoryIndex]);  // Displays the numeric value (u32 on the LCD.
            }  // Ends the current function or control block.
            break;  // Exits the current switch case or loop.

        case ATM_DISPLAY_PIN_CHANGED:  // Selects the operation for this menu/keypad choice.
            CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
            StrLCD("PIN Changed");  // Displays "PIN Changed" on the LCD.
            break;  // Exits the current switch case or loop.

        case ATM_DISPLAY_PIN_MISMATCH:  // Selects the operation for this menu/keypad choice.
            CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
            StrLCD("PIN Mismatch");  // Displays "PIN Mismatch" on the LCD.
            break;  // Exits the current switch case or loop.

        case ATM_DISPLAY_NO_HISTORY:  // Selects the operation for this menu/keypad choice.
            CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
            StrLCD("No Transactions");  // Displays "No Transactions" on the LCD.
            break;  // Exits the current switch case or loop.

        default:  // Handles a choice that does not match any listed case.
            /* No active ATM screen to restore. */
            break;  // Exits the current switch case or loop.
    }  // Ends the current function or control block.
}  // Ends the current function or control block.

/* Poll the RTC flag during display-only waits as well as keypad waits. */
static void ATM_DelayWithRTC(unsigned int milliseconds)  // Begins the ATM_DelayWithRTC function definition.
{  // Begins the current function or control block.
    while (milliseconds > 0U)  // Repeats the block while the condition remains true.
    {  // Begins the current function or control block.
        CheckRTCInterrupt();  // Checks whether the RTC interrupt needs to be handled.

        if (milliseconds >= 10U)  // Checks this condition before executing the following statement or block.
        {  // Begins the current function or control block.
            delay_ms(10);  // Waits for 10 milliseconds.
            milliseconds -= 10U;  // Assigns or updates the value of the variable.
        }  // Ends the current function or control block.
        else  // Executes this alternative block when the preceding condition was false.
        {  // Begins the current function or control block.
            delay_ms(milliseconds);  // Waits for milliseconds milliseconds.
            milliseconds = 0U;  // Assigns or updates the value of the variable.
        }  // Ends the current function or control block.
    }  // Ends the current function or control block.

    CheckRTCInterrupt();  // Checks whether the RTC interrupt needs to be handled.
}  // Ends the current function or control block.

/*--------------------------------------------------
  ATM LOGIN  // Continues the current ATM operation.
--------------------------------------------------*/  // Continues the current ATM operation.

int ATM_Login(void)  // Begins the ATM_Login function definition.
{  // Begins the current function or control block.
    while (attempts < MAX_ATTEMPTS)  // Repeats the block while the condition remains true.
    {  // Begins the current function or control block.
        CmdLCD(0x01);  // Sends the clear-display command to the LCD.
        delay_ms(5);  // Waits for 5 milliseconds.

        CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
        StrLCD("Enter Account:");  // Displays "Enter Account:" on the LCD.

        CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

        i = 0;  // Assigns or updates the value of the variable.

        while (i < 4)  // Repeats the block while the condition remains true.
        {  // Begins the current function or control block.
            atmDisplayState = ATM_DISPLAY_ACCOUNT;  // Updates the saved ATM screen state for later display restoration.
            atmDisplayCount = i;  // Saves the current number of entered characters.
            keyv = KeyScan();  // Scans the keypad and stores the pressed key.
            delay_ms(500);  // Waits for 500 milliseconds.

            if (keyv >= 0 && keyv <= 9)  // Checks this condition before executing the following statement or block.
            {  // Begins the current function or control block.
                account[i] = (char)(keyv + '0');  // Assigns or updates the value of the variable.

                CharLCD(account[i]);  // Displays the specified character on the LCD.

                delay_ms(10);  // Waits for 10 milliseconds.

                i++;  // Moves to the next position in the input array.
            }  // Ends the current function or control block.
            else if (keyv == 11 && i > 0)  // Checks this alternative condition if the previous condition was false.
            {  // Begins the current function or control block.
                /* B = Backspace */

                i--;  // Moves back one position after a backspace.

                CmdLCD(0x10);  // Executes this statement as part of the current ATM operation.
                delay_ms(5);  // Waits for 5 milliseconds.

                CharLCD(' ');  // Displays the specified character on the LCD.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(0x10);  // Executes this statement as part of the current ATM operation.
                delay_ms(5);  // Waits for 5 milliseconds.
            }  // Ends the current function or control block.
        }  // Ends the current function or control block.

        account[4] = '\0';  // Adds the null terminator so the character array is a valid C string.

        CmdLCD(0x01);  // Sends the clear-display command to the LCD.
        delay_ms(5);  // Waits for 5 milliseconds.

        CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
        StrLCD("Enter PIN:");  // Displays "Enter PIN:" on the LCD.

        CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

        i = 0;  // Assigns or updates the value of the variable.

        while (i < 4)  // Repeats the block while the condition remains true.
        {  // Begins the current function or control block.
            atmDisplayState = ATM_DISPLAY_PIN;  // Updates the saved ATM screen state for later display restoration.
            atmDisplayCount = i;  // Saves the current number of entered characters.
            keyv = KeyScan();  // Scans the keypad and stores the pressed key.
            delay_ms(500);  // Waits for 500 milliseconds.

            if (keyv >= 0 && keyv <= 9)  // Checks this condition before executing the following statement or block.
            {  // Begins the current function or control block.
                pin[i] = (char)(keyv + '0');  // Assigns or updates the value of the variable.

                CharLCD('*');  // Displays the specified character on the LCD.

                delay_ms(10);  // Waits for 10 milliseconds.

                i++;  // Moves to the next position in the input array.
            }  // Ends the current function or control block.
            else if (keyv == 11 && i > 0)  // Checks this alternative condition if the previous condition was false.
            {  // Begins the current function or control block.
                /* B = Backspace */

                i--;  // Moves back one position after a backspace.

                CmdLCD(0x10);  // Executes this statement as part of the current ATM operation.
                delay_ms(5);  // Waits for 5 milliseconds.

                CharLCD(' ');  // Displays the specified character on the LCD.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(0x10);  // Executes this statement as part of the current ATM operation.
                delay_ms(5);  // Waits for 5 milliseconds.
            }  // Ends the current function or control block.
        }  // Ends the current function or control block.

        pin[4] = '\0';  // Adds the null terminator so the character array is a valid C string.

        if (strcmp(account, ACCOUNT_NO) == 0 &&  // Checks this condition before executing the following statement or block.
            strcmp(pin, ACCOUNT_PIN) == 0)  // Stores the default four-digit account PIN as a null-terminated string.
        {  // Begins the current function or control block.
            attempts = 0;  // Resets the failed-login attempt counter.

            CmdLCD(0x01);  // Sends the clear-display command to the LCD.
            delay_ms(5);  // Waits for 5 milliseconds.

            CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
            StrLCD("Login Success");  // Displays "Login Success" on the LCD.

            UART_LogTransaction("LOGIN_SUCCESS", 0U, (u32)balance);  // Logs this ATM event, amount, balance, and RTC timestamp through UART0.

            delay_ms(1500);  // Waits for 1500 milliseconds.
            atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.

            return 1;  // Returns a value to the caller or exits the current function.
        }  // Ends the current function or control block.

        attempts++;  // Increases the number of failed login attempts.

        UART_LogTransaction("LOGIN_FAILED", 0U, (u32)balance);  // Logs this ATM event, amount, balance, and RTC timestamp through UART0.

        CmdLCD(0x01);  // Sends the clear-display command to the LCD.
        delay_ms(5);  // Waits for 5 milliseconds.

        CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
        StrLCD("Invalid Login");  // Displays "Invalid Login" on the LCD.

        CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

        if (attempts < MAX_ATTEMPTS)  // Checks this condition before executing the following statement or block.
            StrLCD("Try Again");  // Displays "Try Again" on the LCD.

        delay_ms(1500);  // Waits for 1500 milliseconds.
    }  // Ends the current function or control block.

    /* Buzzer after three failed attempts */

    UART_LogTransaction("ATM_LOCKED", 0U, (u32)balance);  // Logs this ATM event, amount, balance, and RTC timestamp through UART0.

    /* Buzzer ON for 3 seconds */
    IOSET0 = (1U << BUZZER_PIN);  // Sets the buzzer output pin HIGH to turn the buzzer ON.
    delay_ms(3000);  // Waits for 3000 milliseconds.
    IOCLR0 = (1U << BUZZER_PIN);  // Clears the buzzer output pin LOW to turn the buzzer OFF.

    /* Display lock message */
    CmdLCD(0x01);  // Sends the clear-display command to the LCD.
    delay_ms(5);  // Waits for 5 milliseconds.

    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
    StrLCD("ATM LOCKED");  // Displays "ATM LOCKED" on the LCD.

    CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
    StrLCD("TRY AFTER 24H");  // Displays "TRY AFTER 24H" on the LCD.

    delay_ms(2000);  // Waits for 2000 milliseconds.

    /* Clear LCD */
    CmdLCD(0x01);  // Sends the clear-display command to the LCD.
    delay_ms(5);  // Waits for 5 milliseconds.

    /* Reset attempts */
    attempts = 0;  // Resets the failed-login attempt counter.

    atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.
    return 0;  // Returns a value to the caller or exits the current function.
}  // Ends the current function or control block.


/*--------------------------------------------------
  BALANCE ENQUIRY  // Continues the current ATM operation.
--------------------------------------------------*/  // Continues the current ATM operation.

void Balance_enquiry(void)  // Begins the Balance_enquiry function definition.
{  // Begins the current function or control block.
    atmDisplayState = ATM_DISPLAY_BALANCE;  // Updates the saved ATM screen state for later display restoration.
    CmdLCD(0x01);  // Sends the clear-display command to the LCD.
    delay_ms(5);  // Waits for 5 milliseconds.

    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
    StrLCD("Balance:");  // Displays "Balance:" on the LCD.

    CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

    U32LCD((u32)balance);  // Displays the numeric value (u32 on the LCD.

    UART_LogTransaction("BALANCE_ENQUIRY", 0U, (u32)balance);  // Logs this ATM event, amount, balance, and RTC timestamp through UART0.

    ATM_DelayWithRTC(2500);  // Waits for the specified duration while continuing to check RTC interrupts.
    atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.
}  // Ends the current function or control block.


/*--------------------------------------------------
  WITHDRAWAL  // Continues the current ATM operation.
--------------------------------------------------*/  // Continues the current ATM operation.

void withdraw(void)  // Begins the withdraw function definition.
{  // Begins the current function or control block.
    u32 amount = 0;  // Declares a variable used by the ATM program.

    int key;  // Declares a variable used by the ATM program.
    int count = 0;  // Declares a variable used by the ATM program.

    CmdLCD(0x01);  // Sends the clear-display command to the LCD.
    delay_ms(5);  // Waits for 5 milliseconds.

    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
    StrLCD("Enter Amount:");  // Displays "Enter Amount:" on the LCD.

    CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

    while (1)  // Repeats the block while the condition remains true.
    {  // Begins the current function or control block.
        atmDisplayState = ATM_DISPLAY_WITHDRAW;  // Updates the saved ATM screen state for later display restoration.
        atmDisplayAmount = amount;  // Saves the amount currently being entered.
        atmDisplayCount = count;  // Saves the current number of entered characters.
        key = KeyScan();  // Scans the keypad and stores the pressed key.
        delay_ms(500);  // Waits for 500 milliseconds.

        if (key >= 0 && key <= 9)  // Checks this condition before executing the following statement or block.
        {  // Begins the current function or control block.
            if (count < 5)  // Checks this condition before executing the following statement or block.
            {  // Begins the current function or control block.
                amount = (amount * 10U) + (u32)key;  // Appends the newly entered digit to the amount.

                count++;  // Increases the number of digits entered.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                delay_ms(5);  // Waits for 5 milliseconds.

                StrLCD("                ");  // Displays "                " on the LCD.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                delay_ms(5);  // Waits for 5 milliseconds.

                U32LCD(amount);  // Displays the numeric value amount on the LCD.

                delay_ms(10);  // Waits for 10 milliseconds.
            }  // Ends the current function or control block.
        }  // Ends the current function or control block.
        else if (key == 11)  // Checks this alternative condition if the previous condition was false.
        {  // Begins the current function or control block.
            /* B = Backspace */

            if (count > 0)  // Checks this condition before executing the following statement or block.
            {  // Begins the current function or control block.
                count--;  // Decreases the digit count after a backspace.

                amount = amount / 10U;  // Removes the last entered digit from the amount.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                delay_ms(5);  // Waits for 5 milliseconds.

                StrLCD("                ");  // Displays "                " on the LCD.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                delay_ms(5);  // Waits for 5 milliseconds.

                if (count > 0)  // Checks this condition before executing the following statement or block.
                    U32LCD(amount);  // Displays the numeric value amount on the LCD.

                delay_ms(10);  // Waits for 10 milliseconds.
            }  // Ends the current function or control block.
        }  // Ends the current function or control block.
        else if (key == 10)  // Checks this alternative condition if the previous condition was false.
        {  // Begins the current function or control block.
            /* A = Confirm */

            break;  // Exits the current switch case or loop.
        }  // Ends the current function or control block.
        else if (key == 13)  // Checks this alternative condition if the previous condition was false.
        {  // Begins the current function or control block.
            /* D = Cancel */

            CmdLCD(0x01);  // Sends the clear-display command to the LCD.
            delay_ms(5);  // Waits for 5 milliseconds.

            StrLCD("Cancelled");  // Displays "Cancelled" on the LCD.

            delay_ms(1000);  // Waits for 1000 milliseconds.
            atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.

            return;  // Returns a value to the caller or exits the current function.
        }  // Ends the current function or control block.
    }  // Ends the current function or control block.

    atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.

    if (count == 0 || amount == 0 ||  // Checks this condition before executing the following statement or block.
        ((amount % 100U) != 0u)||(amount > (u32)balance))  // Continues the current ATM operation.
    {  // Begins the current function or control block.

        UART_LogTransaction("WITHDRAW_REJECTED",  // Logs this ATM event, amount, balance, and RTC timestamp through UART0.
                            amount,  // Continues the current ATM operation.
                            (u32)balance);  // Executes this statement as part of the current ATM operation.
                if(amount > (u32)balance)  // Checks this condition before executing the following statement or block.
                {  // Begins the current function or control block.
        CmdLCD(0x01);  // Sends the clear-display command to the LCD.
        delay_ms(5);  // Waits for 5 milliseconds.

        CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
        StrLCD("insufficient amount");  // Displays "insufficient amount" on the LCD.

        delay_ms(1500);  // Waits for 1500 milliseconds.
                }  // Ends the current function or control block.
                else if(( amount%100)!=0)  // Checks this alternative condition if the previous condition was false.
         {  // Begins the current function or control block.
        CmdLCD(0x01);  // Sends the clear-display command to the LCD.
        delay_ms(5);  // Waits for 5 milliseconds.

        CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
        StrLCD("invalid amount");  // Displays "invalid amount" on the LCD.

        delay_ms(1500);  // Waits for 1500 milliseconds.
                }  // Ends the current function or control block.
        return;  // Returns a value to the caller or exits the current function.
    }  // Ends the current function or control block.

    balance -= (int)amount;  // Subtracts the withdrawn amount from the account balance.

    SaveTransaction(amount, 2);  // Saves the completed transaction in the transaction-history arrays.

    UART_LogTransaction("WITHDRAW",  // Logs this ATM event, amount, balance, and RTC timestamp through UART0.
                        amount,  // Continues the current ATM operation.
                        (u32)balance);  // Executes this statement as part of the current ATM operation.

    atmDisplayState = ATM_DISPLAY_NEW_BALANCE;  // Updates the saved ATM screen state for later display restoration.
    CmdLCD(0x01);  // Sends the clear-display command to the LCD.
    delay_ms(5);  // Waits for 5 milliseconds.

    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
    StrLCD("New Balance:");  // Displays "New Balance:" on the LCD.

    CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

    U32LCD((u32)balance);  // Displays the numeric value (u32 on the LCD.

    ATM_DelayWithRTC(3000);  // Waits for the specified duration while continuing to check RTC interrupts.
    atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.
}  // Ends the current function or control block.


/*--------------------------------------------------
  DEPOSIT  // Continues the current ATM operation.
--------------------------------------------------*/  // Continues the current ATM operation.

void deposit(void)  // Begins the deposit function definition.
{  // Begins the current function or control block.
    u32 amount = 0;  // Declares a variable used by the ATM program.

    int key;  // Declares a variable used by the ATM program.
    int count = 0;  // Declares a variable used by the ATM program.

    CmdLCD(0x01);  // Sends the clear-display command to the LCD.
    delay_ms(5);  // Waits for 5 milliseconds.

    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
    StrLCD("Enter Amount:");  // Displays "Enter Amount:" on the LCD.

    CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

    while (1)  // Repeats the block while the condition remains true.
    {  // Begins the current function or control block.
        atmDisplayState = ATM_DISPLAY_DEPOSIT;  // Updates the saved ATM screen state for later display restoration.
        atmDisplayAmount = amount;  // Saves the amount currently being entered.
        atmDisplayCount = count;  // Saves the current number of entered characters.
        key = KeyScan();  // Scans the keypad and stores the pressed key.
        delay_ms(500);  // Waits for 500 milliseconds.

        if (key >= 0 && key <= 9)  // Checks this condition before executing the following statement or block.
        {  // Begins the current function or control block.
            if (count < 5)  // Checks this condition before executing the following statement or block.
            {  // Begins the current function or control block.
                amount = (amount * 10U) + (u32)key;  // Appends the newly entered digit to the amount.

                count++;  // Increases the number of digits entered.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                delay_ms(5);  // Waits for 5 milliseconds.

                StrLCD("                ");  // Displays "                " on the LCD.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                delay_ms(5);  // Waits for 5 milliseconds.

                U32LCD(amount);  // Displays the numeric value amount on the LCD.

                delay_ms(10);  // Waits for 10 milliseconds.
            }  // Ends the current function or control block.
        }  // Ends the current function or control block.
        else if (key == 11)  // Checks this alternative condition if the previous condition was false.
        {  // Begins the current function or control block.
            /* B = Backspace */

            if (count > 0)  // Checks this condition before executing the following statement or block.
            {  // Begins the current function or control block.
                count--;  // Decreases the digit count after a backspace.

                amount = amount / 10U;  // Removes the last entered digit from the amount.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                delay_ms(5);  // Waits for 5 milliseconds.

                StrLCD("                ");  // Displays "                " on the LCD.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
                delay_ms(5);  // Waits for 5 milliseconds.

                if (count > 0)  // Checks this condition before executing the following statement or block.
                    U32LCD(amount);  // Displays the numeric value amount on the LCD.

                delay_ms(10);  // Waits for 10 milliseconds.
            }  // Ends the current function or control block.
        }  // Ends the current function or control block.
        else if (key == 10)  // Checks this alternative condition if the previous condition was false.
        {  // Begins the current function or control block.
            /* A = Confirm */

            break;  // Exits the current switch case or loop.
        }  // Ends the current function or control block.
        else if (key == 13)  // Checks this alternative condition if the previous condition was false.
        {  // Begins the current function or control block.
            /* D = Cancel */

            CmdLCD(0x01);  // Sends the clear-display command to the LCD.
            delay_ms(5);  // Waits for 5 milliseconds.

            StrLCD("Cancelled");  // Displays "Cancelled" on the LCD.

            delay_ms(1000);  // Waits for 1000 milliseconds.
            atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.

            return;  // Returns a value to the caller or exits the current function.
        }  // Ends the current function or control block.
    }  // Ends the current function or control block.

    atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.

    if (count == 0 || amount == 0 ||  // Checks this condition before executing the following statement or block.
        ((amount % 100U) != 0U))  // Continues the current ATM operation.
    {  // Begins the current function or control block.
        UART_LogTransaction("DEPOSIT_REJECTED",  // Logs this ATM event, amount, balance, and RTC timestamp through UART0.
                            amount,  // Continues the current ATM operation.
                            (u32)balance);  // Executes this statement as part of the current ATM operation.

        CmdLCD(0x01);  // Sends the clear-display command to the LCD.
        delay_ms(5);  // Waits for 5 milliseconds.

        CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
        StrLCD("Invalid Amount");  // Displays "Invalid Amount" on the LCD.

        delay_ms(1500);  // Waits for 1500 milliseconds.

        return;  // Returns a value to the caller or exits the current function.
    }  // Ends the current function or control block.

    balance += (int)amount;  // Adds the deposited amount to the account balance.

    SaveTransaction(amount, 1);  // Saves the completed transaction in the transaction-history arrays.

    UART_LogTransaction("DEPOSIT",  // Logs this ATM event, amount, balance, and RTC timestamp through UART0.
                        amount,  // Continues the current ATM operation.
                        (u32)balance);  // Executes this statement as part of the current ATM operation.

    atmDisplayState = ATM_DISPLAY_NEW_BALANCE;  // Updates the saved ATM screen state for later display restoration.
    CmdLCD(0x01);  // Sends the clear-display command to the LCD.
    delay_ms(5);  // Waits for 5 milliseconds.

    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
    StrLCD("New Balance:");  // Displays "New Balance:" on the LCD.

    CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

    U32LCD((u32)balance);  // Displays the numeric value (u32 on the LCD.

    ATM_DelayWithRTC(3000);  // Waits for the specified duration while continuing to check RTC interrupts.
    atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.
}  // Ends the current function or control block.


/*--------------------------------------------------
  CHANGE PIN  // Continues the current ATM operation.
--------------------------------------------------*/  // Continues the current ATM operation.

void Change_PIN(void)  // Begins the Change_PIN function definition.
{  // Begins the current function or control block.
    char newPin[5];  // Declares a variable used by the ATM program.

    char confirmPin[5];  // Declares a variable used by the ATM program.

    int key;  // Declares a variable used by the ATM program.

    CmdLCD(0x01);  // Sends the clear-display command to the LCD.
    delay_ms(5);  // Waits for 5 milliseconds.

    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
    StrLCD("Enter New PIN:");  // Displays "Enter New PIN:" on the LCD.

    CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

    i = 0;  // Assigns or updates the value of the variable.

    while (i < 4)  // Repeats the block while the condition remains true.
    {  // Begins the current function or control block.
        atmDisplayState = ATM_DISPLAY_CHANGE_NEW_PIN;  // Updates the saved ATM screen state for later display restoration.
        atmDisplayCount = i;  // Saves the current number of entered characters.
        key = KeyScan();  // Scans the keypad and stores the pressed key.
        delay_ms(500);  // Waits for 500 milliseconds.

        if (key >= 0 && key <= 9)  // Checks this condition before executing the following statement or block.
        {  // Begins the current function or control block.
            newPin[i] = (char)(key + '0');  // Assigns or updates the value of the variable.

            CharLCD('*');  // Displays the specified character on the LCD.

            delay_ms(10);  // Waits for 10 milliseconds.

            i++;  // Moves to the next position in the input array.
        }  // Ends the current function or control block.
        else if (key == 11 && i > 0)  // Checks this alternative condition if the previous condition was false.
        {  // Begins the current function or control block.
            /* B = Backspace */

            i--;  // Moves back one position after a backspace.

            CmdLCD(0x10);  // Executes this statement as part of the current ATM operation.
            delay_ms(5);  // Waits for 5 milliseconds.

            CharLCD(' ');  // Displays the specified character on the LCD.
            delay_ms(5);  // Waits for 5 milliseconds.

            CmdLCD(0x10);  // Executes this statement as part of the current ATM operation.
            delay_ms(5);  // Waits for 5 milliseconds.
        }  // Ends the current function or control block.
    }  // Ends the current function or control block.

    newPin[4] = '\0';  // Adds the null terminator so the character array is a valid C string.

    CmdLCD(0x01);  // Sends the clear-display command to the LCD.
    delay_ms(5);  // Waits for 5 milliseconds.

    CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
    StrLCD("Confirm PIN:");  // Displays "Confirm PIN:" on the LCD.

    CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

    i = 0;  // Assigns or updates the value of the variable.

    while (i < 4)  // Repeats the block while the condition remains true.
    {  // Begins the current function or control block.
        atmDisplayState = ATM_DISPLAY_CHANGE_CONFIRM_PIN;  // Updates the saved ATM screen state for later display restoration.
        atmDisplayCount = i;  // Saves the current number of entered characters.
        key = KeyScan();  // Scans the keypad and stores the pressed key.
        delay_ms(500);  // Waits for 500 milliseconds.

        if (key >= 0 && key <= 9)  // Checks this condition before executing the following statement or block.
        {  // Begins the current function or control block.
            confirmPin[i] = (char)(key + '0');  // Assigns or updates the value of the variable.

            CharLCD('*');  // Displays the specified character on the LCD.

            delay_ms(10);  // Waits for 10 milliseconds.

            i++;  // Moves to the next position in the input array.
        }  // Ends the current function or control block.
        else if (key == 11 && i > 0)  // Checks this alternative condition if the previous condition was false.
        {  // Begins the current function or control block.
            /* B = Backspace */

            i--;  // Moves back one position after a backspace.

            CmdLCD(0x10);  // Executes this statement as part of the current ATM operation.
            delay_ms(5);  // Waits for 5 milliseconds.

            CharLCD(' ');  // Displays the specified character on the LCD.
            delay_ms(5);  // Waits for 5 milliseconds.

            CmdLCD(0x10);  // Executes this statement as part of the current ATM operation.
            delay_ms(5);  // Waits for 5 milliseconds.
        }  // Ends the current function or control block.
    }  // Ends the current function or control block.

    confirmPin[4] = '\0';  // Adds the null terminator so the character array is a valid C string.

    if (strcmp(newPin, confirmPin) == 0)  // Checks this condition before executing the following statement or block.
    {  // Begins the current function or control block.
        strcpy(ACCOUNT_PIN, newPin);  // Copies the new PIN string into the account PIN variable.

        UART_LogTransaction("PIN_CHANGED", 0U, (u32)balance);  // Logs this ATM event, amount, balance, and RTC timestamp through UART0.

        atmDisplayState = ATM_DISPLAY_PIN_CHANGED;  // Updates the saved ATM screen state for later display restoration.
        CmdLCD(0x01);  // Sends the clear-display command to the LCD.
        delay_ms(5);  // Waits for 5 milliseconds.

        StrLCD("PIN Changed");  // Displays "PIN Changed" on the LCD.
    }  // Ends the current function or control block.
    else  // Executes this alternative block when the preceding condition was false.
    {  // Begins the current function or control block.
        UART_LogTransaction("PIN_CHANGE_FAILED", 0U, (u32)balance);  // Logs this ATM event, amount, balance, and RTC timestamp through UART0.

        atmDisplayState = ATM_DISPLAY_PIN_MISMATCH;  // Updates the saved ATM screen state for later display restoration.
        CmdLCD(0x01);  // Sends the clear-display command to the LCD.
        delay_ms(5);  // Waits for 5 milliseconds.

        StrLCD("PIN Mismatch");  // Displays "PIN Mismatch" on the LCD.
    }  // Ends the current function or control block.

    ATM_DelayWithRTC(1500);  // Waits for the specified duration while continuing to check RTC interrupts.
    atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.
}  // Ends the current function or control block.


/*--------------------------------------------------
  TRANSACTION HISTORY  // Continues the current ATM operation.
--------------------------------------------------*/  // Continues the current ATM operation.

void TransactionHistory(void)  // Begins the TransactionHistory function definition.
{  // Begins the current function or control block.
    int index;  // Declares a variable used by the ATM program.

    if (transactionCount == 0)  // Checks this condition before executing the following statement or block.
    {  // Begins the current function or control block.
        atmDisplayState = ATM_DISPLAY_NO_HISTORY;  // Updates the saved ATM screen state for later display restoration.
        CmdLCD(0x01);  // Sends the clear-display command to the LCD.
        delay_ms(5);  // Waits for 5 milliseconds.

        StrLCD("No Transactions");  // Displays "No Transactions" on the LCD.

        ATM_DelayWithRTC(1500);  // Waits for the specified duration while continuing to check RTC interrupts.
        atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.
        return;  // Returns a value to the caller or exits the current function.
    }  // Ends the current function or control block.

    for (index = 0; index < transactionCount; index++)  // Repeats the block using the specified loop counter and condition.
    {  // Begins the current function or control block.
        atmHistoryIndex = index;  // Records which transaction-history entry is being shown.
        atmDisplayState = ATM_DISPLAY_HISTORY_AMOUNT;  // Updates the saved ATM screen state for later display restoration.
        CmdLCD(0x01);  // Sends the clear-display command to the LCD.
        delay_ms(5);  // Waits for 5 milliseconds.

        CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.

        if (transactionType[index] == 1)  // Checks this condition before executing the following statement or block.
            StrLCD("DEPOSIT");  // Displays "DEPOSIT" on the LCD.
        else  // Executes this alternative block when the preceding condition was false.
            StrLCD("WITHDRAW");  // Displays "WITHDRAW" on the LCD.

        CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

        U32LCD((u32)transactionAmount[index]);  // Displays the numeric value (u32 on the LCD.

        ATM_DelayWithRTC(2000);  // Waits for the specified duration while continuing to check RTC interrupts.

        atmDisplayState = ATM_DISPLAY_HISTORY_BALANCE;  // Updates the saved ATM screen state for later display restoration.
        CmdLCD(0x01);  // Sends the clear-display command to the LCD.
        delay_ms(5);  // Waits for 5 milliseconds.

        CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
        StrLCD("Balance:");  // Displays "Balance:" on the LCD.

        CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.

        U32LCD((u32)transactionBalance[index]);  // Displays the numeric value (u32 on the LCD.

        ATM_DelayWithRTC(2000);  // Waits for the specified duration while continuing to check RTC interrupts.
    }  // Ends the current function or control block.

    atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.
}  // Ends the current function or control block.


/*--------------------------------------------------
  ATM MENU  // Continues the current ATM operation.
--------------------------------------------------*/  // Continues the current ATM operation.

void ATM_Menu(void)  // Begins the ATM_Menu function definition.
{  // Begins the current function or control block.
    int key;  // Declares a variable used by the ATM program.

    while (1)  // Repeats the block while the condition remains true.
    {  // Begins the current function or control block.
        CheckRTCInterrupt();  // Checks whether the RTC interrupt needs to be handled.

        CmdLCD(0x01);  // Sends the clear-display command to the LCD.
        delay_ms(5);  // Waits for 5 milliseconds.

        CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
        StrLCD("1.B 2.W/D 3.DEP");  // Displays "1.B 2.W/D 3.DEP" on the LCD.

        CmdLCD(GOTO_LINE2_POS0);  // Moves the LCD cursor to the second row, first column.
        StrLCD("4.P 5.H D.LOGOUT");  // Displays "4.P 5.H D.LOGOUT" on the LCD.

        atmDisplayState = ATM_DISPLAY_MENU;  // Updates the saved ATM screen state for later display restoration.
        atmDisplayCount = 0;  // Saves the current number of entered characters.
        key = KeyScan();  // Scans the keypad and stores the pressed key.
        delay_ms(500);  // Waits for 500 milliseconds.

        switch (key)  // Selects which operation to perform based on the given value.
        {  // Begins the current function or control block.
            case 1:  // Selects the operation for this menu/keypad choice.

                Balance_enquiry();  // Executes this statement as part of the current ATM operation.

                break;  // Exits the current switch case or loop.

            case 2:  // Selects the operation for this menu/keypad choice.

                withdraw();  // Executes this statement as part of the current ATM operation.

                break;  // Exits the current switch case or loop.

            case 3:  // Selects the operation for this menu/keypad choice.

                deposit();  // Executes this statement as part of the current ATM operation.

                break;  // Exits the current switch case or loop.

            case 4:  // Selects the operation for this menu/keypad choice.

                Change_PIN();  // Executes this statement as part of the current ATM operation.

                break;  // Exits the current switch case or loop.

            case 5:  // Selects the operation for this menu/keypad choice.

                TransactionHistory();  // Executes this statement as part of the current ATM operation.

                break;  // Exits the current switch case or loop.

            case 13:  // Selects the operation for this menu/keypad choice.

                UART_LogTransaction("LOGOUT",  // Logs this ATM event, amount, balance, and RTC timestamp through UART0.
                                    0U,  // Continues the current ATM operation.
                                    (u32)balance);  // Executes this statement as part of the current ATM operation.

                CmdLCD(0x01);  // Sends the clear-display command to the LCD.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                StrLCD("LOGGED OUT");  // Displays "LOGGED OUT" on the LCD.

                delay_ms(1500);  // Waits for 1500 milliseconds.
                                 CmdLCD(0x01);  // Sends the clear-display command to the LCD.
                                timedisplay();  // Executes this statement as part of the current ATM operation.
                atmDisplayState = ATM_DISPLAY_NONE;  // Updates the saved ATM screen state for later display restoration.

                return;  // Returns a value to the caller or exits the current function.

            default:  // Handles a choice that does not match any listed case.

                CmdLCD(0x01);  // Sends the clear-display command to the LCD.
                delay_ms(5);  // Waits for 5 milliseconds.

                CmdLCD(GOTO_LINE1_POS0);  // Moves the LCD cursor to the first row, first column.
                StrLCD("INVALID OPTION");  // Displays "INVALID OPTION" on the LCD.

                delay_ms(1000);  // Waits for 1000 milliseconds.

                break;  // Exits the current switch case or loop.
        }  // Ends the current function or control block.
    }  // Ends the current function or control block.
}  // Ends the current function or control block.
