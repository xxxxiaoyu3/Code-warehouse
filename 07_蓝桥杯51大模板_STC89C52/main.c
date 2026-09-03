#include <STC15F2K60S2.h>
#include <Stdio.h>
#include <Intrins.h>
#include "Nixie.h"
#include "KeyGet.h"
#include "Init.h"
	
//数码管缓存区
uchar Nixie_Num[]={0,12,11,10,10,10,10,5,10};
//用户变量区

//标志位缓存区
uchar Key_Slow_Down=0;

////按键检测函数
//void Key_get(void)
//{
//	static uchar flag=0;
//	if(Key == 0 && flag == 0)
//	{
//		flag=1;
//		Count++;Count%=10000;
//	}else if(Key)
//	{
//		flag=0;
//	}
//}

//}
void Timer1_Init(void)
{
	TMOD &= 0X0F;
	AUXR &=0X7F;
	TH1=(65535-1000)/256;
	TL1=(65535-1000)%256;
	TF1=1;
	TR1=1;
	ET1=1;
	EA=1;
	PT1=0;
}

uchar Key_ser(void)	//按键处理函数
{
	static uchar KeyNum_Pi=5;
	static uchar KeyNum=0,Key_Down,Key_Up,Key_Old;
	if(Key_Slow_Down)			//10us扫描一次
		return 0;
	Key_Slow_Down=1;
	KeyNum=KeyGet();
	
	Key_Down=KeyNum &(KeyNum^Key_Old);		//辅助扫描变量
	Key_Up=KeyNum &(KeyNum^Key_Old);		//下降沿检测扫描变量
	Key_Old = KeyNum;			//辅助扫描变量
	
	//键值处理区域
	switch(Key_Down)
	{
		case 8:KeyNum_Pi++;KeyNum_Pi%=10;break;
		case 9:KeyNum_Pi--;KeyNum_Pi%=10;break;
	}
	
	Nixie_Num[7]=KeyNum_Pi;
	//Nixie_Num[8]=KeyNum_Pi;
	//***************
	return 0;
}
/*数码管处理函数*/
void Nixie_dis(void)		//数码管刷新显示函数
{
	uchar i=0;
	uchar point=0;
	for(i=0;i<9;++i)
	{
		if(i==2)			//要加小数点的位置
			point =1;
		else
			point =0;
		
		Nixie_diplay(Nixie_Num[i],i,point);
	}
}

void Timer1_Rountine(void)	interrupt 3
{
	//static uint i=0;
	TH1=(65535-1000)/256;
	TL1=(65535-1000)%256;
	if(++Key_Slow_Down == 10)
		Key_Slow_Down=0;
	
}
void main(void)
{
	System_Init();
	Timer1_Init();
	while(1)
	{
		Key_ser();
		Nixie_dis();
	}
}