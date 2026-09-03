#ifndef __LCD1602_H__
#define __LCD1602_H__

void LCD_Delay();
void LCD_Check_Busy();
void LCD_Write_Cmd(unsigned char Command);
void LCD_Write_Data(unsigned char Data);
void LCD_SetCursor(unsigned char Line,unsigned char Column);
void LCD_Inte();
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char);
void LCD_ShowString(unsigned char Line,unsigned char Column,char *Char);
int LCD_Pow(int X,int Y);
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_reset(unsigned char Num);		
#endif