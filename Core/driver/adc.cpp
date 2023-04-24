/*
 * adc.cpp
 *
 *  Created on: Apr 7, 2023
 *      Author: yaa3k
 */
#include "adc.hpp"

void ADC::init(void){
	dma_set();
	HAL_Delay(100);
	for(uint8_t i = 0; i < 16; i++){
		for(uint8_t j = 0; j < (int)ADC_data::n; j++){
			adc_init[j] += adc_dma[j];
		}
	}
	for(uint8_t i = 0; i < (int)ADC_data::n; i++){
		adc_init[i] /= 16;
	}
}

void ADC::get_i_uvw(uvw_t *uvw){
	uvw->u = (-adc_dma[(int)ADC_data::U_I] + adc_init[(int)ADC_data::U_I]) * i_gain;
	uvw->w = ( adc_dma[(int)ADC_data::W_I] - adc_init[(int)ADC_data::W_I]) * i_gain;
	uvw->v = - uvw->u - uvw->w;
}

void ADC::get_v_uvw(uvw_t *uvw){
	uvw->u =  adc_dma[(int)ADC_data::U_V] - adc_init[(int)ADC_data::U_V] * v_gain;
	uvw->v =  adc_dma[(int)ADC_data::V_V] - adc_init[(int)ADC_data::V_V] * v_gain;
	uvw->w =  adc_dma[(int)ADC_data::W_V] - adc_init[(int)ADC_data::W_V] * v_gain;
}

void ADC::dma_set(void){
	HAL_ADC_Start_DMA(adc1, (uint32_t*) adc_dma, 4);
	HAL_ADC_Start_DMA(adc2, (uint32_t*) &adc_dma[(int)ADC_data::U_I], 3);
}

void ADC::dma_stop(void){
	HAL_ADC_Stop_DMA(adc1);
	HAL_ADC_Stop_DMA(adc2);
}
