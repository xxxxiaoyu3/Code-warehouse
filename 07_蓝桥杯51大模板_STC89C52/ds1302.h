#ifndef __DS1302_H
#define __DS1302_H

#include <reg52.h>
#include <intrins.h>

sbit SCK = P1^7;		
sbit SDA = P2^3;		
sbit RST = P1^3; 

#define Ds1302_En 0X8E 	//写保护:0X80,写使能:0X00;
#define Ds1302_Year 0X8C 	//年
#define Ds1302_Day 0X8A 	//星期
#define Ds1302_Month 0X88 	//月
#define Ds1302_Date 0X86 	//日
#define Ds1302_Hour 0X84 	//时
#define Ds1302_Min 0X82 	//分
#define Ds1302_Sec 0X80 	//秒

//void Write_Ds1302(unsigned char temp);
//void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
//unsigned char Read_Ds1302_Byte( unsigned char address );
void Set_Time(unsigned char *Time);
void Read_Time(unsigned char *Time);

#endif
