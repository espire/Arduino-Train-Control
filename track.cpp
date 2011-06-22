#include "track.h"

Track::Track(int8_t motorNum, float acceleration, float drag, int speedLimit, int8_t freq) : AF_DCMotor(motorNum, freq) {
	this->motorNum = motorNum;
	this->acceleration = acceleration;
	this->drag = drag;
	this->speedLimit = speedLimit;
	targetSpeed = 0;
	nextSpeed = 0;
	motorSpeed = 0;
	
	Serial.begin(9600);
}

int Track::setThrottle(int throttle) {
	if(throttle > speedLimit) {
		throttle = speedLimit;
	}
	// don't give the train so little power as to idle it
	else if(throttle < 10) {
		throttle = 0;
	}
	
	targetSpeed = throttle;
	
	Serial.println("Throttle set to " + (String)targetSpeed);
	
	return throttle;
}

float Track::setNextSpeed() {
	if(targetSpeed == 0) {
	    nextSpeed = 0;
	  }
	  else if(targetSpeed > motorSpeed) {
	    nextSpeed = (float)motorSpeed + acceleration;
	    Serial.println("Accelerating... next speed set to " + (String)(int)nextSpeed + " rounded");
	  }
	  else if(targetSpeed < motorSpeed) {
	    nextSpeed = (float)motorSpeed - drag;
	    Serial.println("Coasting... next speed set to " + (String)(int)nextSpeed + " rounded");
	  }
	  else {
	    // targetSpeed == motorSpeed, all is well
	  }
	
	return nextSpeed;
}

int Track::changeSpeed() {
  motorSpeed = (int)nextSpeed;
  if(nextSpeed > 0) {
    setSpeed(nextSpeed);
    run(FORWARD);
  }
  else {
    setSpeed(0);
    run(RELEASE);
  }
}