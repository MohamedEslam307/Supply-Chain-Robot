/*
 * Servo.h
 *
 *  Created on: 10 Oct 2022
 *      Author: El-Wattaneya
 */

#ifndef HAL_SERVO_MOTOR_SERVO_H_
#define HAL_SERVO_MOTOR_SERVO_H_

#include "../../MCAL/Timer1/Timer1.h"

#define CHANNEL_A 0
#define CHANNEL_B 1

#define PRESCALER_1       1
#define PRESCALER_8       8
#define PRESCALER_64      64
#define PRESCALER_256     256
#define PRESCALER_1024    1024

typedef struct{
	uint8 channel:1;
	uint16 prescaler;
}servo_motor_t;

void Servo_move_angle(uint8 angle);

#endif /* HAL_SERVO_MOTOR_SERVO_H_ */
