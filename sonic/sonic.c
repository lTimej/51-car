#include "sonic.h"
#include "lcd/lcd.h"
#include "public/public.h"
#include "pwm/pwm.h"


#define ORG_TIME 0x3CB0		
#define SOUND_SPEED 340

int flag = 0;
unsigned int time=0;
unsigned int length = 0; 


char distance[] = "distance:   cm";
char speed_str[] = "speed:    %";

// 使用 Timer2 做超声波计时，避免与 PWM 用的 Timer0 冲突
void time_config()
{
	T2CON = 0;           // Timer2 为 16 位定时器模式
	TH2 = 0x3C;
	TL2 = 0xB0;
	EA = 1;
}


void Display()
{
	// 显示 PWM 速度（第一行）
	// pwm 范围 0-10，对应 0%-100%
	speed_str[6] = (pwm == 10) ? '1' : ' ';
	speed_str[7] = (pwm == 10) ? '0' : pwm + '0';
	speed_str[8] = '0';
	LCD1602_write_com(0x80);
	LCD1602_write_word(speed_str);
	
	// 显示距离（第二行）
	if(flag == 1)
	{
		TH2 = 0x3C;
		TL2 = 0xB0;
		flag = 0;
		distance[9]=distance[10]=distance[11]='-';
		LCD1602_write_com(0x80+0x40);
		LCD1602_write_word(distance);
	}
	else
	{
		time = (TH2<<8) + TL2 - ORG_TIME;
		length = time/20*(SOUND_SPEED/10)/100;
		
		distance[9] = length/100? length/100+ '0': ' ';
		distance[10] = length/10%10 + '0';
		distance[11]= length%10 + '0';

		LCD1602_write_com(0x80+0x40);
		LCD1602_write_word(distance);
		TH2 = 0x3C;
		TL2 = 0xB0;
	}
}

void send_start_sfr()
{
	TR=1;			    
	delay_10us();
	TR=0;
}