#include "turnout.h"

// Turnout (extends AF_DCMotor)
// A turnout on the railroad.
// We control it via short DC pulses.

Turnout::Turnout(uint8_t motorNum) : AF_DCMotor(motorNum) {
	this->motorNum = motorNum;
	state = UNKNOWN;
	setSpeed(255);

	Serial.begin(9600);
}

void Turnout::setStraight() {
	if(state != STRAIGHT) {
		run(FORWARD);
		delay(15);
		run(RELEASE);
		state = STRAIGHT;
		
		Serial.println("Turnout set straight");
	}
}

void Turnout::setTurned() {
	if(state != TURNED) {
    	run(BACKWARD);
    	delay(20);
    	run(RELEASE);
    	state = TURNED;

		Serial.println("Turnout set turned");
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
		Serial.println("Turnout state uknown, so it was not changed");
	}
	
	return state;
}

SwitchState Turnout::getState() {
	return state;
}
