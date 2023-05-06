/*
 * board_data.hpp
 *
 *  Created on: 2023/04/03
 *      Author: yaa3k
 */

#ifndef DRIVER_BOARD_DATA_HPP_
#define DRIVER_BOARD_DATA_HPP_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define ARM_MATH_CM4
#include "arm_math.h"
#include "arm_const_structs.h"

#include "../inc/main.h"

#define USE_CMSIS
#define TIM3_INT
#define TIM3_FRQ 10000
//#define SPI_DMA

enum class ADC_data{
	//adc1
	BATT_V,
	W_V,
	V_V,
	U_V,
	//adc2
	U_I,
	W_I,
	SERVO,
	n
};

enum class PHASE{
	U,
	V,
	W
};


#define TABLE_SIZE 1048
#define COSP 256
#define V_PHASE 341
#define W_PHASE 683

typedef struct dq{
	float d;
	float q;
}dq_t;
typedef struct ab{
	float a;
	float b;
}ab_t;
typedef struct uvw{
	float u;
	float v;
	float w;
}uvw_t;

#endif /* DRIVER_BOARD_DATA_HPP_ */
