#include <STC15F2K60S2.h>
#include "IIC.h"
#include "AT24C04.h"
#include "Delay.h"
//AT24C04只有数据寄存器没有指令所以直接读写就好了,无需发送其他指令
void AT24C04_Init(void)
{
	IIC_Init();
}
/**
	* @brief 写入一个字节
	* @param ID_Address:器件ID地址
	* @param Address:内存地址,32页，一页8个byte
	* @param Data:8位数据
	* @param Part(AT24C04有两个部分):部分选择区;0X00第一部分,0X02第二部分
	* @retval return:0 发送失败	return:1 发送成功
  */
uchar AT24C04_WriteByte(uchar ID_Address,uint Address,uchar Data)
{
	uchar flag;
	IIC_Start();
	if(Address>=0X0100){IIC_SendByte(ID_Address|0X02);}//最低位0写1读
	else if(Address<0X0100){IIC_SendByte(ID_Address|0X00);}		
	IIC_ReceiveAck();
	IIC_SendByte(Address);
	IIC_ReceiveAck();
	IIC_SendByte(Data);
	flag=IIC_ReceiveAck();	
	IIC_Stop();
	if(flag){return 1;}
	else{return 0;}	
}

/**
	* @brief 写入一个字节
	* @param ID_Address:器件ID地址
	* @param Address:内存地址,32页，一页8个byte
	* @param Data:8位数据
	* @param Part(AT24C04有两个部分):部分选择区;0X00第一部分,0X02第二部分
	* @retval return:0 发送失败	return:1 发送成功
  */
uchar AT24C04_ReadByte(uchar ID_Address,uint Address)
{
	uchar Byte;
	IIC_Start();
	if(Address>=0X0100){IIC_SendByte(ID_Address|0X02);}//最低位0写1读
	else if(Address<0X0100){IIC_SendByte(ID_Address|0X00);}		
	IIC_ReceiveAck();				//等待ACK
	IIC_SendByte(Address);
	IIC_ReceiveAck();
	IIC_Start();
	if(Address>=0X0100){IIC_SendByte(ID_Address|0X02|0X01);}//最低位0写1读
	else if(Address<0X0100){IIC_SendByte(ID_Address|0X00|0X01);}	
	IIC_ReceiveAck();						//等待ACK
	Byte=IIC_ReceiveByte();					//读取数据
	IIC_SendAck(1);
	IIC_Stop();
	return 	Byte;
}
//页写入
/**
  * @brief  
  * @param 
* @retval Page页数:最大64页
  */
uchar AT24C04_PageWriteByte(uchar ID_Address,uint Page,uchar *Data,uchar length)
{
	uchar flag,i;
	IIC_Start();
	if(Page>=32){IIC_SendByte(ID_Address|0X02);}//最低位0写1读
	else if(Page<32){IIC_SendByte(ID_Address|0X00);}		
	IIC_ReceiveAck();
	IIC_SendByte(Page*8);
	IIC_ReceiveAck();
	for(i=0;i<length;i++)
	{
		IIC_SendByte(Data[i]);
		flag=IIC_ReceiveAck();	
	}
	IIC_Stop();
	if(flag){return 1;}
	else{return 0;}	
}
//页读取
/**
  * @brief  
  * @param 
* @retval Page页数:最大64页
  */
void AT24C04_PageReadByte(uchar ID_Address,uint Page,uchar *Data,uchar length)
{
	uchar i;
	if(length<=0){return;}		//长度不合法直接跳出
	IIC_Start();
	if(Page>=32){IIC_SendByte(ID_Address|0X02);}//最低位0写1读
	else if(Page<32){IIC_SendByte(ID_Address|0X00);}	
	IIC_ReceiveAck();				//等待ACK
	IIC_SendByte(Page*8);
	IIC_ReceiveAck();
	
	IIC_Start();
	if(Page>=32){IIC_SendByte(ID_Address|0X02|0X01);}//最低位0写1读
	else if(Page<32){IIC_SendByte(ID_Address|0X00|0X01);}	
	IIC_ReceiveAck();						//等待ACK
	for(i=0;i<length;i++)		//读的数据长度
	{
		Data[i]=IIC_ReceiveByte();					//读取数据
		if(i<length-1){IIC_SendAck(0);}//否则发送ACK_0
		else{IIC_SendAck(1);}		//当最后一位已经写完了就发送1:NACK
	}
	IIC_Stop();
}
