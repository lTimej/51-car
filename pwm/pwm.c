#include <reg52.h>
#include <stdio.h>
#include <pwm.h>
#include <monitor/monitor.h>


void SetPwm(void) {
    
    if(++count >= 10) count = 0;
    
    if(pwm <= 0) {
        Motor_Stop();
    } else {
        Motor_Forward();
    }
}

// ============ 设置电机速度 ============
void MotorSpeedSet(unsigned char speed_level) {
    unsigned char speed_value;
    
    switch(speed_level) {
        case '1':  // 低速 - 30%占空比
            speed_value = 30;
            break;
            
        case '2':  // 中速 - 60%占空比
            speed_value = 60;
            break;
            
        case '3':  // 高速 - 90%占空比
            speed_value = 90;
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
            speed_value = 30;
    }
    
    // 更新速度值
    if(speed_level == '1' || speed_level == '2' || speed_level == '3' || 
       speed_level == '+' || speed_level == '-') {
        motor_speed = speed_value;
    }
    
    // 使能PWM
    pwm_enable = 1;
}
