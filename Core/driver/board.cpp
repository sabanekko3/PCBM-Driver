/*
 * driver.c
 *
 *  Created on: Mar 26, 2023
 *      Author: yaa3k
 */

#include "board.hpp"

void BOARD::init(void){
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	driver.pwms_start();
	driver.out(0, 0.0);

	HAL_Delay(100);

	adc.init();

	for(float i = 0; i < 0.5;i +=0.01){
		driver.out(0, i);
		HAL_Delay(3);
	}
	HAL_Delay(200);
	enc.init();
	driver.out(0, 0);
}

void BOARD::loop(void){
#ifndef TIM3_INT
	adc.dma_stop();
	inthandle();
	adc.dma_set();
#endif
	angle_e_pwm += servo*0.01;

	//printf("%d,%d\r\n",enc.enc_init_val,angle_e_real);
	printf("%4.1f,%4.1f,%4.1f,%4.1f,%4.1f,%d\r\n", phase_i.u, phase_i.v,phase_i.w, dq_val.d, dq_val.q,angle_e_real);
	//printf("%4.1f,%4.1f,%4.1f,%4.1f,%4.1f\r\n", phase_i.u, phase_i.w,servo);
	//printf("%4.1f,%4.1f,%4.1f\r\n", adc.get_i_abc().a, adc.get_i_abc().b,adc.get_i_abc().c);
	//printf("%d,%d,%d\r\n",adc.adc_dma[(int)ADC_data::U_I],adc.adc_dma[(int)ADC_data::W_I],adc.adc_dma[(int)ADC_data::SERVO]);
	HAL_Delay(1);
}

void BOARD::inthandle(void){
	adc.dma_stop();

	adc.get_i_uvw(&phase_i);
	enc.get_angle(&angle_e_real);
	servo = adc.get_servo();

	math.dq_from_uvw(phase_i, angle_e_real,&dq_val);

	if(servo < 10){
		driver.out(angle_e_pwm,0.0);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	}else{
		//uvw_t uvw = math.uvw_from_dq({0,servo*0.0002}, (uint8_t)angle_e_real);
		//driver.out(uvw.u,uvw.v,uvw.w);

		//angle_e_pwm = (uint8_t)angle_e_real + 64;
		//angle_e_pwm += servo*0.0005;
		float power = servo*0.0001 + 0.1;
		driver.out(angle_e_pwm,power);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	}


	enc.read_start();
	adc.dma_set();
}
