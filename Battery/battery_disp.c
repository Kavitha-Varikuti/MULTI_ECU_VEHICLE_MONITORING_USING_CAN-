#include <LPC21xx.h>
#include "lcd.h"
void StoreCustCharFont(unsigned char p)
{
unsigned char i;
unsigned char batt0[8] =
{
0x0E,
0x11,
0x11,
0x11,
0x11,
0x11,
0x11,
0x1F
};
cmdlcd(0x40);
for(i=0;i<8;i++)
charlcd(batt0[i]);
}
void StoreCustCharFont1(unsigned char p)
{
unsigned char i;
unsigned char batt25[8] =
{
0x0E,
0x11,
0x11,
0x11,
0x11,
0x1F,
0x1F,
0x1F
};
cmdlcd(0x48);
for(i=0;i<8;i++)
charlcd(batt25[i]);
}
void StoreCustCharFont2(unsigned char p)
{
unsigned char i;
unsigned char batt50[8] =
{
0x0E,
0x11,
0x11,
0x1F,
0x1F,
0x1F,
0x1F,
0x1F
};
cmdlcd(0x50);
for(i=0;i<8;i++)
charlcd(batt50[i]);
}
void StoreCustCharFont3(unsigned char p)
{
unsigned char i;
unsigned char batt75[8] =
{
0x0E,
0x11,
0x1F,
0x1F,
0x1F,
0x1F,
0x1F,
0x1F
};
cmdlcd(0x58);
for(i=0;i<8;i++)
charlcd(batt75[i]);
}
void StoreCustCharFont4(unsigned char p)
{
unsigned char i;
unsigned char batt100[8] =
{
0x0E,
0x1F,
0x1F,
0x1F,
0x1F,
0x1F,
0x1F,
0x1F
};
cmdlcd(0x60);
for(i=0;i<8;i++)
charlcd(batt100[i]);
}

