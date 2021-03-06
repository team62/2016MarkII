#pragma config(Sensor, in1,    gyro,           sensorGyro)
#pragma config(Sensor, in8,    leftCatapult,   sensorPotentiometer)
#pragma config(Sensor, dgtl1,  ,               sensorLEDtoVCC)
#pragma config(Sensor, dgtl2,  ,               sensorLEDtoVCC)
#pragma config(Sensor, dgtl3,  ,               sensorLEDtoVCC)
#pragma config(Sensor, dgtl4,  ,               sensorLEDtoVCC)
#pragma config(Sensor, dgtl5,  ,               sensorLEDtoVCC)
#pragma config(Sensor, dgtl6,  ,               sensorLEDtoVCC)
#pragma config(Sensor, dgtl7,  ,               sensorLEDtoVCC)
#pragma config(Sensor, dgtl8,  ,               sensorLEDtoVCC)
#pragma config(Sensor, dgtl9,  gyroCalib,      sensorTouch)
#pragma config(Sensor, dgtl10, ballIntake,     sensorTouch)
#pragma config(Sensor, dgtl12, rightCatapult,  sensorTouch)
#pragma config(Motor,  port1,           rightWheel1,   tmotorVex393TurboSpeed_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           rightWheel2,   tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           leftWheel1,    tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           catapult1,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           leftWheel3,    tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           catapult2,     tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           leftWheel2,    tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           rightWheel3,   tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           catapult3,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          intake,        tmotorVex393TurboSpeed_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

#pragma systemFile

//Competition Control and Duration Settings
#pragma competitionControl(Competition)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

bool lockMode = false;

/*///////////////////////////////////////////////////////////
/////____________/\\\\\____/\\\\\\\\\_____              /////
///// ________/\\\\////___/\\\///////\\\___             /////
/////  _____/\\\///_______\///______\//\\\__            /////
/////   ___/\\\\\\\\\\\______________/\\\/___           /////
/////    __/\\\\///////\\\_________/\\\//_____          /////
/////     _\/\\\______\//\\\_____/\\\//________         /////
/////      _\//\\\______/\\\____/\\\/___________        /////
/////       __\///\\\\\\\\\/____/\\\\\\\\\\\\\\\_       /////
/////        ____\/////////_____\///////////////__      /////
///// Mark II Robot                                     /////
///// Main Code                                         /////
///// Authors: Jonathan Damico (jj_damico@yahoo.com)    /////
///// Since: Nov. 27, 2015                              /////
*////////////////////////////////////////////////////////////

#warning "setLeftWheelSpeed"
/** Sets the speeds of all the left wheels. **/
void setLeftWheelSpeed (int speed) {
  motor[leftWheel1]   = speed;
  motor[leftWheel2]   = speed;
  motor[leftWheel3]   = speed;
}

#warning "setRightWheelSpeeds"
/** Sets the speeds of all the right wheels. **/
void setRightWheelSpeed (int speed) {
  motor[rightWheel1]  = speed;
  motor[rightWheel2]  = speed;
  motor[rightWheel3]  = speed;
}

//Intake is on back of robot
#warning "setWheelSpeeds"
/** Sets the speeds of all wheels. **/
void setWheelSpeeds (int left, int right) {
  setLeftWheelSpeed(left);
  setRightWheelSpeed(right);
}

#warning "setCatapultSpeed"
/** Sets all the catapult motors. **/
void setCatapultSpeed (int speed) {
  motor[catapult1]     = speed;
  motor[catapult2]     = speed;
  motor[catapult3]		 = speed;
}

#warning "lock"
/** Locks the drivebase during shooting. **/
void lock ( int speed = 15 ) {
	motor[leftWheel2] = -speed;
	motor[rightWheel2] = -speed;
}

#warning "tankDrive"
/** Controlls the drivebase. **/
void tankDrive () {

	int rawLeft = vexRT(Ch3);
	int rawRight = vexRT(Ch2);

	if( rawLeft > 0 )
		setLeftWheelSpeed( rawLeft*rawLeft < 3 ? 0 : rawLeft*rawLeft/127 );
	else
		setLeftWheelSpeed( rawLeft*rawLeft < 3 ? 0 : rawLeft*rawLeft/-127 );

	if( rawRight > 0 )
		setRightWheelSpeed( rawRight*rawRight < 3 ? 0 : rawRight*rawRight/127 );
	else
		setRightWheelSpeed( rawRight*rawRight < 3 ? 0 : rawRight*rawRight/-127 );

}

#warning "intakeControl"
/** Controlls the intake for balls. Currently not toggle, can easialy be. **/
void intakeControl() {
  if(vexRT(Btn5U))
    motor[intake] = 127;
  else if(vexRT(Btn5D))
    motor[intake] = -127;
  else
    motor[intake] = 0;
}

#warning "orient"
/** Orients robot with gryo **/
float	Kp = 0.05;
float	Ki = 0.22;
float	Kd = 0.07;
task orient () {
	float error, integral, derivative, lastError;
	int speed;

	while (true) {
			//P
			error = 0 - SensorValue[gyro];

			//I
			if(error == 0)
				integral = 0;
			else if (abs(error)>10)
				integral = 0;
			else
				integral += error;

			//D
			derivative = error - lastError;

			lastError = error;


			speed = Kp*error+Ki*integral+Kd*derivative;
			if(speed>-5 && speed<5)
				speed = 0;
			else if(speed>127)
				speed = 127;
			else if(speed<-127)
				speed = -127;
			else if(speed<19 && speed>0)
				speed = 21;
			else if(speed>-19 && speed<0)
				speed = -21;
			setWheelSpeeds(-speed,speed);
	}
}

/* Globals for Catapult */
int catapultDelay = 250; //delay for cycle to start reading sensor
int catapultHoldPower = 20; //Power to keep catapult in same place - was 15
int ballLoadDelay = 2000; //Delay for the ball to be loaded by human drivervalidateLongAddresvalidateLongAddress

#warning "primeCatapult"
/** Prepares the catapult to be shot. **/
task primeCatapult () {
	while(SensorValue[leftCatapult]>2250) {
    setCatapultSpeed(127);
    wait1Msec(30);
  }
  setCatapultSpeed(catapultHoldPower);
  stopTask(primeCatapult);
}

#warning "catapultKick"
/** Controls manual catapult use. **/
task catapultKick() {
  while(true) {
    if(vexRT(Btn6U)) {
    	stopTask(primeCatapult);
      setCatapultSpeed(127);
      wait1Msec(catapultDelay); //May not need
      setCatapultSpeed(0);
    }
    wait1Msec(30);
  }
}

#warning "catapultKickUserLoad"
/** Controlls the catapult for driver loads and autonomous **/
task catapultKickUserLoad() {
  while(true) {
  	stopTask(primeCatapult); //Stop robot from destroying itself
    setCatapultSpeed(127);
    wait1Msec(catapultDelay); //May not need
    while(!SensorValue[rightCatapult] && !SensorValue[leftCatapult])
      setCatapultSpeed(127);
    setCatapultSpeed(catapultHoldPower);
  	wait1Msec(ballLoadDelay);
  	lockMode = true;
  }
}

#warning "lightsWaitTime"
int lightsWaitTime = 50;
/** Pretty Lights. Nuff said. **/
task prettyLights() {
  while(true) {
    for(int i = 8; i<=16; i++) {
      SensorValue[i] = 1;
      //sensorValue[((i-4)%8)+8] = 0;
      wait1Msec(lightsWaitTime);
    }
    for(int i = 8; i<=16; i++) {
      SensorValue[i] = 0;
      wait1Msec(lightsWaitTime);
    }
  }
}

/** Pre autonomous task (who actually uses this?).
    May put in some pretty flashing indicator lights **/
void pre_auton() {
  bStopTasksBetweenModes = true;

}

/** Autonomous task - 15 seconds.
    Moves forward after shooting driver loads **/
task autonomous() {
  SensorValue[gyro] = 0; //Calibrates Gyro
  clearTimer(T1);
  startTask(catapultKickUserLoad);
  while(time1[T1]<5000) //Time to wait for shooting, needs to be as small as possible
  	lock();
  stopTask(catapultKickUserLoad);
  setWheelSpeeds(127,127);
  wait1Msec(1100);
  setWheelSpeeds(0,0);
}

/*
ch3               = run all right side motors
ch2               = run all left side motors
Btn5U             = 127 intake
Btn5D             = -127 intake
Btn6U tapped      = run catapults once in a cycle
Btn6U holded down = full auto
Btn8D             = run driver load shooter (basically full auto) - don’t have to run intake
Btn8U             = stop driver load shooter
Btn7D             = auto align with gyro (not necessary yet)
Btn7U             = calibrate gryo
Btn7L							= lock wheels
*/
/** Usercontrol task **/
task usercontrol() {
  //startTask(prettyLights);
  startTask(catapultKick);
  lockMode = false;
  for (int i = 8; i<=16; i++)
  	SensorValue[i] = 0;
  while (true) {
    if(vexRT(Btn8D)) {
      startTask(catapultKickUserLoad);
      lockMode = true;
    } else if(vexRT(Btn8U)) {
      stopTask(catapultKickUserLoad);
      setCatapultSpeed(0);
      lockMode = false;
    }

    if(SensorValue[ballIntake] && !vexRT(Btn6U))
      startTask(primeCatapult);

    SensorValue[8] = lockMode;

    //Gyro - 7D
    if(vexRT(Btn7D))
      startTask(orient);
		else
			stopTask(orient);

    if(vexRT(Btn7U))
      SensorValue[gyro] = 0;

    if(vexRT(Btn7L)) {
    	lockMode = !lockMode;
    	while(vexRT(Btn7L)) {}
    }

    if(lockMode && abs(vexRT(Ch3))< 10  && abs(vexRT(Ch2)) < 10)
    	lock(); //locks drivebase during shooting
   	else
   		tankDrive(); //Controls drivebase

    intakeControl(); //Controls intake

    //Anywhere from 25-50 Msec pause to prevent cortex overload
    wait1Msec(30);
  }
}
