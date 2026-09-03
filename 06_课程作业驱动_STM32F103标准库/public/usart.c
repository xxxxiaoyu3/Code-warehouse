#include "usart.h"

int fputc(int ch, FILE *p)
{
    USART_SendData(USART1, (u8)ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
	  return ch;
}

void USART1_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/*配置GPIO的模式10口*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX  //串口输出PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);/*初始化串口输入10*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); /*初始化GPIO*/ 
	
	
	//USART1初始化设置
	USART_InitStructure.USART_BaudRate=bound;//波特率设置
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode= USART_Mode_Rx|USART_Mode_Tx;//收发模式
	USART_Init(USART1, &USART_InitStructure);//初始化串口1
	USART_Cmd(USART1,ENABLE);//使能串口1
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启相关中断
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);//根据指定的参数初始化VIC寄存器
}


void USART1_IRQHandler(void)
{
	u8 r;
	if(USART_GetFlagStatus(USART1,USART_IT_RXNE )!=RESET)
	{
		r=USART_ReceiveData(USART1);//USART1>DR接收中断
		USART_SendData(USART1,r);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	}
	USART_ClearFlag(USART1,USART_FLAG_TC);
}




