#include "public/public.h"
#include "bluetooth.h"
#include "monitor/monitor.h"
#include "pwm/pwm.h"


// 串口初始化函数
void UART_Init(void) {
    // 设置定时器1为模式2（8位自动重装）
    TMOD &= 0x0F;
    TMOD |= 0x20;    // TMOD高4位不变，低4位设为0010
    
    // 设置波特率为9600
    // 晶振11.0592MHz时，TH1 = 0xFD
    // 晶振12MHz时，TH1 = 0xF3
    TH1 = 0xFD;      // 波特率9600
    TL1 = 0xFD;
    
    // 启动定时器1
    TR1 = 1;
    
    // 设置串口工作方式1（10位异步收发）
    SCON = 0x50;     // 0101 0000，模式1，允许接收
    
    // 使能串口中断
    ES = 1;          // 允许串口中断
    EA = 1;          // 开启总中断
    
    // 清空接收标志位
    RI = 0;
    TI = 0;
}

// 发送一个字符
void UART_SendChar(unsigned char ch) {
    SBUF = ch;        // 将字符放入发送缓冲区
    while(!TI);       // 等待发送完成
    TI = 0;           // 清除发送中断标志
}

// 发送字符串
void UART_SendString(unsigned char *str) {
    while(*str != '\0') {
        UART_SendChar(*str);
        str++;
    }
}

// 串口中断服务函数
void UART_ISR(void) interrupt 4 {
    unsigned char received_data;
    
    if(RI) {
        RI = 0;                    // 清除接收中断标志
        received_data = SBUF;      // 读取接收到的数据
        
        // 状态指示灯闪烁
        LED = ~LED;
        
        // 根据接收到的命令控制电机
        switch(received_data) {
            case 'F':
            case 'f':
                motor_direction = 1;  // 前进
                Motor_Forward();      // 立即响应
                UART_SendString("前进\r\n");
                break;
                
            case 'B':
            case 'b':
                motor_direction = 2;  // 后退
                Motor_Backward();     // 立即响应
                UART_SendString("后退\r\n");
                break;
                
            case 'L':
            case 'l':
                motor_direction = 3;  // 左转
                Motor_TurnLeft();     // 立即响应
                UART_SendString("左转\r\n");
                break;
                
            case 'R':
            case 'r':
                motor_direction = 4;  // 右转
                Motor_TurnRight();    // 立即响应
                UART_SendString("右转\r\n");
                break;
                
            case 'S':
            case 's':
                motor_direction = 0;  // 停止
                pwm_enable = 0;       // 关闭PWM
                Motor_Stop();
                UART_SendString("停止\r\n");
                break;
                
            case '1':  // 加速
            case '2':  // 减速
            case '3':
            case 1:
            case 2:
            case 3:
                MotorSpeedSet(received_data);
                pwm_enable = 1;
                break;
                
            case '\r':  // 忽略回车
            case '\n':  // 忽略换行
            case ' ':   // 忽略空格
                break;
                
            default:
                motor_direction = 0;
                Motor_Stop();
                UART_SendString("未知命令，请输入F/B/L/R/S\r\n");
                break;
        }
        
        // 回显接收到的字符
        UART_SendChar(received_data);
        UART_SendChar('\r');
        UART_SendChar('\n');
    }
}