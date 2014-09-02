#include "main.h"

void drive(int encoderDistance, int driveLBSpeed, int driveLFSpeed, int driveRBSpeed, int driveRFSpeed)
{
	clearPreviousErrors();
	resetEncoders();

	bool lBGoalReached = false;
	bool lFGoalReached = false;
	bool rBGoalReached = false;
	bool rFGoalReached = false;

	driveAutonVal = abs(encoderDistance);
	driveActive = true;

	while (((!lBGoalReached) || (!lFGoalReached) || (!rBGoalReached) || (rFGoalReached)) && (driveActive))
	{
		if (encoder(driveLB) < driveAutonVal)
		{
			motor[dLB] = driveLBSpeed;
		}
		else
		{
			motor[dLB] = 0;
			lBGoalReached = true;
		}
		if (encoder(driveRB) < driveAutonVal)
		{
			motor[dRB] = driveRBSpeed;
		}
		else
		{
			motor[dRB] = 0;
			rBGoalReached = true;
		}
		if (encoder(driveLF) < driveAutonVal)
		{
			motor[dLF] = driveLFSpeed;
		}
		else
		{
			motor[dLF] = 0;
			lFGoalReached = true;
		}
		if (encoder(driveRF) < driveAutonVal)
		{
			motor[dRF] = driveRFSpeed;
		}
		else
		{
			motor[dRF] = 0;
			rFGoalReached = true;
		}
	}
	driveActive = false;
}

void lift(int potLT, int potRT, int speed)
{
	potLTarget = potLT;
	potRTarget = potRT;
	liftTargetSpeed = speed;
}

void intake(int speed)
{
	motor[iL] = speed;
	motor[iR] = speed;
}

task motorController()
{
	while (true)
	{
		motor[liftLU] = liftL;
		motor[liftLD] = liftL;
		motor[liftRU] = liftR;
		motor[liftRD] = liftR;
		wait1Msec(60);
	}
}

task autonomous()
{
	resetEncoders();
	StartTask(antiJam);
	StartTask(motorController);

	liftMove(1000, 100);
	intakeOut();
	liftMove(0, 100);
	drivePointRight(90, 100);
	driveForward(100, 200);
	intakeIn();
}