#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    potLL,          sensorPotentiometer)
#pragma config(Sensor, in2,    potRL,          sensorPotentiometer)
#pragma config(Sensor, in3,    aCh,            sensorPotentiometer)
#pragma config(Sensor, in4,    turningGyro,    sensorGyro)
#pragma config(Sensor, in5,    lineL,          sensorLineFollower)
#pragma config(Sensor, in6,    lineM,          sensorLineFollower)
#pragma config(Sensor, in7,    lineR,          sensorLineFollower)
#pragma config(Sensor, dgtl1,  ultraL,         sensorSONAR_mm)
#pragma config(Sensor, dgtl3,  ultraR,         sensorSONAR_mm)
#pragma config(Sensor, dgtl6,  encoderIN,      sensorQuadEncoder)
#pragma config(Sensor, dgtl12, piston,         sensorDigitalOut)
#pragma config(Sensor, I2C_1,  encoderRDB,     sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  encoderRDF,     sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  encoderLDF,     sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_4,  encoderLDB,     sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           driveRF,       tmotorVex393, openLoop, encoder, encoderPort, I2C_2, 1000)
#pragma config(Motor,  port2,           driveRB,       tmotorVex393, openLoop, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port3,           liftLU,        tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port4,           liftLD,        tmotorVex393, openLoop)
#pragma config(Motor,  port5,           liftRU,        tmotorVex393, openLoop)
#pragma config(Motor,  port6,           liftRD,        tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port7,           intakeR,       tmotorVex393, openLoop)
#pragma config(Motor,  port8,           intakeL,       tmotorVex393, openLoop)
#pragma config(Motor,  port9,           driveLB,       tmotorVex393, openLoop, reversed, encoder, encoderPort, I2C_4, 1000)
#pragma config(Motor,  port10,          driveLF,       tmotorVex393, openLoop, encoder, encoderPort, I2C_3, 1000)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

//Header Files
#include "/headers/definitions.h"

#include "/headers/variables.h"

#include "/headers/main.h"

//Includes Files

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

//Pre Auton
#include "/includes/preauton.c"

//Autonomous
#include "/includes/autonomous.c"

//Usercontrol
#include "/includes/usercontrol.c"
