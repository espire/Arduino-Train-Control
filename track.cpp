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
	
	char * out;
	bool printSpeed = true;
	
	// emergency braking is on: don't run the engine
	if(emergency) {
		nextSpeed = 0;
		Serial.print("Emergency!");
	}
	
	// special case: we are going directly between forward and reverse
	// skip over not giving the train much power: about -30 to 30
	else if((targetSpeed >= 25) && (motorSpeed > -25 && motorSpeed < 25)) {
		nextSpeed = 25;
		Serial.print("Switching into forward gear.");
	}
	else if((targetSpeed <= -25) && (motorSpeed > -25 && motorSpeed < 25)) {
		nextSpeed = -25;
		Serial.print("Switching into reverse gear.");
	}
	
	// normal cases
	else if((targetSpeed > motorSpeed) && (motorSpeed >= 0)) {
		if(abs(targetSpeed - motorSpeed) < acceleration)
			nextSpeed = targetSpeed;
		else nextSpeed = motorSpeed + acceleration;
		nextDirection = FORWARD;
		Serial.print("Moving forwards and accelerating.");
	}
	else if((targetSpeed < motorSpeed) && (motorSpeed > 0)) {
		if(abs(targetSpeed - motorSpeed) < braking)
			nextSpeed = targetSpeed;
		else nextSpeed = motorSpeed - braking;
		nextDirection = FORWARD;
		Serial.print("Moving forwards and braking.");
	}
	else if((targetSpeed < motorSpeed) && (motorSpeed <= 0)) {
		if(abs(targetSpeed - motorSpeed) < acceleration)
			nextSpeed = targetSpeed;
		else nextSpeed = motorSpeed - acceleration;
		nextDirection = BACKWARD;
		Serial.print("Moving backwards and accelerating.");
	}
	else if((targetSpeed > motorSpeed) && (motorSpeed < 0)) {
		if(abs(targetSpeed - motorSpeed) < braking)
			nextSpeed = targetSpeed;
		else nextSpeed = motorSpeed + braking;
		nextDirection = BACKWARD;
		Serial.print("Moving backwards and braking.");
	}
	else {
		// targetSpeed == motorSpeed, all is well
		printSpeed = false;
	}
	
	if(printSpeed) {
		dtostrf(nextSpeed, 0, 2, out);
		Serial.println(" Next speed set to " + (String)out);
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
	if(acceleration > 0) {
		this->acceleration = acceleration;
		char * out;
		dtostrf(acceleration, 0, 2, out);
		Serial.println("Acceleration set to " + (String)out);
	}
	else {
		Serial.println("Error: out of range");
	}
	return acceleration;
}

float Track::setBraking(float braking) {
	if(braking > 0) {
		this->braking = braking;
		char * out;
		dtostrf(braking, 0, 2, out);
		Serial.println("Braking set to " + (String)out);
	}
	else {
		Serial.println("Error: out of range");
	}
	return braking;
}