
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
    motor[port1]  = VexRT(Btn7U);
    motor[port2]  = VexRT(Btn7D);
    motor[port3]  = VexRT(Btn7L);
    motor[port4]  = VexRT(Btn7R);
    motor[port5]  = VexRT(Btn8U);
    motor[port6]  = VexRT(Btn8D);
    motor[port7]  = VexRT(Btn8L);
    motor[port8]  = VexRT(Btn8R);
    motor[port9]  = VexRT(Btn5U);
    motor[port10] = VexRT(Btn6U);
  }
}
