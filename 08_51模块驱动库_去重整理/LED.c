#include <STC15F2K60S2.h>
#include "LED.h"
/*
	LED初始化
*/
void LED_Init(void)
{
	//让LED不点亮		Y4
	P0=0XFF;		//P0口给高电平
	P2 = P2 & 0X1F|0X80;		//P2_5_6_7清零,同时选择Y4
	P2 &= 0X1F;		//P2_5_6_7清零，防止数据错乱，以及锁住寄存器
	//让蜂鸣器继电器不工作
	P0=0X00;		//Y5
	P2 = P2 & 0X1F|0XA0;		//P2_5_6_7清零,同时选择Y4
	P2 &= 0X1F;		//P2_5_6_7清零，防止数据错乱，以及锁住寄存器*/
}
/*
	设置LED全部的状态
*/
void LED_Set(unsigned char LED)
{
	P0=~LED;		//P0口LED高电平不显示,低电平显示所以取反
	P2 = P2 & 0X1F|0X80;		//P2_5_6_7清零,同时选择Y4
	P2 &= 0X1F;		//P2_5_6_7清零，防止数据错乱，以及锁住寄存器
}
/*
	设置选择的LED的状态
	Num:选择LED的位置，1-8
	state:状态，0或1
*/
void LED_SetNum(unsigned char Num,bit state)
{
	Num-=1;				//为了对上数字标号L1,L2...
	if(state)
	{
		P0 = ~(0X01<<Num);
	}else
	{
		P0 = (0X01<<Num);
	}
	P2 = P2 & 0X1F|0X80;		//P2_5_6_7清零,同时选择Y4
	P2 &= 0X1F;		//P2_5_6_7清零，防止数据错乱，以及锁住寄存器
}