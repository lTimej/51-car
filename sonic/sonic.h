#ifndef _sonic_H
#define _sonic_H

#include "public/public.h"

sbit EC = P1^0;
sbit TR = P1^1;
extern  int flag;
extern  unsigned int time;
extern unsigned int length; 

void time_config();
void Display();
void send_start_sfr();


#endif