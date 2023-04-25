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

	bool data_new = false;
	uint8_t enc_val[2] = {0};


public:
	uint16_t enc_init_val;
	AS5600(I2C_HandleTypeDef *_i2c):i2c(_i2c){}

	void reset_position(void);
	void init(void);
	void read_start(void);
	bool get_angle(uint16_t *angle);

	void set_flag(bool f){
		data_new = f;
	}
};

class AS5048{
private:
	SPI_HandleTypeDef *spi;
	GPIO_TypeDef *ss_port;
	uint8_t ss_pin;

	bool data_new = false;
	uint8_t enc_val[2] = {0};


public:
	uint16_t enc_init_val;
	AS5048(SPI_HandleTypeDef *_spi,GPIO_TypeDef *_ss_port,uint8_t _ss_pin)
		:spi(_spi),ss_port(_ss_port),ss_pin(_ss_pin){}

	void reset_position(void);
	void init(void);
	void read_start(void);
	bool get_angle(uint16_t *angle);

	void set_flag(bool f){
		data_new = f;
	}
};


#endif /* DRIVER_ENCODER_HPP_ */
