# ATM-SIMULATOR-LPC2148
ATM Simulator with Real-Time Transaction Logging using LPC2148
## System Architecture

```mermaid
flowchart TD
    A[User] --> B[Keypad]
    B --> C[LPC2148]
    C --> D[LCD Display]
    C --> E[RTC]
    C --> F[UART]
    F --> G[PC Serial Terminal]
    C --> H[Buzzer]
    I[Interrupt Button] --> C
