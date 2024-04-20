/*
 * IR_Sensor.c
 *
 *  Created on: 2 Feb 2023
 *      Author: El-Wattaneya
 */
#include "IR_Sensor.h"

Std_ReturnType IR_Sensor_init(const IR_Sensor *_pin){
	Std_ReturnType ret = E_NOT_OK;
	ret = gpio_pin_dirction_init(&(_pin->DO));
	return ret;
}


IR_Sensor_s IR_Sensor_Status(const IR_Sensor *_pin){
	logic_t Pin_State=GPIO_LOW;
	Std_ReturnType ret = E_NOT_OK;
	ret = gpio_read_logic(&(_pin->DO),&Pin_State);
	IR_Sensor_s IR_Sensor_s = NON_BODY;
	if(Pin_State == GPIO_LOW)
	{
		IR_Sensor_s = BODY;
	}
	else if(Pin_State == GPIO_HIGH)
	{
		IR_Sensor_s = NON_BODY;
	}
	return IR_Sensor_s;
}

