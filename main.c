#include "public/public.h"
#include "monitor/monitor.h"
#include "bluetooth/bluetooth.h"
#include "sonic/sonic.h"
#include "lcd/lcd.h"


// 主函数
void main(void) {

    TR = 0;
	EC = 0;
	delay_ms(2000);			//系统等待硬件就绪
	init_LCD1602();
	show_ready();
	time_config();


    // 初始化引脚
    MOTOR_A_IA = 0;
    MOTOR_A_IB = 0;
    MOTOR_B_IA = 0;
    MOTOR_B_IB = 0;

    MOTOR_C_IA = 0;
    MOTOR_C_IB = 0;
    MOTOR_D_IA = 0;
    MOTOR_D_IB = 0;
    LED = 0;
    
    // 初始化串口
    UART_Init();
    Timer0_Init();
    
    // 发送启动信息
    UART_SendString("HC-08电机控制系统就绪\r\n");
    UART_SendString("命令: F-前进 B-后退 L-左转 R-右转 S-停止\r\n");
    
    while(1) {
        // 主循环，中断处理接收
        // 可以添加其他功能，如按键检测等
        send_start_sfr();		//发送开始信号
        while(!EC);					//等待收到高电平
        T2CON |= 0x04;					//Timer2 开始计时（超声波，TR2=1）
        if(!flag)
            while(EC);
        T2CON &= ~0x04;					//Timer2 停止计时
        Display();					//显示对应数据
        delay_ms(3000);
    }
}