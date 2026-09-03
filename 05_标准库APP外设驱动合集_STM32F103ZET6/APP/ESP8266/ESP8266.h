#ifndef __ESP8266_H_
#define __ESP8266_H_

#include <stdbool.h>      //
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "system.h" 
#include "Delay.h"

#include "esp8266_public.h"
#include "sta_tcpclent_test.h"


#define RX_BUF_MAX_LEN 1024		  //接收缓冲区最大长度
#define TX_BUF_MAX_LEN 1024		  //发送缓冲区最大长度

#define ESP8266_USART(fmt, ...)	 USART_printf (USART2, fmt, ##__VA_ARGS__)
#define PC_USART(fmt, ...)	 printf (fmt, ##__VA_ARGS__)


#define ESP8266_RST_Pin          GPIO_Pin_6
#define ESP8266_RST_Pin_Port     GPIOE
#define ESP8266_RST_Pin_Periph_Clock  RCC_APB2Periph_GPIOE

#define ESP8266_CH_PD_Pin     GPIO_Pin_7
#define ESP8266_CH_PD_Pin_Port     GPIOG
#define ESP8266_CH_PD_Pin_Periph_Clock  RCC_APB2Periph_GPIOG


#define ESP8266_RST_Pin_SetH     GPIO_SetBits(ESP8266_RST_Pin_Port,ESP8266_RST_Pin)
#define ESP8266_RST_Pin_SetL     GPIO_ResetBits(ESP8266_RST_Pin_Port,ESP8266_RST_Pin)


#define ESP8266_CH_PD_Pin_SetH     GPIO_SetBits(ESP8266_CH_PD_Pin_Port,ESP8266_CH_PD_Pin)
#define ESP8266_CH_PD_Pin_SetL     GPIO_ResetBits(ESP8266_CH_PD_Pin_Port,ESP8266_CH_PD_Pin)


extern struct STRUCT_USART_Fram	  //串口帧结构体
{
	char Data_RX_BUF[RX_BUF_MAX_LEN];
    char Data_TX_BUF[TX_BUF_MAX_LEN];
	union 
	{
    	__IO u16 InfAll;
    	struct 
		{
		  	__IO u16 FramLength       :15;                               // 14:0 
		  	__IO u16 FramFinishFlag   :1;                                // 15 
	  	}InfBit;
  	}; 
}ESP8266_Fram_Record_Struct;

typedef enum
{
    ESP8266_MODE_STA = 0,
    ESP8266_MODE_AP,
    ESP8266_MODE_AP_STA
} ENUM_Net_ModeTypeDef;

typedef enum
{
    enumTCP = 0,
    enumUDP
} ENUM_NetPro_TypeDef;



void ESP8266_Init(u32 bound);
bool ESP8266_Send_AT_Cmd(char *cmd,char *ack1,char *ack2,u32 time);
void ESP8266_Rst(void);
void ESP8266_AT_Test(void);


#endif
