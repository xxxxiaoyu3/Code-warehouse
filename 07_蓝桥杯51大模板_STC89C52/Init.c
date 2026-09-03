#include <STC15F2K60S2.h>

void System_Init(void)
{
	P0=0X00;				
	P2= P2&0X1F |0XA0;		//上电默认关闭蜂鸣器和继电器
	P2&=0X1F;
	
	P0=0XFF;
	P2= P2&0X1F |0X80;		//上电默认关闭LED灯
	P2 &=0X1F;
}