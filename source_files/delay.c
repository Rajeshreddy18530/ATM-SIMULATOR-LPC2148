// delay.c - Contains delay functions for microsecond, millisecond and second delays

#include "types.h"       // Includes user-defined data types such as u32

// Function to generate delay in microseconds
void delay_us(u32 dlyUS) // dlyUS represents the delay time in microseconds
{
    for(dlyUS *= 12; dlyUS > 0; dlyUS--); // Multiplies by 12 for the delay loop and decrements until zero
}

// Function to generate delay in milliseconds
void delay_ms(u32 dlyMS) // dlyMS represents the delay time in milliseconds
{
    for(dlyMS *= 12000; dlyMS > 0; dlyMS--); // Multiplies by 12000 and decrements until zero
}

// Function to generate delay in seconds
void delay_s(u32 dlyS) // dlyS represents the delay time in seconds
{
    for(dlyS *= 12000000; dlyS > 0; dlyS--); // Multiplies by 12000000 and decrements until zero
	
}
