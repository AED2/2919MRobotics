#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    PotentiometerLL, sensorPotentiometer)
#pragma config(Sensor, in2,    PotentiometerRL, sensorPotentiometer)
#pragma config(Sensor, in3,    Gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  Ultrasonic,     sensorSONAR_mm)
#pragma config(Sensor, I2C_1,  EncoderRDB,     sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  EncoderRDF,     sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  EncoderLDF,     sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_4,  EncoderLDB,     sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           RDF,           tmotorVex393, openLoop, encoder, encoderPort, I2C_2, 1000)
#pragma config(Motor,  port2,           RDB,           tmotorVex393, openLoop, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port3,           LLU,           tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port4,           LLD,           tmotorVex393, openLoop)
#pragma config(Motor,  port5,           RLU,           tmotorVex393, openLoop)
#pragma config(Motor,  port6,           RLD,           tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port7,           RIN,           tmotorVex393, openLoop)
#pragma config(Motor,  port8,           LIN,           tmotorVex393, openLoop)
#pragma config(Motor,  port9,           LDB,           tmotorVex393, openLoop, reversed, encoder, encoderPort, I2C_4, 1000)
#pragma config(Motor,  port10,          LDF,           tmotorVex393, openLoop, encoder, encoderPort, I2C_3, 1000)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

//Main Header
#include "main.h"

//Definitions
#include "definitions.c"

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

//Pre Auton
#include "preauton.c"

//Autonomous
#include "autonomous.c"

//Usercontrol
#include "usercontrol.c"
