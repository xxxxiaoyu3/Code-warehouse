#include "LED.h"
void LED_Init(uint16_t LED_Pin)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=LED_Pin;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	LED_SET(LED_Pin,0);
}
/**
  * @brief  
  * @param LED_Pin 选择亮的引脚
  * @param flag :1或0 1亮0闭
  * @retval 
  */
void LED_SET(uint16_t LED_Pin,uint8_t flag)
{
	if(flag == 1)
	{
		GPIO_WriteBit(GPIOC,LED_Pin,Bit_RESET);
	}else 
	{
		GPIO_WriteBit(GPIOC,LED_Pin,Bit_SET);
	}
}
