#include "main.h"

typedef enum { NONE, MIN, MAX, LOW_GOAL, MID_LOW_GOAL, MID_HIGH_GOAL, HIGH_GOAL } presets;
int liftPreset = NONE;

bool xmtr2Connected = false;

/*
	Modifies input control into an exponential scale
*/
int expControl(int input)
{
	if (expScalingEnabled)
	{
		int sign = input / abs(input);
		input = abs(input);
		float scalingVal = 127.0 / (expScalingVal - 1);
		float percentMax = input / 127.0;
		float expMod = (float)(pow(expScalingVal, percentMax) - 1);
		int output = (int)round(scalingVal * expMod * sign);
		return output;
	}
	return input;
}

/*
	Check for preset buttons and assign it if preset buttons are pressed	
*/
void setPreset()
{
	if (liftPresetsEnabled)
	{
		if (vexRT[Btn8U] == 1)
		{
			liftPreset = HIGH_GOAL;
		}
		if (vexRT[Btn8L] == 1)
		{
			liftPreset = MID_HIGH_GOAL;
		}
		if (vexRT[Btn8R] == 1)
		{
			liftPreset = MID_LOW_GOAL;
		}
		if (vexRT[Btn8D] == 1)
		{
			liftPreset = LOW_GOAL;
		}
		if (vexRT[Btn7U] == 1)
		{
			liftPreset = MAX;
		}
		if (vexRT[Btn7D] == 1)
		{
			liftPreset = MIN;
		}
		if (vexRT[Btn7L] == 1)
		{
			if (!xmtr2Connected)
			{
				intakeSkyrise = (intakeSkyrise == 0 ? 1 : 0);
			}
		}
		if (xmtr2Connected)
		{
			if (vexRT[Btn7LXmtr2] == 1)
			{
				intakeSkyrise = 0; //Closed
			}
			if (vexRT[Btn7RXmtr2] == 1)
			{
				intakeSkyrise = 1; //Open
			}
		}
	}
}

void assignPreset()
{
	int index = liftPreset - 1;
	potLTarget = liftLVal[index];
	potRTarget = liftRVal[index];
}

// User control task
task usercontrol()
{
	potLTarget = 0;
	potRTarget = 0;
	liftActive = false;
	driveActive = false;
	int stickPrimary;
	int stickSecondary;
	StartTask(liftController);
	StartTask(pidController);

	while (true)
	{
		// Start music task 
		if (vexRT[Btn7R] == 1)
		{
			StartTask(playMusic);
		}

		// Limit movement
		stickPrimary = (abs(vexRT[Ch3]) > abs(vexRT[Ch4]) ? vexRT[Ch3] : vexRT[Ch4]);
		stickSecondary = (abs(vexRT[Ch3]) > abs(vexRT[Ch4]) ? vexRT[Ch3] : -vexRT[Ch4]);

		// Movement and strafing
		driveLF = stickPrimary;
		driveRB = stickPrimary;
		driveLB = stickSecondary;
		driveRF = stickSecondary;

		// Turning
		driveLF += vexRT[Ch1];
		driveLB += vexRT[Ch1];
		driveRF -= vexRT[Ch1];
		driveRB -= vexRT[Ch1];

		// Scaling
		driveLF = expControl(driveLF);
		driveLB = expControl(driveLB);
		driveRF = expControl(driveRF);
		driveRB = expControl(driveRB);

		// Assigning
		motor[dRF] = driveRF;
		motor[dRB] = driveRB;
		motor[dLF] = driveLF;
		motor[dLB] = driveLB;

		// Lift functions
		liftL = (vexRT[Btn5U] - vexRT[Btn5D]) * 127;
		liftR = liftL;
		setPreset();
		if ((vexRT[Btn5U] == 1) || (vexRT[Btn5D] == 1))
		{
			liftPreset = NONE;
			potLTarget = potRTarget = 0;
			liftActive = false;
		}
		else
		{
			if (liftPreset > NONE)
			{
				assignPreset();
			}
		}
		
		// Assign lift
		if ((vexRT[Btn5U] == 1) || (vexRT[Btn5D] == 1) || (liftPreset > NONE))
		{
			motor[liftLU] = liftL;
			motor[liftLD] = liftL;
			motor[liftRU] = liftR;
			motor[liftRD] = liftR;
		}

		// Intake
		intakeL = (vexRT[Btn6U] * 100 - vexRT[Btn6D] * 80) + 10;
		intakeR = intakeL;

		motor[iL] = intakeL;
		motor[iR] = intakeR;

		wait1Msec(20);
	}
}