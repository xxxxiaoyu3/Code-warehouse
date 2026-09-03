#include <STC15F2K60S2.h>
#include <intrins.h>
#include "ultrasonic.h"

void Ultra_Init(void)				//定时器1
{
	TMOD &= 0X0F;		//16位自动重载计数值最大65535
//	AUXR &= 0XBF;		//12T
	TH1=0;
	TL1=0;
	TF1 = 0;		//标志清零
	TR1 = 0;		//不开启定时器
}

void Ultra_start(bit flag)
{
	TR1 = flag;
}

void Ultra_Delay(void)		//12us
{
	unsigned char data i;
	_nop_();
	_nop_();
	_nop_();
	i = 30;
	while (--i);
}

void Send_wave(uchar Num)			//发送超声波
{
	uchar i =0;
	for(i=0;i<Num;i++)
	{
		TX_Ultra=1;
		Ultra_Delay();
		TX_Ultra=0;
		Ultra_Delay();
	}
}

uint Ultra_distance(void)			//超声波测距
{
	uint Time =0,distance=0;	//distance:距离,s=声速
	Ultra_Init();
	Send_wave(8);				//发送超声波
	Ultra_start(1);				//开始计数
	while(TF1==0 && RX_Ultra==1);		//等待返回
	Ultra_start(0);				//关闭计数
	if(TF1 == 0)					//数据正常
	{
		Time =(TH1<<8)|TL1;
		distance=(Time*0.017);				//距离等于高电平时间次数*声速/2u*t/2
		return distance;
	}else if(TF1 == 1)			//数据溢出
	{
		TF1=0;
		return 0;
	}
	return 0;
}

/*
void Int1_Routine(void) interrupt 2
{
	static uint Tcont=0;
	TH1=(65535-1000)/256;
	TL1=(65535-1000)%256;
}
*/

