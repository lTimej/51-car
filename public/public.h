#ifndef _public_H
#define _public_H

#include <reg52.h>
#include <intrins.h>

extern bit pwm_enable;
extern unsigned char motor_direction;  // 0=停止, 1=前进, 2=后退, 3=左转, 4=右转

void delay_10us();
void delay_ms(unsigned int ms);

#endif