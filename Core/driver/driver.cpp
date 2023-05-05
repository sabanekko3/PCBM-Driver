/*
 * driver.cpp
 *
 *  Created on: Mar 27, 2023
 *      Author: yaa3k
 */
#include "driver.hpp"

DRIVER::DRIVER(PWM &_pwm_u,PWM &_pwm_v,PWM &_pwm_w,sin_table &_table)
	: pwm_u(_pwm_u),pwm_v(_pwm_v),pwm_w(_pwm_w),table(_table){
}

void DRIVER::out(uint16_t degree,float power){
	pwm_u.out(power*(*table.get_cos(degree)));
	pwm_v.out(power*(*table.get_cos(degree-V_PHASE)));
	pwm_w.out(power*(*table.get_cos(degree-W_PHASE)));
}
void DRIVER::out(uvw_t uvw){
	pwm_u.out(uvw.u);
	pwm_v.out(uvw.v);
	pwm_w.out(uvw.w);
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
