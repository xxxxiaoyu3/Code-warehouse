#include "exti.h"
#include "SysTick.h"
#include "key.h"
#include "led.h"
#include "beep.h"

void My_EXTI_Init(void)
{
	//配置结构体变量-
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//开启AFIO时钟
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);//引脚映射
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
	
	//中断通道 K UP
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;
	//抢占式优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	//开启中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	//初始化
	NVIC_Init(&NVIC_InitStructure);
	
	//配置结构体变量-中断通道 K1
	NVIC_InitStructure.NVIC_IRQChannel=EXTI2_IRQn;
	//抢占式优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	//开启中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	//初始化
	NVIC_Init(&NVIC_InitStructure);
	
	//配置结构体变量-中断通道 K2
	NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;
	//抢占式优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	//开启中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	//初始化
	NVIC_Init(&NVIC_InitStructure);
	
	//配置结构体变量-中断通道 K3
	NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;
	//抢占式优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	//开启中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	//初始化
	NVIC_Init(&NVIC_InitStructure);
	
	
	//初始化EXTI，选择触发方式
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;//UP下拉电阻，上升沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line2|EXTI_Line3|EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//K1上拉电阻，下降沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
}


//中断函数
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)==1)
	{
		delay_ms(10);
		if(KEY_UP==1)
		{
			led2=0;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);//清除中断标志
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)==1)
	{
		delay_ms(10);
		if(KEY1==0)
		{
			led2=1;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);//清除中断标志
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)==1)
	{
		delay_ms(10);
		if(KEY0==0)
		{
			led3=0;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line2);//清除中断标志
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)==1)
	{
		delay_ms(10);
		if(KEY2==0)
		{
			led3=1;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);//清除中断标志
}

