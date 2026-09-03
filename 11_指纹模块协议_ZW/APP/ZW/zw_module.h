#ifndef __ZW_MODULE_H__
#define __ZW_MODULE_H__

#include <stdint.h>

/**
 * @brief 指纹匹配成功的回调函数，需在外部应用层（如 main.c）实现
 * @param finger_id 匹配成功的指纹 ID
 */
extern void fingerprint_match_success_callback(uint16_t finger_id);

/**
 * @brief 初始化指纹模块
 */
void ZW_Init(void);

/**
 * @brief 指纹模块核心非阻塞状态机
 * 需在 main.c 的 while(1) 循环中持续调用
 */
void ZW_Process(void);

/**
 * @brief 触发指纹录入流程
 * @param enroll_id 指定存储的指纹 ID
 */
void ZW_StartEnroll(uint16_t enroll_id);

/**
 * @brief 串口数据解析接口
 * 当底层串口驱动接收到数据时，应调用此函数将数据喂给协议栈
 * @param data 接收到的原始数据缓冲区
 * @param len 数据长度
 */
void ZW_ProtocolParse(uint8_t *data, uint16_t len);

#endif /* __ZW_MODULE_H__ */
