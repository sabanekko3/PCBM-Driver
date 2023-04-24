/*
 * encoder.cpp
 *
 *  Created on: 2023/04/11
 *      Author: yaa3k
 */

#include "encoder.hpp"

void AS5600::reset_position(void){
	i2c_start();
	uint16_t enc = 0;
	HAL_Delay(100);
	while(!get_angle(&enc));
	enc_init_val = enc;
}

void AS5600::init(void){
	uint8_t txdata = 0x0c;
	HAL_I2C_Master_Transmit(i2c, as5600_id<<1, &txdata, 1, 100);

	reset_position();
}


void AS5600::i2c_start(void){
	i2c_new = false;
	memset(enc_val,0,2);
#ifdef I2C_DMA
	 HAL_I2C_Master_Receive_DMA(i2c, as5600_id<<1, enc_val, 2);
#else
	 HAL_I2C_Master_Receive(i2c, as5600_id<<1, enc_val, 2, 100);
	 i2c_new = true;
#endif
}

bool AS5600::get_angle(uint16_t *angle){
	*angle = (enc_val[0]<<8)|enc_val[1];
	*angle = (*angle-enc_init_val)&0xFFF;
	if(i2c_new){
		i2c_new = false;
		return true;
	}else{
		return false;
	}
}
