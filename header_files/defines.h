#include "types.h"  // Includes user-defined data types

#define SETBIT(WORD,BITPOS) WORD|=1<<BITPOS
// Sets the specified bit position to 1

#define CLRBIT(WORD,BITPOS) WORD&=~(1<<BITPOS)
// Clears the specified bit position to 0

#define CPLBIT(WORD,BITPOS) WORD^=(1<<BITPOS)
// Toggles the specified bit (0 becomes 1 and 1 becomes 0)

#define WRITEBIT(WORD,BITPOS,BIT) WORD=((WORD&~(1<<BITPOS))|(BIT<<BITPOS))
// Writes the given BIT value at the specified bit position

#define READBIT(WORD,BITPOS) WORD=((WORD>>BITPOS)&1)
// Reads the specified bit and stores its value in WORD

#define READWRITEBIT(WORD,DBIT,SBIT) \
WORD=((WORD&~(1<<DBIT))|(((WORD>>SBIT)&1)<<DBIT)) // Defines a macro to copy one bit to another bit position in the same WORD

// Copies the bit at SBIT to DBIT within the same WORD

#define READWRITEBIT2(DWORD,DBIT,SWORD,SBIT) \
DWORD=((DWORD&~(1<<DBIT))|(((SWORD>>SBIT)&1)<<DBIT)) // Defines a macro to copy a bit from SWORD to DWORD

// Copies the bit at SBIT in SWORD to DBIT in DWORD

#define WRITENIBBLE(WORD,STARTBIT,VAL)\
WORD=((WORD&~(15<<STARTBIT))|(VAL<<STARTBIT))// Defines a macro to write a 4-bit value (nibble) into WORD
// Clears four bits from STARTBIT and writes the new value

#define READNIBBLE(WORD,STARTBIT) WORD=((WORD>>STARTBIT)&15)
// Reads four bits starting from STARTBIT and stores the result in WORD

#define WRITEBYTE(WORD,STARTBIT,VAL)\
WORD=((WORD&~(255<<STARTBIT))|(VAL<<STARTBIT))// Defines a macro to write an 8-bit value (byte) into WORD
// Clears eight bits from STARTBIT and writes the new value

#define READBYTE(WORD,STARTBIT) WORD=((WORD>>STARTBIT)&255)
// Reads eight bits starting from STARTBIT and stores the result in WORD


