#include<LPC21xx.h>                    // Includes LPC21xx microcontroller register definitions
#include "delay.h"                     // Includes delay_ms() function
#include "can.h"                       // Includes CAN structure and CAN functions
#include "types.h"                     // Includes user-defined data types such as u8

#define OFF 0                          // Defines OFF state as 0
#define LEFT 1                         // Defines LEFT indicator state as 1
#define RIGHT 2                        // Defines RIGHT indicator state as 2

#define LED_START 23                   // First LED is connected to P1.23

#define LED_MASK (0xFF << LED_START)   // Creates an 8-bit mask starting from P1.23
                                       // LEDs are connected to P1.23-P1.30

CANF rxF;                              // CAN frame variable used to store received CAN data

volatile u8 indicator = OFF;           // Stores current indicator state
                                       // Initially indicator is OFF

volatile u8 pos = 0;                   // Stores current LED position
                                       // Initially position is 0

/* Prototypes */
void blink_left_step(void);            // Function prototype for left indicator LED movement
void blink_right_step(void);           // Function prototype for right indicator LED movement


int main(void)                        // Program execution starts from main()
{
    Init_CAN1();                       // Initializes CAN1 peripheral

    IODIR1 |= LED_MASK;                // Configures P1.23-P1.30 as output pins

    IOSET1 = LED_MASK;                 // Sets all LED pins HIGH
                                       // LEDs are OFF because they are active LOW

    while (1)                          // Infinite loop
    {
        /* -------- CAN RX -------- */

        if (C1GSR & (1 << 0))          // Checks Receive Buffer Status (RBS) bit
                                       // If bit 0 is set, CAN data is available
        {
            CAN1_Rx(&rxF);             // Receives CAN frame and stores it in rxF

            C1CMR = (1 << 2);          // Releases/clears the CAN receive buffer

            if (rxF.ID == 0x11)         // Checks whether received CAN ID is 0x11
            {
                if (rxF.Data1 == 'L')  // Checks whether received command is 'L'
                {
                    indicator = LEFT;  // Selects LEFT indicator mode
                    pos = 0;            // Starts LED position from 0
                }

                else if (rxF.Data1 == 'R') // Checks whether received command is 'R'
                {
                    indicator = RIGHT; // Selects RIGHT indicator mode
                    pos = 7;            // Starts LED position from 7
                }

                else if (rxF.Data1 == 'O') // Checks whether received command is 'O'
                {
                    indicator = OFF;   // Turns indicator OFF
                    pos = 0;            // Resets LED position

                    IOSET1 = LED_MASK; // Forces all LEDs OFF
                }
            }
        }


        /* -------- LED ACTION -------- */

        if (indicator == LEFT)          // Checks whether LEFT indicator is selected
        {
            blink_left_step();         // Performs one step of left LED movement
        }

        else if (indicator == RIGHT)   // Checks whether RIGHT indicator is selected
        {
            blink_right_step();        // Performs one step of right LED movement
        }

        else                            // Executes when indicator is OFF
        {
            IOSET1 = LED_MASK;          // Turns all LEDs OFF
        }

        delay_ms(50);                   // Waits 50 ms before performing the next step
    }
}


/* -------- LEFT SHIFT -------- */

void blink_right_step(void)             // Function that moves LED in one direction
{
    IOSET1 = LED_MASK;                  // Turns all LEDs OFF first

    IOCLR1 = (1 << (LED_START + pos));
                                        // Turns ON the LED at current position
                                        // Since LEDs are active LOW, clearing the bit turns LED ON

    pos++;                              // Moves to the next LED position

    if (pos >= 8)                      // Checks whether all 8 LEDs have been completed
        pos = 0;                       // Restarts from the first LED
}


/* -------- RIGHT SHIFT -------- */

void blink_left_step(void)              // Function that moves LED in the opposite direction
{
    IOSET1 = LED_MASK;                  // Turns all LEDs OFF first

    if (pos == 0)                      // Checks whether position has reached first LED
        pos = 7;                       // Wraps around to the last LED

    else
        pos--;                          // Moves to the previous LED

    IOCLR1 = (1 << (LED_START + pos));
                                        // Turns ON the LED at the current position
}