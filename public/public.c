#include <reg52.h>
#include <intrins.h>
#include "public.h"

bit pwm_enable = 0;
void delay_10us()
{
	unsigned char i;

	i = 10;
	while (--i);
}

void delay_ms(unsigned int ms) {  
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 123; j++);
}