#pragma config(Sensor, dgtl1,  UL,             sensorSONAR_mm)
#pragma config(Sensor, dgtl3,  UR,             sensorSONAR_mm)
#pragma config(Sensor, dgtl5,  ,               sensorSONAR_mm)
#pragma config(Sensor, dgtl7,  ,               sensorSONAR_mm)
#pragma config(Sensor, dgtl9,  ,               sensorDigitalIn)
#pragma config(Sensor, dgtl10, ,               sensorDigitalOut)
#pragma config(Sensor, dgtl11, ,               sensorDigitalIn)
#pragma config(Sensor, dgtl12, ,               sensorDigitalOut)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define round(x) ((int)((x)>=0?(long)((x)+0.5):(long)((x)-0.5)))

int fineControlValue = 5;
int speed = 7;
int sensor;

int slopeLimiter(int oldValue, int newValue, int maxStepSize)
{
        int delta = newValue - oldValue;
        if (delta > maxStepSize)
        {
                delta = maxStepSize;
        }
        if (delta < 0 - maxStepSize)
        {
                delta = 0 - maxStepSize;
        }
		if (newValue == 0)
		{
			delta = 0;
		}
        return delta;
}

//Code for finer driving control
int fineControl(int input) //input from value, mod is set to driver preferences
{
		int sign = input / abs(input);

		input = abs(input);

		float percentage = (input/127.0); //Find relative speed to max speed
		float fineMod = (pow(fineControlValue,percentage)-1); //find the modifier value to multiply by
		float scalingValue = (127.0/(fineControlValue-1)); //value to multiply back by after modification

		int output = (int)(round(scalingValue*sign*fineMod));

		return(output);
}

task increaseSpeed()
{
	while(true)
	{
		while (speed < 127)
		{
			wait1Msec(5000);
			speed += 10;
		}
		while (speed > -127)
		{
			wait1Msec(5000);
			speed -= 10;
		}
	}
}

int controllerSpeed;
int motorSpeed = 0;
int oldMotorSpeed = 0;

task main()
{
	StartTask(increaseSpeed);
	while(true)
	{
		sensor = SensorValue[UR] + SensorValue[UL];
		controllerSpeed = fineControl(speed);
		oldMotorSpeed = motorSpeed;
		motorSpeed += slopeLimiter(oldMotorSpeed,controllerSpeed,1);
		wait1Msec(20);
	}
}
