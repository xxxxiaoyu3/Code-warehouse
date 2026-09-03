#ifndef __PCF8591_H__
#define __PCF8591_H__

#define uchar unsigned char
#define uint unsigned int
	
#define PCF8591_Add_1 0X90
#define PCF8591_Add_2 0X92
#define PCF8591_CH_All 0X45		//读取所有通道同时打开DA输出转换
#define PCF8591_CH_0 0X00		//读取0通道同时打开DA输出转换,如果需要读取ADC同时DAC则用:0X40
#define PCF8591_CH_1 0X01		//0X41
#define PCF8591_CH_2 0X10		//0X50
#define PCF8591_CH_3 0X11		//0X51
#define PCF8591_AOUT 0X44		//这是AOUT写命令无需修改


void PCF8591_Init(void);
uchar PCF8591_ReadVal(uchar ID_Address,uchar Ch_Address);		//读取单个通道电压
void PCF8591_ReadAllVal(uchar ID_Address,uchar *Num);			//读取4个通道电压
void PCF8591_SendVal(uchar ID_Address,uchar Val_Num);				//输出AOUT
float PCF8591_Transition_Val(uchar Val_Num);					//单个数值电压转换
void PCF8591_Transition_Val_S(uchar *Val_Num,float *Val,uchar length);//多个数值电压转换

#endif
