#include <LPC21xx.h>          // Includes LPC21xx microcontroller register definitions
#include "adc.h"              // Includes ADC initialization and ADC reading functions
#include "delay.h"            // Includes delay functions such as delay_ms()
#include "types.h"            // Includes user-defined data types like u32 and f32
#include "can.h"              // Includes CAN communication functions and CANF structure
#include "lcd.h"              // Includes LCD functions such as initlcd() and cmdlcd()

CANF txF;                     // Creates a CAN frame variable for transmitting data

f32 eAR;                      // Stores the analog equivalent voltage obtained from ADC

u32 adcDVal;                  // Stores the digital ADC value

int main(void)                // Program execution starts from main()
{
    u32 volt_mv;              // Variable to store calculated voltage value

    initlcd();                // Initializes the LCD

    Init_CAN1();              // Initializes CAN1 communication

    initadc(1);               // Initializes ADC channel 1

    txF.ID = 0x301;           // Sets CAN message identifier to 0x301

    txF.BFV.RTR = 0;          // Sets RTR bit to 0, meaning this is a data frame

    txF.BFV.DLC = 4;          // Sets CAN Data Length Code to 4 bytes

    while(1)                  // Infinite loop; continuously reads and transmits ADC data
    {
        cmdlcd(0x01);         // Clears the LCD display

        Read_ADC(1, &eAR, &adcDVal);
                              // Reads ADC channel 1
                              // eAR receives analog voltage
                              // adcDVal receives digital ADC value

        volt_mv = (eAR * 100);
                              // Converts the ADC voltage into a value
                              // Here the result is stored in volt_mv

        txF.Data1 = volt_mv;  // Stores the calculated value in CAN Data1

        txF.Data2 = 0;       // Stores 0 in CAN Data2

        CAN1_Tx(txF);        // Transmits the CAN frame through CAN1

        delay_ms(500);       // Waits for 500 milliseconds before the next transmission
    }
}