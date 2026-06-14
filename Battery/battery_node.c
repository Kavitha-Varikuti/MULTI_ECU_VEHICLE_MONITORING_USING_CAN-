#include <LPC21xx.h>
#include "adc.h"
#include "delay.h"
#include "types.h"
#include "can.h"
#include "lcd.h"
CANF txF;
f32 eAR;
u32 adcDVal;
int main(void)
{
u32 volt_mv;
initlcd();
Init_CAN1();
initadc(1);
txF.ID = 0x301;
txF.BFV.RTR = 0;
txF.BFV.DLC = 4;
while(1)
{
cmdlcd(0x01);
Read_ADC(1, &eAR, &adcDVal);
volt_mv = (eAR * 100);
txF.Data1 = volt_mv;
txF.Data2 = 0;
CAN1_Tx(txF);
delay_ms(500);
}
}

