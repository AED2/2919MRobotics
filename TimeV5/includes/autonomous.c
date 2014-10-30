#include "main.h"

//Autonomous functions

void drive(int encoderDistance,int driveLBSpeed,int driveLFSpeed,int driveRBSpeed,int driveRFSpeed)
{
	clearPreviousError();
	clearEncoders();

	bool driveLBGoalReached = false;
	bool driveRBGoalReached = false;
	bool driveLFGoalReached = false;
	bool driveRFGoalReached = false;

	//Set Drive to Active
	driveActive = true;
	encoderDistance = abs(encoderDistance);

	while (((driveLBGoalReached == false)||(driveRBGoalReached == false)||(driveRFGoalReached == false)||(driveLFGoalReached == false)) && (driveActive))
	{
		if (abs(nMotorEncoder[driveLB]) < encoderDistance)
		{

			motor[driveLB] = driveLBSpeed;
		}
		else
		{
			motor[driveLB] = 0;
			driveLBGoalReached=true;
		}

		if (abs(nMotorEncoder[driveRB]) < encoderDistance)
		{
			motor[driveRB] = driveRBSpeed;
		}
		else
		{
			motor[driveRB] = 0;
			driveRBGoalReached=true;
		}

		if (abs(nMotorEncoder[driveLF]) < encoderDistance)
		{
			motor[driveLF] = driveLFSpeed;
		}
		else
		{
			motor[driveLF] = 0;
			driveLFGoalReached=true;
		}

		if (abs(nMotorEncoder[driveRF]) < encoderDistance)
		{
			motor[driveRF] = driveRFSpeed;
		}
		else
		{
			motor[driveRF] = 0;
			driveRFGoalReached=true;
		}
	}

	//Set Drive to inactive
	driveActive = false;
	motor[driveRF]=0;

}

void gyroTurn(int degrees,int driveLBSpeed,int driveLFSpeed,int driveRBSpeed,int driveRFSpeed)
{

	bool driveGoalReached = false;

	//Set Drive to Active
	driveActive = true;
	degrees = abs(degrees);

	while (!(driveGoalReached) && (driveActive))
	{
		if ((abs(SensorValue[turningGyro]) < degrees)) //not within 100 of set angle
		{

			motor[driveLB] = driveLBSpeed;
			motor[driveRB] = driveRBSpeed;
			motor[driveLF] = driveLFSpeed;
			motor[driveRF] = driveRFSpeed;
		}
		else
		{
			motor[driveLB] = (abs(driveLBSpeed)/driveLBSpeed)*10;
			motor[driveRB] = (abs(driveRBSpeed)/driveRBSpeed)*10;
			motor[driveLF] = (abs(driveLFSpeed)/driveLFSpeed)*10;
			motor[driveRF] = (abs(driveRFSpeed)/driveRFSpeed)*10;
			wait1Msec(50);
			motor[driveLB] = 0;
			motor[driveRB] = 0;
			motor[driveLF] = 0;
			motor[driveRF] = 0;
			driveGoalReached=true;
		}
	}

	//Set Drive to inactive
	driveActive = false;

}

void liftAutonMonitor()
{
	if ((potLTarget != 0) && (potRTarget != 0))
	{
		liftActive = true;

		bool LLGoalReached = false;
		bool RLGoalReached = false;

		string direction;

		if (potR < potRTarget)
		{
			direction = "up";
		}
		if (potR > potRTarget)
		{
			direction = "down";
			potRTarget = potRTarget + 100;
		}

		if ((abs(potR)-30 < abs(potRTarget)) && ((abs(potR)+30 > abs(potRTarget))))
		{
			lL = 0;
			lR = 0;
			potRTarget = 0;
			potLTarget = 0;
		}

		else if ((potRTarget != 0) && (potLTarget != 0) && (liftActive))
		{
			if (direction == "down")
			{
				while ((LLGoalReached == false) || (RLGoalReached == false) && (liftActive) )
				{
					if (potR > potRTarget)
					{
						lL = -(liftTargetSpeed);
					}
					else
					{
						LLGoalReached = true;
						lL = 0;
					}
					if (potR > potRTarget)
					{
						lR = -(liftTargetSpeed);
					}
					else
					{
						RLGoalReached = true;
						lR = 0;
					}
				}



			}

			else if (direction == "up")
			{
				while ((LLGoalReached == false) || (RLGoalReached == false) && (liftActive))
				{
					if (potR < potRTarget)
					{
						lL = (liftTargetSpeed);
					}
					else
					{
						LLGoalReached = true;
						lL = 0;
					}
					if (potR < potRTarget)
					{
						lR = liftTargetSpeed;
					}
					else
					{
						RLGoalReached = true;
						lR = 0;
					}
				}
			}
		}

		liftActive = false;
	}
}

void lift(int potLT,int potRT,int speed)
{
	potLTarget = potLT;
	potRTarget = potRT;
	liftTargetSpeed = speed;
}

void intake(int speed)
{
	motor[intakeL] = speed;
	motor[intakeR] = speed;
}

void tLiftUp(int time,int speed)
{
			lL = speed;
			lR = speed;
			wait1MSec(time);
			lL = 0;
			lR = 0;
		

}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task liftController()
{

	while(true)
	{
		liftAutonMonitor();
		EndTimeSlice();
	}
}

task motorController() //Assigns motor values from buffer
{
	while(true)
	{
		motor[liftLU] = lL+10;
		motor[liftLD] = lL+10;
		motor[liftRU] = lR+10;
		motor[liftRD] = lR+10;

		wait1Msec(20);
	}
}

task autonomous()
{
	//Initialise Autonomous
	StartTask(motorController);
	StartTask(liftController);
//	StartTask(antiJam);
	initialiseDrive();
	
	
	// Autonomous Routine
	// Feel free to modify the values
/*
	if (autonChooser < 1000) //Pot is on Right
	{
		//STRAFES RIGHT
		liftMove(500,127);
		wait1Msec(2000);
		intake(-120);
		wait1Msec(3000);
		intakeStop;
		driveStraightBack(500,127);
		wait1Msec(500);
		driveStrafeRight(650,80);
		driveStrafeLeft(400,100);
	}
	else if (autonChooser > 3000)
	{
		//STRAFES LEFT
		liftMove(500,127);
		wait1Msec(2000);
		intake(-120);
		wait1Msec(3000);
		intakeStop;
		driveStraightBack(500,127);
		wait1Msec(500);
		driveStrafeLeft(650,80);
		driveStrafeRight(400,100);
	}
	else if ((autonChooser > 1001) && (autonChooser < 2999))
	{
		//SCORES SINGLE CUBE ON POST
		liftMove(500,127);
		wait1Msec(2000);
		intake(-120);
		wait1Msec(3000);
		intakeStop;
		driveStraightBack(300,127);
	}
*/
	//Full blue auton below
/*

	//Part 1
	driveStraightForward(400,127);
	intakeIn;
	wait1Msec(500);
	intakeStop;
	liftMove(1000, 127);
	driveStraightBack(400,127);


	//Part2
	drivePointTurnLeft(350,127);
	intakeIn;
	wait1Msec(3000);
	intakeStop;
	liftMove(1,127);
	drivePointTurnLeft(1500,127);


	// Part 3
	driveStraightForward(300,127);
	intakeIn;
	wait1Msec(1000);
	intakeStop;
	//driveStrafeRight(300,127); //May not be needed
	liftMove(1400,127);
	driveSwerveTurnLeft(300,127);
	intakeOut;
	wait1Msec(1000);
	intakeStop;*/

	//Full red auton below
	//Part 1

	driveStraightForward(400,127);
	intakeIn;
	wait1Msec(500);
	intakeStop;
	liftMove(600, 127);
	driveStraightBack(400,127);


	//Part2
	gyroPointTurnLeft(250,80);	
	driveStraightForward(100,127);
	intakeOut;
	wait1Msec(3000);
	intakeStop;
	liftMove(1,127);
/*


	// Part 3
	driveStraightForward(300,127);
	intakeIn;
	wait1Msec(1000);
	intakeStop;
	//driveStrafeLeft(300,127); //May not be needed
	liftMove(1400,127);
	driveSwerveTurnRight(300,127);
	intakeOut;
	wait1Msec(1000);
	intakeStop;
*/
	//Backup Auton - 2 points
	/*
	intakeOut;
	wait1Msec(300);
	intakeIn;
	wait1Msec(300);
	intakeOut;
	wait1Msec(75);
	intakeStop;
	driveStraightForward(400,127);
	intakeIn;
	wait1Msec(3000);
	intakeStop;
	*/

	//Backup Auton - 3 points
	/*
	liftMove(500,127);
	intake(-100);
	wait1Msec(4000);
	intakeStop;

	*/

}
