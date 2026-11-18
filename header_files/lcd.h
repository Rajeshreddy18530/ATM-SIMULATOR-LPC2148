// lcd.h - Header file containing LCD function declarations

#include "types.h"  // Includes user-defined data types such as u8, u32, s32 and f32

void WriteLCD(u8 byte);  // Declares a function to send a byte (command or data) to the LCD

void CmdLCD(u8 cmd);  // Declares a function to send a command to the LCD

void InitLCD(void);  // Declares a function to initialize the LCD

void CharLCD(u8 asciiVal);  // Declares a function to display a single character on the LCD

void StrLCD(const char *s);  // Declares a function to display a string on the LCD

void U32LCD(u32);  // Declares a function to display an unsigned 32-bit integer on the LCD

void S32LCD(s32);  // Declares a function to display a signed 32-bit integer on the LCD

void F32LCD(f32,u8 nDP);  // Declares a function to display a floating-point number with the specified decimal places

void BuildCGRAM(const u8 *p,u8 nBytes);  // Declares a function to create custom characters in LCD CGRAM
