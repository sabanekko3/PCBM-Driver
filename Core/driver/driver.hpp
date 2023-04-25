/*
 * driver.hpp
 *
 *  Created on: Mar 27, 2023
 *      Author: yaa3k
 */

#ifndef INC_DRIVER_HPP_
#define INC_DRIVER_HPP_

#include "board_data.hpp"
#include "pwm.hpp"
#include "motor_math.hpp"

class DRIVER{
private:
	sin_table& table;
	PWM& pwm_u;
	PWM& pwm_v;
	PWM& pwm_w;
public:
	DRIVER(PWM &_pwm_u,PWM &_pwm_v,PWM &_pwm_w,sin_table &_table);
	void out(uint16_t degree,float power);
	void out(float u,float v,float w);

	void pwms_start(void);
	void pwms_stop(void);
};



#endif /* INC_DRIVER_HPP_ */
