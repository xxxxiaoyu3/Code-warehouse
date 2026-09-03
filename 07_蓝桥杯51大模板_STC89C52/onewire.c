#include "onewire.h"

//单总线内部延时函数
void Delay_OneWire(unsigned int t)  
{
	while(t--);
}

//单总线写操作
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//单总线读操作
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//DS18B20初始化
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}

float DS18B20_Read_C()	//读取温度	
{
	unsigned char V_TH,V_TL;
	if(!init_ds18b20()) return 0;
	Write_DS18B20(0XCC);
	Write_DS18B20(0X44);
	if(!init_ds18b20()) return 0;
	Write_DS18B20(0XCC);
	Write_DS18B20(0XBE);
	V_TL=Read_DS18B20();
	V_TH=Read_DS18B20();
	return ((V_TH<<8)|V_TL)/16.0;
}