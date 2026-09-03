#include <STC15F2K60S2.h>
#include "UART1.h"
#include "Delay.h"
/**
  * @brief  串口9600bps@11.0592MHz
  * @param 
  * @retval 
  */
void Uart1_Init(void)	//9600bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器时钟12T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xE8;			//设置定时初始值
	TH1 = 0xFF;			//设置定时初始值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	ES = 1;				//使能串口1中断
	EA=1;
}

void Uart1_SendByte(unsigned char Byte)//发送数据
{
		SBUF=Byte;//直接写入SBUF
		while(TI);//检测是否发送数据，T1为1时发送数据		//发送HEX数据就给TI发送文本就给!TI
		TI=0;//发送后需要手动置0
}

void Uart1_SendString(unsigned char *str)
{
	while(*str!='\0')		//数组内只要没到空字符就一直发
	{
		Uart1_SendByte(*str);
		str++;				//地址+1，发送下一个字符
	}
}

/*

void Uart1_Isr(void) interrupt 4
{
	if (TI)				//检测串口1发送中断
	{
		TI = 0;			//清除串口1发送中断请求位
	}
	if (RI)				//检测串口1接收中断
	{
		RI = 0;			//清除串口1接收中断请求位
	}
}

*/