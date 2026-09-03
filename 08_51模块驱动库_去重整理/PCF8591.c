#include <STC15F2K60S2.h>
#include "IIC.h"
#include "Delay.h"
#include "PCF8591.h"
/**
  * @brief  配置IIC输出引脚为开漏输出
  * @param 
  * @retval 
  */
void PCF8591_Init(void)
{
	IIC_Init();
}
/**
  * @brief  将数字量电压转换为模拟量电压函数
  * @param 	Val_Num:数字量电压
  * @retval Val:模拟量电压
  */
float PCF8591_Transition_Val(uchar Val_Num)
{
	float Val;
	Val=(0.0196)*Val_Num;	//0.0196由5V=255推算出来
	return Val;
}
/**
  * @brief  将数字量电压数组 转换为模拟量电压数组 函数
  * @param 	*Val_Num:数字量电压数组 *Val:模拟量电压数组 length:数组长度
  * @retval	
  */
void PCF8591_Transition_Val_S(uchar *Val_Num,float *Val,uchar length)		
{
	uchar i;
	if(length<=0){return;}		//非法数值直接跳出
	for(i=0;i<length;i++)
	{
		Val[i]=(0.0196)*Val_Num[i];	//将数字量装换位模拟量，由5V=255推算出来
	}
}
/**
  * @brief  读取单通道电压
  * @param 	ID_Address:设备地址	Ch_Address:通道地址
  * @retval 输出数字量电压
  */
uchar PCF8591_ReadVal(uchar ID_Address,uchar Ch_Address)
{
	uchar Num;
	IIC_Start();
	IIC_SendByte(ID_Address);		//地址写
	IIC_ReceiveAck();
	IIC_SendByte(Ch_Address);		//写入通道号,默认单端输入
	IIC_ReceiveAck();
	//Delay(10);						//写完不能马上读？
	IIC_Start();
	IIC_SendByte(ID_Address|0X01);		//读
	IIC_ReceiveAck();
	Num=IIC_ReceiveByte();
	IIC_SendAck(1);
	IIC_Stop();
	return Num;
}

/**
  * @brief  读取所有通道电压
  * @param 	ID_Address:设备地址	*Num:读取的数字量数组
  * @retval 
  */
void PCF8591_ReadAllVal(uchar ID_Address,uchar *Num)
{
	uchar i;
	IIC_Start();
	IIC_SendByte(ID_Address);		//地址写
	IIC_ReceiveAck();
	IIC_SendByte(PCF8591_CH_All);				//读取所有通道
	IIC_ReceiveAck();
	//Delay(10);						//写完不能马上读？
	IIC_Start();
	IIC_SendByte(ID_Address|0X01);		//读
	IIC_ReceiveAck();
	for(i=0;i<4;i++)		//读取四个通道电压
	{
		Num[i]=IIC_ReceiveByte();
		if(i<3){IIC_SendAck(0);}		//当最后一位已经写完了就发送0:NACK	
		else{IIC_SendAck(1);}			//否则发送ACK_1
	}
	IIC_Stop();
}
/**
  * @brief  设置AOUT通道电压
  * @param 	ID_Address:设备地址	Val:要设置的电压
  * @retval 
  */
void PCF8591_SendVal(uchar ID_Address,uchar Val_Num)
{
	IIC_Start();
	IIC_SendByte(ID_Address);		//地址写
	IIC_ReceiveAck();
	IIC_SendByte(PCF8591_AOUT);		//写DA命令
	IIC_ReceiveAck();
	IIC_SendByte(Val_Num);		//发送数字量
	IIC_ReceiveAck();
	IIC_Stop();
}
