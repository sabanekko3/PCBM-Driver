/*
 * motor_math.cpp
 *
 *  Created on: 2023/04/03
 *      Author: yaa3k
 */


#include "../driver/motor_math.hpp"

//sincos table/////////////////////////////////////////////////////////////////////
sin_table::sin_table(void){
	for (int i = 0; i < TABLE_SIZE; i++) {
		float deg = (float)i/(float)TABLE_SIZE * 2*M_PI;
		table[i] = arm_sin_f32(deg);
	}
}

//motor_math//////////////////////////////////////////////////////////////////////
#ifdef USE_CMSIS
void motor_math::dq_from_uvw(uvw_t input,uint16_t deg_e,dq_t *out){
	float *sin = table.get(deg_e&0x3FF);
	float *cos = table.get_cos(deg_e&0x3FF);

	//clarke
	ab_t ab_data;
	arm_clarke_f32(input.u,input.v,&ab_data.a,&ab_data.b);

	//park
	arm_park_f32(ab_data.a,ab_data.b,&(out->d),&(out->q),*sin,*cos);
}
void motor_math::uvw_from_dq(dq_t input,uint16_t deg_e,uvw_t *out){
	float *sin = table.get(deg_e);
	float *cos = table.get(deg_e+COSP);

	//inv park
	ab_t ab_data;
	arm_inv_park_f32(input.d,input.q,&ab_data.a,&ab_data.b,*sin,*cos);

	//inv clarke
	arm_inv_clarke_f32(ab_data.a,ab_data.b,&(out->u),&(out->v));
	out->w = -out->u - out->v;
}
#else
void motor_math::dq_from_uvw(uvw_t input,uint16_t deg_e,dq_t *out){
	float *sin = table.get(deg_e);
	float *cos = table.get_cos(deg_e);

	ab_t ab;
	ab.a = input.u;
	ab.b = (input.u+2*input.v)*sqrt3inv;

	out->d = ab.a*(*cos)+ab.b*(*sin);
	out->q= -ab.a*(*sin)+ab.b*(*cos);
};
void motor_math::uvw_from_dq(dq_t input,uint16_t deg_e,uvw_t *out){

};
#endif
