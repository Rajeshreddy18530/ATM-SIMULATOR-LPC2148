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
