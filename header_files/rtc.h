// RTC function declarations

void initrtc(void);              // Initializes the RTC
void setrtc(void);               // Sets the default RTC date and time
void readrtc(void);              // Reads the current RTC time and date
void DisplayRTC(void);           // Displays RTC time and date on LCD

void UpdateRTC(unsigned int h,   // Receives hours
               unsigned int m,   // Receives minutes
               unsigned int s,   // Receives seconds
               unsigned int d,   // Receives day
               unsigned int mon, // Receives month
               unsigned int y); // Receives year

void timedisplay(void);          // Displays RTC values for a fixed duration


/* Shared RTC time variables */

extern unsigned int seconds;     // Declares seconds variable defined in rtc.c
extern unsigned int minutes;     // Declares minutes variable defined in rtc.c
extern unsigned int hours;       // Declares hours variable defined in rtc.c
extern int count;                // Declares count variable defined in rtc.c
extern int tick;                 // Declares tick variable defined in rtc.c


/* Shared RTC date variables */

extern unsigned int day;         // Declares day variable defined in rtc.c
extern unsigned int month;       // Declares month variable defined in rtc.c
extern unsigned int year;        // Declares year variable defined in rtc.c


// System clock and peripheral clock Macros

#define FOSC 12000000            // External crystal frequency = 12 MHz

#define CCLK (5*FOSC)            // CPU clock = 5 * 12 MHz = 60 MHz

#define PCLK (CCLK/4)            // Peripheral clock = 60 MHz / 4 = 15 MHz


// RTC Prescaler Calculation Macros

// RTC requires a 32.768 kHz clock for one-second increments.

// PREINT and PREFRAC registers divide PCLK to generate 32.768 kHz.

// PREINT = int(PCLK / 32768) - 1;

// PREFRAC = PCLK - ((PREINT + 1) * 32768);

// Note: This information is collected from the LPC2129 manual.

#define PREINT_VAL (int)((PCLK / 32768) - 1) // Calculates integer prescaler value

#define PREFRAC_VAL (PCLK - ((PREINT_VAL + 1) * 32768)) // Calculates fractional prescaler value


// RTC Control Register (CCR) Bit Definitions

// Bit 0: Clock Enable
// 1 = Enable RTC counters
// 0 = Disable RTC counters

#define RTC_ENABLE (1<<0)        // Sets bit 0 of CCR to enable RTC


// Bit 1: Clock Reset
// 1 = Reset RTC divider
// 0 = Normal operation

#define RTC_RESET (1<<1)         // Sets bit 1 of CCR to reset the divider


// Only for LPC2148

// Bit 4: Clock Source Select

// 1 = Use external 32.768 kHz oscillator

// 0 = Use internal PCLK as RTC clock source

#define RTC_CLKSRC (1<<4)        // Sets bit 4 to select external RTC clock
