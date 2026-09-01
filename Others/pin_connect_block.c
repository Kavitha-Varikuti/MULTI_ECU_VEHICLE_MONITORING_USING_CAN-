#include "types.h"                         // Includes user-defined data types such as u32

#include <LPC21xx.h>                      // Includes LPC21xx microcontroller register definitions


void cfgportpinfunc(u32 PortNo,           // Port number: 0 or 1
                    u32 PinNo,            // Pin number: 0 to 31
                    u32 PinFunc)          // Function number to be selected for the pin
{
    if(PortNo==0)                         // Checks whether Port 0 is selected
    {
        if(PinNo<16)                      // Checks whether pin number is from P0.0 to P0.15
        {
            PINSEL0 = ((PINSEL0 &~       // Reads PINSEL0 and clears the existing function bits
                      (3<<(PinNo*2)))    // Each pin uses 2 bits in PINSEL0
                      |                    // OR operation combines the cleared register with new function
                      (PinFunc<<(PinNo*2)));
                                           // Places the required pin function in the correct 2-bit position
        }


        else if(PinNo>=16 && PinNo<=31)   // Checks whether pin is from P0.16 to P0.31
        {
            PINSEL1 = ((PINSEL1 &~        // Reads PINSEL1 and clears the existing function bits
                      (3<<((PinNo-16)*2))) // Calculates the 2-bit position for P0.16-P0.31
                      |                     // OR operation inserts the new function
                      (PinFunc<<((PinNo-16)*2)));
                                           // Places the required function in the correct PINSEL1 position
        }

    }


    else if(PortNo==1)                    // Checks whether Port 1 is selected
    {

        //not required for LPC2129             // Port 1 pin function configuration is not required here

    }

}