
#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

void pre_auton() {
  bStopTasksBetweenModes = true;

}

task autonomous() {

}


task usercontrol() {
  while (true) {
    motor[port1]  = VexRT(Btn7U)*127;
    motor[port2]  = VexRT(Btn7D)*127;
    motor[port3]  = VexRT(Btn7L)*127;
    motor[port4]  = VexRT(Btn7R)*127;
    motor[port5]  = VexRT(Btn8U)*127;
    motor[port6]  = VexRT(Btn8D)*127;
    motor[port7]  = VexRT(Btn8L)*127;
    motor[port8]  = VexRT(Btn8R)*127;
    motor[port9]  = VexRT(Btn5U)*127;
    motor[port10] = VexRT(Btn6U)*127;
  }
}
