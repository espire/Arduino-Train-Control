#include "track.h"
#include <AFMotor.h>

Track::Track(int8_t motorNum, float acceleration, float braking, int speedLimit, int8_t freq) : AF_DCMotor(motorNum, freq) {
	this->motorNum = motorNum;
	this->acceleration = acceleration;
	this->braking = braking;
	this->speedLimit = speedLimit;
	targetSpeed = 0;
	nextSpeed = 0;
	motorSpeed = 0;
	nextDirection = FORWARD;
	emergency = false;
	
	Serial.begin(9600);
}

int Track::setThrottle(int throttle) {
	if(throttle > speedLimit) {
		throttle = speedLimit;
	}
	else if(throttle < (speedLimit * -1)) {
		throttle = speedLimit * -1;
	}
	else if(throttle < 25 && throttle > -25) {
		// don't give the train so little power as to idle it
		throttle = 0;
	}
	
	targetSpeed = throttle;
	Serial.println("Throttle set to " + (String)targetSpeed);
	return throttle;
}

float Track::setNextSpeed() {
	
	// emergency braking is on: don't run the engine
	if(emergency) {
		nextSpeed = 0;
		Serial.println("Emergency! next speed set to 0");
	}
	
	// special case: we are going directly between forward and reverse
	// skip over not giving the train much power: about -30 to 30
	else if((targetSpeed >= 25) && (motorSpeed > -25 && motorSpeed < 25)) {
		nextSpeed = 25;
		Serial.println("Switching into forward gear...next speed set to " + (String)(int)nextSpeed);
	}
	else if((targetSpeed <= -25) && (motorSpeed > -25 && motorSpeed < 25)) {
		nextSpeed = -25;
		Serial.println("Switching into reverse gear... next speed set to " + (String)(int)nextSpeed);
	}
	
	// normal cases
	else if((targetSpeed > motorSpeed) && (motorSpeed >= 0)) {
		if(abs(targetSpeed - motorSpeed) < acceleration)
			nextSpeed = targetSpeed;
		else nextSpeed = motorSpeed + acceleration;
		nextDirection = FORWARD;
		Serial.println("Moving forwards and accelerating... next speed set to " + (String)(int)nextSpeed);
	}
	else if((targetSpeed < motorSpeed) && (motorSpeed > 0)) {
		if(abs(targetSpeed - motorSpeed) < braking)
			nextSpeed = targetSpeed;
		else nextSpeed = motorSpeed - braking;
		nextDirection = FORWARD;
		Serial.println("Moving forwards and braking... next speed set to " + (String)(int)nextSpeed);
	}
	else if((targetSpeed < motorSpeed) && (motorSpeed <= 0)) {
		if(abs(targetSpeed - motorSpeed) < acceleration)
			nextSpeed = targetSpeed;
		else nextSpeed = motorSpeed - acceleration;
		nextDirection = BACKWARD;
		Serial.println("Moving backwards and accelerating... next speed set to " + (String)(int)nextSpeed);
	}
	else if((targetSpeed > motorSpeed) && (motorSpeed < 0)) {
		if(abs(targetSpeed - motorSpeed) < braking)
			nextSpeed = targetSpeed;
		else nextSpeed = motorSpeed + braking;
		nextDirection = BACKWARD;
		Serial.println("Moving backwards and braking... next speed set to " + (String)(int)nextSpeed);
	}
	else {
		// targetSpeed == motorSpeed, all is well
	}
	
	return nextSpeed;
}

void Track::emergencyBrake() {
	emergency = true;
	nextSpeed = 0;
	Serial.println("Emergency! E-brake enabling.");
}

void Track::disableEmergency() {
	emergency = false;
	Serial.println("Emergency over. E-brake disabled.");
}

float Track::changeSpeed() {
	motorSpeed = nextSpeed;
	if(nextSpeed != 0) {
		// the motor must be given a non-negative value 
		setSpeed((int)abs(nextSpeed));
		run(nextDirection);
	}
	else {
		setSpeed(0);
		run(RELEASE);
	}
	
	return motorSpeed;
}

float Track::setAcceleration(float acceleration) {
	this->acceleration = acceleration;
	return acceleration;
}

float Track::setBraking(float braking) {
	this->braking = braking;
	return braking;
}