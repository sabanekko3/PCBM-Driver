/*
 * pid.hpp
 *
 *  Created on: 2023/04/13
 *      Author: yaa3k
 */

#ifndef DRIVER_PID_HPP_
#define DRIVER_PID_HPP_

#include "board_data.hpp"

class PID{
private:
	float kp = 0;
	float ki = 0;
	float kd = 0;
	float error = 0;
	float error_sum = 0;
	float old_error = 0;
public:
	PID(float _kp,float _ki,float _kd):kp(_kp),ki(_ki),kd(_kd){}
	float compute(float target,float feedback);
};

#endif /* DRIVER_PID_HPP_ */
