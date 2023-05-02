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
		HAL_Delay(5);
	}
	driver.out(0, 0.5);
	HAL_Delay(1000);
	enc.init();
	driver.out(0, 0);
}

static float C = 0;
static float F = 0;
void BOARD::loop(void){
#ifndef TIM3_INT
	adc.dma_stop();
	inthandle();
	adc.dma_set();
#endif
	C += F;
	printf("%d,%d,%d\r\n",angle_e_pwm,angle_e_real,angle_e_pwm-angle_e_real);

	//printf("%d,%d,%d\r\n",(angle_e_pwm-angle_e_real)&0x3FF,angle_e_real,enc.enc_init_val);
	//printf("%4.1f,%4.1f,%4.1f,%4.1f,%4.1f,%d,%d\r\n", phase_i.u, phase_i.v,phase_i.w, dq_i.d, dq_i.q,angle_e_real,(angle_e_pwm)&0x3FF);
	//printf("%d,%d,%d\r\n",adc.adc_dma[(int)ADC_data::U_I],adc.adc_dma[(int)ADC_data::W_I],adc.adc_dma[(int)ADC_data::SERVO]);
	HAL_Delay(1);
}

PID pid(0.1,0.1,0);
void BOARD::inthandle(void){
	adc.dma_stop();

	adc.get_i_uvw(&phase_i);
	enc.get_angle(&angle_real);
	angle_e_real = (angle_real>>1) & 0x3ff;
	servo = adc.get_servo();

	math.dq_from_uvw(phase_i, angle_e_real,&dq_i);

//#define VECTOR
#ifndef VECTOR
	if(servo < 10){
		driver.out(0,0.1);
		F = 0;
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	}else{
		F = 0.05;
		angle_e_pwm = (angle_e_real+256)&0x3FF;
		float power = servo*0.0001 + 0.1;
		driver.out(angle_e_pwm,power);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	}
#endif
#ifdef VECTOR
	dq_t target_dq;
	if(servo < 10){
		target_dq.d = 0;
		target_dq.q = 0;
		dq_v.d = pid.compute(target_dq.d,dq_i.d);
		dq_v.q = pid.compute(target_dq.d, dq_i.q);

		driver.out(0,0.0);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	}else{
		target_dq.d = 0;
		target_dq.q = servo*0.0002;

		dq_v.d = pid.compute(target_dq.d,dq_i.d);
		dq_v.q = pid.compute(target_dq.d, dq_i.q);
		math.uvw_from_dq(dq_v, angle_e_real,&phase_v);

		float tmp = phase_v.u;
		phase_v.u = phase_v.w;
		phase_v.w = tmp;

		driver.out(phase_v);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	}


#endif

	enc.read_start();
	adc.dma_set();
}
