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
		Track(int8_t, float acceleration = 1, float braking = 1, int speedLimit = 120, int8_t freq = MOTOR12_1KHZ);
		int setThrottle(int); // tell the track how fast we want to go
		float setNextSpeed(); // tell the track to calculate the next speed increment
		void emergencyBrake(); // emergency! all stop!
		void disableEmergency(); // all clear!
		float changeSpeed(); // tell the track to change its motor's speed to the set increment
		float setAcceleration(float);
		float setBraking(float);
		
	private:
		int8_t motorNum;
		
		float acceleration;
		float braking;
		
		int speedLimit;
		
		// all speeds can be negative, indicating travel in reverse
		int targetSpeed; // how fast would we like to go?
		float nextSpeed; // how fast will we set the motor next?
		uint8_t nextDirection; // in what direction will the motor next?
		float motorSpeed; // how fast is the motor going at this moment?
		
		bool emergency; // if emergency braking is on
};

#endif
