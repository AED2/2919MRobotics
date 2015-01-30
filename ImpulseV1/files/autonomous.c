#include "main.h"

//Trim code
#include "/codebits/lifttrim.c"

//Compensation for skyrise weight code
#include "/codebits/liftcompensation.c"

//Autonomous functions

void clearEncoders()
{
	nMotorEncoder[lDrive2] = 0;
	nMotorEncoder[rDrive2] = 0;
}

void encoderMove(int encoderDistance,int leftDriveOneSpeed = 127, int leftDriveTwoSpeed = leftDriveOneSpeed, int rightDriveOneSpeed = leftDriveOneSpeed, int rightDriveTwoSpeed = leftDriveOneSpeed)
{
	clearEncoders();

	bool driveLBGoalReached = false;
	bool driveRBGoalReached = false;

	while (((driveLBGoalReached == false)||(driveRBGoalReached == false)))
	{
		leftTrackSpeed = leftDriveOneSpeed;
		leftTrackSpeed = leftDriveTwoSpeed;
		rightTrackSpeed = rightDriveOneSpeed;
		rightTrackSpeed = rightDriveTwoSpeed;

		if (abs(nMotorEncoder[lDrive2]) > encoderDistance)
		{
			driveLBGoalReached=true;
		}

		if (abs(nMotorEncoder[rDrive2]) > encoderDistance)
		{
			driveRBGoalReached=true;
		}
	}

	leftTrackSpeed = round(-(leftDriveOneSpeed)/driveBrakeConstant);
	leftTrackSpeed = round(-(leftDriveTwoSpeed)/driveBrakeConstant);
	rightTrackSpeed = round(-(rightDriveOneSpeed)/driveBrakeConstant);
	rightTrackSpeed = round(-(rightDriveTwoSpeed)/driveBrakeConstant);

	wait1Msec(100);

	leftTrackSpeed = 0;
	leftTrackSpeed = 0;
	rightTrackSpeed = 0;
	rightTrackSpeed = 0;
}

void gyroMove(int degrees,int leftDriveOneSpeed = 127, int leftDriveTwoSpeed = leftDriveOneSpeed, int rightDriveOneSpeed = leftDriveOneSpeed, int rightDriveTwoSpeed = leftDriveOneSpeed)
{
	degrees = abs(degrees*10);

	while (((abs(SensorValue[turningGyro]) > degrees - 50) && (abs(SensorValue[turningGyro]) > degrees + 50)) || ((abs(SensorValue[turningGyro]) < degrees - 50) && (abs(SensorValue[turningGyro]) < degrees + 50))) //not within 100 of set angle
	{
		leftTrackSpeed = leftDriveOneSpeed;
		leftTrackSpeed = leftDriveTwoSpeed;
		rightTrackSpeed = rightDriveOneSpeed;
		rightTrackSpeed = rightDriveTwoSpeed;
	}

	leftTrackSpeed = round(-(leftDriveOneSpeed)/driveBrakeConstant);
	leftTrackSpeed = round(-(leftDriveTwoSpeed)/driveBrakeConstant);
	rightTrackSpeed = round(-(rightDriveOneSpeed)/driveBrakeConstant);
	rightTrackSpeed = round(-(rightDriveTwoSpeed)/driveBrakeConstant);

	wait1Msec(100);

	leftTrackSpeed = 0;
	leftTrackSpeed = 0;
	rightTrackSpeed = 0;
	rightTrackSpeed = 0;
}

void ultrasonicMove(int distance, int leftDriveOneSpeed = 127, int leftDriveTwoSpeed = leftDriveOneSpeed, int rightDriveOneSpeed = leftDriveOneSpeed, int rightDriveTwoSpeed = leftDriveOneSpeed, bool LS = true, bool RS = true)
{
	if (LS && RS)
	{
		bool lSonicComplete = false;
		bool rSonicComplete = false;

		while (!lSonicComplete && !rSonicComplete)
		{
			if ((distance - SensorValue[sonicLeft]) < 0)
			{
				while(SensorValue[sonicLeft] > distance)
				{
					leftTrackSpeed = leftDriveOneSpeed;
					leftTrackSpeed = leftDriveTwoSpeed;
					rightTrackSpeed = rightDriveOneSpeed;
					rightTrackSpeed = rightDriveTwoSpeed;
				}
			}

			else
			{
				while (SensorValue[sonicLeft] < distance)
				{
					leftTrackSpeed = leftDriveOneSpeed;
					leftTrackSpeed = leftDriveTwoSpeed;
					rightTrackSpeed = rightDriveOneSpeed;
					rightTrackSpeed = rightDriveTwoSpeed;
				}
			}
		}
	}
	else if (LS)
	{
		if ((distance - SensorValue[sonicLeft]) < 0)
		{
			while (SensorValue[sonicLeft] > distance)
			{
				leftTrackSpeed = leftDriveOneSpeed;
				leftTrackSpeed = leftDriveTwoSpeed;
				rightTrackSpeed = rightDriveOneSpeed;
				rightTrackSpeed = rightDriveTwoSpeed;
			}
		}

		else
		{
			while (SensorValue[sonicLeft] < distance)
			{
				leftTrackSpeed = leftDriveOneSpeed;
				leftTrackSpeed = leftDriveTwoSpeed;
				rightTrackSpeed = rightDriveOneSpeed;
				rightTrackSpeed = rightDriveTwoSpeed;
			}
		}
	}

	else if (RS)
	{
		if ((distance - SensorValue[sonicRight]) < 0)
		{
			while (SensorValue[sonicRight] > distance)
			{
				leftTrackSpeed = leftDriveOneSpeed;
				leftTrackSpeed = leftDriveTwoSpeed;
				rightTrackSpeed = rightDriveOneSpeed;
				rightTrackSpeed = rightDriveTwoSpeed;
			}
		}

		else
		{
			while (SensorValue[sonicRight] < distance)
			{
				leftTrackSpeed = leftDriveOneSpeed;
				leftTrackSpeed = leftDriveTwoSpeed;
				rightTrackSpeed = rightDriveOneSpeed;
				rightTrackSpeed = rightDriveTwoSpeed;
			}
		}
	}

	leftTrackSpeed = round(-(leftDriveOneSpeed)/driveBrakeConstant);
	leftTrackSpeed = round(-(leftDriveTwoSpeed)/driveBrakeConstant);
	rightTrackSpeed = round(-(rightDriveOneSpeed)/driveBrakeConstant);
	rightTrackSpeed = round(-(rightDriveTwoSpeed)/driveBrakeConstant);

	wait1Msec(100);

	leftTrackSpeed = 0;
	leftTrackSpeed = 0;
	rightTrackSpeed = 0;
	rightTrackSpeed = 0;
}

void setLift(int potRT = 280,int speed = 127)
{
	potRTarget = potRT;
	liftTargetSpeed = speed;
	liftPreset = 10;
}

void moveLiftAuton()
{
	if (liftPreset > -1)
	{
		bool RLGoalReached = false;

		if (SensorValue[rPot] < potRTarget)
		{
			liftDirection = "up";
		}
		if (SensorValue[rPot] > potRTarget)
		{
			liftDirection = "down";
		}

		if (liftDirection == "up")
		{
			while (!(RLGoalReached) && (liftPreset > -1))
			{
				proportionalSpeed = abs(potRTarget - SensorValue[rPot])/proportionalSpeedScaling +0.3;

				if (proportionalSpeed > 1)
				{
					proportionalSpeed = 1;
				}

				if (SensorValue[rPot] < potRTarget)
				{
					leftLiftSpeed = round(liftTargetSpeed);
					rightLiftSpeed = round(liftTargetSpeed);

					liftCompensation();
				}
				else
				{
					RLGoalReached = true;
				}

				wait1Msec(20);
			}

			leftLiftSpeed = 0;
			rightLiftSpeed = 0;

			liftPreset = -1;
			}
		else if (liftDirection == "down")
		{
			while (!(RLGoalReached) && (liftPreset > -1))
			{
				proportionalSpeed = abs(potRTarget - SensorValue[rPot])/proportionalSpeedScaling;

				if (proportionalSpeed > 1)
				{
					proportionalSpeed = 1;
				}

				if (SensorValue[rPot] > potRTarget)
				{
					leftLiftSpeed = round(-liftTargetSpeed*proportionalSpeed);
					rightLiftSpeed = round(-liftTargetSpeed*proportionalSpeed);

					liftCompensation();
				}
				else
				{
					RLGoalReached = true;
				}

				wait1Msec(20);
			}

			leftLiftSpeed = 0;
			rightLiftSpeed = 0;

			liftPreset = -1;
		}

	}
}

void waitForLift()
{
	while (liftPreset > -1)
	{
		wait1Msec(20);
	}
}

void skyriseControl(int value, int delay, int threshold = lightSensorThreshold, bool fromDark = false)
{
	if (fromDark)
	{
		while(SensorValue[skyLight] > threshold)
		{
			wait1Msec(20);
		}
	}
	else
	{
		while(SensorValue[skyLight] < threshold)
		{
			wait1Msec(20);
		}
	}
	wait1Msec(delay);
	SensorValue[skyPiston] = value;
}

void needleControl(int value, int delay = 2000)
{
	wait1Msec(delay);
	SensorValue[needle] = value;
}

task autonLiftProcessing()
{
	while(true)
	{
		moveLiftAuton(); //Has a while loop
		liftTrim();
		wait1Msec(20);
	}
}

task autonMotorController()
{
	while(true)
	{
		motor[lDrive1] = leftTrackSpeed;
		motor[lDrive2] = leftTrackSpeed;
		motor[rDrive2] = rightTrackSpeed;
		motor[rDrive1] = rightTrackSpeed;

		motor[leftLift1] = leftLiftSpeed;
		motor[leftLift2] = leftLiftSpeed;
		motor[leftLift3] = leftLiftSpeed;
		motor[rightLift1] = rightLiftSpeed;
		motor[rightLift2] = rightLiftSpeed;
		motor[rightLift3] = rightLiftSpeed;

		wait1Msec(20);
	}
}

task songPlayer()
{
	surprise();
	StopTask(songPlayer);
}

task autonomous()
{
	//Initialise Autonomous
	StartTask(autonLiftProcessing);
	StartTask(autonMotorController);
	//StartTask(songPlayer);

	firstRun = false;

	//Claw Deployment
	leftLiftSpeed = 127;
	rightLiftSpeed = 127;
	wait1Msec(200);
	leftLiftSpeed = -127;
	rightLiftSpeed = -127;
	wait1Msec(200);
	leftLiftSpeed = 0;
	rightLiftSpeed = 0;

	/*Autonomous Selector
	if (compensation == -1) //Claw on left
	{
	}
	else if (compensation == 1) //claw on right
	{
	}
	else //Got nothing
	{
	}
	Most of this selection code is in pre-auton, look there */

	//Rubberband Deployment + Autoloader Height
	setLift(500,127);
	waitForLift();
	wait1Msec(50);

	//First Skyrise
	SensorValue[skyPiston] = 1; //Grab Skyrise
	setLift(550); //Lift skyrise out of autoloader
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 620
	setLift(300,5); //Lower Skyrise into base
	waitForLift();
	SensorValue[skyPiston] = 0; //Drop Skyrise
	setLift(500); //Lift above autoloader height
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 560

	//Second Skyrise
	skyriseControl(1,200); //Grab Skyrise
	setLift(550); //Lift skyrise out of autoloader
	waitForLift();
	setLift(50);
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 620
	setLift(500,5); //Lower Skyrise into Skyrise
	waitForLift();
	SensorValue[skyPiston] = 0; //Drop Skyrise
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 560

	//Third Skyrise
	skyriseControl(1,200); //Grab Skyrise
	setLift(550); //Lift skyrise out of autoloader
	waitForLift();
	setLift(200);
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 620
	setLift(700,5); //Lower Skyrise into Skyrise
	waitForLift();
	SensorValue[skyPiston] = 0; //Drop Skyrise
	setLift(500); //Lower to autoloader height
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 560

	//Fourth Skyrise
	skyriseControl(1,300); //Grab Skyrise
	setLift(550); //Lift skyrise out of autoloader
	waitForLift();
	setLift(450);
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 620
	setLift(850,5); //Lower Skyrise into Skyrise
	waitForLift();
	SensorValue[skyPiston] = 0; //Drop Skyrise
	setLift(500); //Lower to autoloader height
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 560

	//Fifth Skyrise
	skyriseControl(1,1000); //Grab Skyrise
	setLift(550);
	waitForLift();
	setLift(650);
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 620
	setLift(1000,60);
	waitForLift();
	SensorValue[skyPiston] = 0; //Drop Skyrise
	setLift(500);
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 560

	//Sixth Skyrise
	skyriseControl(1,1000); //Grab Skyrise
	setLift(550);
	waitForLift();
	setLift(800);
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 620
	setLift(1150,60);
	waitForLift();
	SensorValue[skyPiston] = 0; //Drop Skyrise
	setLift(500);
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 560

	//Seventh Skyrise
	skyriseControl(1,1000); //Grab Skyrise
	setLift(550);
	waitForLift();
	setLift(950);
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 620
	setLift(1300,60);
	waitForLift();
	SensorValue[skyPiston] = 0; //Drop Skyrise
	setLift(500);
	//ADD FAST TO SLOW DRIVE FUNCTION HERE FOR VALUE 560

}
