#include <STC15F2K60S2.h>
#include "Delay.h"
#include "KeyGet.h"
#define uchar unsigned char 
//#define key_DATA_Scan_PORT P0			//用于按键检测的口
/**
  * @brief  获取4X4矩阵键盘
  * @param 	无
  * @retval 返回数字0-15
  */
unsigned char KeyGet(void)
{
	uchar KeyNum=17;
	
	P3=0X3F;P42=1;P44=1;
	P34=0;
	if(P30==0){Delay(20);while(P30==0);KeyNum=0;}
	if(P31==0){Delay(20);while(P31==0);KeyNum=4;}
	if(P32==0){Delay(20);while(P32==0);KeyNum=8;}
	if(P33==0){Delay(20);while(P33==0);KeyNum=12;}
	
	P3=0X3F;P42=1;P44=1;
	P35=0;
	if(P30==0){Delay(20);while(P30==0);KeyNum=1;}
	if(P31==0){Delay(20);while(P31==0);KeyNum=5;}
	if(P32==0){Delay(20);while(P32==0);KeyNum=9;}
	if(P33==0){Delay(20);while(P33==0);KeyNum=13;}
	
	P3=0X3F;P42=0;P44=1;
	//P42=0;
	if(P30==0){Delay(20);while(P30==0);KeyNum=2;}
	if(P31==0){Delay(20);while(P31==0);KeyNum=6;}
	if(P32==0){Delay(20);while(P32==0);KeyNum=10;}
	if(P33==0){Delay(20);while(P33==0);KeyNum=14;}
	
	P3=0X3F;P42=1;P44=0;
	//P44=0;
	if(P30==0){Delay(20);while(P30==0);KeyNum=3;}
	if(P31==0){Delay(20);while(P31==0);KeyNum=7;}
	if(P32==0){Delay(20);while(P32==0);KeyNum=11;}
	if(P33==0){Delay(20);while(P33==0);KeyNum=15;}
	
	return KeyNum;
}

/*******************************以上函数无未按下判断功能***************************************/

//unsigned char BUTTON_scan()			//矩阵按钮扫描子程序
//{
//	uchar Key_on=17;	//开机不显示	
//	uchar key_DATA_Scan_PORT=0x0f;				
//	//Delay(5);		//电平抖动延时	
//	if(key_DATA_Scan_PORT!=0x0f)		//确认按键按下
//	{
//		switch(key_DATA_Scan_PORT)			//列扫描
//		{
//			case 0x0e: Key_on=0;	break;	
//			case 0x0d: Key_on=4;	break;
//			case 0x0b: Key_on=8;	break;
//			case 0x07: Key_on=12;	break;
//			default:   Key_on=17;return Key_on;	//无效按下时显示—
//		}
//		Delay(1);
//		key_DATA_Scan_PORT=0xf0;	//换行扫描
//		switch(key_DATA_Scan_PORT)			//行扫描
//		{
//			case 0xe0: Key_on+=0;	break;
//			case 0xd0: Key_on+=1;	break;
//			case 0xb0: Key_on+=2;	break;
//			case 0x70: Key_on+=3;	break;
//			default:   Key_on=17;return Key_on;
//		}
//		while((key_DATA_Scan_PORT&0xf0)!=0xf0);		//等待松开并输出!!!!!!!!这一句很重要!!!!!!!不要漏了，一定要加上松手检测
//	}
//	return Key_on;
//}