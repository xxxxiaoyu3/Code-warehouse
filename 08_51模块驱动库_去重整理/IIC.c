#include <STC15F2K60S2.h>
#include <Intrins.h>
#include "IIC.h"
#include "Delay.h"
void IIC_Delay5us(void)		//@11.0592MHz	5us
{
	unsigned char data i;

	_nop_();
	i = 11;
	while (--i);
}
void IIC_Init(void)
{
	P1M1=1;P1M0=1;		//设置开漏模式
}

void IIC_W_SCL(uchar BitValue)
{
	SCL=BitValue;IIC_Delay5us();
}

void IIC_W_SDA(uchar BitValue)
{
	SDA=BitValue;IIC_Delay5us();
}

uchar IIC_R_SDA(void)
{
	if(SDA){return 1;}
	else{return 0;}
}

void IIC_Start(void)
{
	Delay(20);		//需要等待20MS
	IIC_W_SDA(1);
	IIC_W_SCL(1);
	IIC_W_SDA(0);
	IIC_W_SCL(0);
}

void IIC_Stop(void)
{
	IIC_W_SDA(0);
	IIC_W_SCL(1);
	IIC_W_SDA(1);
	Delay(10);		//防止IIC_Start与IIC_Stop冲突,这里停止后必须等待一会(至少10MS以上),特别注意！！！！！！！！！！！！！！！！！！！！！
}

void IIC_SendByte(uchar Byte)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		IIC_W_SDA(Byte&(0X80>>i));
		IIC_W_SCL(1);
		IIC_W_SCL(0);
	}
}

uchar IIC_ReceiveByte(void)
{
	uchar i,Byte=0X00;
	IIC_W_SDA(1);		//释放总线
	for(i=0;i<8;i++)
	{
		IIC_W_SCL(1);
		if(IIC_R_SDA()==1){Byte|=0X80>>i;}
		IIC_W_SCL(0);
	}
	return Byte;
}

//发送应答信号
void IIC_SendAck(uchar AckBit)
{
	IIC_W_SDA(AckBit);
	IIC_W_SCL(1);
	IIC_W_SCL(0);
}

//接收应答信号
uchar IIC_ReceiveAck(void)
{
	uchar Ack;
	IIC_W_SDA(1);		//释放总线
	IIC_W_SCL(1);
	Ack=IIC_R_SDA();
	IIC_W_SCL(0);
	return Ack;
}