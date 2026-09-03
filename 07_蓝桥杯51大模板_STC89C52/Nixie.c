#include <STC15F2K60S2.h>
#include "Nixie.h"
//0-1-2-3-4-5-6-7-8-9,10:不显示，11:分隔符,12:U
uchar code Nixie_DuanMa[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0XFF,0XBF,0XC1};//断码表
void Nixie_diplay(uchar Data,uchar Com,uchar point)		//Data;显示的数字,Com:位置，Point哪个位置要加小数点
{
	P0=0XFF;					
	P2 = P2&0X1F | 0XE0;		//手动消除影子
	P2 &= 0X1F;		//P2_5_6_7清零，防止数据错乱，以及锁住寄存器
	
	P0=0X01<<(Com-1);
	P2 = P2&0X1F | 0XC0;		//Y6C:Com
	P2 &= 0X1F;		//P2_5_6_7清零，防止数据错乱，以及锁住寄存器
	
	P0=Nixie_DuanMa[Data];
	if(point==1)
		P0 &=0X7F;				//加个小数点
	P2 = P2&0X1F | 0XE0;		//Y7C:Data
	P2 &= 0X1F;		//P2_5_6_7清零，防止数据错乱，以及锁住寄存器
}