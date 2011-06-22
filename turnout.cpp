#include "turnout.h"

Turnout::Turnout(uint8_t motorNum) : AF_DCMotor(motorNum) {
	this->motorNum = motorNum;
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
	if(state == TURNED) {
		setStraight();
	}
	else if(state == STRAIGHT) {
		setTurned();
	}
	else {
		state = UNKNOWN;
	}
	
	return state;
}

SwitchState Turnout::getState() {
	return state;
}