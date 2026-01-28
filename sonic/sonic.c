#include "sonic.h"
#include "lcd/lcd.h"
#include "public/public.h"


#define ORG_TIME 0x3CB0		
#define SOUND_SPEED 340

int flag = 0;
unsigned int time=0;
unsigned int length = 0; 


char distance[] = "distance:   cm";

void time_config()
{
	TMOD=0x01;    
	TH0=0x3C;
	TL0=0xB0;
	ET0=1;
	EA=1;
}


void Display()
{
	if(flag == 1)
	{
		TH0=0x3C;
		TL0=0xB0;
		flag = 0;
		distance[9]=distance[10]=distance[11]='-';
		LCD1602_write_com(0x80+0x40);
		LCD1602_write_word(distance);
	}
	else
	{
		time = (TH0<<8) + TL0 - ORG_TIME;
		length = time/20*(SOUND_SPEED/10)/100;
		
		distance[9] = length/100? length/100+ '0': ' ';
		distance[10] = length/10%10 + '0';
		distance[11]= length%10 + '0';

		LCD1602_write_com(0x80+0x40);
		LCD1602_write_word(distance);
		TH0=0x3C;
		TL0=0xB0;
	}
}

void send_start_sfr()
{
	TR=1;			    
	delay_10us();
	TR=0;
}