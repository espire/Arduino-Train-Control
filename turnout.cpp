#include "turnout.h"

Turnout::Turnout(uint8_t motorNum) : AF_DCMotor(motorNum) {
  _motorNum = motorNum;
  setSpeed(255);
}

void Turnout::setStraight() {
  if(state != STRAIGHT) {
    run(FORWARD);
    delay(20);
    run(RELEASE);
    state = STRAIGHT;
  }
}

void Turnout::setTurned() {
  if(state != TURNED) {
    run(BACKWARD);
    delay(20);
    run(RELEASE);
    state = TURNED;
  }
}

SwitchState Turnout::throwSwitch() {
  if(state != STRAIGHT) {
    run(FORWARD);
    delay(20);
    run(RELEASE);
    state = STRAIGHT;
  }
  else if(state != TURNED) {
    run(BACKWARD);
    delay(20);
    run(RELEASE);
    state = TURNED;
  }
  
  return state;
}

SwitchState Turnout::getState() {
  return state;
}
