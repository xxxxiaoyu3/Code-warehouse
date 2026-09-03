#ifndef __ultrasonic_H__
#define __ultrasonic_H__

#define uchar unsigned char
#define uint unsigned int
sbit TX_Ultra=P1^0;
sbit RX_Ultra=P1^1;

uint Ultra_distance(void);

#endif
