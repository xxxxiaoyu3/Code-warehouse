#ifndef __ONEWIRE_H
#define __ONEWIRE_H

#include "reg52.h"

sbit DQ = P1^4;  
#define DS18B20_Rom 0XCC
#define DS18B20_Con_T 0X44		//开启温度转换
#define DS18B20_Read 0XBE		//读取温度

unsigned char rd_temperature(void);  
float DS18B20_Read_C();

#endif
