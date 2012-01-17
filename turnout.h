#ifndef turnout_h
#define turnout_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <stdlib.h>
#include <AFMotor.h>

enum SwitchState {STRAIGHT, TURNED, UNKNOWN};

class Turnout : public AF_DCMotor {
public:
	Turnout(uint8_t);
	void setStraight();
	void setTurned();
	SwitchState throwSwitch();
	SwitchState getState();
private:
	uint8_t motorNum;
	SwitchState state;
};

#endif
