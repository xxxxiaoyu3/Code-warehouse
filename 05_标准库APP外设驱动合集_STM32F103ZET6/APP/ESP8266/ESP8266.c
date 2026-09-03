#include "ESP8266.h"

struct STRUCT_USART_Fram ESP8266_Fram_Record_Struct = { 0 };

void USART2_DMA_Config(void)
{
    // Configure DMA for USART2
    // 使能DMA1时钟，为后续DMA配置做准备
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStructure;

    // 配置DMA1通道6用于USART2的接收（RX）
    DMA_DeInit(DMA1_Channel6); // 先复位通道6，保证初始状态
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR; // 外设基地址为USART2数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)ESP8266_Fram_Record_Struct.Data_RX_BUF;      // 内存基地址为接收缓冲区
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC; // 数据流向：外设到内存
    DMA_InitStructure.DMA_BufferSize         = RX_BUF_MAX_LEN;               // 缓冲区大小
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable; // 外设地址不递增
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;      // 内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据宽度：字节
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;    // 内存数据宽度：字节
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular; // 循环模式，接收数据可循环覆盖
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High; // 优先级高
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;   // 禁止内存到内存模式
    DMA_Init(DMA1_Channel6, &DMA_InitStructure); // 初始化DMA1通道6
/*
    // 配置DMA1通道7用于USART2的发送（TX）
    DMA_DeInit(DMA1_Channel7); // 先复位通道7，保证初始状态
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR; // 外设基地址为USART2数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)ESP8266_Fram_Record_Struct.Data_TX_BUF;      // 内存基地址为发送缓冲区
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST; // 数据流向：内存到外设
    DMA_InitStructure.DMA_BufferSize         = TX_BUF_MAX_LEN;               // 缓冲区大小
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable; // 外设地址不递增
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;      // 内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据宽度：字节
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;    // 内存数据宽度：字节
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal; // 单次发送数据
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High; // 优先级高
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;   // 禁止内存到内存模式
    DMA_Init(DMA1_Channel7, &DMA_InitStructure); // 初始化DMA1通道7
*/
    // 使能USART2的DMA接收和发送请求
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE); // 使能USART2接收DMA

    DMA_ClearFlag(DMA1_FLAG_TC6);//清除DMA1通道6的传输完成标志

    DMA_Cmd(DMA1_Channel6, ENABLE);          // 启动DMA1通道6（接收）
}

void USART2_Init(u32 bound)
{
    // Initialize UART for communication with ESP8266
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 
    
    //串口引脚初始化
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2 ; // PA2 for TX
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3; // PA3 for RX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &USART_InitStructure);

    USART_Cmd(USART2, ENABLE);

    // 必须在USART_Cmd之后使能中断
	//  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//使能接收中断		//开启了DMA传输就不要开接收使能中断，不然会和DMA冲突
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//使能空闲中断

    NVIC_InitTypeDef NVIC_InitStructure;        
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 移动到USART配置之后
    USART2_DMA_Config();
}
void USART2_IRQHandler(void)
{
    if( USART_GetITStatus( USART2, USART_IT_IDLE ) != RESET )                                         //
        {
            ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 1;
            USART_ReceiveData( USART2 );                                                              //清除空闲中断标志
            ESP8266_Fram_Record_Struct .InfBit .FramLength = RX_BUF_MAX_LEN-DMA_GetCurrDataCounter(DMA1_Channel6);	//计算接收长度
			PC_USART("%s",ESP8266_Fram_Record_Struct .Data_RX_BUF);
            TcpClosedFlag = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;//判断TCP连接是否关闭

        }
}

void ESP8266_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(ESP8266_RST_Pin_Periph_Clock|ESP8266_CH_PD_Pin_Periph_Clock, ENABLE);

	GPIO_InitStructure.GPIO_Pin = ESP8266_RST_Pin;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO速度设置为50MHz
	GPIO_Init(ESP8266_RST_Pin_Port, &GPIO_InitStructure);

   	GPIO_InitStructure.GPIO_Pin = ESP8266_CH_PD_Pin;			   
	GPIO_Init(ESP8266_CH_PD_Pin_Port, &GPIO_InitStructure);
	
	USART2_Init(bound);

	ESP8266_RST_Pin_SetH;
	ESP8266_CH_PD_Pin_SetH;	
}

//对ESP8266发送AT指令
// cmd:待发送的命令
// ack1->ack2:期待返回的指令，NULL为不需要
// time:需要延时的时间
// 返回:1:发送成功 0:发送失败
bool ESP8266_Send_AT_Cmd(char *cmd,char *ack1,char *ack2,u32 time)
{
	ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;	//接收长度清零
    ESP8266_USART("%s\r\n", cmd);		//发送AT指令

    if(ack1==0&&ack2==0)	 //不需要接收数据
	{
		return true;
	}
	Delay_ms(time);	  //等待time毫秒

	ESP8266_Fram_Record_Struct.Data_RX_BUF[ESP8266_Fram_Record_Struct.InfBit.FramLength ] = '\0'; //添加结束符

    PC_USART("%s", ESP8266_Fram_Record_Struct .Data_RX_BUF);

	if(ack1!=0&&ack2!=0)
	{
		return ( ( bool ) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack1 ) || 
						 ( bool ) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack2 ) );
	}
	else if( ack1 != 0 )
		return ( ( bool ) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack1 ) );
	
	else
		return ( ( bool ) strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, ack2 ) );
}

//复位ESP8266
void ESP8266_Rst(void)
{
	ESP8266_RST_Pin_SetL;
	Delay_ms(500); 
	ESP8266_RST_Pin_SetH;
}


//测试AT是否在线
void ESP8266_AT_Test(void)
{
	char count=0;
	ESP8266_Rst();

	Delay_ms(1000);	
	while(count < 10)
	{
		if(ESP8266_Send_AT_Cmd("AT","OK",NULL,500)) 
		{
			PC_USART("ESP8266...OK!!");
			return;
		}	
		ESP8266_Rst();
		++ count;
	}
	PC_USART("ESP8266...ERR");
}


//选择ESP8266的工作模式
// enumMode：工作模式
//返回值：1：成功 0：失败
bool ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode)

void ESP8266_ReceiveData(char* buffer, size_t length)
{
    // Receive data from the ESP8266 module
}
