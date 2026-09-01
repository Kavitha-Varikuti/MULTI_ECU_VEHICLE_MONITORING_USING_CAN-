#include <lpc21xx.h>          // Includes LPC21xx microcontroller register definitions

#include "types.h"             // Includes user-defined data types such as u32 and f32
#include "delay.h"             // Includes delay_us() function


// ADC clock and processor clock definitions

#define FOSC 12000000          // Crystal oscillator frequency = 12 MHz

#define CCLK (FOSC*5)          // CPU clock = 12 MHz × 5 = 60 MHz

#define PCLK (CCLK/4)          // Peripheral clock = 60 MHz / 4 = 15 MHz

#define ADCCLK 3000000         // Required ADC clock = 3 MHz

#define CLKDIV ((PCLK/ADCCLK)-1) // Calculates ADC clock divider
                                // (15 MHz / 3 MHz) - 1 = 4


// Bit positions in the ADCR (ADC Control Register)

#define CLKDIV_BITS 8          // ADC clock divider starts from bit 8

#define PDN_BIT 21             // Power-down bit of ADC is bit 21

#define ADC_CONV_START_BIT 24  // Conversion START bit is bit 24


// Bit definitions related to ADDR (ADC Data Register)

#define DIGITAL_DATA_BITS 6    // 10-bit ADC result starts from bit 6

#define DONE_BIT 31            // ADC conversion DONE flag is bit 31


// PINSEL1 bit patterns for selecting ADC pins

#define AIN0_PIN_0_27 0x00400000 // Selects P0.27 as ADC channel 0

#define AIN1_PIN_0_28 0x01000000 // Selects P0.28 as ADC channel 1

#define AIN2_PIN_0_29 0x04000000 // Selects P0.29 as ADC channel 2

#define AIN3_PIN_0_30 0x10000000 // Selects P0.30 as ADC channel 3


// ADC channel numbers

#define CH0 0                   // ADC Channel 0

#define CH1 1                   // ADC Channel 1

#define CH2 2                   // ADC Channel 2

#define CH3 3                   // ADC Channel 3


// Function to configure a particular pin for a required function

void cfgportpinfunc(u32 portNo, u32 pinNo, u32 pinFunc)
{
    if(portNo==0)               // Check whether Port 0 is selected
    {
        if(pinNo<=15)           // Check if pin number is between P0.0 and P0.15
        {
            PINSEL0=((PINSEL0&~(3<<(pinNo*2)))|
                     (pinFunc<<(pinNo*2)));
                                // Clear the existing function bits for the pin
                                // and then select the required pin function
        }
        else if((pinNo>=16)&&(pinNo<=31))
                                // Check if pin number is between P0.16 and P0.31
        {
            PINSEL1=((PINSEL1&~(3<<((pinNo-16)*2)))|
                     (pinFunc<<((pinNo-16)*2)));
                                // Clear the existing function bits in PINSEL1
                                // and configure the required pin function
        }
    }
}


// Array containing PINSEL configurations for ADC channels 0 to 3

u32 adcChSel[4]={AIN0_PIN_0_27,
                 AIN1_PIN_0_28,
                 AIN2_PIN_0_29,
                 AIN3_PIN_0_30
                };
// adcChSel[0] → configuration for ADC channel 0
// adcChSel[1] → configuration for ADC channel 1
// adcChSel[2] → configuration for ADC channel 2
// adcChSel[3] → configuration for ADC channel 3


// Function to initialize the ADC

void initadc(u32 chNo)
{
    PINSEL1&=~(adcChSel[chNo]);
                                // Clears the selected ADC pin configuration

    PINSEL1|=adcChSel[chNo];
                                // Configures the selected pin for ADC function

    ADCR|=(1<<PDN_BIT)|(CLKDIV<<CLKDIV_BITS);
                                // Powers ON the ADC using PDN bit
                                // and sets the ADC clock divider
}


// Function to perform ADC conversion

void Read_ADC(u32 chNo,f32 *eAR,u32 *adcDVal)
{
    ADCR&=0xFFFFFF00;           // Clears ADC channel-selection and related lower bits

    ADCR|=((1<<ADC_CONV_START_BIT)|(1<<chNo));
                                // Sets START bit to begin conversion
                                // Selects the required ADC channel

    delay_us(3);                // Waits for a short time for ADC conversion

    while(((ADDR>>DONE_BIT)&1)==0);
                                // Continuously checks DONE bit
                                // Waits until ADC conversion is completed

    ADCR&=~(1<<ADC_CONV_START_BIT);
                                // Clears START bit after conversion is complete

    *adcDVal=((ADDR>>DIGITAL_DATA_BITS)&1023);
                                // Extracts the 10-bit ADC digital value
                                // 1023 = 0x3FF = binary 10 ones

    *eAR=*adcDVal * (3.3/1023);
                                // Converts ADC digital value into voltage
                                // ADC reference voltage = 3.3 V
}


// Function to read temperature from LM35 sensor

f32 read_lm35()
{
    u32 adcDVal;                // Stores the 10-bit ADC digital value

    f32 eAR,tDeg;               // eAR = analog voltage
                                // tDeg = temperature in degree Celsius

    Read_ADC(CH1,&eAR,&adcDVal);
                                // Reads ADC Channel 1
                                // Gets voltage and digital ADC value

    tDeg=eAR * 100;             // LM35 gives 10 mV/°C
                                // Therefore temperature = voltage × 100

    return tDeg;                // Returns temperature in °C
}


// Function to read differential temperature using two ADC channels

f32 Read_LM35_NP()
{
    u32 adcDVal1,adcDVal2;      // Stores ADC values from two channels

    f32 eAR1,eAR2,tDeg;          // Stores voltages and calculated temperature

    Read_ADC(CH0,&eAR1,&adcDVal1);
                                // Reads voltage from ADC Channel 0

    Read_ADC(CH1,&eAR2,&adcDVal2);
                                // Reads voltage from ADC Channel 1

    tDeg=(eAR1-eAR2) * 100;      // Calculates voltage difference
                                // and converts it into temperature

    return tDeg;                // Returns calculated temperature
}