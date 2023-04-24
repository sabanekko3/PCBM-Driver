/*
 * pid.cpp
 *
 *  Created on: 2023/04/13
 *      Author: yaa3k
 */

#include "pid.hpp"
float PID::compute(float target,float feedback){
	error = target - feedback;
	float p = error * kp;

	error_sum += error;
	float i = error_sum * ki;

	float error_d = error - old_error;
	float d = error_d * kd;
	old_error = error;

	return p+i+d;
}
