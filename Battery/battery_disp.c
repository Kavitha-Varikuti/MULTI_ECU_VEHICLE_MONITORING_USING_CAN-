#include <LPC21xx.h>              // Includes LPC21xx microcontroller register definitions
#include "lcd.h"                  // Includes LCD functions such as cmdlcd() and charlcd()

void StoreCustCharFont(unsigned char p)   // Function to store 0% battery custom character
{
    unsigned char i;              // Loop variable

    unsigned char batt0[8] =      // Array containing 8 rows of the 0% battery symbol
    {
        0x0E,                     // Row 1 pattern
        0x11,                     // Row 2 pattern
        0x11,                     // Row 3 pattern
        0x11,                     // Row 4 pattern
        0x11,                     // Row 5 pattern
        0x11,                     // Row 6 pattern
        0x11,                     // Row 7 pattern
        0x1F                      // Row 8 pattern
    };

    cmdlcd(0x40);                 // Set CGRAM address to 0x00 for storing custom character

    for(i=0;i<8;i++)              // Loop through all 8 rows of the character
        charlcd(batt0[i]);        // Send each row pattern to LCD CGRAM
}


void StoreCustCharFont1(unsigned char p)   // Function to store 25% battery custom character
{
    unsigned char i;              // Loop variable

    unsigned char batt25[8] =     // Array containing 8 rows of the 25% battery symbol
    {
        0x0E,                     // Row 1 pattern
        0x11,                     // Row 2 pattern
        0x11,                     // Row 3 pattern
        0x11,                     // Row 4 pattern
        0x11,                     // Row 5 pattern
        0x1F,                     // Row 6 pattern
        0x1F,                     // Row 7 pattern
        0x1F                      // Row 8 pattern
    };

    cmdlcd(0x48);                 // Set CGRAM address to 0x08 for custom character 1

    for(i=0;i<8;i++)              // Loop through all 8 rows
        charlcd(batt25[i]);       // Send each row pattern to LCD CGRAM
}


void StoreCustCharFont2(unsigned char p)   //