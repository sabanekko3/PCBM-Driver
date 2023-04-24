/*
 * encoder.hpp
 *
 *  Created on: 2023/04/10
 *      Author: yaa3k
 */

#ifndef DRIVER_ENCODER_HPP_
#define DRIVER_ENCODER_HPP_

#include "board_data.hpp"



class AS5600{
private:
	I2C_HandleTypeDef *i2c;
	uint16_t as5600_id = 0x36;

	bool i2c_new = false;
	uint8_t enc_val[2] = {0};


public:
	uint16_t enc_init_val;
	AS5600(I2C_HandleTypeDef *_i2c):i2c(_i2c){}

	void reset_position(void);
	void init(void);
	void i2c_start(void);
	bool get_angle(uint16_t *angle);

	void set_flag(bool f){
		i2c_new = f;
	}
};


#endif /* DRIVER_ENCODER_HPP_ */
