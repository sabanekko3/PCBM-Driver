/*
 * table.hpp
 *
 *  Created on: Mar 18, 2023
 *      Author: yaa3k
 */

#ifndef INC_PWM_HPP_
#define INC_PWM_HPP_

#include "board_data.hpp"

class PWM{
private:
	TIM_HandleTypeDef *tim;
	const uint32_t ch;
	const uint32_t tim_period;
	uint32_t pwm_val;
public:
	PWM(TIM_HandleTypeDef *_tim,uint32_t _ch,uint32_t _tim_period)
		: tim(_tim),ch(_ch),tim_period(_tim_period){}

	bool out(float val);//-1~1

	void start(void);
	void stop(void);
};

#endif


