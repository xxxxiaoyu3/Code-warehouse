#ifndef __UART1_H__
#define __UART1_H__

#define uchar unsigned char
#define uint unsigned int

void Uart1_Init(void);
void Uart1_SendByte(unsigned char Byte);
void Uart1_SendString(unsigned char *Str);
#endif
