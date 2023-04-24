/*
 * adc.hpp
 *
 *  Created on: Apr 7, 2023
 *      Author: yaa3k
 */

#ifndef DRIVER_ADC_HPP_
#define DRIVER_ADC_HPP_

#include "board_data.hpp"

class ADC{
private:
	ADC_HandleTypeDef *adc1;
	ADC_HandleTypeDef *adc2;
	const float i_gain;
	const float v_gain;
	uint16_t adc_init[7];
public:
	uint16_t adc_dma[7];
	ADC(ADC_HandleTypeDef *_adc1,ADC_HandleTypeDef *_adc2,float _i_gain,float _v_gain)
		:adc1(_adc1),adc2(_adc2),i_gain(_i_gain),v_gain(_v_gain){}
	void init(void);

	void get_i_uvw(uvw_t *uvw);
	void get_v_uvw(uvw_t *uvw);

	void dma_set(void);
	void dma_stop(void);

	uint16_t get_servo(void){
		return adc_dma[(int)ADC_data::SERVO];
	}
	float get_batt_v(void){
		return adc_dma[(int)ADC_data::BATT_V] * v_gain;
	}
};



#endif /* DRIVER_ADC_HPP_ */
