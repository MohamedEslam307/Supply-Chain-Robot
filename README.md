# Supply Chain Robot

## Description
This project is aimed at controlling a robotic arm system using a microcontroller. The system utilizes servo motors to control different joints of the robotic arm and motor drivers to move the robot in various directions.

## Author
- Mohamed Eslam

## Version
- 01V


## Pin Configuration
- The pin configurations for the demultiplexer selector lines, motors, ultrasonic sensor, and buzzer are defined in `main.c`.

## Functionality
- The program initializes the required peripherals and sets up the system.
- It continuously monitors user input to control the robotic arm and movement of the robot.
- Servo motors are controlled based on user input sliders, while the robot's movement is controlled by button presses.
- An ultrasonic sensor is used to detect obstacles, and a buzzer is activated when an obstacle is detected within a certain range.


