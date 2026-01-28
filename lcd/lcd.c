#include "public/public.h"
#include "lcd.h"



#define LCD1602_PORT P0
sbit LCD1602_RS = P1^4;
sbit LCD1602_WR = P1^3;
sbit LCD1602_EN = P1^2;



void LCD1602_write_com(u8 com){
    LCD1602_RS = 0;
    LCD1602_WR = 0;
    LCD1602_EN = 1;
    delay_ms(1);
    LCD1602_PORT = com;
    LCD1602_EN = 0;
    delay_ms(1);
}

void LCD1602_write_data(u8 dat){
    LCD1602_RS = 1;
    LCD1602_WR = 0;
    LCD1602_EN = 1;
    LCD1602_PORT = dat;
    delay_ms(1);
    LCD1602_EN = 0;
}

void LCD1602_write_word(u8 *s){
    while(*s > 0){
        LCD1602_write_data(*s);
        s++;
    }
}


void init_LCD1602(){
    LCD1602_write_com(0x38);
    LCD1602_write_com(0x0F);
    LCD1602_write_com(0x06);
    LCD1602_write_com(0x01);
}

void show_ready(){
    LCD1602_write_com(0x80);
    LCD1602_write_word(" speed: ");
    LCD1602_write_com(0x80+0x40);
    LCD1602_write_word(" world ");
}