#ifndef __DS1302_H__
#define __DS1302_H__

#define uchar unsigned char
#define uint unsigned int
sbit DS1302_IO=P2^3;
sbit DS1302_SCL=P1^7;
sbit DS1302_EN=P1^3;

#define DS1302_SECOND		0x80		//秒
#define DS1302_MINUTE		0x82		//分
#define DS1302_HOUR			0x84		//时
#define DS1302_DATE			0x86		//天
#define DS1302_MONTH		0x88		//月
#define DS1302_DAY			0x8A		//星期
#define DS1302_YEAR			0x8C		//年
#define DS1302_WP			0x8E		//写保护 0X00:解除写保护		0X80:开启写保护

//时间数组格式参考如下，索引0~6分别为年、月、日、时、分、秒、星期
//unsigned char DS1302_Time[]={19,11,16,12,59,55,6};
void DS1302_Delay(void);
void DS1302_Init(void);
void DS1302_WriteByet(uchar Command,uchar Byte);
uchar DS1302_ReadByet(uchar Command);
void DS1302_SetTime(uchar *Time);
void DS1302_ReadTime(uchar *Time);
#endif
