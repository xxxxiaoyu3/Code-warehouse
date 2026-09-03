#include "usart.h"		 

int fputc(int ch,FILE *p)  //重定向printf函数使用
{
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}

/*******************************************************************************
* 函数名称         : USART1_Init
* 函数功能		   : USART1初始化函数
* 输入参数         : bound:波特率
* 输出参数         : 无
*******************************************************************************/ 
void USART1_Init(u32 bound)
{
   //GPIO端口配置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 // 使能AFIO时钟，用于GPIO复用功能
 
	
	/*  配置GPIO模式为IO模式 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //配置USART1_TX PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化USART1_TX IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //配置USART1_RX PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //浮空输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */
	

   //USART1 初始化配置
	USART_InitStructure.USART_BaudRate = bound;//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据位为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化USART1
	
	USART_Cmd(USART1, ENABLE);  //使能USART1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能接收中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//USART1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//初始化指定的NVIC寄存器的值	
}

/*******************************************************************************
* 函数名称         : USART1_IRQHandler
* 函数功能		   : USART1中断服务函数
* 输入参数         : 无
* 输出参数         : 无
*******************************************************************************/ 
void USART1_IRQHandler(void)                	//USART1中断服务函数
{
	u8 r;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		r =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		USART_SendData(USART1,r);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	} 
	USART_ClearFlag(USART1,USART_FLAG_TC);
} 	

 



