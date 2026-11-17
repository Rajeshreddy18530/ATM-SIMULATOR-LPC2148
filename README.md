# ATM Simulator with Real-Time Transaction Logging using LPC2148

## 1. Project Overview

The ATM Simulator with Real-Time Transaction Logging is an embedded systems project developed using the LPC2148 ARM7 microcontroller.

This project simulates basic ATM operations such as:

- User authentication
- Balance enquiry
- Cash withdrawal
- Cash deposit
- PIN change
- Transaction history

The system uses a 16x2 LCD to display information, a 4x4 matrix keypad for user input, an internal RTC to maintain date and time, and UART communication to transmit transaction details to a computer.

An external interrupt is used to access the RTC date and time editing menu.

## 2. Project Objectives

1. To develop an ATM simulation using the LPC2148 microcontroller.
2. To implement PIN-based authentication.
3. To perform cash withdrawal and deposit operations.
4. To display the available account balance.
5. To maintain transaction history.
6. To implement RTC-based date and time.
7. To transmit transaction details through UART.
8. To implement external interrupt functionality.

## 3. Project Features

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
- LCD display.
- Buzzer indication.

## 4. Hardware Requirements

- LPC2148 ARM7 Microcontroller
- 16x2 LCD Display
- 4x4 Matrix Keypad
- Buzzer
- Push Button
- Computer
- USB-to-UART Interface

## 5. Software Requirements

- Keil µVision IDE
- Embedded C Programming
- Flash Magic
- UART Serial Terminal
## 6. System Architecture

The ATM Simulator uses the LPC2148 microcontroller as the main controller. The keypad is used to enter the PIN and select ATM operations. The LCD displays messages and account details. The RTC provides date and time information, while UART0 sends transaction details to the serial terminal.

### System Block Diagram

```mermaid
flowchart TD
    A[User] --> B[4x4 Keypad]
    B --> C[LPC2148 Microcontroller]
    C --> D[16x2 LCD Display]
    C --> E[RTC Module]
    C --> F[UART0 Communication]
    C --> G[Buzzer]
    F --> H[Serial Terminal]
```

### Working Principle

1. The user enters the PIN using the keypad.
2. The LPC2148 checks the entered PIN.
3. After successful login, the ATM displays the available operations.
4. The user selects deposit, withdrawal, balance enquiry, or transaction history.
5. The RTC provides the current date and time.
6. Transaction details are transmitted through UART0.
7. The LCD displays the transaction result.
## 7. Hardware Interfacing

The LPC2148 microcontroller is connected to different hardware components to perform ATM operations.

The following components are interfaced with the microcontroller:

- 16x2 LCD Display
- 4x4 Matrix Keypad
- Real-Time Clock (RTC)
- UART0 Communication
- Buzzer
- External Interrupt Button

### 7.1 LCD Interfacing

A 16x2 LCD is used to display ATM messages, PIN entry prompts, account balance, and transaction status.

**LCD Pin Connections**

| LCD Pin | LPC2148 Pin |
|---|---|
| D0-D7 | P0.8-P0.15 |
| RS | P0.16 |
| RW | P0.17 |
| EN | P0.18 |

### 7.2 Keypad Interfacing

A 4x4 matrix keypad is used to enter the ATM PIN, select menu options, and enter transaction amounts.

**Keypad Pin Connections**

| Keypad Pins | LPC2148 Pins |
|---|---|
| Row 1 | P1.16 |
| Row 2 | P1.17 |
| Row 3 | P1.18 |
| Row 4 | P1.19 |
| Column 1 | P1.20 |
| Column 2 | P1.21 |
| Column 3 | P1.22 |
| Column 4 | P1.23 |

**Working:**

1. The microcontroller scans the keypad rows and columns.
2. When a key is pressed, the corresponding row and column are identified.
3. The pressed key value is returned to the ATM program.
4. The entered value is used for PIN verification or ATM operations.
