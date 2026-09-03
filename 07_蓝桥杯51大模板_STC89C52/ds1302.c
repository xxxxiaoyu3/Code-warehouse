#include "ds1302.h"  									

//写字节
void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK = 0;
		SDA = temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

//向DS1302寄存器写入数据
void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

//从DS1302寄存器读出数据
unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
	address|=0x01;
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}
/*
#define Ds1302_En 0X8E 	//写保护:0X80,写使能:0X00;
#define Ds1302_Year 0X8C 	//年
#define Ds1302_Day 0X8A 	//星期
#define Ds1302_Month 0X88 	//月
#define Ds1302_Date 0X86 	//日
#define Ds1302_Hour 0X84 	//时
#define Ds1302_Min 0X82 	//分
#define Ds1302_Sec 0X80 	//秒

*/
/*
10转16:Time[0]/10*16+Time[0]%10
16转10:Time[0]/16*10+Time[0]%16
Time[3]={小时，分钟，秒};
*/
void Set_Time(unsigned char *Time)
{
	Write_Ds1302_Byte(Ds1302_En,0X00);		//写使能
	Write_Ds1302_Byte(Ds1302_Hour,Time[0]/10*16+Time[0]%10);
	Write_Ds1302_Byte(Ds1302_Min,Time[1]/10*16+Time[1]%10);
	Write_Ds1302_Byte(Ds1302_Sec,Time[2]/10*16+Time[2]%10);
	Write_Ds1302_Byte(Ds1302_En,0X80);		//写保护
}
void Read_Time(unsigned char *Time)
{
	unsigned char Temp;
	Temp=Read_Ds1302_Byte(Ds1302_Hour);
	Time[0]=Temp/16*10+Temp%16;
	Temp=Read_Ds1302_Byte(Ds1302_Min);
	Time[1]=Temp/16*10+Temp%16;
	Temp=Read_Ds1302_Byte(Ds1302_Sec);
	Time[2]=Temp/16*10+Temp%16;
}
