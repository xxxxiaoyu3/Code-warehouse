#include "xpt2046.h"

/**********************************************************************
* Function Name      : SPI1_Config
* Description        : 初始化 SPI1
* Input              : None
* Output             : None
* Return             : None
*********************************************************************/
void SPI1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	/* 1) 使能 GPIOA 与 SPI1 时钟（访问寄存器前必须使能时钟） */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* 2) 配置 SPI1 的 GPIO 引脚：
	 *    PA5 - SCK
	 *    PA6 - MISO
	 *    PA7 - MOSI
	 * 将 SCK/MOSI 设置为复用推挽输出，MISO 由外设自动管理
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /* 复用推挽 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; /* MISO */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 3) 配置 SPI 参数（根据 XPT2046 通信要求设置） */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; /* 全双工 */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                     /* 主机模式 */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                 /* 8 位数据 */
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                        /* 时钟空闲低 */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                      /* 第1个边沿采样 */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                         /* 软件管理 NSS */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; /* 波特率预分频 */
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                /* 高位先出，或高位先入 */
	SPI_InitStructure.SPI_CRCPolynomial = 7;                         /* CRC 多项式（未使用时可留默认）*/
	SPI_Init(SPI1, &SPI_InitStructure);

	/* 4) 启用 SPI1 外设 */
	SPI_Cmd(SPI1, ENABLE);
	TOUCH_CS_SET; /* CS片选初始值为高 */
}

 /**********************************************************************
    * Function Name      : TOUCH_Init
    * Description        : 初始化触摸屏
    * Input              : None
    * Output             : None
    * Return             : None
**********************************************************************/
void TOUCH_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;/* 上拉输入 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	SPI1_Config();
}

uint16_t SPI_Send_Read_Data(SPI_TypeDef* SPIx, uint16_t Data)
{
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) != SET)
	{
	}
	SPI_I2S_SendData(SPIx,Data);

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) != SET)
	{
	}
	return SPI_I2S_ReceiveData(SPIx);
}

static uint16_t TOUCH_ReadData(uint8_t Cmd)
{
	uint8_t i;
	uint16_t ReadValue[TOUCH_Read_Times] = {0};
	uint16_t Value=0;

	for (i = 0; i < TOUCH_Read_Times; i++)
	{
		TOUCH_CS_CLR; /* 片选拉低，开始通信 */

		SPI_Send_Read_Data(SPI1,Cmd);	/* 发送命令 */
		ReadValue[i] = SPI_Send_Read_Data(SPI1,0XFF); /* 接收数据 */
		ReadValue[i]<<=8;/* 左移8位 */
		ReadValue[i] |= SPI_Send_Read_Data(SPI1,0XFF); /* 接收数据 */
		ReadValue[i]>>=3;	/*12位精度去掉没用的低3位*/
		
		TOUCH_CS_SET; /* 片选拉高，结束通信 */
	}

	/*数值滤波处理*/
	/*去掉最大值，去掉最小值，求平均值*/
	/*从大到小排序*/
	for(i=0;i<(TOUCH_Read_Times-1);i++)
	{
		if(ReadValue[i]>ReadValue[i+1])/*交换*/
		{
			uint16_t temp;
			temp=ReadValue[i];
			ReadValue[i]=ReadValue[i+1];
			ReadValue[i+1]=temp;
		}
	}

	for(i=1;i<(TOUCH_Read_Times-2);i++)/*去掉最大值，去掉最小值*/
	{
		Value+=ReadValue[i];
	}

	Value /= (TOUCH_Read_Times - 2); /* 求平均值 */
	return Value;
}
/*
	触摸屏读取X、Y坐标
	返回值：0xFF表示无效触摸，其他值表示有效触摸
	*xValue: X坐标值
	*yValue: Y坐标值
*/
uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue)
{
	if(TOUCH_PEN == 1)
		return 0XFF;
	uint16_t xValue1, yValue1, xValue2, yValue2;
	xValue1 = TOUCH_ReadData(TOUCH_X_CMD); 
	yValue1 = TOUCH_ReadData(TOUCH_Y_CMD); 
	xValue2 = TOUCH_ReadData(TOUCH_X_CMD); 
	yValue2 = TOUCH_ReadData(TOUCH_Y_CMD);
	
	/* 查看两个点之间的只采样值差距 */
	if(xValue1 >= xValue2)
	{	
		*xValue = xValue1 - xValue2;
	}
	else
	{	
		*xValue = xValue2 - xValue1;
	}
	if(yValue1 >= yValue2)
	{
		*yValue = yValue1 - yValue2;
	}
	else
	{
		*yValue = yValue2 - yValue1;
	}
	/* 判断采样差值是否在可控范围内 */
/*	if((*xValue > TOUCH_MAX) || (*yValue > TOUCH_MAX))
	{
		return 0xFF;
	}
*/

	/* 求平均值 */
	 *xValue = (xValue1 + xValue2) / 2;
	 *yValue = (yValue1 + yValue2) / 2;
	
	/* 判断得到的值，是否在取值范围之内 */
	if((*xValue > TOUCH_X_MAX) || (*xValue < TOUCH_X_MIN)
		|| (*yValue > TOUCH_Y_MAX) || (*yValue < TOUCH_Y_MIN))
	{
		return 0xFF;
	}

	/*极限值转换坐标值*/
	/*TOUCH_X_MAX-TOUCH_X_MIN;*/
	uint16_t xTemp = TOUCH_X_MAX-TOUCH_X_MIN;
	uint16_t yTemp = TOUCH_Y_MAX-TOUCH_Y_MIN;

	float xSlope = (float)xTemp / (float)LD_HOR_RES_MAX;	// X轴斜率
	float ySlope = (float)yTemp / (float)LD_VER_RES_MAX;	// Y轴斜率

	/*2. 得到像素坐标*/
	*xValue=LD_HOR_RES_MAX-(*xValue-TOUCH_X_MIN)/xSlope;
    *yValue=LD_VER_RES_MAX-(*yValue-TOUCH_Y_MIN)/ySlope;
	printf("xValue: %d, yValue: %d\n \r", *xValue, *yValue);
		
	return 0;
}
