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
		float deg = (float)i/(float)TABLE_SIZE * 360;
		table[i] = arm_sin_f32(deg);
	}
}

//motor_math//////////////////////////////////////////////////////////////////////
#ifdef USE_CMSIS
void motor_math::dq_from_uvw(uvw_t input,uint16_t deg_e,dq_t *out){
	float *sin = table.get(deg_e);
	float *cos = table.get(deg_e+COSP);

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
dq_t motor_math::dq_from_uvw(uvw_t input,uint16_t deg_e){
	sincos_t sincos = table.get(deg_e);
	dq_t dq;

	float park_matrix[2][3] = {{sincos.cos_val, sincos.cos_val - 2.0 * sincos.sin_val / sqrt3, sincos.cos_val + 2.0 * sincos.sin_val / sqrt3},
	                                {sincos.sin_val, sincos.sin_val + sqrt3 * sincos.cos_val / sqrt3, sincos.sin_val - sqrt3 * sincos.cos_val / sqrt3}};

	dq.d = park_matrix[0][0] * input.u + park_matrix[0][1] * input.v + park_matrix[0][2] * input.w;
	dq.q = park_matrix[1][0] * input.u + park_matrix[1][1] * input.v + park_matrix[1][2] * input.w;

	return dq;
};
uvw_t motor_math::uvw_from_dq(dq_t input,uint16_t deg_e){
	sincos_t sincos_data = table.get(deg_e);

	uvw_t uvw;

	return uvw;
};
#endif
