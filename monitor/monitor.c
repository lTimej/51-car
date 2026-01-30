#include "public/public.h"
#include "bluetooth/bluetooth.h"
#include "monitor.h"



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
        SetPwm();
    }
}
