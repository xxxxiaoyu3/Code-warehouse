#include <STC15F2K60S2.h>
#include "KeyGet.h"

uchar KeyGet(void)
{
	uchar Num=20;
	ET0=0;
	
	P44=0;
	P42=1;
	P35=1;
	P34=1;
	if(P33 == 0)
		Num=4;
	if(P32 == 0)
		Num=5;
	if(P31 == 0)
		Num=6;
	if(P30 == 0)
		Num=7;
	P44=1;
	P42=0;
	P35=1;
	P34=1;
	if(P33 == 0)
		Num=8;
	if(P32 == 0)
		Num=9;
	if(P31 == 0)
		Num=10;
	if(P30 == 0)
		Num=11;
	P44=1;
	P42=1;
	P35=0;
	P34=1;
	if(P33 == 0)
		Num=12;
	if(P32 == 0)
		Num=13;
	if(P31 == 0)
		Num=14;
	if(P30 == 0)
		Num=15;
	P44=1;
	P42=1;
	P35=1;
	P34=0;
	if(P33 == 0)
		Num=16;
	if(P32 == 0)
		Num=14;
	if(P31 == 0)
		Num=18;
	if(P30 == 0)
		Num=19;
	ET0=1;
	P3=0XFF;
	return Num;
}