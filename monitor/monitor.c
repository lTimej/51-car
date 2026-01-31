#include "public/public.h"
#include "bluetooth/bluetooth.h"
#include "monitor.h"
#include "pwm/pwm.h"


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
    // 左侧电机 A、C 正转
    MOTOR_A_IA = 1;
    MOTOR_A_IB = 0;
    MOTOR_C_IA = 1;
    MOTOR_C_IB = 0;

    // 右侧电机 B、D 正转（安装方向相反，信号反过来）
    MOTOR_B_IA = 0;
    MOTOR_B_IB = 1;
    MOTOR_D_IA = 0;
    MOTOR_D_IB = 1;
}

void Motor_Backward(void) {
    // 左侧电机 A、C 反转
    MOTOR_A_IA = 0;
    MOTOR_A_IB = 1;
    MOTOR_C_IA = 0;
    MOTOR_C_IB = 1;

    // 右侧电机 B、D 反转（安装方向相反，信号反过来）
    MOTOR_B_IA = 1;
    MOTOR_B_IB = 0;
    MOTOR_D_IA = 1;
    MOTOR_D_IB = 0;
}

void Motor_TurnLeft(void) {
    // 左转：左轮停止，右轮正转
    // 左侧 A、C 停止
    MOTOR_A_IA = 0;
    MOTOR_A_IB = 0;
    MOTOR_C_IA = 0;
    MOTOR_C_IB = 0;

    // 右侧 B、D 正转（安装方向相反，信号反过来）
    MOTOR_B_IA = 0;
    MOTOR_B_IB = 1;
    MOTOR_D_IA = 0;
    MOTOR_D_IB = 1;
}

void Motor_TurnRight(void) {
    // 右转：左轮正转，右轮停止
    // 左侧 A、C 正转
    MOTOR_A_IA = 1;
    MOTOR_A_IB = 0;
    MOTOR_C_IA = 1;
    MOTOR_C_IB = 0;

    // 右侧 B、D 停止
    MOTOR_B_IA = 0;
    MOTOR_B_IB = 0;
    MOTOR_D_IA = 0;
    MOTOR_D_IB = 0;
}



// 定时器初始化（用于产生PWM）
void Timer0_Init(void) {
    TMOD &= 0xF0; 
    TMOD |= 0x01;      // 定时器0，模式1（16位定时器）
    // 设置定时器初值，产生约10KHz的PWM
    TH0 = 0xFC;  // 100us中断一次 -> 10KHz
    TL0 = 0x73;
    ET0 = 1;           // 使能定时器0中断
    TR0 = 1;           // 启动定时器0
    EA = 1;            // 开启总中断
}


// 根据 motor_direction 执行对应的电机动作
static void Motor_Run(void) {
    switch(motor_direction) {
        case 1: Motor_Forward();  break;
        case 2: Motor_Backward(); break;
        case 3: Motor_TurnLeft(); break;
        case 4: Motor_TurnRight(); break;
        default: Motor_Stop();    break;
    }
}

// 定时器0中断服务函数
void Timer0_ISR(void) interrupt 1 {
    static unsigned char counter = 0;
    
    TH0 = 0xFC;          // 重装初值
    TL0 = 0x73;
    
    // PWM周期为10ms（10个1ms中断）
    if(++counter >= 10) {
        counter = 0;
    }
    // PWM控制逻辑
    if(pwm_enable) { 
        if(motor_direction == 0 || pwm == 0) {
            // 停止或占空比为0%
            Motor_Stop();
        } 
        else if(pwm == 10) {
            // 占空比为100%，电机一直转
            Motor_Run();
        }
        else {
            // 根据占空比控制电机
            if(counter < pwm) {
                Motor_Run();      // 高电平期间电机转
            } else {
                Motor_Stop();     // 低电平期间电机停
            }
        }
    }
}
