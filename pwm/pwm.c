#include <reg52.h>
#include <stdio.h>
#include "pwm.h"
#include "monitor/monitor.h"
#include "bluetooth/bluetooth.h"

// 全局变量
unsigned char pwm = 5;      // 默认50%占空比
unsigned char pwm_counter = 0;
  

// void SetPwm(void) {

//     if(++pwm_counter >= 10) pwm_counter = 0;
    
//     if(motor_speed <= 0) {
//         Motor_Stop();
//     } else {
//         Motor_Forward();
//     }

// }

// ============ 设置电机速度 ============
void MotorSpeedSet(unsigned char speed) {
    
    switch(speed) {
        case '1': 
        case 1: 
            if(pwm < 10) pwm += 1;  // 加速，最大10
            break;
        case '2': 
        case 2: 
            if(pwm > 1) pwm -= 1;   // 减速，最小1
            break;
        default:
            pwm = 5;
            break;
    }
    // 如果还没设置方向，默认前进
    if(motor_direction == 0) {
        motor_direction = 1;
    }
    // 设置速度后使能电机
    pwm_enable = 1;
    // 反馈设置的速度
    UART_SendString("速度设置为: ");
    UART_SendChar(speed);
    UART_SendString(" (占空比 ");
    UART_SendChar(pwm + '0');
    UART_SendString("0%)\r\n");
}
