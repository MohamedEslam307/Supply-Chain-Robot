/*
 * H_Bridge.c
 *
 *  Created on: 10/10/2022
 *      Author: Mohamed Samir
 */

#include "DC_motor.h"

Std_ReturnType Motor_Init(Motor_t *_motor){
	Std_ReturnType ret = E_NOT_OK;
	if(((void*)0)==_motor){
		ret = E_NOT_OK;
	}
	else{
		ret = gpio_pin_init(&(_motor->in_1));
		ret &= gpio_pin_init(&(_motor->in_2));
	}
	return ret;
}

Std_ReturnType Motor_Rotate_Right(Motor_t* _motor)
{
	Std_ReturnType ret = E_NOT_OK;
	if(((void*)0)==_motor){
		ret = E_NOT_OK;
	}
	else{
		ret = gpio_write_logic(&_motor->in_1,GPIO_HIGH);
		ret &= gpio_write_logic(&_motor->in_2,GPIO_LOW);
	}
	return ret;


}
Std_ReturnType Motor_Rotate_Left(Motor_t* _motor)
{
	Std_ReturnType ret = E_NOT_OK;
	if(((void*)0)==_motor){
		ret = E_NOT_OK;
	}
	else{
		ret = gpio_write_logic(&_motor->in_1,GPIO_LOW);
		ret &= gpio_write_logic(&_motor->in_2,GPIO_HIGH);
	}
	return ret;

}
Std_ReturnType Motor_Stop(Motor_t *_motor)
{
	Std_ReturnType ret = E_NOT_OK;
	if(((void*)0)==_motor){
		ret = E_NOT_OK;
	}
	else{
		ret = gpio_write_logic(&_motor->in_1,GPIO_LOW);
		ret &= gpio_write_logic(&_motor->in_2,GPIO_LOW);
	}
	return ret;

}

