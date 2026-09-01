#include<LPC21xx.h>                 // Includes LPC21xx microcontroller register definitions
#include "delay.h"                  // Includes delay functions like delay_ms() and delay_us()
#include "lcd.h"                    // Includes LCD function declarations
#include "defines.h"                // Includes macros such as WRITEBIT() and WRITEBYTE()

#define LCD_DAT 8                   // LCD data pins start from P0.8
#define RS 17                       // RS (Register Select) connected to P0.17
#define RW 18                       // RW (Read/Write) connected to P0.18
#define EN 19                       // EN (Enable) connected to P0.19


void writelcd(char ch)              // Function to send a command/data byte to LCD
{
    WRITEBIT(IOCLR0,RW,1);          // Clears RW pin → RW = 0 → selects WRITE operation

    WRITEBYTE(IOPIN0,LCD_DAT,ch);   // Sends 8-bit data/command to LCD data pins P0.8-P0.15

    WRITEBIT(IOSET0,EN,1);          // Sets Enable pin HIGH → starts LCD data transfer

    WRITEBIT(IOCLR0,EN,1);          // Sets Enable pin LOW → completes LCD data transfer

    delay_ms(2);                    // Provides delay for LCD to process the data
}


void cmdlcd(char cmd)               // Function to send a command to LCD
{
    WRITEBIT(IOCLR0,RS,1);          // RS = 0 → tells LCD that the value is a command

    writelcd(cmd);                  // Sends the command byte to LCD
}


void initlcd(void)                  // Function to initialize the LCD
{
    WRITEBYTE(IODIR0,LCD_DAT,0xFF); // Configures P0.8-P0.15 as output pins for LCD data

    WRITEBIT(IODIR0,RS,1);          // Configures RS pin as output

    WRITEBIT(IODIR0,RW,1);          // Configures RW pin as output

    WRITEBIT(IODIR0,EN,1);          // Configures EN pin as output

    delay_ms(15);                   // Waits 15 ms after powering up the LCD

    cmdlcd(0x30);                   // Sends initialization command to LCD

    delay_ms(5);                    // Waits 5 ms

    cmdlcd(0x30);                   // Sends initialization command again

    delay_us(100);                  // Waits 100 microseconds

    cmdlcd(0x30);                   // Sends initialization command again

    //delay_ms(1);                  // Optional 1 ms delay, currently disabled

    cmdlcd(0x38);                   // Configures LCD: 2 lines, 8-bit mode, 5x7 font

    cmdlcd(0x0C);                   // Display ON, cursor OFF, cursor blink OFF

    cmdlcd(0x06);                   // Cursor moves right after writing; display does not shift

    cmdlcd(0x01);                   // Clears the LCD display
}


void charlcd(char dat)              // Function to display one character on LCD
{
    WRITEBIT(IOSET0,RS,1);           // RS = 1 → selects data/character mode

    writelcd(dat);                   // Sends the character to LCD
}


void strlcd(char *p)                // Function to display a string on LCD
{
    while(*p)                       // Continues until NULL character '\0' is reached
        charlcd(*p++);              // Displays current character and increments pointer
}


void u32lcd(signed int n)            // Function to display an integer on LCD
{
    char a[10],i=0;                 // Array stores individual digits; i is array index

    if(n<0)                          // Checks whether the number is negative
    {
        charlcd('-');                // Displays minus sign

        n=-n;                        // Converts negative number to positive
    }

    do                               // Executes at least once even if n = 0
    {
        a[i++]=n%10+48;              // Extracts last digit and converts it to ASCII
                                      // ASCII value of '0' = 48

        n=n/10;                       // Removes the last digit from the number

    }while(n);                        // Continues until all digits are extracted

    for(;i>0;i--)                    // Traverses stored digits in reverse order
        charlcd(a[i-1]);              // Displays each digit from most significant to least significant
}


void f32lcd(float f,char i)          // Function to display a floating-point number
                                    // f = floating-point value
                                    // i = number of digits to display after decimal point
//1st arg(f) is the float number to be displayed on LCD
//2nd arg(i) is number digits to be displayed after the dot('.')
{
    unsigned long int n=f;           // Extracts integer part of floating-point number

    u32lcd(n);                       // Displays the integer part

    charlcd('.');                   // Displays decimal point

    for(;i>0;i--)                   // Repeats according to required decimal places
    {
        f=f*10;                     // Shifts the next decimal digit to integer position

        n=f;                        // Extracts the integer part

        charlcd(n%10+48);           // Extracts last digit and converts it to ASCII
    }
}