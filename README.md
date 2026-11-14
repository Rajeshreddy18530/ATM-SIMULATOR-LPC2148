ATM Simulator with Real-Time Transaction Logging using LPC2148

1. Project Overview

The ATM Simulator with Real-Time Transaction Logging is an embedded systems project developed using the LPC2148 ARM7 microcontroller.

This project simulates ATM operations such as user authentication, balance enquiry, cash withdrawal, cash deposit, PIN change, and transaction history.

It uses a 16x2 LCD for displaying information, a 4x4 matrix keypad for user input, an internal RTC for date and time, and UART communication for transaction logging.

2. Objectives

- To develop an ATM simulation using the LPC2148 microcontroller.
- To implement PIN-based authentication.
- To perform deposit and withdrawal operations.
- To display account balance.
- To maintain transaction history.
- To implement RTC-based date and time.
- To send transaction details through UART.
- To implement external interrupt functionality.

3. Features

- PIN-based login authentication.
- Three-attempt login restriction.
- Balance enquiry.
- Cash withdrawal.
- Cash deposit.
- PIN change.
- Transaction history.
- Real-Time Clock.
- External interrupt for RTC editing.
- UART-based transaction logging.
- LCD display and buzzer indication.

4. Hardware Requirements

S.No| Component| Purpose
1| LPC2148 ARM7| Main controller
2| 16x2 LCD| Display
3| 4x4 Keypad| User input
4| Buzzer| Audio indication
5| Push Button| External interrupt
6| Computer| Transaction monitoring
7| USB-to-UART interface| Serial communication, if required

5. Software Requirements

S.No| Software| Purpose
1| Keil µVision| Embedded C development
2| Embedded C| Programming language
3| Flash Magic| Microcontroller programming
4| Serial Terminal| UART monitoring

6. System Architecture

The LPC2148 acts as the central controller. It communicates with the keypad, LCD, RTC, buzzer, and computer.

flowchart TD
    A[User] --> B[4x4 Keypad]
    B --> C[LPC2148 ARM7]
    C --> D[16x2 LCD]
    C --> E[Internal RTC]
    C --> F[UART0]
    F --> G[Computer Serial Terminal]
    C --> H[Buzzer]
    I[External Interrupt Button] --> C

7. Hardware Interfacing

7.1 LCD Interfacing

The 16x2 LCD is used to display ATM menus, account information, balance, and transaction status.

LCD Pin| LPC2148 Pin
D0| P0.8
D1| P0.9
D2| P0.10
D3| P0.11
D4| P0.12
D5| P0.13
D6| P0.14
D7| P0.15
RS| P0.16
RW| P0.17
EN| P0.18

LCD Block Diagram

flowchart LR
    A[LPC2148] --> B[Data Lines D0-D7]
    A --> C[Control Lines RS RW EN]
    B --> D[16x2 LCD]
    C --> D

7.2 Keypad Interfacing

A 4x4 matrix keypad is used to enter the account number, PIN, amount, and menu selections.

Keypad Pin| LPC2148 Pin
Row 0| P1.16
Row 1| P1.17
Row 2| P1.18
Row 3| P1.19
Column 0| P1.20
Column 1| P1.21
Column 2| P1.22
Column 3| P1.23

Keypad Block Diagram

flowchart TD
    A[4x4 Matrix Keypad] --> B[Row Scanning]
    A --> C[Column Scanning]
    B --> D[LPC2148 GPIO]
    C --> D
    D --> E[Key Identification]
    E --> F[ATM Operations]

7.3 RTC Interfacing

The internal RTC of LPC2148 maintains the current date and time.

It provides seconds, minutes, hours, day, month, and year.

RTC Working Flowchart

flowchart TD
    A[Start] --> B[Initialize RTC]
    B --> C[Read RTC Registers]
    C --> D[Get Date and Time]
    D --> E[Display Time on LCD]
    E --> F[Use Timestamp in Transaction Log]

7.4 External Interrupt

The external interrupt EINT1 is connected to P0.3.

When the interrupt button is pressed, the system opens the RTC editing menu.

External Interrupt Flowchart

flowchart TD
    A[ATM Main Program] --> B{Interrupt Button Pressed?}
    B -- No --> A
    B -- Yes --> C[EINT1 Interrupt]
    C --> D[Execute ISR]
    D --> E[Open RTC Menu]
    E --> F[Edit Date or Time]
    F --> G[Update RTC]
    G --> H[Restore ATM Display]
    H --> A

7.5 Buzzer Interfacing

The buzzer is connected to P0.19.

- Logic HIGH: Buzzer ON.
- Logic LOW: Buzzer OFF.

The buzzer provides audio indication during specified ATM events.

7.6 UART Interfacing

UART0 is used to transmit transaction information to a computer.

UART Signal| LPC2148 Pin
TXD0| P0.0
RXD0| P0.1

UART Configuration

Parameter| Value
Baud Rate| 9600
Data Bits| 8
Parity| None
Stop Bits| 1

UART Communication Diagram

flowchart LR
    A[ATM Transaction] --> B[LPC2148]
    B --> C[UART0]
    C --> D[Serial Communication]
    D --> E[Computer]
    E --> F[Serial Terminal]
    F --> G[Transaction Details]

8. ATM Operations

8.1 User Authentication

The user enters an account number and PIN through the keypad. The system validates the entered details before allowing access.

Demonstration credentials:

Parameter| Value
Account Number| 2004
Initial PIN| 1234
Initial Balance| 5000

8.2 Balance Enquiry

The user selects balance enquiry to view the available account balance on the LCD.

8.3 Cash Withdrawal

The user enters the withdrawal amount. The system checks the amount and available balance before processing the withdrawal.

The withdrawal amount must be a multiple of 100.

8.4 Cash Deposit

The user enters the deposit amount. The system adds the entered amount to the account balance.

8.5 PIN Change

The user can change the existing PIN by entering the current PIN and providing a new PIN.

8.6 Transaction History

The system stores recent transactions in an internal transaction history array. The program supports up to 10 stored transactions.

9. Complete ATM Working Flowchart

flowchart TD
    A([Start]) --> B[Initialize LPC2148 Peripherals]
    B --> C[Initialize LCD and Keypad]
    C --> D[Initialize RTC and UART]
    D --> E[Initialize External Interrupt]
    E --> F[Display Welcome Message]
    F --> G[Enter Account Number]
    G --> H[Enter PIN]
    H --> I{Credentials Valid?}
    I -- No --> J[Increase Failed Attempts]
    J --> K{Attempts Reached Limit?}
    K -- Yes --> L[Activate Buzzer and Show Lock Message]
    L --> G
    K -- No --> G
    I -- Yes --> M[Display ATM Menu]
    M --> N{Select Operation}
    N --> O[Balance Enquiry]
    N --> P[Cash Withdrawal]
    N --> Q[Cash Deposit]
    N --> R[Change PIN]
    N --> S[Transaction History]
    O --> T[Return to Menu]
    P --> T
    Q --> T
    R --> T
    S --> T
    T --> U{Exit ATM?}
    U -- No --> M
    U -- Yes --> V([End])

10. Software Architecture

The project is divided into multiple source and header files for modular programming.

flowchart TD
    A[main.c] --> B[atm.c]
    A --> C[interrupt.c]
    A --> D[rtc.c]
    A --> E[UART Module]
    A --> F[Keypad Module]
    A --> G[LCD Module]
    A --> H[Delay Module]
    B --> D
    B --> E
    C --> D
    C --> F

11. UART Transaction Logging

Whenever a transaction is performed, the system can transmit transaction details through UART0.

Transaction information can include:

- Transaction type.
- Transaction amount.
- Updated balance.
- Date and time.

Transaction Logging Sequence

sequenceDiagram
    actor User
    participant Keypad
    participant LPC2148
    participant RTC
    participant UART
    participant PC

    User->>Keypad: Enter transaction
    Keypad->>LPC2148: Send key values
    LPC2148->>RTC: Read date and time
    RTC-->>LPC2148: Return timestamp
    LPC2148->>LPC2148: Update account balance
    LPC2148->>UART: Send transaction details
    UART->>PC: Transmit serial data
    PC-->>User: Display transaction log

12. Project Folder Structure

ATM-Simulator-LPC2148/
│
├── README.md
│
├── source_files/
│   ├── main.c
│   ├── atm.c
│   ├── interrupt.c
│   ├── rtc.c
│   ├── uart.c
│   ├── kpm.c
│   ├── lcd.c
│   └── delay.c
│
├── header_files/
│   ├── atm.h
│   ├── interrupt.h
│   ├── rtc.h
│   ├── uart.h
│   ├── uart_defines.h
│   ├── kpm.h
│   ├── kpm_defines.h
│   ├── lcd.h
│   ├── lcd_defines.h
│   ├── delay.h
│   ├── defines.h
│   └── types.h
│
├── images/
├── videos/
└── test_results/

13. Testing

Test Case| Expected Result
Valid account and PIN| ATM menu displayed
Invalid PIN| Login attempt rejected
Balance enquiry| Current balance displayed
Cash withdrawal| Balance updated
Cash deposit| Balance updated
PIN change| New PIN accepted
Transaction history| Stored transactions displayed
Interrupt button| RTC menu opened
UART communication| Transaction details received on PC

14. Limitations

- This is an educational ATM simulation, not a real banking system.
- Account information and balance are demonstration values.
- Transaction history is limited by internal storage.
- UART logging requires a serial connection to a computer.
- The lock message is demonstration logic and does not implement a secure 24-hour account lock.

15. Future Enhancements

- Add EEPROM-based permanent transaction storage.
- Add multiple user accounts.
- Add SD card-based transaction logging.
- Add GSM or Wi-Fi notifications.
- Improve PIN security through encryption.
- Develop a graphical interface for transaction monitoring.

16. Learning Outcomes

- ARM7 LPC2148 microcontroller programming.
- Embedded C programming.
- GPIO interfacing.
- LCD interfacing.
- Matrix keypad scanning.
- RTC register programming.
- External interrupt handling.
- UART serial communication.
- Modular embedded software development.

17. Conclusion

The LPC2148 ATM Simulator demonstrates how an ARM7 microcontroller can be used to develop an embedded ATM application.

It combines keypad input, LCD display, RTC timekeeping, external interrupt handling, buzzer indication, and UART-based transaction logging into one integrated system.

The project provides practical experience in embedded C programming and peripheral interfacing.

---

Project: ATM Simulator with Real-Time Transaction Logging

Microcontroller: LPC2148 ARM7

IDE: Keil µVision

Programming Language: Embedded C
