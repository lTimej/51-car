#include <reg52.h>
#include <stdio.h>
#include "pwm.h"
#include "monitor/monitor.h"
#include "bluetooth/bluetooth.h"

// 全局变量
unsigned char pwm = 8;           // 默认挡位3对应80%占空比
unsigned char speed_level = 3;   // 当前挡位 1-5
unsigned char pwm_counter = 0;

// 5个挡位对应的PWM值：60%, 70%, 80%, 90%, 100%
unsigned char code pwm_table[5] = {6, 7, 8, 9, 10};

// ============ 设置电机速度 ============
void MotorSpeedSet(unsigned char speed) {
    
    switch(speed) {
        case '1': 
        case 1: 
            if(speed_level < 5) speed_level++;  // 加速，最大5挡
            break;
        case '2': 
        case 2: 
            if(speed_level > 1) speed_level--;  // 减速，最小1挡
            break;
        default:
            speed_level = 3;  // 默认3挡
            break;
    }
    // 根据挡位设置PWM值
    pwm = pwm_table[speed_level - 1];
    
    // 如果还没设置方向，默认前进
    if(motor_direction == 0) {
        motor_direction = 1;
    }
    // 设置速度后使能电机
    pwm_enable = 1;
    // 反馈设置的速度
    UART_SendString("挡位: ");
    UART_SendChar(speed_level + '0');
    UART_SendString(" (占空比 ");
    if(pwm == 10) {
        UART_SendString("100");
    } else {
        UART_SendChar(pwm + '0');
        UART_SendChar('0');
    }
    UART_SendString("%)\r\n");
}
