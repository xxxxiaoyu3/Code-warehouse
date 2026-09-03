#include "time.h"
#include "led.h"

void TIM4_Init(u16 pre,u16 psc)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);			//开启使能
	
	//初始化定时器参数
	
	
	TIM_TimeBaseInitStructure.TIM_Period=pre;//自动重载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;//预分频
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//分频时钟因子为1
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//计数模式向上模式
	TIM_TimeBaseInit(TIM4 ,&TIM_TimeBaseInitStructure );
	
	//设置定时器中断类型
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	//设置中断优先级
	
		//中断通道 K UP
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	//抢占式优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	//开启中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	//初始化
	NVIC_Init(&NVIC_InitStructure);
	
	//开启定时器
	TIM_Cmd (TIM4 ,ENABLE);
	
	
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		led2=!led2;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}




