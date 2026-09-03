#include "Beep.h"
/*
 * @函数类型  	底层驱动函数
 * @brief  		配置蜂鸣器硬件引脚
 * @param  None
 * @retval None
 */
void Beep_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_Beep_Port,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=Beep_Pin;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(Beep_Port,&GPIO_InitStruct);
	GPIO_SetBits(Beep_Port, Beep_Pin); // Beep off
}
/*
 * @函数类型  	底层驱动函数
 * @brief  		开启蜂鸣器
 * @param  None
 * @retval None
 */
void Beep_On(void)
{
    GPIO_ResetBits(Beep_Port, Beep_Pin);
}
/*
 * @函数类型  	底层驱动函数
 * @brief  		关闭蜂鸣器
 * @param  None
 * @retval None
 */
void Beep_Off(void)
{
    GPIO_SetBits(Beep_Port, Beep_Pin);
}
/*
 * @函数类型  	 应用层函数
 * @brief  		蜂鸣器滴声函数，执行该函数时，蜂鸣器发出参数频率和滴声时长的声音
 * @param 		Beep_HZ: 蜂鸣器频率
 * @param  		Beep_Time_MS: 蜂鸣器滴声时长(单位ms)
 * @retval None
 */
void Beep_Dot(uint32_t Beep_HZ, uint32_t Beep_Time_MS)
{
    if (Beep_HZ == 0 || Beep_Time_MS == 0) {
        return; // 避免除以零和无效输入
    }

    uint32_t halfPeriodUs = 1000000UL / Beep_HZ / 2;	 			// 计算半个周期的时间(微秒)
    uint32_t totalDurationUs = (uint32_t)Beep_Time_MS * 1000; 		//滴的时间单位转换ms->us
    uint32_t cycleUs = halfPeriodUs * 2;							// 计算周期时间(微秒)

    if (cycleUs == 0) {
        return; // 周期时间为 0，避免除以零
    }

    uint32_t loopCount = totalDurationUs / cycleUs;			// 计算循环次数(滴的时间/周期时间=循环次数)

    for (uint32_t i = 0; i < loopCount; i++) {
        Beep_On();
        Delay_us(halfPeriodUs);  // 高电平时间
        Beep_Off();
        Delay_us(halfPeriodUs);  // 低电平时间
    }
}
