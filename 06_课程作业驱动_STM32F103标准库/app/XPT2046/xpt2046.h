#ifndef __xpt2046_h
#define __xpt2046_h

#include "stm32f10x.h"
#include <stdio.h>

#define TOUCH_CS_CLR GPIO_ResetBits(GPIOD, GPIO_Pin_6)  /*片选信号拉低*/ 
#define TOUCH_CS_SET GPIO_SetBits(GPIOD, GPIO_Pin_6)    /*片选信号拉高*/ 
#define TOUCH_Read_Times 5  /* 读取次数 (次数越多精度越高反应越慢)*/
#define TOUCH_X_CMD 0x90
#define TOUCH_Y_CMD 0xD0
#define TOUCH_MAX 5 /* 误差最大值 */
#define TOUCH_X_MAX 2370    /* X 轴最大值 */
#define TOUCH_X_MIN 150       /* X 轴最小值 */
#define TOUCH_Y_MAX 2460    /* Y 轴最大值 */
#define TOUCH_Y_MIN 270       /* Y 轴最小值 */
#define TOUCH_PEN GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) /* 触摸笔按下状态 */
#define LD_HOR_RES_MAX          (240)       // 水平分辨率最大值(X)
#define LD_VER_RES_MAX          (400)       // 垂直分辨率最大值(Y)

void TOUCH_Init(void);
uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue);

#endif
