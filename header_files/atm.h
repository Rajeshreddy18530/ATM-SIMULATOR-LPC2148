#ifndef ATM_H
// Checks whether ATM_H has not been defined.
// Prevents multiple inclusion of this header file.

#define ATM_H
// Defines ATM_H to avoid duplicate declarations.

#include "types.h"
// Includes user-defined data types such as u8, u32, s32 and f32.

/* ATM Configuration */

#define ACCOUNT_NO "2004"
// Defines the account number as a string.

#define MAX_ATTEMPTS 3
// Specifies the maximum number of incorrect PIN attempts allowed.

#define MAX_TRANSACTIONS 10
// Specifies the maximum number of transactions stored in the history.

#define BUZZER_PIN 19
// Defines GPIO pin number 19 for controlling the buzzer.

/* Account information */

extern char ACCOUNT_PIN[5];
// Declares the account PIN array.
// extern indicates that the actual array is defined in another source file.
// Size 5 accommodates a 4-digit PIN and NULL character.

extern int balance;
// Declares the account balance variable.
// Stores the available account balance.

/* Login variables */

extern char account[5];
// Declares an array to store the account number entered by the user.

extern char pin[5];
// Declares an array to store the PIN entered by the user.

extern int keyv;
// Stores the keypad key value pressed by the user.

extern int attempts;
// Tracks the number of incorrect login attempts.

extern int i;
// Declares a loop counter used in the program.

/* Transaction history */

extern int transactionAmount[MAX_TRANSACTIONS];
// Stores the amount involved in each transaction.

extern int transactionType[MAX_TRANSACTIONS];
// Stores the type of transaction (deposit or withdrawal).

extern int transactionBalance[MAX_TRANSACTIONS];
// Stores the remaining account balance after each transaction.

extern int transactionCount;
// Tracks the total number of transactions recorded.

/* Shared interrupt and LCD restoration flags */

extern volatile int interruptFlag;
// Declares a shared interrupt flag.
// volatile ensures that the compiler recognizes changes made by an ISR.

extern volatile int rtcDisplayRestore;
// Indicates whether the LCD should restore the ATM display
// after displaying RTC information.

/* Function declarations */

int ATM_Login(void);
// Checks the account number and PIN entered by the user.
// Returns the login status.

void ATM_Menu(void);
// Displays the main ATM menu and handles user selections.

void Balance_enquiry(void);
// Displays the current account balance.

void withdraw(void);
// Performs a withdrawal operation.
// Deducts the requested amount from the account balance.

void deposit(void);
// Performs a deposit operation.
// Adds the deposited amount to the account balance.

void Change_PIN(void);
// Allows the user to change the existing account PIN.

void TransactionHistory(void);
// Displays previously recorded deposit and withdrawal transactions.

void Display_welcome(void);
// Displays the welcome message on the LCD.

void RestoreATMDisplay(void);
// Restores the normal ATM LCD display after an interrupt or RTC display.

/* UART transaction logging */

void UART_LogTransaction(char *type,
                         u32 amount,
                         u32 remainingBalance);
// Sends transaction information through UART.
// type: Transaction type (Deposit or Withdrawal).
// amount: Transaction amount.
// remainingBalance: Account balance after the transaction.

/* Transaction storage */

void SaveTransaction(u32 amount, int type);
// Saves transaction details in the transaction history arrays.
// amount: Transaction amount.
// type: Identifies whether the transaction is a deposit or withdrawal.

#endif
// Ends the conditional compilation block.
// Prevents multiple inclusion of atm.h.
