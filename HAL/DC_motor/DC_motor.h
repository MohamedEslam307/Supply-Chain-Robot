/*
 * H_Bridge.h
 *
 *  Created on: 10/10/2022
 *      Author: Mohamed Samir
 */

#ifndef HAL_DC_MOTOR_DC_MOTOR_H_
#define HAL_DC_MOTOR_DC_MOTOR_H_

#include "../../MCAL/Dio/Dio.h"
#include "DC_motor_Cfg.h"

typedef struct {
	pin_cofig_t in_1;
	pin_cofig_t in_2;
}Motor_t;

Std_ReturnType Motor_Init(Motor_t *_motor);
Std_ReturnType Motor_Rotate_Right(Motor_t *_motor);
Std_ReturnType Motor_Rotate_Left(Motor_t *_motor);
Std_ReturnType Motor_Stop(Motor_t *_motor);

#endif /* HAL_DC_MOTOR_DC_MOTOR_H_ */
