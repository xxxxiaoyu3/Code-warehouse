#include "Time.h"
#include "LED.h"  // 用于测试 LED 闪烁判断是否正常运行
#include "lvgl.h"
// 定时器相关全局变量
volatile uint16_t Task_Time_100MS = 0; // 100ms计数器，用于定时任务
volatile uint16_t Time_Key_20MS = 0; // 100ms计数器，用于定时任务
/**
 * @brief  TIM4定时器初始化
 *         配置为1ms中断一次，用于系统定时和按键消抖
 */
void Time4_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // 使能TIM4时钟
    TIM_InternalClockConfig(TIM4);

    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_InitStruct.TIM_Period = 1000 - 1;         // 自动重装载值，1ms
    TIM_InitStruct.TIM_Prescaler = 72 - 1;        // 预分频，72MHz/72=1MHz
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4, &TIM_InitStruct);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);         // 清除更新中断标志
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);    // 使能更新中断

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    TIM_Cmd(TIM4, ENABLE); // 启动定时器
}

/**
 * @brief  TIM4中断服务函数
 *         1ms进入一次，主要用于LED测试和按键消抖定时
 */
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update))
	{
		lv_tick_inc(1);
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update); 
}