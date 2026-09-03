#include "zw_module.h"
#include <string.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"

/* ----------------- 宏定义 ----------------- */
#define ZW_HEAD     0xEF01
#define ZW_ADDR     0xFFFFFFFF

#define ZW_CMD_AUTOSEARCH   0x55
#define ZW_CMD_AUTOLOGIN    0x54

/* 协议标识 */
#define ZW_IDENT_CMD        0x01
#define ZW_IDENT_ACK        0x07

/* 应答确认码 */
#define ZW_ACK_SUCCESS      0x00
#define ZW_ACK_NO_FINGER    0x02
#define ZW_ACK_NOT_SEARCH   0x09

/* 接收缓冲区大小 */
#define ZW_RX_BUF_SIZE      64

/* ----------------- 类型定义 ----------------- */
typedef enum {
    ZW_STATE_IDLE,
    ZW_STATE_WAIT_MATCH_ACK,
    ZW_STATE_WAIT_ENROLL_ACK
} ZW_Status_t;

typedef enum {
    RX_STATE_HEAD_H,
    RX_STATE_HEAD_L,
    RX_STATE_ADDR,
    RX_STATE_IDENT,
    RX_STATE_LEN_H,
    RX_STATE_LEN_L,
    RX_STATE_DATA,
    RX_STATE_SUM
} ZW_RxState_t;

/* ----------------- 静态变量 ----------------- */
static ZW_Status_t g_zw_status = ZW_STATE_IDLE;
static uint16_t g_enroll_id = 0;

/* 接收解析相关变量 */
static uint8_t  g_rx_buffer[ZW_RX_BUF_SIZE];
static uint16_t g_rx_cnt = 0;
static uint16_t g_rx_len = 0;
static uint16_t g_rx_sum_calc = 0;
static uint16_t g_rx_sum_recv = 0;
static uint8_t  g_frame_ready = 0;

/* 外部 UART 引用与 发送函数 */
extern UART_HandleTypeDef huart1;
// 声明底层已有的发送接口
extern void USART_PutString(uint8_t *data, uint16_t size);

/* ----------------- 私有函数 ----------------- */

/**
 * @brief 计算校验和
 */
static uint16_t ZW_CalcChecksum(uint8_t *data, uint16_t len) {
    uint16_t sum = 0;
    for (uint16_t i = 0; i < len; i++) {
        sum += data[i];
    }
    return sum;
}

/**
 * @brief 发送 AutoSearch 指令 (0x55)
 */
static void ZW_SendAutoSearch(void) {
    uint8_t pkt[11+6]; // 包头(2) + 地址(4) + 标识(1) + 长度(2) + 指令(1) + 待指(1) + 开始(2) + 个数(2) + 校验和(2)
    uint16_t pos = 0;

    // Header
    pkt[pos++] = (uint8_t)(ZW_HEAD >> 8);
    pkt[pos++] = (uint8_t)(ZW_HEAD);
    // Addr
    pkt[pos++] = (uint8_t)(ZW_ADDR >> 24);
    pkt[pos++] = (uint8_t)(ZW_ADDR >> 16);
    pkt[pos++] = (uint8_t)(ZW_ADDR >> 8);
    pkt[pos++] = (uint8_t)(ZW_ADDR);
    // Ident
    pkt[pos++] = ZW_IDENT_CMD;
    // Length (Cmd(1) + Wait(1) + Start(2) + Count(2) + Sum(2) = 8)
    pkt[pos++] = 0x00;
    pkt[pos++] = 0x08;
    // Data
    pkt[pos++] = ZW_CMD_AUTOSEARCH;
    pkt[pos++] = 0x00; // 待指时长 (0则快速扫描一次)
    pkt[pos++] = 0x00; // 起始序号 H
    pkt[pos++] = 0x00; // 起始序号 L
    pkt[pos++] = 0x00; // 搜索个数 H
    pkt[pos++] = 0x64; // 搜索个数 L (100)

    // Checksum (Ident + Length + Data)
    uint16_t sum = ZW_CalcChecksum(&pkt[6], 9);
    pkt[pos++] = (uint8_t)(sum >> 8);
    pkt[pos++] = (uint8_t)(sum);

    USART_PutString(pkt, pos);
}

/**
 * @brief 发送 AutoLogin 指令 (0x54)
 */
static void ZW_SendAutoLogin(uint16_t id) {
    uint8_t pkt[20];
    uint16_t pos = 0;

    pkt[pos++] = (uint8_t)(ZW_HEAD >> 8);
    pkt[pos++] = (uint8_t)(ZW_HEAD);
    pkt[pos++] = (uint8_t)(ZW_ADDR >> 24);
    pkt[pos++] = (uint8_t)(ZW_ADDR >> 16);
    pkt[pos++] = (uint8_t)(ZW_ADDR >> 8);
    pkt[pos++] = (uint8_t)(ZW_ADDR);
    pkt[pos++] = ZW_IDENT_CMD;
    // Length (Cmd(1) + Wait(1) + Intv(1) + Cnt(1) + Storage(2) + Repeat(1) + Sum(2) = 9)
    pkt[pos++] = 0x00;
    pkt[pos++] = 0x09;
    // Data
    pkt[pos++] = ZW_CMD_AUTOLOGIN;
    pkt[pos++] = 0x1E; // 待指时长 (30 * 100ms = 3s)
    pkt[pos++] = 0x05; // 采样间隔
    pkt[pos++] = 0x03; // 按指次数 (如录入3次)
    pkt[pos++] = (uint8_t)(id >> 8);
    pkt[pos++] = (uint8_t)(id);
    pkt[pos++] = 0x00; // 重复标志 (0是不允许重复录入)

    uint16_t sum = ZW_CalcChecksum(&pkt[6], 10);
    pkt[pos++] = (uint8_t)(sum >> 8);
    pkt[pos++] = (uint8_t)(sum);

    USART_PutString(pkt, pos);
}

/* ----------------- 公共接口 ----------------- */

void ZW_Init(void) {
    g_zw_status = ZW_STATE_IDLE;
    g_frame_ready = 0;
    g_rx_cnt = 0;
}

void ZW_ProtocolParse(uint8_t *data, uint16_t len) {
    static ZW_RxState_t state = RX_STATE_HEAD_H;
    
    for (uint16_t i = 0; i < len; i++) {
        uint8_t byte = data[i];
        
        switch (state) {
            case RX_STATE_HEAD_H:
                if (byte == 0xEF) {
                    g_rx_cnt = 0;
                    g_rx_buffer[g_rx_cnt++] = byte;
                    state = RX_STATE_HEAD_L;
                }
                break;
            case RX_STATE_HEAD_L:
                if (byte == 0x01) {
                    g_rx_buffer[g_rx_cnt++] = byte;
                    state = RX_STATE_ADDR;
                } else state = RX_STATE_HEAD_H;
                break;
            case RX_STATE_ADDR:
                g_rx_buffer[g_rx_cnt++] = byte;
                if (g_rx_cnt == 6) state = RX_STATE_IDENT;
                break;
            case RX_STATE_IDENT:
                g_rx_buffer[g_rx_cnt++] = byte;
                state = RX_STATE_LEN_H;
                break;
            case RX_STATE_LEN_H:
                g_rx_buffer[g_rx_cnt++] = byte;
                g_rx_len = byte << 8;
                state = RX_STATE_LEN_L;
                break;
            case RX_STATE_LEN_L:
                g_rx_buffer[g_rx_cnt++] = byte;
                g_rx_len |= byte;
                // 长度包含数据和校验和(2B)，所以数据长度是 g_rx_len - 2
                state = RX_STATE_DATA;
                break;
            case RX_STATE_DATA:
                g_rx_buffer[g_rx_cnt++] = byte;
                if (g_rx_cnt == (9 + g_rx_len - 2)) state = RX_STATE_SUM;
                break;
            case RX_STATE_SUM:
                g_rx_buffer[g_rx_cnt++] = byte;
                if (g_rx_cnt == (9 + g_rx_len)) {
                    // 校验和校验 (Sum of Ident + Len + Data)
                    g_rx_sum_calc = ZW_CalcChecksum(&g_rx_buffer[6], g_rx_len + 3 - 2); 
                    // Wait, length is g_rx_len. Buffer index 6 is Ident. 
                    // Ident(1) + Len(2) + Data(g_rx_len - 2) = g_rx_len + 1. 
                    g_rx_sum_calc = ZW_CalcChecksum(&g_rx_buffer[6], g_rx_len + 1); 
                    g_rx_sum_recv = (g_rx_buffer[g_rx_cnt-2] << 8) | g_rx_buffer[g_rx_cnt-1];
                    
                    if (g_rx_sum_calc == g_rx_sum_recv) {
                        g_frame_ready = 1;
                    }
                    state = RX_STATE_HEAD_H;
                }
                break;
        }
        
        if (g_rx_cnt >= ZW_RX_BUF_SIZE) {
            state = RX_STATE_HEAD_H;
            g_rx_cnt = 0;
        }
    }
}

void ZW_Process(void) {
    if (g_frame_ready) {
        g_frame_ready = 0;
        uint8_t ident = g_rx_buffer[6];
        uint8_t confirm_code = g_rx_buffer[9];

        if (ident == ZW_IDENT_ACK) {
            if (g_zw_status == ZW_STATE_WAIT_MATCH_ACK) {
                if (confirm_code == ZW_ACK_SUCCESS) {
                    // 解析 ID
                    uint16_t found_id = (g_rx_buffer[10] << 8) | g_rx_buffer[11];
                    fingerprint_match_success_callback(found_id);
                }
                // 无论是否成功，回到 IDLE 重新探测
                g_zw_status = ZW_STATE_IDLE;
            } 
            else if (g_zw_status == ZW_STATE_WAIT_ENROLL_ACK) {
                if (confirm_code == ZW_ACK_SUCCESS) {
                    // 外部可定义成功回调，或在此打印
                    // fingerprint_enroll_success_callback(g_enroll_id);
                }
                g_zw_status = ZW_STATE_IDLE;
            }
        }
    }

    // 状态机流转
    static uint32_t last_tick = 0;
    switch (g_zw_status) {
        case ZW_STATE_IDLE:
            // 简单的轮询探测频率控制 (例如每200ms发起一次搜索)
            if (HAL_GetTick() - last_tick > 200) {
                ZW_SendAutoSearch();
                g_zw_status = ZW_STATE_WAIT_MATCH_ACK;
                last_tick = HAL_GetTick();
            }
            break;

        case ZW_STATE_WAIT_MATCH_ACK:
        case ZW_STATE_WAIT_ENROLL_ACK:
            // 超时处理：如果5秒无应答，强制退回 IDLE
            if (HAL_GetTick() - last_tick > 5000) {
                g_zw_status = ZW_STATE_IDLE;
            }
            break;
    }
}

void ZW_StartEnroll(uint16_t enroll_id) {
    g_enroll_id = enroll_id;
    ZW_SendAutoLogin(enroll_id);
    g_zw_status = ZW_STATE_WAIT_ENROLL_ACK;
}
