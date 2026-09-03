#ifndef __IIC_H__
#define __IIC_H__

#define uchar unsigned char
#define uint unsigned int

sbit SDA=P2^1;
sbit SCL=P2^0;

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendByte(uchar Byte);
uchar IIC_ReceiveByte(void);
void IIC_SendAck(uchar AckBit);
uchar IIC_ReceiveAck(void);

#endif
