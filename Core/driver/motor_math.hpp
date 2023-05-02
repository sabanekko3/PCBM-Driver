/*
 * motor_math.hpp
 *
 *  Created on: 2023/04/03
 *      Author: yaa3k
 */

#ifndef DRIVER_MOTOR_MATH_HPP_
#define DRIVER_MOTOR_MATH_HPP_

#include "board_data.hpp"


class sin_table{
private:
	float table[TABLE_SIZE];
public:
	sin_table(void);
	float *get(uint16_t angle){
		return &(table[angle & 0x3FF]);
	}
	float *get_cos(uint16_t angle){
		return &(table[(angle+256) & 0x3FF]);
	}
};


class motor_math{
private:
	sin_table &table;
	float sqrt3 = sqrt(3.0);
	float sqrt3inv = 1/sqrt(3.0);
public:
	motor_math(sin_table &_table):table(_table){}
	void dq_from_uvw(uvw_t input,uint16_t deg_e,dq_t *out);
	void uvw_from_dq(dq_t input,uint16_t deg_e,uvw_t *out);
};

#endif /* DRIVER_MOTOR_MATH_HPP_ */
