// kpm.h - Header file containing keypad function declarations

#include "types.h"  // Includes user-defined data types such as u32

void InitKPM(void); // Declares a function to initialize the keypad

u32 ColScan(void);  // Declares a function to check whether any keypad key is pressed

u32 RowCheck(void); // Declares a function to identify the row of the pressed key

u32 ColCheck(void); // Declares a function to identify the column of the pressed key

u32 KeyScan(void);  // Declares a function to scan the keypad and return the pressed key value

u32 ReadNum(void);  // Declares a function to read a numeric value from the keypad
