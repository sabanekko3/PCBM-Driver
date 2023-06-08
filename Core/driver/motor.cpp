/*
 * driver.c
 *
 *  Created on: Mar 26, 2023
 *      Author: yaa3k
 */

#include "motor.hpp"

void MOTOR::init(void){
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	driver.pwms_start();
	driver.out(0, 0.0);

	HAL_Delay(100);

	adc.init();
	uint16_t _enc = 0;

	for(int i = 0; i<8;i++){
		for(int j = 0; j < 0x3FF;j+=8){
			driver.out(j, 0.3);
			HAL_Delay(1);
		}
		driver.out(0, 0.3);
		HAL_Delay(200);
		_enc += enc.read_raw()&0x3FF;
	}

	enc.set_origin(_enc/8);
	driver.out(0, 0);
}

void MOTOR::set_torque(float qi){
	target_dq.d = 0;
	target_dq.q = qi;
}

static const float POS_C = 2*M_PI/0x3FFF;
static const float SPD_C = POS_C*TIM3_FRQ/16;
void MOTOR::print_debug(void){
	static int16_t diff[16] = {0};
	for(int i = 1; i<16;i++){
		diff[i] = diff[i-1];
	}
	diff[0] = enc_diff;
	int sum = 0;
	for(int i = 0; i<16;i++){
		sum += diff[i];
	}
	float rotor_rad = angle_real * POS_C;
	float rotor_speed = sum*SPD_C;
	//printf("%4.2f,%4.2f\r\n",rotor_rad,rotor_speed);
	//printf("%d,%d,%d\r\n",angle_e_pwm,angle_e_real,angle_e_pwm-angle_e_real);
	//printf("%d,%d,%d\r\n",(angle_e_pwm-angle_e_real)&0x3FF,angle_e_real,enc.enc_init_val);
	printf("%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%d,%d\r\n", phase_i.u, phase_i.v,phase_i.w, dq_i.d, dq_i.q,angle_e_real,(angle_e_pwm)&0x3FF);
	//printf("%d,%d,%d\r\n",adc.adc_dma[(int)ADC_data::U_I],adc.adc_dma[(int)ADC_data::W_I],adc.adc_dma[(int)ADC_data::SERVO]);
}

#define VECTOR
PID pid_d(0.01,0.01,0);
PID pid_q(0.01,0.01,0);
void MOTOR::control(void){
	adc.get_i_uvw(&phase_i);

	uint16_t tmp = angle_real;
	if(enc.get_angle(&angle_real)){
		int16_t _diff = angle_real - tmp;
		if (_diff < -1 << 13) {
			_diff += (1 << 14);
		} else if (_diff > (1 << 13)) {
			_diff -= (1 << 14);
		}
		enc_diff = _diff;
	}

	angle_e_real = (angle_real>>1) & 0x3ff;
	math.dq_from_uvw(phase_i, angle_e_real,&dq_i);

#ifndef VECTOR
	if(servo < 10){
		driver.out(0,0);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	}else{
		angle_e_pwm = angle_e_real+0xFF;
		float power = servo*0.0001 + 0.1;
		driver.out(angle_e_pwm,power);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	}
#endif
#ifdef VECTOR

	dq_v.d = pid_d.compute(target_dq.d,dq_i.d);
	dq_v.q = pid_q.compute(target_dq.q,dq_i.q);


	math.uvw_from_dq(dq_v, angle_e_real,&phase_v);
	driver.out(phase_v);

	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, target_dq.q > 0.01 ? GPIO_PIN_SET : GPIO_PIN_RESET);

#endif

	enc.read_start();
}
