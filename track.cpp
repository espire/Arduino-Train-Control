#include "track.h"

/**
 * Track (extends AF_DCMotor)
 * A physical block of track on the railroad.
 * We cannot direclty control trains on a DC layout;
 * Instead, we control "all trains running on a specific block".
 */
 
Track::Track(int8_t motorNum, float acceleration, float braking, int speedLimit, int8_t freq)
		: AF_DCMotor(motorNum, freq) {
	this->motorNum = motorNum;
	this->acceleration = acceleration;
	this->braking = braking;
	this->speedLimit = speedLimit;
	throttleSpeed = 0;
	nextSpeed = 0;
	motorSpeed = 0;
	nextDirection = FORWARD;
	emergency = false;
	
	Serial.begin(9600);
}

/**
 * Set the Track's desired eventual speed.
 * @return the moderated throttle value.
 */
int Track::setThrottle(int throttle) {
	if (throttle > speedLimit) {
		throttle = speedLimit;
	} else if (throttle < (speedLimit * -1)) {
		throttle = speedLimit * -1;
	} else if(throttle < 5 && throttle > -5) {
		// Don't give the train so little power as to idle it.
		throttle = 0;
	}
	
	throttleSpeed = throttle;
	Serial.println("Throttle set to " + (String) throttleSpeed);
	return throttle;
}

/**
 * Calculate the Track's next speed setting on the way to its throttle value.
 * @return the next speed setting.
 */
float Track::findNextSpeed() {
	
	char out[7];
	bool printSpeed = true;
	
	if (emergency) {
		// Emergency braking is on: don't run the engine.
		printSpeed = false;
		nextSpeed = 0;
		
	} else if (motorSpeed > -5 && motorSpeed < 5) {
		// Special case: we are going directly between forward and reverse
		// Skip over not giving the train much power: -5 to 5.
		
		if (throttleSpeed >= 5) {
			nextSpeed = 5;
			Serial.println("Switching into forward gear.");
		} else if (throttleSpeed <= -5)
		nextSpeed = -5;
		Serial.println("Switching into reverse gear.");
		
	} else if ((throttleSpeed > motorSpeed) && (motorSpeed >= 0)) {
		// We are moving forwards and the target speed is higher;
		// Add to our current speed, speeding up.
		
		if (abs(throttleSpeed - motorSpeed) < acceleration) {
			nextSpeed = throttleSpeed;
		} else {
			nextSpeed = motorSpeed + acceleration;
		}
		nextDirection = FORWARD;
		Serial.println("Moving forwards and accelerating.");
		
	} else if ((throttleSpeed < motorSpeed) && (motorSpeed > 0)) {
		// We are moving forwards and the target speed is lower;
		// Subtract from our current speed, slowing down.
		
		if (abs(throttleSpeed - motorSpeed) < braking) {
			nextSpeed = throttleSpeed;
		} else {
			nextSpeed = motorSpeed - braking;
		}
		nextDirection = FORWARD;
		Serial.println("Moving forwards and braking.");
		
	} else if ((throttleSpeed < motorSpeed) && (motorSpeed <= 0)) {
		// We are moving backwards and the target speed is lower;
		// Subtract from our current speed, speeding up.
		
		if (abs(throttleSpeed - motorSpeed) < acceleration) {
			nextSpeed = throttleSpeed;
		} else {
			nextSpeed = motorSpeed - acceleration;
		}
		nextDirection = BACKWARD;
		Serial.println("Moving backwards and accelerating.");
		
	} else if ((throttleSpeed > motorSpeed) && (motorSpeed < 0)) {
		// We are moving backwards and the target speed is higher;
		// Add to our current speed, slowing down.
		
		if (abs(throttleSpeed - motorSpeed) < braking) {
			nextSpeed = throttleSpeed;
		} else {
			nextSpeed = motorSpeed + braking;
		}
		nextDirection = BACKWARD;
		Serial.println("Moving backwards and braking.");
		
	} else {
		// throttleSpeed and motorSpeed are equal.
		printSpeed = false;
	}
	
	if (printSpeed) {
		dtostrf(nextSpeed, 0, 2, out);
		Serial.print("Next speed set to ");
		Serial.println(out);
	}
	
	return nextSpeed;
}

/** Enable the emergency brake, stopping all movement. */
void Track::emergencyBrake() {
	emergency = true;
	nextSpeed = 0;
	Serial.println("Emergency! E-brake enabled.");
}

/** Disable the emergency brake. */
void Track::disableEmergency() {
	if (emergency) {
		Serial.println("Emergency over. E-brake disabled.");
	}
	emergency = false;
}

/**
 * Sets the motor's speed to the previously calculated next speed.
 * @return the motor's new speed.
 */
float Track::changeSpeed() {
	motorSpeed = nextSpeed;
	if (nextSpeed != 0) {
		// the motor must be given a non-negative value 
		setSpeed((int)abs(nextSpeed));
		run(nextDirection);
	} else {
		setSpeed(0);
		run(RELEASE);
	}
	
	return motorSpeed;
}

/**
 * Sets the track's acceleration value.
 * @return the new acceleration.
 */
float Track::setAcceleration(float acceleration) {
	char out[7];
	if (acceleration > 0) {
		this->acceleration = acceleration;
		dtostrf(acceleration, 0, 2, out);
		Serial.print("Acceleration set to ");
		Serial.println(out);
	} else {
		Serial.println("Error: out of range");
	}
	return acceleration;
}

/**
 * Sets the track's braking value.
 * @return the new braking.
 */
float Track::setBraking(float braking) {
	char out[7];
	if (braking > 0) {
		this->braking = braking;
		char * out;
		dtostrf(braking, 0, 2, out);
		Serial.print("Braking set to ");
		Serial.println (out);
	} else {
		Serial.println("Error: out of range");
	}
	return braking;
}

/**
 * Sets the track's speed limit.
 * @return the new speed limit.
 */
float Track::setSpeedLimit(float speedLimit) {
	char out[7];
	if (speedLimit > 0) {
		this->speedLimit = speedLimit;
		char * out;
		dtostrf(speedLimit, 0, 2, out);
		Serial.print("Speed limit set to ");
		Serial.println (out);
	} else {
		Serial.println("Error: out of range");
	}
	return speedLimit;
}
