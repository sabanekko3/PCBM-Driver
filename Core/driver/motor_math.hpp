/*
 * motor_math.hpp
 *
 *  Created on: 2023/04/03
 *      Author: yaa3k
 */

#ifndef DRIVER_MOTOR_MATH_HPP_
#define DRIVER_MOTOR_MATH_HPP_

#include "board_data.hpp"



class sincos_table{
private:
	sincos_t table[TABLE_SIZE];
public:
	sincos_table(void);
	sincos_t *get(uint16_t degree);

};


class motor_math{
private:
	sincos_table &table;
	float sqrt3 = sqrt(3.0);
public:
	motor_math(sincos_table &_table):table(_table){}
	void dq_from_uvw(uvw_t input,uint16_t deg_e,dq_t *out);
	void uvw_from_dq(dq_t input,uint16_t deg_e,uvw_t *out);
};

#endif /* DRIVER_MOTOR_MATH_HPP_ */
