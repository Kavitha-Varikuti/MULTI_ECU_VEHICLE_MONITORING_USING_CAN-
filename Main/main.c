#include <LPC21xx.h>
#include "delay.h"
#include "can.h"
#include "types.h"
#include "lcd.h"
#include "pin_connect_block.h"
#include "ds18b20.h"
#include "battery_disp.h"
void temp_symbol()
{
u8 i;
u8 tem[8] = {0x0E,0x0A,0x0A,0x1f,0x1f,0x1f,0x1f,0x1F};
cmdlcd(0x78);
for(i=0;i<8;i++)
{
charlcd(tem[i]);
}
}
/* -------- Defines -------- */
#define OFF 0
#define LEFT 1
#define RIGHT 2
u32 convert(u32 x)
{
if(x<1)
{
x=0;
}
if(x>330)
{
x=330;
}
return (x*100)/330;
}
volatile u32 percent=0;
f32 voltage;
u8 battery_percent,i;
u32 level=0;
/* -------- Globals -------- */
volatile u8 indicator_status = OFF;
CANF txF;
CANF rxF;
/* -------- Custom LCD chars -------- */
u8 left_arrow[8] = {0x03,0x07,0x0F,0x1F,0x0F,0x07,0x03,0x01};
u8 right_arrow[8] = {0x18,0x1C,0x1E,0x1F,0x1E,0x1C,0x18,0x10};
/* -------- Prototypes -------- */
void LoadCustomChars(void);
void sendsignal(void);
void eint0_isr(void) __irq;
void eint1_isr(void) __irq;
int temp;
u8 tp, tpd;
int main(void)
{
Init_CAN1();
initlcd();
cmdlcd(0x80);
strlcd(" <<< DASH BOARD >>> ");
temp_symbol();
cmdlcd(0xC0);
strlcd("Indicator ");
/* Load custom arrows */
LoadCustomChars();
StoreCustCharFont(0);
StoreCustCharFont1(25);
StoreCustCharFont2(50);
StoreCustCharFont3(75);
StoreCustCharFont4(100);
/* Show steady <> */
cmdlcd(0xC0 + 13);
charlcd(5);
cmdlcd(0xC0 + 15);
charlcd(6);
/* -------- EINT config -------- */
cfgportpinfunc(0,1,3); // EINT0
cfgportpinfunc(0,3,3); // EINT1
EXTMODE |= (1<<0) | (1<<1);
EXTPOLAR &= ~((1<<0) | (1<<1));
EXTINT = (1<<0) | (1<<1);
VICVectCntl0 = (1<<5) | 14;
VICVectAddr0 = (u32)eint0_isr;
VICVectCntl1 = (1<<5) | 15;
VICVectAddr1 = (u32)eint1_isr;
VICIntEnable = (1<<14) | (1<<15);
while(1)
{
/* LEFT indicator blink */
if(indicator_status == LEFT)
{
cmdlcd(0xC0 + 13);
charlcd(5);
delay_ms(300);
cmdlcd(0xC0 + 13);
charlcd(' ');
cmdlcd(0xC0 + 15);
charlcd(6);
}
/* RIGHT indicator blink */
else if(indicator_status == RIGHT)
{
cmdlcd(0xC0 + 13);
charlcd(5);
cmdlcd(0xC0 + 15);
charlcd(6);
delay_ms(300);
cmdlcd(0xC0 + 15);
charlcd(' ');
}
/* OFF state */
else
{
cmdlcd(0xC0 + 13);
charlcd(5);
cmdlcd(0xC0 + 15);
charlcd(6);
}
delay_ms(100);
temp = ReadTemp();
tp = temp >> 4;
tpd = (temp & 0x08) ? '5' : '0';
cmdlcd(0xD4);
strlcd("Temp ");
charlcd(7);
strlcd(" : ");
u32lcd(tp);
charlcd('.');
charlcd(tpd);
strlcd(" C ");
if (C1GSR & (1 << 0))
{
/* Battery Node */
CAN1_Rx(&rxF);
/* Battery message */
if(rxF.ID)
{
percent = convert(rxF.Data1);
cmdlcd(0x94);
strlcd("Battery: ");
u32lcd(percent);
strlcd("% ");
if(percent <= 0)
{
level=0;
}
else if(percent <= 25)
{
level=1;
}
else if(percent <= 50)
{
level=2;
}
else if(percent <= 75)
{
level=3;
}
else
{
level=4;
}
cmdlcd(0x94+13);
charlcd(level);
}
}
delay_ms(200);
}
}
/* -------- EINT0 : LEFT -------- */
void eint0_isr(void) __irq
{
indicator_status = (indicator_status == LEFT) ? OFF : LEFT;
sendsignal();
EXTINT = (1<<0);
VICVectAddr = 0;
}
/* -------- EINT1 : RIGHT -------- */
void eint1_isr(void) __irq
{
indicator_status = (indicator_status == RIGHT) ? OFF : RIGHT;
sendsignal();
EXTINT = (1<<1);
VICVectAddr = 0;
}
/* -------- CAN TRANSMIT -------- */
void sendsignal(void)
{
txF.ID = 0x11;
txF.BFV.RTR = 0;
txF.BFV.DLC = 1;
txF.Data1 = (indicator_status == LEFT) ? 'L' :
(indicator_status == RIGHT) ? 'R' : 'O';
CAN1_Tx(txF);
}
/* -------- Load CGRAM arrows -------- */
void LoadCustomChars(void)
{
u8 i;
cmdlcd(0x68); // char 0 = <
for(i=0;i<8;i++)
charlcd(left_arrow[i]);
cmdlcd(0x70); // char 1 = >
for(i=0;i<8;i++)
charlcd(right_arrow[i]);
}

