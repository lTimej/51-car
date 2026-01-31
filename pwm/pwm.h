#ifndef _pwm_H
#define _pwm_H
             // PWM使能标志

extern unsigned char pwm;
extern unsigned char speed_level;
extern unsigned char pwm_counter;



void MotorSpeedSet(unsigned char speed);
// void SetPwm(void);


#endif