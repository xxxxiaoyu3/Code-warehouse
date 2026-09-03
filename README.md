# Code warehouse — 个人可复用代码仓库

从 `E:\Project_Keil5` 各历史项目中提取的**可复用代码库**，按用途分类存放。
整理日期：2026-09-03。所有代码均为你本人项目中所写/所用，编译产物已剔除。

## 总索引

| 仓库 | 内容 | 来源项目 | 芯片 / 库 |
|---|---|---|---|
| `01_游戏手柄BSP与菜单UI_STM32F103_HAL` | 板级驱动（Beep/Key/LED/Nrf24l01/Remote_ADC/OLED 双驱动）+ 菜单 UI 框架（MenuCore/MenuSelector/MenuSetting/Block）+ OLED 显示封装 + LOGO 字模 | Gamepad project | STM32F103 · HAL · Keil |
| `02_USB自定义HID_STM32F103` | USB Custom HID 设备完整配置：描述符、usbd_conf、usbd_custom_hid_if（报告收发） | Gamepad project | STM32F103 · USB FS Device |
| `03_XPT2046触摸驱动_STM32标准库` | XPT2046 电阻触摸驱动，两个版本：`ZET6_LVGL开发板版`（PD6/PD7，带两点校准）与 `ZuoYe独立版` | STM32F103ZET6_Project / STM32F103_ZuoYe | STM32 标准外设库 |
| `04_LVGL显示触摸移植层` | LVGL 移植层 `lv_port_disp / lv_port_indev / lv_port_fs`（V7.10.3 与 V8.3.10 两套）+ 按键 `key` 驱动 | STM32F103ZET6_Project（需要移植的 porting 文件夹） | STM32F103 · LVGL |
| `05_标准库APP外设驱动合集_STM32F103ZET6` | 26 个 APP 模块：24Cxx、adc、beep、can、dac、dma、ds18b20、**ESP8266**（STA+TCP Client）、exti、hwjs(红外)、iic、input、iwdg、key、led、pwm、pwm_dac、rs485、rtc、smg(数码管)、tftlcd、time、touch_key、wkup、wwdg、XPT2046 + Public（Delay/SysTick/system/usart） | Project_Wifi_1 | STM32F103ZET6 · 标准外设库 |
| `06_课程作业驱动_STM32F103标准库` | app：beep、exti、key、led、pwm、time、XPT2046；public：system、systick、usart | STM32F103_ZuoYe | STM32 标准外设库 |
| `07_蓝桥杯51大模板_STC89C52` | 蓝桥杯 CT107D 开发板大模板：Init、LED、Nixie(数码管)、KeyGet、iic、ds1302、DS18B20 + main 示例 | ST_demo/基本蓝桥杯大模板 | STC89C52 · Keil C51 |
| `08_51模块驱动库_去重整理` | 15 个去重后的 51 驱动：Delay、LED、Nixie_tube、KeyGet、Timer/Timer0、LCD1602、IIC、AT24C04、PCF8591、DS1302、DS18B20+OneWire、UART1、ultrasonic | ST_demo/基于开发板模块所写（各模块重复代码已校验一致后合并） | STC89C52 · Keil C51 |
| `09_DshanMCU外设驱动库_HAL` | 19 个模块化驱动：有源/无源蜂鸣器、LED/彩色LED、按键、LCD、OLED、DHT11、DS18B20、MPU6050、红外收发、红外避障、光敏、电机、旋转编码器、W25Q64 SPI Flash、UART、Timer、超声波 SR04 + ascii 字库 | STM32F4_Hall/01_freertos_template/Drivers | STM32 HAL（F1 板） |
| `10_FreeRTOS工程模板` | 两个可直接打开编译的 FreeRTOS 工程模板（含 `.ioc` 可用 CubeMX 重新生成）：`STM32F103_freertos模版`、`STM32F4_freertos模板`。已剔除 Objects/Listings/build/.cmsis/DSP 库等约 200M 编译产物与冗余 | STM32F103_Hall_FreeRTOS / STM32F4_Hall | STM32F103 / F4 · HAL · FreeRTOS |
| `11_指纹模块协议_ZW` | 指纹模块非阻塞状态机 `zw_module`（初始化/录入/匹配回调/协议解析）+ `fp_syno_protocol` 协议实现 | STM32F103_HID_ZW | 通用（串口协议，芯片无关） |

## 使用说明

- **05 与 06** 均为 STM32 标准外设库风格，需配合 `Libraries/STM32F10x_StdPeriph_Driver` 与 CMSIS 启动文件使用；移植时把所需 APP 子文件夹和 `Public` 一起拷入新工程并添加头文件路径。
- **08** 中 `Delay.c`、`LCD1602.c`、`IIC.c` 等在多个模块中重复出现，已用 MD5 校验一致后只保留一份。
- **07** 与 **08** 的引脚映射均按各自开发板编写，换板需改引脚宏。
- **10** 打开 `MDK-ARM` 下的 `.uvprojx` 即可编译；Keil 首次打开若提示缺 RTE 组件，按提示 Pack 安装即可。

## 未纳入仓库的项目（说明）

- `STM32F103_Demo`（PC13 闪烁、DAP-LINK 例程）：一次性测试例程，无复用价值。
- `ST_demo/基于Proteus所写`：Proteus 仿真练习，驱动与 `08` 重复。
- `ST_demo/电子类（省赛）`：蓝桥杯历届赛题解题代码，属练习性质，可按需从原目录查阅。
- `STM32F103ZET6_Project` 的 LVGL 各 Demo 工程：其可复用部分（移植层、XPT2046、按键）已提取至 `03`、`04`。
- 两份 `Gamepad project*.zip` 备份：与现有工程重复。
