#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    LL_pot,         sensorPotentiometer)
#pragma config(Sensor, in2,    RL_pot,         sensorPotentiometer)
#pragma config(Sensor, in3,    claw_position,            sensorPotentiometer)
#pragma config(Sensor, I2C_1,  RDB_encoder,    sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  RDF_encoder,    sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  LDF_encoder,    sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_4,  LDB_encoder,    sensorQuadEncoderOnI2CPort,    , AutoAssign)
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

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

//Definitions
#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

//Global Variables
bool speedstep_drive_enabled = false;
bool speedstep_lift_enabled = false;
bool exp_ctrl_enabled = true;
bool lift_preset_enabled = true;
bool interrupt = false;
bool mounted_skyrise;
int lift_preset = 0;
const int exp_scaling_val = 5;
const int lift_val[] = {1,1550,680,1140,1400,1500};
int lift_auton_val[] = {0,0};
int lift_error = SensorValue[claw_position]-40;
const int limit_LL_pot = 1460;

//const int limit_RL_pot = 1592;
const int trim_switch = 150;

//Motor Usercontrol Variables
int rightLift;
int leftLift;
int leftDF;
int rightDF;
int leftDB;
int rightDB;

//Initialise speedstep variables
//	float target_scale;
	int speed = 0;
	int error = 0;
	int integral = 0;
	int derivitive = 0;
	int previous_error[] = {0,0,0,0};

//Tune these values
	float Kp = 0.7;
	float Ki = 0.12;
	float Kd = 0.1;


//Potentiometer Scaling
int target_LL_pot(int target_RL_pot)
{
	int target = (int) round(-457.9*((target_RL_pot-1408)/-478.2)+1375);

	if (target > limit_LL_pot)
	{
		target = limit_LL_pot;
	}
	else if (target < 0)
	{
		target = 0;
	}

	return(target);
}

//Experimental Functions
	//Controller Exponential Scaling
	int expCtrl(int input,int modifier) //ctrlIN from controller value, mod is set to driver prefrences
	{
		if (exp_ctrl_enabled == true)
		{
			int sign = input/abs(input);

			input = abs(input);
			modifier = abs(modifier);

			float scaling_value = 127.0/(modifier-1);
			float percent_max = input/127.0;
			float exp_modifier = (float)(pow(modifier,percent_max)-1);
			int output = (int)round(scaling_value*exp_modifier*sign);
			return(output);
		}
		else
		{
			return(input);
		}
	}

	//Speedstep for Drive
	int speedstep_drive(int target,int speed_limit,int sensor_reading,int array_id) //PID for Autonomus Drive
	{
		if (speedstep_drive_enabled == true)
		{
			error = target - sensor_reading;
			integral = integral + error;

			if (error == 0)
			{
				integral = 0;
			}

			if (abs(error) > 40)
			{
				integral = 0;
			}

			derivitive = error - previous_error[array_id];
			previous_error[array_id] = error;
			speed = (int) round(Kp*error + Ki*integral + Kd*derivitive);

			if (speed > speed_limit)
			{
				speed = speed_limit;
			}
			return speed;
		}
		else
			{
				return speed_limit;
			}
	}
	//Speedstep for Lift
	int speedstep_lift(int target,int speed_limit,int sensor_reading,int array_id) //PID for Autonomus Drive
	{
		if (speedstep_lift_enabled == true)
		{
			error = target - sensor_reading;
			integral = integral + error;

			if (error == 0)
			{
				integral = 0;
			}

			if (abs(error) > 40)
			{
				integral = 0;
			}

			derivitive = error - previous_error[array_id];
			previous_error[array_id] = error;
			speed = (int) round(Kp*error + Ki*integral + Kd*derivitive);

			if (speed > speed_limit)
			{
				speed = speed_limit;
			}
			return speed;
		}
		else
			{
				return speed_limit;
			}
	}

//Autonomous Functions

void clear_encoders()
{
	nMotorEncoder[LDF] = 0;
	nMotorEncoder[RDF] = 0;
	nMotorEncoder[LDB] = 0;
	nMotorEncoder[RDB] = 0;
}

void clear_previous_error()
{
	previous_error[0] = 0;
	previous_error[1] = 0;
	previous_error[2] = 0;
	previous_error[3] = 0;
}

void extStoppers()
{
	motor[LDB]=127;
	motor[RDB]=127;
	motor[LDF]=-127;
	motor[RDF]=-127;
	wait1Msec(1000);
	motor[LDB]=0;
	motor[RDB]=0;
	motor[LDF]=0;
	motor[RDF]=0;
}

void tMove(int time,int speed) //Goes forward, neg speed for back
{
	motor[LDB]=speed;
	motor[RDB]=speed;
	motor[LDF]=speed;
	motor[RDF]=speed;
	wait1Msec(time);
	motor[LDB]=0;
	motor[RDB]=0;
	motor[LDF]=0;
	motor[RDF]=0;
}

void tStrafe(int time, int speed) //Strafes right, neg speed for left
{
	motor[LDF]=speed;
	motor[LDB]=-speed;
	motor[RDF]=-speed;
	motor[RDB]=speed;
}

void tRTurn(int time,int speed) //Turns to right, neg speed for left
{
	motor[LDF]=speed;
	motor[LDB]=speed;
	motor[RDF]=-speed;
	motor[RDB]=-speed;
	wait1Msec(time);
	motor[LDB]=0;
	motor[RDB]=0;
	motor[LDF]=0;
	motor[RDF]=0;
}

void tLiftUp(int time,int speed)
{
	bool RL_goal_reached = false;

	while (RL_goal_reached == false)
	{
		if (SensorValue[RL_pot] < time)
		{
			motor[LLD] = speed;
			motor[LLU] = speed;
			motor[RLD] = speed;
			motor[RLU] = speed;
			//if ((lift_error >= 200) && (lift_error <= 250))
			//{
			//}
			//else if (lift_error < 0)
			//{
			//motor[LLD] = speed;
			//motor[LLU] = speed;
			//motor[RLD] = speed-60;
			//motor[RLU] = speed-60;
			//}
			//if ((lift_error >= 200) && (lift_error <= 250))
			//{
			//}

			//else if (lift_error > 0)
			//{
			//motor[LLD] = speed-60;
			//motor[LLU] = speed-60;
			//motor[RLD] = speed;
			//motor[RLU] = speed;
			//}
		}

		else
		{
			RL_goal_reached = true;
			motor[LLD] = 20;
			motor[LLU] = 20;
			motor[RLD] = 20;
			motor[RLU] = 20;
		}
	}
}
void tLiftDown(int time,int speed)
{
	bool LL_goal_reached = false;
	bool RL_goal_reached = false;

	while (RL_goal_reached == false)
	{
		if (SensorValue[RL_pot] > time)
		{
			motor[LLD] = -speed;
			motor[LLU] = -speed;
			motor[RLD] = -speed;
			motor[RLU] = -speed;
			if ((lift_error >= 200) && (lift_error <= 250))
			{
			}
			else if (lift_error < 0)
			{
			motor[LLD] = -speed;
			motor[LLU] = -speed;
			motor[RLD] = -speed+100;
			motor[RLU] = -speed+100;
						}
			if ((lift_error >= 200) && (lift_error <= 250))
			{
			}

			else if (lift_error > 0)
				{
			motor[LLD] = -speed+100;
			motor[LLU] = -speed+100;
			motor[RLD] = -speed;
			motor[RLU] = -speed;
				}

		}
	else
	{
		RL_goal_reached = true;
		motor[LLD] = 20;
		motor[LLU] = 20;
		motor[RLD] = 20;
		motor[RLU] = 20;
	}
}
}



void drive_move(int encoder_distance,int speed)
{
	clear_previous_error();
	clear_encoders();

	bool LDB_goal_reached = false;
	bool RDB_goal_reached = false;
	bool LDF_goal_reached = false;
	bool RDF_goal_reached = false;

	//Data validation
	speed = abs(speed);

	while ((LDB_goal_reached == false)||(RDB_goal_reached == false)||(LDF_goal_reached == false)||(RDF_goal_reached == false))
	{
		if (abs(nMotorEncoder[LDB]) < abs(encoder_distance))
		{
			motor[LDB] = speedstep_drive(encoder_distance,speed,nMotorEncoder[LDB],3);
		}
		else
		{
			motor[LDB] = 0;
			LDB_goal_reached=true;
		}

		if (abs(nMotorEncoder[RDB]) < abs(encoder_distance))
		{
			motor[RDB] = speedstep_drive(encoder_distance,speed,nMotorEncoder[RDB],0);
		}
		else
		{
			motor[RDB] = 0;
			RDB_goal_reached=true;
		}

		if (abs(nMotorEncoder[LDF]) < abs(encoder_distance))
		{
			motor[LDF] = speedstep_drive(encoder_distance,speed,nMotorEncoder[LDF],2);
		}
		else
		{
			motor[LDF] = 0;
			LDF_goal_reached=true;
		}

		if (abs(nMotorEncoder[RDF]) < abs(encoder_distance))
		{
			motor[RDF] = speedstep_drive(encoder_distance,speed,nMotorEncoder[RDF],1);
		}
		else
		{
			motor[RDF] = 0;
			RDF_goal_reached=true;
		}
	}
}

void drive_turn(int encoder_distance,int speed)
{
	clear_previous_error();
	clear_encoders();

	bool LDB_goal_reached = false;
	bool RDB_goal_reached = false;
	bool LDF_goal_reached = false;
	bool RDF_goal_reached = false;

	while ((LDB_goal_reached == false)||(RDB_goal_reached == false)||(LDF_goal_reached == false)||(RDF_goal_reached == false))
	{
		if (abs(nMotorEncoder[LDB]) < abs(encoder_distance))
		{
			motor[LDB] = speedstep_drive(encoder_distance,speed,nMotorEncoder[LDB],3);
		}
		else
		{
			motor[LDB] = 0;
			LDB_goal_reached=true;
		}

		if (abs(nMotorEncoder[RDB]) < abs(encoder_distance))
		{
			motor[RDB] = -(speedstep_drive(encoder_distance,speed,nMotorEncoder[RDB],0));
		}
		else
		{
			motor[RDB] = 0;
			RDB_goal_reached=true;
		}

		if (abs(nMotorEncoder[LDF]) < abs(encoder_distance))
		{
			motor[LDF] = speedstep_drive(encoder_distance,speed,LDF_encoder,2);
		}
		else
		{
			motor[LDF] = 0;
			LDF_goal_reached=true;
		}

		if (abs(nMotorEncoder[RDF]) < abs(encoder_distance))
		{
			motor[RDF] = -(speedstep_drive(encoder_distance,speed,RDF_encoder,1));
		}
		else
		{
			motor[RDF] = 0;
			RDF_goal_reached=true;
		}
	}
}

void drive_strafe(int encoder_distance,int speed)
{
	clear_previous_error();

	bool LDB_goal_reached = false;
	bool RDB_goal_reached = false;
	bool LDF_goal_reached = false;
	bool RDF_goal_reached = false;


	while ((LDB_goal_reached == false)||(RDB_goal_reached == false)||(LDF_goal_reached == false)||(RDF_goal_reached == false))
	{
		if (abs(nMotorEncoder[LDB]) < abs(encoder_distance))
		{
			motor[LDB] = -(speedstep_drive(encoder_distance,speed,nMotorEncoder[LDB],3));
		}
		else
		{
			motor[LDB] = 0;
			LDB_goal_reached=true;
		}

		if (abs(nMotorEncoder[RDB]) < abs(encoder_distance))
		{
			motor[RDB] = speedstep_drive(encoder_distance,speed,nMotorEncoder[RDB],0);
		}
		else
		{
			motor[RDB] = 0;
			RDB_goal_reached=true;
		}

		if (abs(nMotorEncoder[LDF]) < abs(encoder_distance))
		{
			motor[LDF] = speedstep_drive(encoder_distance,speed,LDF_encoder,2);
		}
		else
		{
			motor[LDF] = 0;
			LDF_goal_reached=true;
		}

		if (abs(nMotorEncoder[RDF]) < abs(encoder_distance))
		{
			motor[RDF] = -(speedstep_drive(encoder_distance,speed,RDF_encoder,1));
		}
		else
		{
			motor[RDF] = 0;
			RDF_goal_reached=true;
		}
	}
}

void lift_move(int target,int speed)
{
	clear_previous_error();
	lift_auton_val[0] = target;
	lift_auton_val[1] = speed;
}

void lift_auton_monitor()
{
	if (lift_auton_val[0] == 0)
	{

	}

	else
	{
		bool LL_goal_reached = false;
		bool RL_goal_reached = false;

		string direction;

		if (SensorValue[RL_pot] < lift_auton_val[0])
		{
			direction = "up";
		}
		if (SensorValue[RL_pot] > lift_auton_val[0])
		{
			direction = "down";
		}

		if ((abs(SensorValue[RL_pot])-30 < abs(lift_auton_val[0])) && ((abs(SensorValue[RL_pot])+30 > abs(lift_auton_val[0]))))
		{
			leftLift = 0;
			rightLift = 0;
			lift_auton_val[0] = 0;
			lift_preset = 0;
		}

		else if ((lift_auton_val[0] != 0))

			lift_auton_val[0] = SensorValue[RL_pot];

			if (direction == "down")
			{
				while ((LL_goal_reached == false) || (RL_goal_reached == false))
				{
					if (SensorValue[RL_pot] > lift_auton_val[0])
					{
						leftLift = -(lift_auton_val[1]);
						if ((lift_error >= 200) && (lift_error <= 250))
						{
						}
						else if (lift_error < 0)
						{
							leftLift = leftLift + 80;
							if (mounted_skyrise)
							{
								leftLift = leftLift + 120;
							}
						}
					}
					else
					{
						LL_goal_reached = true;
						leftLift = 0;
					}
					if (SensorValue[RL_pot] > lift_auton_val[0])
					{
						rightLift = -(lift_auton_val[1]);

					if ((lift_error >= 200) && (lift_error <= 250))
						{
						}

					else if (lift_error > 0)
						{
							rightLift = rightLift + 80;
							if (mounted_skyrise)
							{
								rightLift = rightLift + 120;
							}
						}
					}
					else
					{
						RL_goal_reached = true;
						rightLift = 0;
					}
				motor[LLD] = leftLift;
				motor[LLU] = leftLift;
				motor[RLD] = rightLift;
				motor[RLU] = rightLift;
				}



			}

			else if (direction == "up")
			{
				while ((LL_goal_reached == false) || (RL_goal_reached == false))
				{
					if (SensorValue[RL_pot] < lift_auton_val[0])
					{
						leftLift = (lift_auton_val[1]);

					if ((lift_error >= 200) && (lift_error <= 250))
						{
						}
					else if (lift_error > 0)
						{
							leftLift = leftLift - 60;
						}
					}
					else
					{
						LL_goal_reached = true;
						leftLift = 0;
					}
					if (SensorValue[RL_pot] < lift_auton_val[0])
					{
						rightLift = lift_auton_val[1];
						if ((lift_error >= 200) && (lift_error <= 250))
						{
						}
						else if (lift_error < 0)
						{
							rightLift = rightLift - 60;
						}
					}
					else
					{
						RL_goal_reached = true;
						rightLift = 0;
					}
				motor[LLD] = leftLift;
				motor[LLU] = leftLift;
				motor[RLD] = rightLift;
				motor[RLU] = rightLift;
				}
			}

	}
}

void intake(int speed)
{
	motor[LIN] = speed;
	motor[RIN] = speed;
}
//Driver Functions
//Preset Button Control
	void preset_buttons()
	{
		if (lift_preset_enabled == true)
		{
			if (vexRT[Btn8U] == 1)
			{
				clear_previous_error();
				lift_preset = 6;
			}
			if (vexRT[Btn8L] == 1)
			{
				clear_previous_error();
				lift_preset = 5;
			}
			if (vexRT[Btn8R] == 1)
			{
				clear_previous_error();
				lift_preset= 4;
			}
			if (vexRT[Btn8D] == 1)
			{
				clear_previous_error();
				lift_preset= 3;
			}
			if (vexRT[Btn7U] == 1)
			{
				clear_previous_error();
				lift_preset= 2;
			}
			if (vexRT[Btn7D] == 1)
			{
				clear_previous_error();
				lift_preset= 1;
			}
			if (vexRT[Btn7L] == 1)
			{
				clear_previous_error();
				mounted_skyrise = true;
			}
			if (vexRT[Btn7R] == 1)
			{
				clear_previous_error();
				mounted_skyrise = false;
			}
		}
	}

void lift_preset_monitor()
{
	if (lift_auton_val[0] == 0)
	{

	}

	else
	{
		bool LL_goal_reached = false;
		bool RL_goal_reached = false;

		string direction;

		if (SensorValue[RL_pot] < lift_auton_val[0])
		{
			direction = "up";
		}
		if (SensorValue[RL_pot] > lift_auton_val[0])
		{
			direction = "down";
		}

		if ((abs(SensorValue[RL_pot])-30 < abs(lift_auton_val[0])) && ((abs(SensorValue[RL_pot])+30 > abs(lift_auton_val[0]))))
		{
			leftLift = 0;
			rightLift = 0;
			lift_auton_val[0] = 0;
			lift_preset = 0;
		}

		else if ((lift_auton_val[0] != 0))
		{
			lift_auton_val[0] = target_LL_pot(lift_auton_val[0]);

			if (direction == "down")
			{
				if ((LL_goal_reached == false) || (RL_goal_reached == false))
				{
					if (SensorValue[RL_pot] > lift_auton_val[0])
					{
						leftLift = -(lift_auton_val[1]);
						if ((lift_error >= 200) && (lift_error <= 250))
						{
						}
						else if (lift_error < 0)
						{
							leftLift = leftLift + 80;
							if (mounted_skyrise)
							{
								leftLift = leftLift + 120;
							}
						}
					}
					else
					{
						LL_goal_reached = true;
						leftLift = 0;
					}
					if (SensorValue[RL_pot] > lift_auton_val[0])
					{
						rightLift = -(lift_auton_val[1]);
					if ((lift_error >= 200) && (lift_error <= 250))
						{
						}
					else if (lift_error > 0)
						{
							rightLift = rightLift + 80;
							if (mounted_skyrise)
							{
								rightLift = rightLift + 120;
							}
						}
					}
					else
					{
						RL_goal_reached = true;
						rightLift = 0;
					}
				}



			}

			else if (direction == "up")
			{
				if ((LL_goal_reached == false) || (RL_goal_reached == false))
				{
					if (SensorValue[RL_pot] < lift_auton_val[0])
					{
						leftLift = (lift_auton_val[1]);
						if ((lift_error >= 200) && (lift_error <= 250))
							{
							}
						else if (lift_error > 0)
							{
								leftLift = leftLift - 60;
							}
					}
					else
					{
						LL_goal_reached = true;
						leftLift = 0;
					}
					if (SensorValue[RL_pot] < lift_auton_val[0])
					{
						rightLift = lift_auton_val[1];
						if ((lift_error >= 200) && (lift_error <= 250))
						{
						}
						else if (lift_error < 0)
						{
							rightLift = rightLift - 60;
						}
					}
					else
					{
						RL_goal_reached = true;
						rightLift = 0;
					}
				}
			}
		}

	}
}
//Preset Action
	void preset_assign()
	{
		if (lift_preset == 2)
		{
			if (SensorValue[RL_pot] < lift_val[1])
			{
				lift_auton_val[0] = lift_val[1];
				lift_preset_monitor();
			}
			else
			{
				lift_preset= 0;
			}
		}
		if (lift_preset == 1)
		{
			if (SensorValue[RL_pot] > lift_val[0])
			{
				lift_auton_val[0] = lift_val[0];
				lift_preset_monitor();
			}
			else
			{
				lift_preset = 0;
			}
		}
		if (lift_preset == 3)
		{
			if (((SensorValue[RL_pot] < lift_val[2]) || ((SensorValue[LL_pot] > lift_val[2]))))
			{
				lift_auton_val[0] = lift_val[2];
				lift_preset_monitor();
			}
			else
			{
				lift_preset= 0;
			}
		}
		if (lift_preset == 4)
		{
			if (((SensorValue[RL_pot] < lift_val[3]) || ((SensorValue[RL_pot] > lift_val[3]))))
			{
				lift_auton_val[0] = lift_val[3];
				lift_preset_monitor();
			}
			else
			{
				lift_preset= 0;
			}
		}
		if (lift_preset == 5)
		{
			if (((SensorValue[RL_pot] < lift_val[4]) || ((SensorValue[RL_pot] > lift_val[4]))))
			{
				lift_auton_val[0] = lift_val[4];
				lift_preset_monitor();
			}
			else
			{
				lift_preset= 0;
			}
		}
		if (lift_preset == 6)
		{
			if (((SensorValue[RL_pot] < lift_val[4]) || ((SensorValue[RL_pot] > lift_val[4]))))
			{
				lift_auton_val[0] = lift_val[1];
				lift_preset_monitor();
			}
			else
			{
				lift_preset= 0;
			}
		}
	}





void pre_auton()
{
  bStopTasksBetweenModes = true;

	clear_encoders();
	clear_previous_error();
}

//User Created Tasks


/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////
//task autonomous_companion()
//{
//	//Manages Lifts During Auton
//	while(true)
//	{
//		EndTimeSlice();
//	}
//}
task autonomous()
{
	////Blue - 2 pnts
	//motor[LDB]=80;
	//motor[RDB]=80;
	//motor[LDF]=-40;
	//motor[RDF]=-40;
	//wait1Msec(1000);
	//motor[LDB]=0;
	//motor[RDB]=0;
	//motor[LDF]=0;
	//motor[RDF]=0;
	//drive_move(500,80);
	//motor[LIN]=127;
	//motor[RIN]=127;
	//drive_move(100,100);
	//wait1Msec(1500);
	//motor[LIN]=0;
	//motor[RIN]=0;
	//drive_turn(1400,127);
	//drive_move(300,80);
	//intake(-127);
	//wait1Msec(1000);
	//intake(0);
	//while(true)
	//{
	//	wait10Msec(100);
	//}
	//Blue - 5 pnts
	//motor[LDB]=80;
	//motor[RDB]=80;
	//motor[LDF]=-40;
	//motor[RDF]=-40;
	//wait1Msec(1000);
	//motor[LDB]=0;
	//motor[RDB]=0;
	//motor[LDF]=0;
	//motor[RDF]=0;
	//drive_move(500,80);
	//motor[LIN]=127;
	//motor[RIN]=127;
	//drive_move(100,100);
	//wait1Msec(1500);
	//motor[LIN]=0;
	//motor[RIN]=0;
	//drive_strafe(700,100);
	//drive_move(400,80);
	//drive_turn(700,-127);
	//drive_move(200,80);
	//tLiftUp(1000,127);
	//drive_move(100,80);
	//motor[LIN]=-127;
	//motor[RIN]=-127;
	//drive_move(300,60);
	//wait1Msec(4000);
	//motor[LIN]=0;
	//motor[RIN]=0;

		//Red - 2 pnts
	//motor[LDB]=80;
	//motor[RDB]=80;
	//motor[LDF]=-40;
	//motor[RDF]=-40;
	//wait1Msec(1000);
	//motor[LDB]=0;
	//motor[RDB]=0;
	//motor[LDF]=0;
	//motor[RDF]=0;
	//drive_move(500,80);
	//motor[LIN]=127;
	//motor[RIN]=127;
	//drive_move(100,100);
	//wait1Msec(1500);
	//motor[LIN]=0;
	//motor[RIN]=0;
	//drive_turn(1400,-127);
	//drive_move(300,80);
	//intake(-127);
	//wait1Msec(1000);
	//intake(0);
	//while(true)
	//{
	//	wait10Msec(100);
	//}

	//Red - 5 pnts
	motor[LDB]=80;
	motor[RDB]=80;
	motor[LDF]=-40;
	motor[RDF]=-40;
	wait1Msec(1000);
	motor[LDB]=0;
	motor[RDB]=0;
	motor[LDF]=0;
	motor[RDF]=0;
	drive_move(500,80);
	motor[LIN]=127;
	motor[RIN]=127;
	drive_move(100,100);
	wait1Msec(1500);
	motor[LIN]=0;
	motor[RIN]=0;
	drive_strafe(600,-100);
	drive_move(600,80);
	drive_turn(700,127);
	drive_move(200,80);
	tLiftUp(1000,127);
	drive_move(100,80);
	motor[LIN]=-127;
	motor[RIN]=-127;
	drive_move(300,60);
	wait1Msec(4000);
	motor[LIN]=0;
	motor[RIN]=0;

}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////
/*task usercontrol_companion()
{
/* void lift_preset_monitor()
{
	if (lift_auton_val[0] == 0)
	{
		EndTimeSlice();
	}

	else
	{
		bool LL_goal_reached = false;
		bool RL_goal_reached = false;
		target_scale = lift_auton_val[0]/limit_LL_pot;
		int RL_target = (int) round(target_scale*limit_RL_pot);
		string direction;

		if SensorValue[LL_pot] < lift_auton_val[0]
		{
			direction = "up";
		}
		if SensorValue[LL_pot] > lift_auton_val[0]
		{
			direction = "down";
		}

		if ((abs(SensorValue[LL_pot])-30 < abs(lift_auton_val[0])) && ((abs(SensorValue[LL_pot])+30 > abs(lift_auton_val[0]))))
		{
			leftLift = 0;
			rightLift = 0;
			lift_auton_val[0] = 0;
			lift_preset = 0;
		}

		else if ((lift_auton_val[0] != 0))
		{
			while (((LL_goal_reached == false) || (RL_goal_reached = false)) && (direction = "down") && (interrupt = false))
			{
				if (SensorValue[LL_pot] > lift_auton_val[0])
				{
					leftLift = speedstep_lift(lift_auton_val[0],-(lift_auton_val[1]),SensorValue[LL_pot],0);
				}
				else
				{
					LL_goal_reached = true;
					leftLift = 0;
				}
				if (SensorValue[RL_pot] > RL_target)
				{
					rightLift = speedstep_lift(RL_target,-(lift_auton_val[1]),SensorValue[RL_pot],1);
				}
				else
				{
					RL_goal_reached = true;
					rightLift = 0;
				}
			}

			if (((LL_goal_reached == false) || (RL_goal_reached = false)) && (direction = "up") && (interrupt = false))
			{
				if (SensorValue[LL_pot] < lift_auton_val[0])
				{
					leftLift = speedstep_lift(lift_auton_val[0],(lift_auton_val[1]),SensorValue[LL_pot],0);
				}
				else
				{
					LL_goal_reached = true;
					leftLift = 0;
				}
				if (SensorValue[RL_pot] < RL_target)
				{
					rightLift = speedstep_lift(RL_target,lift_auton_val[1],SensorValue[RL_pot],1);
				}
				else
				{
					RL_goal_reached = true;
					rightLift = 0;
				}
			}
		}

	}
}
} */

task usercontrol()
{
	//Resetting auton variables for usercontrol
	lift_auton_val[0] = 0;
	lift_auton_val[1] = 127;
	clear_previous_error();
	int sensor_LL;

	while(true)
	{
		//Refresh Variables
			sensor_LL = (int) round(target_LL_pot(SensorValue[RL_pot]));
		//Buttons
			preset_buttons();
		//Drive
			//Left Stick Control (Strafe and movement)
				leftDF=vexRT[Ch3]+vexRT[Ch4];
				leftDB=vexRT[Ch3]-vexRT[Ch4];
				rightDF=vexRT[Ch3]-vexRT[Ch4];
				rightDB=vexRT[Ch3]+vexRT[Ch4];
			//Right Stick Control (Turning)
				leftDF=leftDF+vexRT[Ch1];
				leftDB=leftDB+vexRT[Ch1];
				rightDF=rightDF-vexRT[Ch1];
				rightDB=rightDB-vexRT[Ch1];
			//apply exp control
				leftDF=expCtrl(leftDF, exp_scaling_val);
				leftDB=expCtrl(leftDB, exp_scaling_val);
				rightDF=expCtrl(rightDF, exp_scaling_val);
				rightDB=expCtrl(rightDB, exp_scaling_val);
			//Give motors values
				motor[LDF]=leftDF;
				motor[LDB]=leftDB;
				motor[RDF]=rightDF;
				motor[RDB]=rightDB;
		//Lift
			//Get Controller Values
				leftLift=(vexRT[Btn5U]-vexRT[Btn5D])*127;
				rightLift=(vexRT[Btn5U]-vexRT[Btn5D])*127;

				if ((abs(leftLift) > 0)||(abs(rightLift) > 0))
				{
					lift_preset = 0;
					interrupt = true;
					if (vexRT[Btn5U] == 1)
					{
						if ((lift_error >= 200) && (lift_error <= 250))
						{
						}
						else if (lift_error < 0)
						{
							rightLift = rightLift - 60;
						}
						if ((lift_error >= 200) && (lift_error <= 250))
						{
						}
						else if (lift_error > 0)
						{
							leftLift = leftLift - 60;
						}
					}
					if (vexRT[Btn5D] == 1)
					{
						if ((lift_error >= 200) && (lift_error <= 230))
						{
						}
						else if (lift_error < 0)
						{
							leftLift = leftLift + 80;
							if (mounted_skyrise)
							{
								leftLift = leftLift + 120;
							}
						}
						else
						{
						if ((lift_error >= 200) && (lift_error <= 250))
						{
						}
						else if (lift_error > 0)
						{
							rightLift = rightLift + 90;
							if (mounted_skyrise)
							{
								rightLift = rightLift + 120;
							}
						}
					}
					}
				}
				else
				{
					//Lift Preset Actions
					if (lift_preset != 0)
					{
						interrupt = false;
						preset_assign();
					}
					else
					//Trim if idle
					{
						if ((SensorValue[RL_pot] < trim_switch))
						{
							leftLift = leftLift - 30;
							rightLift = rightLift - 30;
						}
						else if ((SensorValue[RL_pot] > trim_switch))
						{
							leftLift = leftLift + 20;
							rightLift = rightLift + 20;
						}
					}

				}

			//Give Lift Values
				motor[LLD] = leftLift;
				motor[LLU] = leftLift;
				motor[RLD] = rightLift;
				motor[RLU] = rightLift;
		//Intake
				motor[LIN] = (vexRT[Btn6U]-vexRT[Btn6D])*127;
				motor[RIN] = (vexRT[Btn6U]-vexRT[Btn6D])*127;

	}
}
