#ifndef track_h
#define track_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <stdlib.h>
#include <AFMotor.h>

class Track : public AF_DCMotor {
public:
	Track(int8_t motorNum, float acceleration = 1, float braking = 1, int speedLimit = 120, int8_t freq = MOTOR12_1KHZ);
	int setThrottle(int);
	float findNextSpeed();
	void emergencyBrake();
	void disableEmergency();
	float changeSpeed();
	float setAcceleration(float);
	float setBraking(float);
	float setSpeedLimit(float);

private:
	int8_t motorNum;

	float acceleration;
	float braking;

	int speedLimit;

	// All speeds can be negative, indicating travel in reverse.
	
	// How fast would we like to go?
	int throttleSpeed;
	
	// How fast will we set the motor next?
	float nextSpeed;
	
	// In what direction will the motor spin next?
	uint8_t nextDirection;
	
	float motorSpeed;
	bool emergency;
};

#endif
