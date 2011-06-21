#ifndef track_h
#define track_h

#include <WProgram.h>
#include <stdlib.h>
#include <AFMotor.h>

class Track : public AF_DCMotor {
	public:
		Track(uint8_t, uint8_t freq = MOTOR34_8KHZ);
	private:
		uint8_t _motorNum;
};

#endif