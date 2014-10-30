#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define max(a,b) \({ __typeof__ (a) _a = (a); \ __typeof__ (b) _b = (b); \ _a > _b ? _a : _b; })
#define min(a,b) \({ __typeof__ (a) _a = (a); \ __typeof__ (b) _b = (b); \ _a < _b ? _a : _b; })

#define gyro (SensorValue[turningGyro])
#define encoder(x) (nMotorEncoder[x])
#define potL (SensorValue[potLL])
#define potR (SensorValue[potRL])
#define autonChooser (SensorValue[aCh])
#define relLPotR ((int) round(1.0759 * potL - 75.8155))
#define relRPotL ((int) round(0.92942 * potR - 70.4641))
#define potLTarget (liftAutonVal[0])
#define potRTarget (liftAutonVal[1])
#define liftTargetSpeed (liftAutonVal[2])
#define intakeSkyrise (SensorValue[piston])

#define driveStraightForward(x,y) drive(x,abs(y),abs(y),abs(y),abs(y))
#define driveStraightBack(x,y) drive(x,-abs(y),-abs(y),-abs(y),-abs(y))
#define drivePointTurnLeft(x,y) drive(x,-abs(y),-abs(y),abs(y),abs(y))
#define drivePointTurnRight(x,y) drive(x,abs(y),abs(y),-abs(y),-abs(y))
#define driveSwerveTurnLeft(x,y) drive(x,0,0,abs(y),abs(y))
#define driveSwerveTurnRight(x,y) drive(x,abs(y),abs(y),0,0)
#define driveStrafeLeft(x,y) drive(x,abs(y),-abs(y),-abs(y),abs(y))
#define driveStrafeRight(x,y) drive(x,-abs(y),abs(y),abs(y),-abs(y))
#define gyroPointTurnLeft(x,y) gyroTurn((x*10)-abs(y*2),-abs(y),-abs(y),abs(y),abs(y))
#define gyroPointTurnRight(x,y) gyroTurn((x*10)-abs(y*2),abs(y),abs(y),-abs(y),-abs(y))

#define liftMove(x,y) lift(x,x,y)

#define intakeIn intake(127)
#define intakeOut intake(-127)
#define intakeStop intake(0)



#endif /* DEFINITIONS_H_ */