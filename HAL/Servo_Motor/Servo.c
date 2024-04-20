/*
 * Servo.c
 *
 *  Created on: 10 Oct 2022
 *      Author: El-Wattaneya
 */
#include "Servo.h"
#include "../../MCAL/Dio/Dio.h"



volatile uint8 counter=0;

void Servo_move_angle(uint8 angle){
	//uint16 result = 1000+((uint32)4000*angle)/180;
	/*switch (angle){
	case 0:
		OCR1B=1000;
		break;
	case 90:
		OCR1B=2500;
		break;
	case 180:
		OCR1B=4800;
		break;
	}*/
	OCR1B=1000+(float)angle*21.11111111;
}


