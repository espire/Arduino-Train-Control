#ifndef track_h
#define track_h

#include <WProgram.h>
#include <stdlib.h>
#include <AFMotor.h>

class Track : public AF_DCMotor {
	public:
		Track(int8_t, float acceleration = 1, float drag = 1, int speedLimit = 99, int8_t freq = MOTOR12_2KHZ);
		int setThrottle(int);
		float setNextSpeed();
		int changeSpeed();
	private:
		int8_t motorNum;
		
		float acceleration;
		float drag;
		
		int speedLimit;
		
		int targetSpeed; // how fast would we like to go?
		float nextSpeed; // how fast will we set the motor next?
		int motorSpeed; // how fast is the motor going at this moment?
};

#endif