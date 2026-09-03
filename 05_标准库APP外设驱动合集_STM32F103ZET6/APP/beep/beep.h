#ifndef __Beep_H_
#define __Beep_H_

#include "stm32f10x.h"
#include "system.h" 
#include "Delay.h" 


#define RCC_Beep_Port RCC_APB2Periph_GPIOB //蜂鸣器端口时钟
#define Beep_Port GPIOB //蜂鸣器端口
#define Beep_Pin GPIO_Pin_5 //蜂鸣器引脚
#define BEEP PBout(5) //蜂鸣器控制引脚


void Beep_Init(void); //蜂鸣器初始化
void Beep_On(void); //开启蜂鸣器
void Beep_Off(void); //关闭蜂鸣器
void Beep_Dot(uint32_t Beep_HZ, uint32_t Beep_Time_MS); //蜂鸣器滴声函数

#endif
