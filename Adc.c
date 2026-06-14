#include <lpc21xx.h>
#include "types.h"
#include "delay.h"
//adc_defines.h
#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define ADCCLK 3000000
#define CLKDIV ((PCLK/ADCCLK)-1)
//defines for ADCR sfr
#define CLKDIV_BITS 8 //@8-15
#define PDN_BIT 21
#define ADC_CONV_START_BIT 24
//defines for ADDR sfr
#define DIGITAL_DATA_BITS 6//@6-15
#define DONE_BIT 31
#define AIN0_PIN_0_27 0x00400000
#define AIN1_PIN_0_28 0x01000000
#define AIN2_PIN_0_29 0x04000000
#define AIN3_PIN_0_30 0x10000000
#define CH0 0
#define CH1 1
#define CH2 2
#define CH3 3
void cfgportpinfunc(u32 portNo, u32 pinNo, u32 pinFunc)
{
if(portNo==0)
{
if(pinNo<=15)
{
PINSEL0=((PINSEL0&~(3<<(pinNo*2)))|
(pinFunc<<(pinNo*2)));
}
else if((pinNo>=16)&&(pinNo<=31))
{
PINSEL1=((PINSEL1&~(3<<((pinNo-16)*2)))|
(pinFunc<<((pinNo-16)*2)));
}
}
}
u32 adcChSel[4]={AIN0_PIN_0_27,
AIN1_PIN_0_28,
AIN2_PIN_0_29,
AIN3_PIN_0_30
};
void initadc(u32 chNo)
{
PINSEL1&=~(adcChSel[chNo]);
PINSEL1|=adcChSel[chNo];
ADCR|=(1<<PDN_BIT)|(CLKDIV<<CLKDIV_BITS);
}
void Read_ADC(u32 chNo,f32 *eAR,u32 *adcDVal)
{
ADCR&=0xFFFFFF00;
ADCR|=((1<<ADC_CONV_START_BIT)|(1<<chNo));
delay_us(3);
while(((ADDR>>DONE_BIT)&1)==0);
ADCR&=~(1<<ADC_CONV_START_BIT);
*adcDVal=((ADDR>>DIGITAL_DATA_BITS)&1023);
*eAR=*adcDVal * (3.3/1023);
}
f32 read_lm35()
{
u32 adcDVal;
f32 eAR,tDeg;
Read_ADC(CH1,&eAR,&adcDVal);
tDeg=eAR * 100;
return tDeg;
}
f32 Read_LM35_NP()
{
u32 adcDVal1,adcDVal2;
f32 eAR1,eAR2,tDeg;
Read_ADC(CH0,&eAR1,&adcDVal1);
Read_ADC(CH1,&eAR2,&adcDVal2);
tDeg=(eAR1-eAR2) * 100;
return tDeg;
}

