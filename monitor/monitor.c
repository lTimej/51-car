#include "public/public.h"
#include "bluetooth/bluetooth.h"
#include "monitor.h"

// 全局变量定义
unsigned char motor_speed = 0;       // 当前速度 0-100
unsigned char motor_direction = 0;   // 方向 0:停止 1:正转 2:反转
unsigned char pwm_counter = 0;       // PWM计数器
bit pwm_enable = 0;                  // PWM使能标志


// 电机控制函数
void Motor_Stop(void) {
    MOTOR_A_IA = 0;
    MOTOR_A_IB = 0;
    MOTOR_B_IA = 0;
    MOTOR_B_IB = 0;


    MOTOR_C_IA = 0;
    MOTOR_C_IB = 0;
    MOTOR_D_IA = 0;
    MOTOR_D_IB = 0;
}

void Motor_Forward(void) {
    // 电机A正转
    MOTOR_A_IA = 1;
    MOTOR_A_IB = 0;
    // 电机B正转（如果有第二个电机）
    MOTOR_B_IA = 1;
    MOTOR_B_IB = 0;

    // 电机A正转
    MOTOR_C_IA = 1;
    MOTOR_C_IB = 0;
    // 电机B正转（如果有第二个电机）
    MOTOR_D_IA = 1;
    MOTOR_D_IB = 0;
    
    UART_SendString("前进\r\n");
}

void Motor_Backward(void) {
    // 电机A反转
    MOTOR_A_IA = 0;
    MOTOR_A_IB = 1;
    // 电机B反转
    MOTOR_B_IA = 0;
    MOTOR_B_IB = 1;
    // 电机A反转
    MOTOR_C_IA = 0;
    MOTOR_C_IB = 1;
    // 电机B反转
    MOTOR_D_IA = 0;
    MOTOR_D_IB = 1;
    
    UART_SendString("后退\r\n");
}

void Motor_TurnLeft(void) {
    // 左转：右轮正转，左轮反转或停止
    MOTOR_A_IA = 0;  // 左轮停止
    MOTOR_A_IB = 0;
    MOTOR_B_IA = 1;  // 右轮正转
    MOTOR_B_IB = 0;

    // 左转：右轮正转，左轮反转或停止
    MOTOR_C_IA = 0;  // 左轮停止
    MOTOR_C_IB = 0;
    MOTOR_D_IA = 1;  // 右轮正转
    MOTOR_D_IB = 0;
    
    UART_SendString("左转\r\n");
}

void Motor_TurnRight(void) {
    // 右转：左轮正转，右轮反转或停止
    MOTOR_A_IA = 1;  // 左轮正转
    MOTOR_A_IB = 0;
    MOTOR_B_IA = 0;  // 右轮停止
    MOTOR_B_IB = 0;

    // 右转：左轮正转，右轮反转或停止
    MOTOR_C_IA = 1;  // 左轮正转
    MOTOR_C_IB = 0;
    MOTOR_D_IA = 0;  // 右轮停止
    MOTOR_D_IB = 0;
    
    UART_SendString("右转\r\n");
}



// 定时器初始化（用于产生PWM）
void Timer0_Init(void) {
    TMOD |= 0x01;      // 定时器0，模式1（16位定时器）
    // 设置定时器初值，产生约10KHz的PWM
    TH0 = (65536 - 100) >> 8;  // 100us中断一次 -> 10KHz
    TL0 = (65536 - 100) & 0xFF;
    ET0 = 1;           // 使能定时器0中断
    TR0 = 1;           // 启动定时器0
    EA = 1;            // 开启总中断
}

// 定时器0中断服务函数
void Timer0_ISR(void) interrupt 1 {
    // 重装初值
    TH0 = (65536 - 100) >> 8;
    TL0 = (65536 - 100) & 0xFF;
    
    if(pwm_enable) {
        pwm_counter++;
        if(pwm_counter >= 100) pwm_counter = 0;  // 100级PWM分辨率
        
        // 根据方向和速度控制电机
        if(motor_direction == 1) {  // 正转
            if(pwm_counter < motor_speed) {
                MOTOR_A_IA = 1;
                MOTOR_A_IB = 0;
            } else {
                MOTOR_A_IA = 0;
                MOTOR_A_IB = 0;
            }
        } 
        else if(motor_direction == 2) {  // 反转
            if(pwm_counter < motor_speed) {
                MOTOR_A_IA = 0;
                MOTOR_A_IB = 1;
            } else {
                MOTOR_A_IA = 0;
                MOTOR_A_IB = 0;
            }
        }
        else {  // 停止
            MOTOR_A_IA = 0;
            MOTOR_A_IB = 0;
        }
    }
}

// 完整的Motor_SetSpeed函数
void Motor_SetSpeed(unsigned char speed_level) {
    unsigned char speed_value;
    
    switch(speed_level) {
        case '1':  // 低速 - 30%占空比
            speed_value = 30;
            UART_SendString("低速模式\r\n");
            break;
            
        case '2':  // 中速 - 60%占空比
            speed_value = 60;
            UART_SendString("中速模式\r\n");
            break;
            
        case '3':  // 高速 - 90%占空比
            speed_value = 90;
            UART_SendString("高速模式\r\n");
            break;
            
        case '0':  // 停止
            speed_value = 0;
            motor_direction = 0;
            UART_SendString("停止\r\n");
            break;
            
        case '+':  // 加速（渐进调速）
            if(motor_speed < 90) {
                motor_speed += 10;
            }
            speed_value = motor_speed;
            break;
            
        case '-':  // 减速（渐进调速）
            if(motor_speed > 10) {
                motor_speed -= 10;
            }
            speed_value = motor_speed;
            break;
            
        default:
            return;  // 无效命令，直接返回
    }
    
    // 更新速度值
    if(speed_level == '1' || speed_level == '2' || speed_level == '3' || 
       speed_level == '+' || speed_level == '-') {
        motor_speed = speed_value;
        
        // 发送当前速度信息
        UART_SendChar('S');
        UART_SendChar(':');
        if(motor_speed >= 100) {
            UART_SendChar('1');
            UART_SendChar('0');
            UART_SendChar('0');
        } else {
            UART_SendChar(motor_speed/10 + '0');
            UART_SendChar(motor_speed%10 + '0');
        }
        UART_SendChar('%');
        UART_SendChar('\r');
        UART_SendChar('\n');
    }
    
    // 使能PWM
    pwm_enable = 1;
}