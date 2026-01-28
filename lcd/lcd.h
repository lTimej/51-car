#ifndef _lcd_H
#define _lcd_H

#include "public/public.h"

typedef unsigned int u16;
typedef unsigned char u8;

void LCD1602_write_com(u8 com);
void LCD1602_write_data(u8 dat);
void LCD1602_write_word(u8 *s);
void init_LCD1602();
void show_ready();


#endif