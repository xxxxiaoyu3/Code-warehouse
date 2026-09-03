#ifndef __AT24C04_H__
#define __AT24C04_H__

#define uchar unsigned char
#define uint unsigned int
//Æ÷¼þµØÖ·
#define	AT24C04_Add_1	0XA0	//A1=0;A2=0;
#define	AT24C04_Add_2	0XA4	//A1=1;A2=0;
#define	AT24C04_Add_3	0XA8	//A1=0;A2=1;
#define	AT24C04_Add_4	0XAC	//A1=1;A2=1;
#define	EEPROM_Part1	0X00
#define	EEPROM_Part2	0X02	

void AT24C04_Init(void);
uchar AT24C04_WriteByte(uchar ID_Address,uint Address,uchar Data);
uchar AT24C04_ReadByte(uchar ID_Address,uint Address);
uchar AT24C04_PageWriteByte(uchar ID_Address,uint Page,uchar *Data,uchar length);
void AT24C04_PageReadByte(uchar ID_Address,uint Page,uchar *Data,uchar length);

#endif
