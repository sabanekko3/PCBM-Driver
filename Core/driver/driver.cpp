/*
 * driver.cpp
 *
 *  Created on: Mar 27, 2023
 *      Author: yaa3k
 */
#include "../driver/driver.hpp"

DRIVER::DRIVER(PWM &_pwm_u,PWM &_pwm_v,PWM &_pwm_w,sincos_table &_table)
	: pwm_u(_pwm_u),pwm_v(_pwm_v),pwm_w(_pwm_w),table(_table){
}

void DRIVER::out(uint16_t degree,float power){
	pwm_u.out(power*(table.get(degree)->sin_val));
	pwm_v.out(power*(table.get(degree+V_PHASE)->sin_val));
	pwm_w.out(power*(table.get(degree+W_PHASE)->sin_val));
}
void DRIVER::out(float u,float v,float w){
	pwm_u.out(u);
	pwm_v.out(v);
	pwm_w.out(w);
}


void DRIVER::pwms_start(void){
	pwm_u.start();
	pwm_v.start();
	pwm_w.start();
}
void DRIVER::pwms_stop(void){
	pwm_u.stop();
	pwm_v.stop();
	pwm_w.stop();
}



