#include <STC15F2K60S2.h>
#include "Nixie_tube.h"
#include "Delay.h"
xdata unsigned char NixieTable[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
/*--------数码管初始化-----------*/
void Nixie_tube_Init(void)
{
	/*com1-com8*/
	P0 = 0X00;					//共阴级
	P2 = P2&0X1F | 0XC0;		//Y6
	P2 &= 0X1F;					//清零锁存P2_5.6.7
	/*a-dr*/
	P0 = 0X00;					//共阴级
	P2 = P2&0X1F | 0XE0;		//Y7
	P2 &= 0X1F;					//清零锁存P2_5.6.7
}
/*--------数码管位选显示-----------*/
void Nixie_tube_show(unsigned char Com,unsigned char Data)
{
	//控制数码管输入
	P0=NixieTable[Data];
	Delay(3);		//消除影子
	P2 = P2 & 0X1F|0XE0;		//P2_5_6_7清零,同时选择Y7
	P2 &= 0X1F;		//P2_5_6_7清零，防止数据错乱，以及锁住寄存器
	//控制com
	P0=Com;		
	P2 = P2 & 0X1F|0XC0;		//P2_5_6_7清零,同时选择Y6
	P2 &= 0X1F;		//P2_5_6_7清零，防止数据错乱，以及锁住寄存器
}
/*--------动态数码管显示-----------*/
void Nixie_tube_dynamic_show(unsigned char *Data)
{
	int i=0;
	for(i=0;i<=7;i++)
	{
		Nixie_tube_show(0X01<<i,Data[i]);
	}
}

void Time_segment(unsigned char Count,unsigned char *num)
{
	unsigned char i;
	num[0]=Count/1000;		//个
	num[1]=Count/100%10;	//十
	num[2]=Count/10%10;		//百
	num[3]=Count%10;		//千
	for(i=0;i<=7;i++)		//com1~8
	{
		Nixie_tube_show(0X01<<i,NixieTable[num[i]]);
	}
}