#include "Key.h"

// 按键端口数组，依次对应四个按键
static GPIO_TypeDef* const Key_PORTS[KEY_COUNT] = {
    Key_up_PORT, Key_K2_4_PORT, Key_K2_4_PORT, Key_K2_4_PORT
};

// 按键引脚数组，依次对应四个按键
static const uint16_t Key_PINS[KEY_COUNT] = {
    Key_up_LINE, Key_K2_LINE, Key_K3_LINE, Key_K4_LINE
};

// 简单按键状态机的状态定义
typedef enum {
    KEY_IDLE = 0,     // 空闲状态
    KEY_DEBOUNCE,     // 消抖状态
    KEY_PRESSED       // 已按下状态
} KeyState;

//


// 每个按键的当前状态
static KeyState key_state[KEY_COUNT] = {KEY_IDLE};
/*************外部变量声明*************/

KeyType Read_key = None;

// 按键初始化，配置GPIO
void Key_Init(void)
{
    // Initialize the key (GPIO) here
    GPIO_InitTypeDef GPIO_InitStructure;

    // 开启 Key_up_CLK and Key_K2_4_CLK 的时钟
    RCC_APB2PeriphClockCmd(Key_up_CLK|Key_K2_4_CLK, ENABLE);
    // 配置 PA0 为下拉输入
    GPIO_InitStructure.GPIO_Pin = Key_up_LINE; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;          
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Key_up_PORT, &GPIO_InitStructure);

    // 配置 PE2, PE3, PE4 为上拉输入
    GPIO_InitStructure.GPIO_Pin = Key_K2_LINE | Key_K3_LINE | Key_K4_LINE; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Key_K2_4_PORT, &GPIO_InitStructure);
}

/**
 * 邱Sir
 * @brief  Key_Scan函数实现原理：
 * 采用简单状态机方式对每个按键进行扫描和消抖
 * 每个按键有独立的状态(KEY_IDLE/KEY_DEBOUNCE/KEY_PRESSED)
 * 确认按键按下后传递给数组Key_States_Flag
 * Key_States_Flag数组用于存储每个按键的状态，1表示按下，0表示未按下
 * 注意：Key_States_Flag数组在置1后需要软件清零(本函数不会自己清零)
 */
//i=0:Key_UP
//i=1:Key_LEFT
//i=2:Key_DOWN
//i=3:Key_RIGHT
KeyType Key_Scan(void)
{
    KeyType act_key = None; // 默认返回None
    for (uint8_t i = 0; i < KEY_COUNT; i++)
    {
        // 读取按键当前电平
        uint8_t pinState = GPIO_ReadInputDataBit(Key_PORTS[i], Key_PINS[i]);
        if (i == 0) pinState = !pinState; // Key_up为高电平有效

        switch (key_state[i])
        {
            case KEY_IDLE:
                // 检测到按下，进入消抖
                if (pinState == Bit_RESET) {
                    key_state[i] = KEY_DEBOUNCE;
					act_key = None;		//如果不确定是按下就输出None
                }
                break;
            case KEY_DEBOUNCE:
				if (pinState == Bit_RESET) {
					// 按键确认按下
					switch (i)
					{
						case 0: act_key = KEY_UP; break;
						case 1: act_key = KEY_LEFT; break;
						case 2: act_key = KEY_DOWN; break;
						case 3: act_key = KEY_RIGHT; break;
					}
					key_state[i] = KEY_PRESSED;  //在这里进入 PRESSED 状态
				} else {
					key_state[i] = KEY_IDLE; // 抖动误触
					act_key = None;
				}
				break;
            case KEY_PRESSED:
                // 松开后回到空闲
                if (pinState == Bit_SET) {
                    key_state[i] = KEY_IDLE;
					act_key = None;
                }

                break;
        }
    }
    return act_key; // 返回当前按键状态
}

// 按键任务处理函数(这里处理按键按下后要做什么)
void Key_Task_Process(void)
{
    switch (Read_key) // 根据按键类型执行不同操作
    {
    case KEY_UP:
        /* code */
        //LED_2 = !LED_2;
		printf(" Read_key: %d\r\n", Read_key);
        //Beep_Dot(5000, 100);
		//Read_key = None;
        break;
    case KEY_DOWN:
        /* code */
        //LED_3 = !LED_3;
		printf(" Read_key: %d\r\n", Read_key);
        //Beep_Dot(5000, 100);
		//Read_key = None;
        break;
    case KEY_LEFT:
        /* code */
        //LED_4 = !LED_4;
		printf(" Read_key: %d\r\n", Read_key);

        //Beep_Dot(5000, 100);
		//Read_key = None;
        break;
    case KEY_RIGHT:
        /* code */
        //LED_5 = !LED_5;
		printf(" Read_key: %d\r\n", Read_key);
        //Beep_Dot(5000, 100);
		//Read_key = None;
        break;
    case KEY_ENTER:
        /* code */
        break;
    default:
        break;
    }
}

