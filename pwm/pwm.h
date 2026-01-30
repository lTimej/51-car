#ifndef _pwm_H
#define _pwm_H

// 全局变量
unsigned char pwm = 3;      // 默认中速
unsigned char count = 0;
unsigned char motor_speed = 0;       // 当前速度 0-100
bit pwm_enable = 0;                  // PWM使能标志

void MotorSpeedSet(unsigned char speed);


#endif