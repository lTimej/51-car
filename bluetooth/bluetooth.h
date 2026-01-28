#ifndef _bluetooth_H
#define _bluetooth_H

#include "public/public.h"

// 定义状态指示灯
sbit LED = P1^0;

void UART_Init(void);
void UART_SendChar(unsigned char ch);
void UART_SendString(unsigned char *str);
// void UART_ISR(void);

#endif