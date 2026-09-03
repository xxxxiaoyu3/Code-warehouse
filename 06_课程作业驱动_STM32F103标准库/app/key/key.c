#include "key.h"
#include "SysTick.h"

void Key_Init(void)
{
	//声名一个结构体用来初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能开启时钟E
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//A
	
	//key-up
	GPIO_InitStructure.GPIO_Pin=KEY_UP_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//设置下拉输入
	GPIO_Init(KEY_UP_PORT,&GPIO_InitStructure);
	
	//
	GPIO_InitStructure.GPIO_Pin=KEY0_PIN|KEY1_PIN|KEY2_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//设置上拉输入
	GPIO_Init(KEY_PORT,&GPIO_InitStructure);
}

//如果mode=0单次检测按键
//如果mode=1连续检测按键

u8 Key_scan(u8 mode)//检测按键，控制发出不同的报警声
{
	static u8 key=1;
	if(mode==1) 
		key=1;
	if(key==1&&(KEY_UP==1||KEY0==0||KEY1==0||KEY2==0))//判断key-up是否按下
	{
		delay_ms(10);//消抖
		key =0;
		if(KEY_UP==1)//再次判断是否按下
			return KEY_UP_PRESS;
		else if(KEY0==0)
			return KEY0_PRESS;
		else if(KEY1==0)
			return KEY1_PRESS;
		else if(KEY2==0)
			return KEY2_PRESS;
	}
	else if(key==0&&(KEY_UP==0||KEY0==1||KEY1==1||KEY2==1))
	{
		key =1;
	}
	return 0;	
}
