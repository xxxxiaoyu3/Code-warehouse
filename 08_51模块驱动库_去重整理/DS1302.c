#include <STC15F2K60S2.h>
#include <Stdio.h>
#include "Delay.h"
#include "DS1302.h"
/*
16进制(BCD)转10进制:
	16进制(BCD):X/16*10+X%16
10进制(BCD)转16进制:
	X/10*16+X%10
*/
void DS1302_Delay(void)
{}
void DS1302_Init(void)
{
	DS1302_EN=0;		//不使能
	DS1302_SCL=0;		//初始化时钟电平为低电平
}
void DS1302_WriteByet(uchar Command,uchar Byte)
{
	uchar i;
	DS1302_EN=1;
	for(i=0;i<8;i++)
	{
		DS1302_IO=Command&(0X01<<i);		//低位写入
		DS1302_SCL=1;DS1302_Delay();
		DS1302_SCL=0;DS1302_Delay();
	}
	for(i=0;i<8;i++)
	{
		DS1302_IO=Byte&(0X01<<i);			//低位写入	
		DS1302_SCL=1;DS1302_Delay();
		DS1302_SCL=0;DS1302_Delay();
	}
	DS1302_EN=0;
}

uchar DS1302_ReadByet(uchar Command)
{
	uchar i,ReadByte=0X00;
	Command|=0x01;		//最低位读写位，0:写		1:读
	DS1302_EN=1;
	for(i=0;i<8;i++)
	{
		DS1302_IO=Command&(0X01<<i);		//低位写入
		DS1302_SCL=0;DS1302_Delay();		//上升沿读取
		DS1302_SCL=1;DS1302_Delay();
	}
	for(i=0;i<8;i++)
	{
		DS1302_SCL=1;DS1302_Delay();		//下降沿读取
		DS1302_SCL=0;DS1302_Delay();
		if(DS1302_IO){ReadByte|=(0X01<<i);}	//低位读取
	}
	DS1302_EN=0;
	DS1302_IO=0;	//读取后将IO设置为0，否则读出的数据会出错
	return ReadByte;
}
//时间数组格式参考如下，索引0~6分别为年、月、日、时、分、秒、星期
//unsigned char DS1302_Time[]={19,11,16,12,59,55,6};
void DS1302_SetTime(uchar *Time)
{
	DS1302_WriteByet(DS1302_WP,0X00);		//解除写保护
	DS1302_WriteByet(DS1302_YEAR,Time[0]/10*16+Time[0]%10);//十进制转BCD码后写入
	DS1302_WriteByet(DS1302_MONTH,Time[1]/10*16+Time[1]%10);
	DS1302_WriteByet(DS1302_DATE,Time[2]/10*16+Time[2]%10);
	DS1302_WriteByet(DS1302_HOUR,Time[3]/10*16+Time[3]%10);
	DS1302_WriteByet(DS1302_MINUTE,Time[4]/10*16+Time[4]%10);
	DS1302_WriteByet(DS1302_SECOND,Time[5]/10*16+Time[5]%10);
	DS1302_WriteByet(DS1302_DAY,Time[6]/10*16+Time[6]%10);
	DS1302_WriteByet(DS1302_WP,0X80);		//开启写保护
}
void DS1302_ReadTime(uchar *Time)
{
	uchar Temp;
	Temp=DS1302_ReadByet(DS1302_YEAR);//BCD码转十进制后读取
	Time[0]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByet(DS1302_MONTH);
	Time[1]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByet(DS1302_DATE);
	Time[2]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByet(DS1302_HOUR);
	Time[3]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByet(DS1302_MINUTE);
	Time[4]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByet(DS1302_SECOND);
	Time[5]=Temp/16*10+Temp%16;
	Temp=DS1302_ReadByet(DS1302_DAY);
	Time[6]=Temp/16*10+Temp%16;
}