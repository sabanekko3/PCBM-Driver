/*
 * driver.cpp
 *
 *  Created on: Mar 18, 2023
 *      Author: yaa3k
 */

#include "pwm.hpp"

bool PWM::out(float val){
	if(val < -0.9 || 0.9 < val){
		return false;
	}
	val = val*0.5 + 0.5;
	__HAL_TIM_SET_COMPARE(tim, ch, val*tim_period);
	return true;
}
void PWM::start(void){
	HAL_TIM_PWM_Start(tim, ch);
	HAL_TIMEx_PWMN_Start(tim, ch);
	__HAL_TIM_SET_COMPARE(tim, ch,0);
}

void PWM::stop(void){
	HAL_TIM_PWM_Stop(tim, ch);
	HAL_TIMEx_PWMN_Stop(tim, ch);
	__HAL_TIM_SET_COMPARE(tim, ch,0);
}
