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


void StoreCustCharFont2(unsigned char p)   // Function to store 50% battery custom character
{
    unsigned char i;              // Loop variable

    unsigned char batt50[8] =     // Array containing 8 rows of the 50% battery symbol
    {
        0x0E,                     // Row 1 pattern
        0x11,                     // Row 2 pattern
        0x11,                     // Row 3 pattern
        0x1F,                     // Row 4 pattern
        0x1F,                     // Row 5 pattern
        0x1F,                     // Row 6 pattern
        0x1F,                     // Row 7 pattern
        0x1F                      // Row 8 pattern
    };

    cmdlcd(0x50);                 // Set CGRAM address to 0x10 for custom character 2

    for(i=0;i<8;i++)              // Loop through all 8 rows
        charlcd(batt50[i]);       // Send each row pattern to LCD CGRAM
}


void StoreCustCharFont3(unsigned char p)   // Function to store 75% battery custom character
{
    unsigned char i;              // Loop variable

    unsigned char batt75[8] =     // Array containing 8 rows of the 75% battery symbol
    {
        0x0E,                     // Row 1 pattern
        0x11,                     // Row 2 pattern
        0x1F,                     // Row 3 pattern
        0x1F,                     // Row 4 pattern
        0x1F,                     // Row 5 pattern
        0x1F,                     // Row 6 pattern
        0x1F,                     // Row 7 pattern
        0x1F                      // Row 8 pattern
    };

    cmdlcd(0x58);                 // Set CGRAM address to 0x18 for custom character 3

    for(i=0;i<8;i++)              // Loop through all 8 rows
        charlcd(batt75[i]);       // Send each row pattern to LCD CGRAM
}


void StoreCustCharFont4(unsigned char p)   // Function to store 100% battery custom character
{
    unsigned char i;              // Loop variable

    unsigned char batt100[8] =    // Array containing 8 rows of the 100% battery symbol
    {
        0x0E,                     // Row 1 pattern
        0x1F,                     // Row 2 pattern
        0x1F,                     // Row 3 pattern
        0x1F,                     // Row 4 pattern
        0x1F,                     // Row 5 pattern
        0x1F,                     // Row 6 pattern
        0x1F,                     // Row 7 pattern
        0x1F                      // Row 8 pattern
    };

    cmdlcd(0x60);                 // Set CGRAM address to 0x20 for custom character 4

    for(i=0;i<8;i++)              // Loop through all 8 rows
        charlcd(batt100[i]);      // Send each row pattern to LCD CGRAM
}