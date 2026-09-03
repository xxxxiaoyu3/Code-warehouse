#ifndef __LED_H_
#define __LED_H_

#include "stm32f10x.h"
#include "system.h" 
#define LED_Pin_ALL GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7

void LED_Init(uint16_t LED_Pin);
void LED_SET(uint16_t LED_Pin,uint8_t flag);

#define LED_1	PCout(0)
#define LED_2	PCout(1)
#define LED_3	PCout(2)
#define LED_4	PCout(3)
#define LED_5	PCout(4)

#endif
