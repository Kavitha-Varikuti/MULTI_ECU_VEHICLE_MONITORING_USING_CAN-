#include <LPC21xx.h>                  // Includes LPC21xx microcontroller register definitions
#include "delay.h"                    // Provides delay functions such as delay_ms()
#include "can.h"                      // Provides CAN frame structure and CAN functions
#include "types.h"                    // Provides user-defined data types such as u8, u32, f32
#include "lcd.h"                      // Provides LCD functions
#include "pin_connect_block.h"        // Provides pin configuration function
#include "ds18b20.h"                  // Provides DS18B20 temperature sensor functions
#include "battery_disp.h"             // Provides battery custom-character functions


void temp_symbol()                    // Function to create temperature symbol in LCD CGRAM
{
    u8 i;                             // Loop counter

    u8 tem[8] = {0x0E,0x0A,0x0A,0x1f,
                 0x1f,0x1f,0x1f,0x1F};
                                      // 8-byte pattern for the custom temperature symbol

    cmdlcd(0x78);                     // Sets CGRAM address for custom character

    for(i=0;i<8;i++)                  // Loops through all 8 rows
    {
        charlcd(tem[i]);              // Sends each row pattern to LCD CGRAM
    }
}


/* -------- Defines -------- */

#define OFF 0                         // Defines OFF indicator state
#define LEFT 1                        // Defines LEFT indicator state
#define RIGHT 2                       // Defines RIGHT indicator state


u32 convert(u32 x)                    // Converts received ADC value into percentage
{
    if(x<1)                           // Checks whether ADC value is below 1
    {
        x=0;                          // Sets value to zero
    }

    if(x>330)                         // Checks whether ADC value is greater than 330
    {
        x=330;                        // Limits maximum value to 330
    }

    return (x*100)/330;               // Converts 0-330 range into 0-100 percentage
}


volatile u32 percent=0;               // Stores battery percentage
f32 voltage;                          // Floating-point variable for voltage

u8 battery_percent,i;                 // Variables for battery percentage and loop/index
u32 level=0;                          // Stores battery display level from 0 to 4


/* -------- Globals -------- */

volatile u8 indicator_status = OFF;  // Stores current indicator state
                                      // Initially indicator is OFF

CANF txF;                             // CAN frame used for transmitting indicator command
CANF rxF;                             // CAN frame used for receiving battery information


/* -------- Custom LCD chars -------- */

u8 left_arrow[8] = {0x03,0x07,0x0F,0x1F,
                    0x0F,0x07,0x03,0x01};
                                      // 8-row pattern for LEFT arrow custom character

u8 right_arrow[8] = {0x18,0x1C,0x1E,0x1F,
                     0x1E,0x1C,0x18,0x10};
                                      // 8-row pattern for RIGHT arrow custom character


/* -------- Prototypes -------- */

void LoadCustomChars(void);           // Function prototype to load arrow characters
void sendsignal(void);                // Function prototype to send indicator CAN message

void eint0_isr(void) __irq;            // Interrupt service routine for EINT0
void eint1_isr(void) __irq;            // Interrupt service routine for EINT1


int temp;                             // Stores raw DS18B20 temperature data
u8 tp, tpd;                            // tp = integer temperature, tpd = decimal digit


int main(void)                        // Main program starts here
{
    Init_CAN1();                      // Initializes CAN1 communication

    initlcd();                        // Initializes LCD

    cmdlcd(0x80);                     // Moves LCD cursor to first row, first position

    strlcd(" <<< DASH BOARD >>> ");   // Displays dashboard heading


    temp_symbol();                     // Creates/stores temperature custom character

    cmdlcd(0xC0);                     // Moves cursor to second LCD row

    strlcd("Indicator ");              // Displays "Indicator "


    /* Load custom arrows */

    LoadCustomChars();                // Loads LEFT and RIGHT arrow characters into CGRAM

    StoreCustCharFont(0);             // Stores 0% battery custom character
    StoreCustCharFont1(25);            // Stores 25% battery custom character
    StoreCustCharFont2(50);            // Stores 50% battery custom character
    StoreCustCharFont3(75);            // Stores 75% battery custom character
    StoreCustCharFont4(100);           // Stores 100% battery custom character


    /* Show steady <> */

    cmdlcd(0xC0 + 13);                // Moves cursor to position for LEFT arrow

    charlcd(5);                        // Displays custom character number 5

    cmdlcd(0xC0 + 15);                // Moves cursor to position for RIGHT arrow

    charlcd(6);                        // Displays custom character number 6


    /* -------- EINT config -------- */

    cfgportpinfunc(0,1,3);             // Configures P0.1 as EINT0

    cfgportpinfunc(0,3,3);             // Configures P0.3 as EINT1

    EXTMODE |= (1<<0) | (1<<1);       // Configures EINT0 and EINT1 as edge-triggered interrupts

    EXTPOLAR &= ~((1<<0) | (1<<1));
                                      // Configures EINT0 and EINT1 as falling-edge triggered

    EXTINT = (1<<0) | (1<<1);         // Clears pending EINT0 and EINT1 interrupt flags


    VICVectCntl0 = (1<<5) | 14;       // Enables VIC vector slot 0 and assigns EINT0 interrupt source

    VICVectAddr0 = (u32)eint0_isr;    // Stores address of EINT0 ISR in vector slot 0

    VICVectCntl1 = (1<<5) | 15;       // Enables VIC vector slot 1 and assigns EINT1 interrupt source

    VICVectAddr1 = (u32)eint1_isr;    // Stores address of EINT1 ISR in vector slot 1

    VICIntEnable = (1<<14) | (1<<15);
                                      // Enables EINT0 and EINT1 interrupts in VIC


    while(1)                          // Infinite main loop
    {

        /* LEFT indicator blink */

        if(indicator_status == LEFT)  // Checks whether LEFT indicator is active
        {
            cmdlcd(0xC0 + 13);        // Moves cursor to LEFT arrow position

            charlcd(5);               // Displays LEFT arrow

            delay_ms(300);             // Keeps LEFT arrow ON for 300 ms

            cmdlcd(0xC0 + 13);        // Moves cursor back to LEFT arrow position

            charlcd(' ');              // Clears LEFT arrow by displaying a space

            cmdlcd(0xC0 + 15);        // Moves cursor to RIGHT arrow position

            charlcd(6);               // Displays RIGHT arrow
        }


        /* RIGHT indicator blink */

        else if(indicator_status == RIGHT)
                                      // Checks whether RIGHT indicator is active
        {
            cmdlcd(0xC0 + 13);        // Moves cursor to LEFT arrow position

            charlcd(5);               // Displays LEFT arrow

            cmdlcd(0xC0 + 15);        // Moves cursor to RIGHT arrow position

            charlcd(6);               // Displays RIGHT arrow

            delay_ms(300);             // Keeps RIGHT arrow ON for 300 ms

            cmdlcd(0xC0 + 15);        // Moves cursor to RIGHT arrow position

            charlcd(' ');              // Clears RIGHT arrow
        }


        /* OFF state */

        else                            // Executes when no indicator is active
        {
            cmdlcd(0xC0 + 13);         // Moves cursor to LEFT arrow position

            charlcd(5);                // Displays LEFT arrow

            cmdlcd(0xC0 + 15);         // Moves cursor to RIGHT arrow position

            charlcd(6);                // Displays RIGHT arrow
        }


        delay_ms(100);                  // Small delay between operations


        temp = ReadTemp();              // Reads raw temperature from DS18B20

        tp = temp >> 4;                 // Extracts integer part of temperature

        tpd = (temp & 0x08) ? '5' : '0';
                                      // Determines decimal part as 0 or 5


        cmdlcd(0xD4);                   // Moves cursor to fourth LCD row

        strlcd("Temp ");                // Displays "Temp "

        charlcd(7);                     // Displays temperature custom character

        strlcd(" : ");                  // Displays separator


        u32lcd(tp);                     // Displays integer temperature

        charlcd('.');                   // Displays decimal point

        charlcd(tpd);                   // Displays decimal digit

        strlcd(" C ");                  // Displays Celsius unit


        if (C1GSR & (1 << 0))           // Checks whether CAN receive buffer contains data
        {

            /* Battery Node */

            CAN1_Rx(&rxF);              // Receives CAN frame and stores it in rxF


            /* Battery message */

            if(rxF.ID)                   // Checks whether received CAN ID is non-zero
            {
                percent = convert(rxF.Data1);
                                      // Converts received battery ADC value to percentage


                cmdlcd(0x94);            // Moves cursor to battery display position

                strlcd("Battery: ");     // Displays "Battery: "

                u32lcd(percent);         // Displays battery percentage

                strlcd("% ");            // Displays percentage symbol


                if(percent <= 0)         // Checks whether battery percentage is 0
                {
                    level=0;             // Selects 0% battery symbol
                }

                else if(percent <= 25)  // Checks whether battery is up to 25%
                {
                    level=1;             // Selects 25% battery symbol
                }

                else if(percent <= 50)  // Checks whether battery is up to 50%
                {
                    level=2;             // Selects 50% battery symbol
                }

                else if(percent <= 75)  // Checks whether battery is up to 75%
                {
                    level=3;             // Selects 75% battery symbol
                }

                else                    // Battery percentage is above 75%
                {
                    level=4;             // Selects 100% battery symbol
                }


                cmdlcd(0x94+13);         // Moves cursor to battery icon position

                charlcd(level);          // Displays selected battery custom character
            }
        }

        delay_ms(200);                   // Waits 200 ms before next main-loop cycle
    }
}


/* -------- EINT0 : LEFT -------- */

void eint0_isr(void) __irq               // Interrupt Service Routine for EINT0
{
    indicator_status = (indicator_status == LEFT) ? OFF : LEFT;
                                      // If LEFT is already ON, turn it OFF
                                      // Otherwise select LEFT indicator

    sendsignal();                       // Sends updated indicator status through CAN

    EXTINT = (1<<0);                    // Clears EINT0 interrupt flag

    VICVectAddr = 0;                    // Signals end of interrupt to VIC
}


/* -------- EINT1 : RIGHT -------- */

void eint1_isr(void) __irq               // Interrupt Service Routine for EINT1
{
    indicator_status = (indicator_status == RIGHT) ? OFF : RIGHT;
                                      // If RIGHT is already ON, turn it OFF
                                      // Otherwise select RIGHT indicator

    sendsignal();                       // Sends updated indicator status through CAN

    EXTINT = (1<<1);                    // Clears EINT1 interrupt flag

    VICVectAddr = 0;                    // Signals end of interrupt to VIC
}


/* -------- CAN TRANSMIT -------- */

void sendsignal(void)                 // Function to transmit indicator status through CAN
{
    txF.ID = 0x11;                    // Sets CAN message ID to 0x11

    txF.BFV.RTR = 0;                  // Sets RTR = 0, so this is a data frame

    txF.BFV.DLC = 1;                  // Sets DLC = 1 because only one data byte is transmitted

    txF.Data1 = (indicator_status == LEFT) ? 'L' :
                (indicator_status == RIGHT) ? 'R' : 'O';
                                      // Sends 'L' for LEFT
                                      // Sends 'R' for RIGHT
                                      // Sends 'O' for OFF

    CAN1_Tx(txF);                     // Transmits CAN frame through CAN1
}


/* -------- Load CGRAM arrows -------- */

void LoadCustomChars(void)             // Function to store arrow characters in LCD CGRAM
{
    u8 i;                              // Loop counter

    cmdlcd(0x68);                      // Sets CGRAM address for custom character 5
                                       // Character 5 = LEFT arrow

    for(i=0;i<8;i++)                   // Loops through 8 rows
        charlcd(left_arrow[i]);        // Stores each row of LEFT arrow pattern


    cmdlcd(0x70);                      // Sets CGRAM address for custom character 6
                                       // Character 6 = RIGHT arrow

    for(i=0;i<8;i++)                   // Loops through 8 rows
        charlcd(right_arrow[i]);       // Stores each row of RIGHT arrow pattern
}