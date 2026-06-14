#include<LPC21xx.h>
#include "delay.h"
#include "lcd.h"
#include "defines.h"
#define LCD_DAT 8
#define RS 17
#define RW 18
#define EN 19
void writelcd(char ch)
{
WRITEBIT(IOCLR0,RW,1);
WRITEBYTE(IOPIN0,LCD_DAT,ch);
WRITEBIT(IOSET0,EN,1);
WRITEBIT(IOCLR0,EN,1);
delay_ms(2);
}
void cmdlcd(char cmd)
{
WRITEBIT(IOCLR0,RS,1);
writelcd(cmd);}
void initlcd(void)
{
WRITEBYTE(IODIR0,LCD_DAT,0xFF);
WRITEBIT(IODIR0,RS,1);
WRITEBIT(IODIR0,RW,1);
WRITEBIT(IODIR0,EN,1);
delay_ms(15);
cmdlcd(0x30);
delay_ms(5);
cmdlcd(0x30);
delay_us(100);
cmdlcd(0x30);
//delay_ms(1);
cmdlcd(0x38); // 2 line, 5x7
cmdlcd(0x0C); // Display ON, cursor OFF, blink OFF
cmdlcd(0x06); // Cursor increment, NO shift
cmdlcd(0x01); // Clear display
}
void charlcd(char dat)
{
WRITEBIT(IOSET0,RS,1);
writelcd(dat);
}
void strlcd(char *p)
{
while(*p)
charlcd(*p++);
}
void u32lcd(signed int n)
{
char a[10],i=0;
if(n<0)
{
charlcd('-');
n=-n;
}
do
{
a[i++]=n%10+48;
n=n/10;
}while(n);
for(;i>0;i--)
charlcd(a[i-1]);
}
void f32lcd(float f,char i)
//1st arg(f) is the float number to be displayed on LCD
//2nd arg(i) is number digits to be displayed after the dot('.')
{
unsigned long int n=f;
u32lcd(n);
charlcd('.');
for(;i>0;i--)
{
f=f*10;
n=f;
charlcd(n%10+48);
}
}

