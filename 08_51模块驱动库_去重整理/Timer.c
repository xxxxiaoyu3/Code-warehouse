#include <STC15F2K60S2.h>
#include "Timer.h"

void Timer0_Init(unsigned int XUS,bit state)
{
	TMOD &= 0xF0;
	TH0=(65535-XUS)/256;
	TL0=(65535-XUS)%256;
	TF0=0;
	TR0=state;
	ET0=1;
	EA=1;
}
void Timer1_Init(unsigned int XUS,bit state)
{
	TMOD &= 0x0F;
	TH1=(65535-XUS)/256;
	TL1=(65535-XUS)%256;
	TF1=0;
	TR1=state;
	ET1=1;
	EA=1;
}
/*
void Timer3_Init(unsigned int XUS)
{
	T4T3M &= 0XF0;		//12T,系统时钟12分频
	//T4T3M |= 0X08;	//不开启定时器	
	T3H=(65535-XUS)/256;
	T3L=(65535-XUS)%256;
	IE2|=0X20;		//开启中断ET3
	EA=1;
}

void Timer4_Init(unsigned int XUS)
{
	T4T3M &= 0X0F;		
	T4T3M |= 0XA0;	//1T,系统时钟不分频，65535最大计算0.00546秒
	T4H=(65535-XUS)/256;
	T4L=(65535-XUS)%256;		//0.00546秒中断1次
	IE2|=0X40;		//开启中断ET4
	EA=1;
}
*/
/*
void Timer0_Isr(void) interrupt 1
{
	static unsigned int i=0;
	TH0=(65535-???)/256;
	TL0=(65535-???)%256;
	i++;
	if(i>???)
	{
		i=0;
	}
}
*/
/*--------------Timer1--------------------*/
/*
void Timer1_Isr(void) interrupt 3
{
	static unsigned int i=0,j=1;
	TH1=(65535-1000)/256;
	TL1=(65535-1000)%256;
	i++;
	if(i>1000)
	{
		i=0;
		LED_SetNum(j++,1);j%=9;
	}
}
*/
/*--------------Timer3--------------------*/
/*
void Timer3_Routine(void)	interrupt 19
{
	static uint i=0;
	T3H=(65535-???)/256;
	T3L=(65535-???)%256;
	i++;
	if(i>???)
	{
		i=0;
	}
}
*/

/*--------------Timer4--------------------*/
/*
void Timer4_Routine(void)	interrupt 20
{
	static uint i=0;
	T4H=(65535-???)/256;
	T4L=(65535-???)%256;
	i++;
	if(i>???)		
	{
		i=0;
	}	
}
*/