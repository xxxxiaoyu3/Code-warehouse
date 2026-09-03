#include <STC15F2K60S2.h>
#include <Intrins.h>
#include "Delay.h"
#include "LCD1602.h"

//引脚配置
sbit LCD_RS=P2^0;
sbit LCD_RW=P2^1;
sbit LCD_E=P1^2;
#define LCD_DataPort P0

/**
  * @brief  LCD1602延时函数，12MHz调用可延时1ms
  * @param  无
  * @retval 无
  */
void LCD_Delay()
{
	unsigned char i, j;

	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
}
/**
  * @brief  LCD1602读忙状态
  * @param  无
  * @retval 无
  */
//void LCD_Check_Busy()							//？？？？？？？？？？用了判忙为什么不显示
//{
//	unsigned char temp;
//	unsigned int i=0;
//	LCD_DataPort=0XFF;//将数据初始化置1
//	LCD_RS=0;//0指令
//	LCD_RW=1;//1读

//	do
//	{
//		i++;if(i>5) break;		//		如果一直都忙就直接跳出!!!防止进入死循环！！！！
//		
//		LCD_E=1;
//		temp=LCD_DataPort;//保存Data本次的状态用于判断忙不忙
//		LCD_E=0;//LCD使能开始执行命令
//		LCD_Delay();
//	}while(temp&0X80);//只取第一位判断若为1则忙，0则不忙，跳出
//		LCD_RW=0;//1读0写
//		LCD_RS=1;
//}
/**
  * @brief  LCD1602写命令
  * @param  无
  * @retval 无
  */
void LCD_Write_Cmd(unsigned char Command)
{
	//LCD_Check_Busy();				//？？？？？？？？？？用了判忙为什么不显示
	
	LCD_RS=0;//1数据寄存器，0指令寄存器
	LCD_RW=0;//1读0写
	LCD_DataPort=Command;
	LCD_E =1;
	LCD_Delay();
	LCD_E=0;//液晶屏使能，液晶屏模块执行命令低电平有效
	LCD_Delay();
}
/**
  * @brief  LCD1602写数据
  * @param  无
  * @retval 无
  */
void LCD_Write_Data(unsigned char Data)
{
	//LCD_Check_Busy();//判忙//用了，蜂鸣器会响		？？？？？？？？？？用了判忙为什么不显示
	
	LCD_RS=1;//1数据寄存器，0指令寄存器
	LCD_RW=0;//1读0写
	LCD_DataPort=Data;
	LCD_E=1;
	LCD_Delay();
	LCD_E=0;//液晶屏使能，液晶屏模块执行命令低电平有效
	LCD_Delay();
}
/**
  * @brief  LCD1602设置光标位置
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @retval 无
  */
void LCD_SetCursor(unsigned char Line,unsigned char Column)
{
	if(Line==1)
	{
		LCD_Write_Cmd(0X80|Column-1);
	}
	if(Line==2)
	{
		LCD_Write_Cmd(0X80|(Column-1)+0X40);//第二行
	}
}
/**
  * @brief  LCD1602初始化
  * @param  无
  * @retval 无
  */
void LCD_Inte()
{	
	LCD_Write_Cmd(0x38);//八位数据接口，两行显示，5*7点阵
	LCD_Write_Cmd(0x0C);//显示开，光标关，闪烁关
	LCD_Write_Cmd(0x06);//数据读写操作后，光标自动加一，画面不动
	LCD_Write_Cmd(0x01);//光标复位，清屏
}
/**
  * @brief  在LCD1602指定位置上显示一个字符
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的字符
  * @retval 无
  */
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char)
{
	LCD_SetCursor(Line,Column);//设置位置
	LCD_Write_Data(Char);	
}
/**
  * @brief  在LCD1602指定位置上显示一个字符串
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的字符串
  * @retval 无
  */
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String)
{
	LCD_SetCursor(Line,Column);//设置位置
	while(*String!='\0')
	{
		LCD_Write_Data(*String++);
	}
}

/**
  * @brief  返回值=X的Y次方
  */
int LCD_Pow(int X,int Y)
{
	unsigned char i;
	int Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X;
	}
	return Result;
}
/**
  * @brief  在LCD1602指定位置开始显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~65535
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
		LCD_SetCursor(Line,Column);//设置位置
		for(i=Length;i>0;i--)
		{
			LCD_Write_Data(Number/LCD_Pow(10,i-1)%10+'0');//对10取余得到后几位
		}
}
void LCD_reset(unsigned char Num)		//LCD清屏,Num:要选择的行数
{
	unsigned char i=0;
	if(Num==1)
	LCD_Write_Cmd(0x80);
	else if(Num==2)
	LCD_Write_Cmd(0x80+0x40);
	for(i=0;i<16;i++)
	{
		LCD_Write_Data(0x20);  //无显示
	}
}