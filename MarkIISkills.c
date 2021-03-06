#pragma config(Sensor, in1,    gyro,           sensorGyro)
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
#pragma config(Sensor, dgtl11, leftCatapult,   sensorTouch)
#pragma config(Sensor, dgtl12, rightCatapult,  sensorTouch)
#pragma config(Motor,  port1,           leftWheel1,    tmotorVex393TurboSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           leftWheel2,    tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           rightWheel1,   tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           catapult1,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           rightWheel3,   tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           catapult2,     tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           rightWheel2,   tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           leftWheel3,    tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           catapult3,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          intake,        tmotorVex393TurboSpeed_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

#pragma systemFile

//Competition Control and Duration Settings
#pragma competitionControl(Competition)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

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
///// Driver Skills                                     /////
///// Authors: Jonathan Damico (jj_damico@yahoo.com)    /////
///// Since: Nov. 27, 2015                              /////
*////////////////////////////////////////////////////////////

/** Sets the speeds of all the left wheels. **/
void setLeftWheelSpeed (int speed) {
  motor[leftWheel1]   = speed;
  motor[leftWheel2]   = speed;
  motor[leftWheel3]   = speed;
}

/** Sets the speeds of all the right wheels. **/
void setRightWheelSpeed (int speed) {
  motor[rightWheel1]  = speed;
  motor[rightWheel2]  = speed;
  motor[rightWheel3]  = speed;
}

//Intake is on back of robot
/** Sets the speeds of all wheels. **/
void setWheelSpeeds (int left, int right) {
  setLeftWheelSpeed(left);
  setRightWheelSpeed(right);
}

//2 inv
/** Sets all the catapult motors. **/
void setCatapultSpeed (int speed) {
  motor[catapult1]     = speed;
  motor[catapult2]     = speed;
  motor[catapult3]     = speed;
}

/** Controlls the drivebase. **/
void tankDrive () {

  //left wheels
  if(abs(vexRT(Ch3))<10) //deadbands
    setLeftWheelSpeed(0);
  else
    setLeftWheelSpeed(vexRT(Ch3));

  //right wheels
  if(abs(vexRT(Ch2))<10) //deadbands
    setRightWheelSpeed(0);
  else
    setRightWheelSpeed(vexRT(Ch2));
}

/** Controlls the intake for balls. Currently not toggle, can easialy be. **/
void intakeControl() {
  if(vexRT(Btn5U))
    motor[intake] = 127;
  else if(vexRT(Btn5D))
    motor[intake] = -127;
  else
    motor[intake] = 0;
}

/** Orients robot with gryo **/
void orient() {
  while(abs(SensorValue[gyro])>10) {
    if(SensorValue[gyro] > 50) {
      setWheelSpeeds(50,-50); //may need to be reversed
    } else if(SensorValue[gyro] < -50) {
      setWheelSpeeds(-50,50); //may need to be reversed
    } else {
      int spinValue = SensorValue[gyro]/1270*127+20;
      setWheelSpeeds(-spinValue, spinValue);
    }
  }
  setWheelSpeeds(0,0);
}

/* Globals for Catapult */
int catapultDelay = 250; //delay for cycle to start reading sensor
int catapultHoldPower = 20; //Power to keep catapult in same place - was 15
int ballLoadDelay = 2000; //Delay for the ball to be loaded by human drivervalidateLongAddresvalidateLongAddress

/** Controlls the cataptult for regular field use **/
task primeCatapult () {
  while(!SensorValue[rightCatapult]) {
    setCatapultSpeed(127);
    wait1Msec(30);
  }
  setCatapultSpeed(catapultHoldPower);
  stopTask(primeCatapult);
}

/** Prepares the catapult to be shot. **/
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

/** Controlls the catapult for driver loads and autonomous **/
task catapultKickUserLoad() {
  while(true) {
    setCatapultSpeed(127);
    wait1Msec(catapultDelay); //May not need
    while(!SensorValue[rightCatapult] && !SensorValue[leftCatapult])
      setCatapultSpeed(127);
    setCatapultSpeed(catapultHoldPower);
  wait1Msec(ballLoadDelay);
  }
}

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

/** Pre autonomous task.
    Calibrate SECOND goal by hitting button sensor **/
void pre_auton() {
  while(!SensorValue[gyroCalib]) {
    SensorValue[8] = 1;   //Flash red light to indicate that we need to calibrate
    wait1Msec(50);
    SensorValue[8] = 0;
    wait1Msec(50);
  }
  SensorValue[8] = 0;
  SensorValue[9] = 1;     //Turn on steady amber light to indicate that we are calibrating
  wait1Msec(2000);
  SensorValue[gyro] = 0;
  SensorValue[8] = 0;
  SensorValue[10] = 1;    //Turn on steady green light to indicate that we have calibrated
  bStopTasksBetweenModes = true;

}

/** Controls lights so we know how much time is left in shooting for autonomous. **/
task autonomousLights () {
  while(true) {
    if(time1[T1]<10000) {
      SensorValue[8]  = 1;
      SensorValue[9]  = 0;
      SensorValue[10] = 0;
    } else if (time1[T1<15000]) {
      SensorValue[8]  = 0;
      SensorValue[9]  = 1;
      SensorValue[10] = 0;
    } else {
      SensorValue[8]  = 0;
      SensorValue[9]  = 0;
      SensorValue[10] = 1;
    }
    wait1Msec(200);
  }
}

void autonomousMove () {
  setWheelSpeeds(127,127);
  wait1Msec(2000);
  setWheelSpeeds(50,-50);
  wait1Msec(2000);
  setWheelSpeeds(-127,-127);
  wait1Msec(2000);
  setWheelSpeeds(0,0);
}

void autonomousShoot () {
  clearTimer(T1);
  startTask(catapultKickUserLoad);
  startTask(autonomousLights);
  while(time1[T1]<20000) {} //Time to wait for shooting, needs to be as small as possible
  stopTask(catapultKickUserLoad);
  stopTask(autonomousLights);
  SensorValue[8]  = 0;
  SensorValue[9]  = 0;
  SensorValue[10] = 0;
}

/** Autonomous task - 15 seconds.
    Moves forward after shooting driver loads **/
task autonomous() {
  autonomousShoot();
  autonomousMove();
  orient();
  autonomousShoot();
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
*/
/** Usercontrol task **/
task usercontrol() {
  startTask(prettyLights);
  startTask(catapultKick);
  while (true) {
    if(vexRT(Btn8D)) {
      startTask(catapultKickUserLoad);
    } else if(vexRT(Btn8U)) {
      stopTask(catapultKickUserLoad);
      setCatapultSpeed(0);
    }

    if(SensorValue[ballIntake] && !vexRT(Btn6U))
      startTask(primeCatapult);

    //Gyro - 7D
    if(vexRT(Btn7D))
      orient();

    if(vexRT(Btn7U))
      SensorValue[gyro] = 0;

    tankDrive(); //Controls drivebase

    intakeControl(); //Controls intake

    //Anywhere from 25-50 Msec pause to prevent cortex overload
    wait1Msec(30);
  }
}
