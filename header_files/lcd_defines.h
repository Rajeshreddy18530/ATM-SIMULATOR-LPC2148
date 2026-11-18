// lcd_defines.h - Contains LCD commands and pin definitions

// HD44780 LCD Commands

#define CLEAR_LCD            0x01  // Clears the entire LCD display

#define RET_CUR_HOME         0x02  // Returns the cursor to the home position

#define SHIFT_CUR_RIGHT      0x06  // Moves the cursor position to the right

#define SHIFT_CUR_LEFT       0x07  // Moves the cursor position to the left

#define DSP_OFF              0x08  // Turns OFF the LCD display

#define DSP_ON_CUR_OFF       0x0c  // Turns ON the display and turns OFF the cursor

#define DSP_ON_CUR_ON        0x0E  // Turns ON the display and cursor

#define DSP_ON_CUR_BLINK     0x0F  // Turns ON the display and makes the cursor blink

#define SHIFT_DISP_LEFT      0x10  // Shifts the entire display to the left

#define SHIFT_DISP_RIGHT     0x14  // Shifts the entire display to the right

#define MODE_8BIT_1LINE      0x30  // Configures the LCD for 8-bit mode and 1-line display

#define MODE_4BIT_1LINE      0x20  // Configures the LCD for 4-bit mode and 1-line display

#define MODE_8BIT_2LINE      0x38  // Configures the LCD for 8-bit mode and 2-line display

#define MODE_4BIT_2LINE      0x28  // Configures the LCD for 4-bit mode and 2-line display

#define GOTO_LINE1_POS0      0x80  // Moves the cursor to the first line, position 0

#define GOTO_LINE2_POS0      0xC0  // Moves the cursor to the second line, position 0

#define GOTO_LINE3_POS0      0x94  // Moves the cursor to the third line, position 0

#define GOTO_LINE4_POS0      0xD4  // Moves the cursor to the fourth line, position 0

#define GOTO_CGRAM_START     0x40  // Selects the starting address of CGRAM for custom characters


// LCD Connection Pins

#define LCD_DATA 8   // LCD data pins D0-D7 are connected to LPC2148 pins P0.8-P0.15

#define LCD_RS   16  // LCD RS (Register Select) pin is connected to P0.16

#define LCD_RW   17  // LCD RW (Read/Write) pin is connected to P0.17

#define LCD_EN   18  // LCD EN (Enable) pin is connected to P0.18
