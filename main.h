/*******************************************************************************
 *  main.h
 *  Created on: 15 jan 2024
 *  Author: Mohamed Eslam
 *******************************************************************************/

#ifndef MAIN_H_
#define MAIN_H_

#include "MCAL/Dio/Dio.h"
#include "HAL/DC_motor/DC_motor.h"
#include "HAL/Servo_Motor/Servo.h"
#include "MCAL/Timer1/Timer1.h"
#include "HAL/ultrasonic/ultrasonic.h"
#include "MCAL/USART/USART.h"
#include "HAL/IR_Sensor/IR_Sensor.h"

#define servo_catching 		0
#define servo_wrist			1
#define servo_elbow			2
#define servo_base			3
#define servo_ultra			4
#define servo_non			5

#define servo_catching_selected 	0
#define servo_wrist_selected		1
#define servo_elbow_selected		2
#define servo_base_selected			3
#define servo_ultra_selected		4
#define non_servo_selected			5

#define robot_move_forward_button	181
#define robot_move_backword_button	182
#define robot_move_right_button		183
#define robot_move_left_button		184
#define robot_stop					185


/**
 * @brief this function initialize the program
 * @param void
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType App_Init(void);
/**
 * @brief Move the selected servo from the application by selecting it with the
 * 		  mux selector lines and change the servo angle depending on the slider
 * @param (_cpy_selected_servo) : it's the selected servo by the user
 * 		  (cpy_servo_slider_data) : it's the current slider value
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Move_Selected_Servo(uint8 cpy_selected_servo,uint8 cpy_servo_slider_data);
/**
 * @brief this function select the servo which will move by changing the selector
 * 		  lines values from 0 to 4
 * @param (selected_servo) : it's the selected servo by the user
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Servo_Select_To_Move(uint8 cpy_selected_servo);
/**
 * @brief this function move the robot in any direction depending on  user selection
 * 		  from the 4 buttons to the 4 directions
 * @param (cpy_movement_direction) : it's the movement direction which the robot
 * 									 will go
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Robot_Move_Manually(uint8 cpy_movement_direction);
/**
 * @brief this function move the robot forward
 * @param void
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Robot_Move_Forward(void);
/**
 * @brief this function move the robot backward
 * @param void
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Robot_Move_Backword(void);
/**
 * @brief this function move the robot right
 * @param void
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Robot_Move_Right(void);
/**
 * @brief this function move the robot left
 * @param void
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Robot_Move_Left(void);
/**
 * @brief this function stop moving the robot
 * @param void
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType Robot_Stop(void);

void UART_RX_ISR(void);

#endif /* MAIN_H_ */
