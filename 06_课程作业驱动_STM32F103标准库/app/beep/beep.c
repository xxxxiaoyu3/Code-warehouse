#include "beep.h"




void BEEP_Init()
{
	//声名一个结构体用来初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启GPIO时钟
	RCC_APB2PeriphClockCmd(BEEP_PORT_RCC,ENABLE);//使能开启时钟
	//设置GPIO的模式，和IO口
	GPIO_InitStructure.GPIO_Pin=BEEP_PIN;//选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	//设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//设置传输速率
	GPIO_Init(BEEP_PORT,&GPIO_InitStructure);//初始GPIO
	
}



	


