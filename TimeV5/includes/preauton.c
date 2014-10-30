#include "main.h"

void initialiseGyro()
{
	SensorType[turningGyro] = sensorNone;
	wait1Msec(500);
	SensorType[turningGyro] = sensorGyro;
	wait1Msec(2000);
	SensorScale[turningGyro] = 130;
  	SensorFullCount[turningGyro] = 3600;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
  // Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
	
	initialiseGyro();
	clearEncoders();
}