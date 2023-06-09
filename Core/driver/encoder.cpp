/*
 * encoder.cpp
 *
 *  Created on: 2023/04/11
 *      Author: yaa3k
 */

#include "encoder.hpp"

//AS5600//////////////////////////////////////////////////////////////////////////////
void AS5600::reset_position(void){
	read_start();
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



void AS5600::read_start(void){
	data_new = false;
	memset(enc_val,0,2);
#ifdef I2C_DMA
	 HAL_I2C_Master_Receive_DMA(i2c, as5600_id<<1, enc_val, 2);
#else
	 HAL_I2C_Master_Receive(i2c, as5600_id<<1, enc_val, 2, 100);
	 data_new = true;
#endif
}

bool AS5600::get_angle(uint16_t *angle){
	*angle = (enc_val[0]<<8)|enc_val[1];
	*angle = (*angle-enc_init_val)&0xFFF;
	if(data_new){
		data_new = false;
		return true;
	}else{
		return false;
	}
}

//AS5048//////////////////////////////////////////////////////////////////////////////
void AS5048::reset_position(void){
	read_start();
	uint16_t enc = 0;
	while(!get_angle(&enc));
	enc_init_val = enc;
}

void AS5048::init(void){
	reset_position();
}

void AS5048::set_origin(uint16_t origin){
	enc_init_val = origin;
}
uint16_t AS5048::read_raw(void){
	read_start();
	uint16_t enc;
	while(!get_angle(&enc));
	return -((enc_val[0]<<8)|enc_val[1]);
}

void AS5048::read_start(void){
	data_new = false;
	uint8_t tx[2]={0xFF,0xFF};
#ifdef SPI_DMA
	HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_RESET);
	 HAL_SPI_TransmitReceive_DMA(spi, tx, enc_val,2);
#else
	 HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_RESET);
	 HAL_SPI_TransmitReceive(spi,tx, enc_val,2,1000);
	 HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_SET);
	 data_new = true;
#endif
}

bool AS5048::get_angle(uint16_t *angle){
	*angle = ((enc_val[0]<<8) | enc_val[1]);
	*angle = (*angle + enc_init_val)&0x3FFF;
	if(data_new){
		data_new = false;
		return true;
	}else{
		return false;
	}
}
