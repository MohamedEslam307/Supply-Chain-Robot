/*****************************************************************************
 *  main.c
 *  Created on: 15 jan 2024
 *  Author: Mohamed Eslam
 *  version : 01V
 *****************************************************************************/
#include "main.h"

pin_cofig_t A_demux_selector={
		.port =PORTC_INDEX ,
		.pin = GPIO_PIN0,
		.pin_direction= GPIO_DIRECTION_OUTPUT,
		.pin_logic=GPIO_LOW,
};
pin_cofig_t B_demux_selector={
		.port =PORTC_INDEX ,
		.pin = GPIO_PIN1,
		.pin_direction= GPIO_DIRECTION_OUTPUT,
		.pin_logic=GPIO_LOW,
};
pin_cofig_t C_demux_selector={
		.port =PORTC_INDEX ,
		.pin = GPIO_PIN2,
		.pin_direction= GPIO_DIRECTION_OUTPUT,
		.pin_logic=GPIO_LOW,
};
Motor_t RIGHT_MOTORS ={
		.in_1.port=PORTD_INDEX,
		.in_1.pin=GPIO_PIN2,
		.in_1.pin_direction=GPIO_DIRECTION_OUTPUT,
		.in_1.pin_logic=GPIO_LOW,

		.in_2.port=PORTD_INDEX,
		.in_2.pin=GPIO_PIN3,
		.in_2.pin_direction=GPIO_DIRECTION_OUTPUT,
		.in_2.pin_logic=GPIO_LOW,
};
Motor_t LEFT_MOTORS ={
		.in_1.port=PORTB_INDEX,
		.in_1.pin=GPIO_PIN0,
		.in_1.pin_direction=GPIO_DIRECTION_OUTPUT,
		.in_1.pin_logic=GPIO_LOW,

		.in_2.port=PORTB_INDEX,
		.in_2.pin=GPIO_PIN1,
		.in_2.pin_direction=GPIO_DIRECTION_OUTPUT,
		.in_2.pin_logic=GPIO_LOW,
};
ultrasonic_t ultrasonic={
		.echo.port=PORTD_INDEX,
		.echo.pin=GPIO_PIN6,
		.echo.pin_direction=GPIO_DIRECTION_INPUT,
		.echo.pin_logic=GPIO_LOW,

		.trigger.port=PORTC_INDEX,
		.trigger.pin=GPIO_PIN4,
		.trigger.pin_direction=GPIO_DIRECTION_OUTPUT,
		.trigger.pin_logic=GPIO_LOW,
};
pin_cofig_t buzzer ={
		.port =PORTA_INDEX ,
		.pin = GPIO_PIN3,
		.pin_direction= GPIO_DIRECTION_OUTPUT,
		.pin_logic=GPIO_LOW,
};
uint16 Distance=0;

volatile uint8 servo_slider_data=0,
		       servo_selected_flag=0,
			   received_data=0;

volatile uint8 selected_servo=0,
		       movement_direction=0,
			   move_car_manually_flag=0;

uint8 servo_catching_angle=0,
		   servo_wrist_angle=0,
		   servo_elbow_angle=0,
		   servo_base_angle=0,
		   servo_ultra_angle=0;

int main()
{
	Std_ReturnType ret = E_NOT_OK ;

	uint8 servo_prev_slider_data=0;

	ret = App_Init();

	Servo_Select_To_Move(non_servo_selected);
	while(1){
		/* check if the user moving slider of one servo*/
		if(servo_selected_flag){
			ret = Move_Selected_Servo(selected_servo,servo_slider_data);
		}
		/* check if the user moving the car*/
		else if (move_car_manually_flag){
			/* stop moving any servo*/
			Servo_Select_To_Move(non_servo_selected);
			/* take the ultasonic reading in case if there is any obstacle*/
			ret = Ultrasonic_GetDistance(&ultrasonic,&Distance);
			/* if there is an obstacle in 20cm range stop the robot and turn the
			 * buzzer on*/
			if(Distance<=20){
				Robot_Stop();
				gpio_write_logic(&buzzer,GPIO_HIGH);
			}
			/* if there is no obstacles then move the robot freely and turn the
			 * buzzer off*/
			else{
				gpio_write_logic(&buzzer,GPIO_LOW);
				ret = Robot_Move_Manually(movement_direction);
				move_car_manually_flag=0;
			}
		}
		else{/*Nothing*/}
	}

	return 0;
}

/**
 * @brief this function intialize the program
 * @param void
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType App_Init(void){
	Std_ReturnType ret = E_NOT_OK ;
	//initialize the demultiplexer selector lines
	ret = gpio_pin_init(&A_demux_selector);
	ret = gpio_pin_init(&B_demux_selector);
	ret = gpio_pin_init(&C_demux_selector);
	//initialize the buzzer pin
	ret = gpio_pin_init(&buzzer);
	//initialize the robot wheels motors
	ret = Motor_Init(&LEFT_MOTORS);
	ret = Motor_Init(&RIGHT_MOTORS);
	//enable global interrupt
	sei();
	ret = UltraSonic_Init(&ultrasonic);
	UART_Init();
	UART_RX_InterruptEnable();
	UART_RX_SetCallBack(UART_RX_ISR);

	return ret;
}

void UART_RX_ISR(void){
	UART_Receive_NoBlock(&received_data);
	/* if the user is moving one of the four sliders of servo motors, the servo
	 * will move depending on the slider value */

	/* there are 4 sliders the frist sends data from 0 to 45 so every step moving
	 * the servo by 4 degrees and the second slider sends data from 45 to 90 and so on*/

	if((received_data>=0)&&(received_data<=180)){
		servo_selected_flag=1;
		move_car_manually_flag=0;
		//divide the received data by 45 to know which slider is sending me the data
		selected_servo=received_data/45;
		servo_slider_data=received_data;
	}
	/* if the user wants to move the robot manually, the data would be the direction */
	else if ((received_data>=robot_move_forward_button)&&(received_data<=robot_move_left_button)){
		servo_selected_flag=0;
		move_car_manually_flag=1;
		movement_direction =received_data;
	}
	/* if the data was none of the above then it would be the destination
	 * which the robot go */
	else {
		servo_selected_flag=0;
		move_car_manually_flag=0;
	}
}

/**
 * @brief Move the selected servo from the application by selecting it with the
 * 		  mux selector lines and change the servo angle depending on the slider
 * @param (_cpy_selected_servo) : it's the selected servo by the user
 * 		  (cpy_servo_slider_data) : it's the current slider value
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Move_Selected_Servo(uint8 cpy_selected_servo,uint8 cpy_servo_slider_data){
	Std_ReturnType ret = E_NOT_OK ;

	switch(cpy_selected_servo){
		case servo_catching_selected:
			//assigning the slider value to the catching servo angle
			servo_catching_angle=cpy_servo_slider_data*4;
			//select the catching servo to move by the demultiplexer selector lines
			ret = Servo_Select_To_Move(servo_catching);
			//move the servo to the desired angle
			Servo_move_angle(servo_catching_angle);
			ret = E_OK;
			break;
		case servo_wrist_selected:
			//assigning the slider value to the wrist servo angle
			servo_wrist_angle=(cpy_servo_slider_data-45)*4;
			//select the wrist servo to move by the multiplexer selector lines
			ret = Servo_Select_To_Move(servo_wrist);
			//move the servo to the desired angle
			Servo_move_angle(servo_wrist_angle);
			ret = E_OK;
			break;
		case servo_elbow_selected:
			//assigning the slider value to the elbow servo angle
			servo_elbow_angle=(cpy_servo_slider_data-90)*4;
			//select the elbow servo to move by the multiplexer selector lines
			ret = Servo_Select_To_Move(servo_elbow);
			//move the servo to the desired angle
			Servo_move_angle(servo_elbow_angle);
			ret = E_OK;
			break;
		case servo_base_selected:
			//assigning the slider value to the base servo angle
			servo_base_angle = (cpy_servo_slider_data-135)*4;
			//select the base servo to move by the multiplexer selector lines
			ret = Servo_Select_To_Move(servo_base);
			//move the servo to the desired angle
			Servo_move_angle(servo_base_angle);
			ret = E_OK;
			break;

		default :
			ret = Servo_Select_To_Move(servo_non);
			ret = E_NOT_OK;
			break;
	}
	_delay_ms(50);
	return ret;
}

/**
 * @brief this function select the servo which will move by changing the selector
 * 		  lines values from 0 to 4
 * @param (cpy_selected_servo) : it's the selected servo by the user
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Servo_Select_To_Move(uint8 cpy_selected_servo){
	Std_ReturnType ret = E_NOT_OK ;
	switch(cpy_selected_servo){
		case servo_catching:
			//select the first servo (the catching servo )which is connected to
			//the demultiplexer output to take the output PWM from the MCU by
			//writing 000 on the selectors
			ret = gpio_write_logic(&A_demux_selector,GPIO_LOW);
			ret &= gpio_write_logic(&B_demux_selector,GPIO_LOW);
			ret &= gpio_write_logic(&C_demux_selector,GPIO_LOW);
			break;
		case servo_wrist:
			//select the second servo (the wrist servo )which is connected to
			//the demultiplexer output to take the output PWM from the MCU by
			//writing 001 on the selectors
			ret = gpio_write_logic(&A_demux_selector,GPIO_HIGH);
			ret &= gpio_write_logic(&B_demux_selector,GPIO_LOW);
			ret &= gpio_write_logic(&C_demux_selector,GPIO_LOW);
			break;
		case servo_elbow:
			//select the third servo (the elbow servo )which is connected to
			//the demultiplexer output to take the output PWM from the MCU by
			//writing 010 on the selectors
			ret = gpio_write_logic(&A_demux_selector,GPIO_LOW);
			ret &= gpio_write_logic(&B_demux_selector,GPIO_HIGH);
			ret &= gpio_write_logic(&C_demux_selector,GPIO_LOW);
			break;
		case servo_base:
			//select the fourth servo (the base servo )which is connected to
			//the demultiplexer output to take the output PWM from the MCU by
			//writing 011 on the selectors
			ret = gpio_write_logic(&A_demux_selector,GPIO_HIGH);
			ret &= gpio_write_logic(&B_demux_selector,GPIO_HIGH);
			ret &= gpio_write_logic(&C_demux_selector,GPIO_LOW);
			break;
		case servo_ultra:
			//select the fifth servo (the servo attached to the ultrasonic )which
			//is connected to the demultiplexer output to take the output PWM from
			//the MCU by writing 011 on the selectors
			ret = gpio_write_logic(&A_demux_selector,GPIO_LOW);
			ret &= gpio_write_logic(&B_demux_selector,GPIO_LOW);
			ret &= gpio_write_logic(&C_demux_selector,GPIO_HIGH);
			break;
		default:
			//if none of the servos selected, set all selector lines to prevent
			//moving any of the motors
			ret = gpio_write_logic(&A_demux_selector,GPIO_HIGH);
			ret &= gpio_write_logic(&B_demux_selector,GPIO_HIGH);
			ret &= gpio_write_logic(&C_demux_selector,GPIO_HIGH);
			ret = E_NOT_OK ;
			break;
	}
	return ret;
}

/**
 * @brief this function move the robot in any direction depending on  user selection
 * 		  from the 4 buttons to the 4 directions
 * @param (cpy_movement_direction) : it's the movement direction which the robot
 * 									 will go
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Robot_Move_Manually(uint8 cpy_movement_direction){
	Std_ReturnType ret = E_NOT_OK ;
	switch(cpy_movement_direction){
		case robot_move_forward_button :
			Robot_Move_Forward();
			break;
		case robot_move_backword_button :
			Robot_Move_Backword();
			break;
		case robot_move_right_button :
			Robot_Move_Right();
			break;
		case robot_move_left_button :
			Robot_Move_Left();
			break;
		case robot_stop:
			Robot_Stop();
			break;
	}
	return ret;
}

/**
 * @brief this function move the robot forward
 * @param void
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Robot_Move_Forward(void){
	Std_ReturnType ret = E_NOT_OK ;
	ret = Motor_Rotate_Right(&RIGHT_MOTORS);
	ret &= Motor_Rotate_Right(&LEFT_MOTORS);
	return ret;
}

/**
 * @brief this function stop moving the robot
 * @param void
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Robot_Stop(void){
	Std_ReturnType ret = E_NOT_OK ;
	ret = Motor_Stop(&RIGHT_MOTORS);
	ret &= Motor_Stop(&LEFT_MOTORS);
	return ret;
}

/**
 * @brief this function move the robot backward
 * @param void
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Robot_Move_Backword(void){
	Std_ReturnType ret = E_NOT_OK ;
	ret = Motor_Rotate_Left(&RIGHT_MOTORS);
	ret &= Motor_Rotate_Left(&LEFT_MOTORS);
	return ret;
}

/**
 * @brief this function move the robot right
 * @param void
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Robot_Move_Right(void){
	Std_ReturnType ret = E_NOT_OK ;
	ret = Motor_Rotate_Left(&RIGHT_MOTORS);
	ret &= Motor_Rotate_Right(&LEFT_MOTORS);
	return ret;
}

/**
 * @brief this function move the robot left
 * @param void
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Robot_Move_Left(void){
	Std_ReturnType ret = E_NOT_OK ;
	ret = Motor_Rotate_Right(&RIGHT_MOTORS);
	ret &= Motor_Rotate_Left(&LEFT_MOTORS);
	return ret;
}
