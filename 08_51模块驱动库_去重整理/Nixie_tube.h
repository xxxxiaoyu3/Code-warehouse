#ifndef __Nixie_tube_H__
#define __Nixie_tube_H__

void Nixie_tube_Init(void);
void Nixie_tube_show(unsigned char Num,unsigned char Data);
void Nixie_tube_dynamic_show(unsigned char *Data);
void Time_segment(unsigned char Count,unsigned char *num);

#endif
