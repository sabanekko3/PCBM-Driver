/*
 * board.h
 *
 *  Created on: Mar 26, 2023
 *      Author: yaa3k
 */

#ifndef INC_BOARD_HPP_
#define INC_BOARD_HPP_

#include "board_data.hpp"
#include "driver.hpp"
#include "pwm.hpp"
#include "motor_math.hpp"
#include "adc.hpp"
#include "encoder.hpp"
#include "pid.hpp"

class BOARD{
private:
	DRIVER &driver;
	motor_math &math;
	AS5048 &enc;

	uint16_t angle_real;
	uint16_t angle_e_real;
	uint16_t angle_e_pwm;

	ADC &adc;

	dq_t dq_i;
	dq_t dq_v;
	uvw_t phase_v;
	uvw_t phase_i;
	uint16_t servo;

public:
	BOARD(DRIVER &_driver,ADC &_adc,motor_math &_math,AS5048 &_enc)
		:driver(_driver),adc(_adc),math(_math),enc(_enc){}
	void init(void);
	void loop(void);
	void inthandle(void);
};





#endif /* INC_BOARD_HPP_ */
