#include <STC15F2K60S2.h>
#include "intrins.h"

//引脚定义
sbit OneWire_DQ=P1^4;

void delay_us(unsigned int us)
{
	while(us--)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}

/**
  * @brief  单总线初始化
  * @param  无
  * @retval 从机响应位，0为响应，1为未响应
  */
unsigned char OneWire_Init(void)
{
	unsigned char AckBit;
	OneWire_DQ=1;
	OneWire_DQ=0;
	delay_us(500);
	OneWire_DQ=1;
	delay_us(70);
	AckBit=OneWire_DQ;
	delay_us(500);
	return AckBit;
}

/**
  * @brief  单总线发送一位
  * @param  Bit 要发送的位
  * @retval 无
  */
//void OneWire_SendBit(unsigned char Bit)
//{
//	unsigned char i;
//	OneWire_DQ=0;
//	i = 4;while (--i);			//Delay 10us
//	OneWire_DQ=Bit;
//	i = 24;while (--i);			//Delay 50us
//	OneWire_DQ=1;
//}

/**
  * @brief  单总线接收一位
  * @param  无
  * @retval 读取的位
  */
unsigned char OneWire_ReceiveBit(void)
{
	unsigned char Bit;
	OneWire_DQ=0;
	delay_us(5);			//Delay 5us
	OneWire_DQ=1;
	delay_us(5);			//Delay 5us
	Bit=OneWire_DQ;
	delay_us(50);			//Delay 50us
	return Bit;
	
//	unsigned char dat=0;
//	
//	OneWire_DQ=0;
//	delay_us(5);
//	OneWire_DQ=1;	
//	delay_us(5); //该段时间不能过长，必须在15us内读取数据
//	if(OneWire_DQ)dat=1;	//如果总线上为1则数据dat为1，否则为0
//	else dat=0;
//	delay_us(50);
//	return dat;
}

/**
  * @brief  单总线发送一个字节
  * @param  Byte 要发送的字节
  * @retval 无
  */
void OneWire_SendByte(unsigned char Byte)
{
	unsigned char i=0;
	unsigned char temp=0;

	for(i=0;i<8;i++)//循环8次，每次写一位，且先写低位再写高位
	{
		temp=Byte&0x01;//选择低位准备写入
		Byte>>=1;//将次高位移到低位
		if(temp)
		{
			OneWire_DQ=0;
			delay_us(3);
			OneWire_DQ=1;	
			delay_us(60);
		}
		else
		{
			OneWire_DQ=0;
			delay_us(60);
			OneWire_DQ=1;
			delay_us(3);	
		}	
	}
}

/**
  * @brief  单总线接收一个字节
  * @param  无
  * @retval 接收的一个字节
  */
unsigned char OneWire_ReceiveByte(void)
{
	unsigned char i;
	unsigned char Byte=0x00;
	for(i=0;i<8;i++)
	{
		if(OneWire_ReceiveBit()){Byte|=(0x01<<i);}
	}
	return Byte;
}
