#ifndef __KEY_H
#define __KEY_H

#include <stdio.h>
#include "stm32f10x.h"
#include "led.h"
#include "Beep.h"

// 按键数量
#define KEY_COUNT 4

// Key_up 按键（PA0）相关定义
#define Key_up_LINE GPIO_Pin_0                  // Key_up 对应引脚
#define Key_up_PORT GPIOA                       // Key_up 对应端口
#define Key_up_CLK RCC_APB2Periph_GPIOA         // Key_up 端口时钟使能

// K2/K3/K4 按键（PE2/PE3/PE4）相关定义
#define Key_K2_LINE GPIO_Pin_2                  // K2 对应引脚
#define Key_K3_LINE GPIO_Pin_3                  // K3 对应引脚
#define Key_K4_LINE GPIO_Pin_4                  // K4 对应引脚
#define Key_K2_4_PORT GPIOE                     // K2/K3/K4 共用端口
#define Key_K2_4_CLK RCC_APB2Periph_GPIOE       // K2/K3/K4 端口时钟使能

typedef enum {
    KEY_UP = 0,     // 上键
    KEY_DOWN,       // 下键
    KEY_LEFT,      // 左键
    KEY_RIGHT,     // 右键
    KEY_ENTER,     // 确认键
    None,         // 无按键
} KeyType;

/**
 * @brief  按键初始化函数，配置相关GPIO为输入模式
 */
void Key_Init(void);

/**
 * @brief  按键扫描函数，带消抖和状态机
 * @param  Key_States_Flag 按键状态标志数组指针，按下时对应元素被置1，未按下时对应元素为0
 * @note   注意：Key_States_Flag数组在置1后需要软件清零(本函数不会自己清零)
 */
KeyType Key_Scan(void);

void Key_Task_Process(void);

#endif
