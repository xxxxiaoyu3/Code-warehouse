#include <STC15F2K60S2.h>
#include "KeyGet.h"
/*
	LED[]={0,1,2,3,4,5,6,7};
	LED[]={0,0,0,0,0,0,0,0};
*/
unsigned char Temp1=0X00;
unsigned char Temp1_Old=0XFF;
void LED_dis(unsigned char *LED)		//—°‘ÒLxLEDµ∆¡¡
{
	unsigned char i=0;
	unsigned char Temp=0X00;
	static unsigned char Temp_Old=0XFF;
	
	for(i=0;i<8;i++)
	{
		Temp |= (LED[i]<<i);
	}
	if(Temp != Temp_Old)
	{
		P0=~Temp;
		P2=P2&0X1F | 0X80;
		P2=P2&0X1F;
		Temp=Temp_Old;
	}
}
void Beep(bit Flag)
{
	if(Flag)
		Temp1 |=0X40;
	else
		Temp1 &=~0X40;
	if(Temp1!=Temp1_Old)
	{
		P0=Temp1;
		P2=P2&0X1F |0XA0;
		P2=P2&0X1F;
		Temp1=Temp1_Old;
	}
}

void Relay(bit Flag)
{
	if(Flag)
		Temp1 |=0X10;
	else
		Temp1 &=~0X10;
	if(Temp1!=Temp1_Old)
	{
		P0=Temp1;
		P2=P2&0X1F |0XA0;
		P2=P2&0X1F;
		Temp1=Temp1_Old;
	}
}