#ifndef _monitor_H
#define _monitor_H

#include "public/public.h"

// 定义L9110控制引脚
sbit MOTOR_A_IA = P2^0;  // 电机A控制引脚1
sbit MOTOR_A_IB = P2^1;  // 电机A控制引脚2
sbit MOTOR_B_IA = P2^4;  // 电机B控制引脚1（可选）
sbit MOTOR_B_IB = P2^5;  // 电机B控制引脚2（可选）

sbit MOTOR_C_IA = P2^2;  // 电机A控制引脚1
sbit MOTOR_C_IB = P2^3;  // 电机A控制引脚2
sbit MOTOR_D_IA = P2^6;  // 电机B控制引脚1（可选）
sbit MOTOR_D_IB = P2^7;  // 电机B控制引脚2（可选）


void Motor_Stop(void);
void Motor_Forward(void);
void Motor_Backward(void);
void Motor_TurnLeft(void);
void Motor_TurnRight(void);
void Timer0_Init(void);
void Timer0_ISR(void);
void Motor_SetSpeed(unsigned char speed_level);



#endif