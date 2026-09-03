#include <STC15F2K60S2.h>

/**
  * @brief  定时器0初始化，Run用于控制定时器开关
  * @param  无
  * @retval 无
  */
void Timer0Init(void)
{
	TMOD &=0X10;//后8位清零	
	TMOD |=0X01;//后八位设置0001;
	TH0=0XFC;
	TL0=0X16;
	TF0=0;//初始化
	TR0=1;//定时器开启
	ET0=1;//中断打开
	EA=1;//中断总闸打开
	PT0=0;//优先级为0
}

void Timer0_Run(unsigned char Y)//1开启定时器计时，0关闭
{
	TR0 =Y;
}
/*
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值	每1000微妙响应一次
	TH0 = 0xFC;		//设置定时初值	
	T0Count++;
	if(T0Count>=1000)		//1百万微妙==1秒,速度可在这里改
	{
		T0Count=0;
		
	}
}
*/
