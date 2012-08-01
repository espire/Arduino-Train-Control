#ifndef conductor_h
#define conductor_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "track.h"
#include "turnout.h"

class Conductor {
public:
	Conductor(Track * tracks, Turnout * turnouts, char * type);
	
	// Track control
	int setThrottle(int who, int value);
	int findNextSpeed(int who);
	bool emergencyBrake();
	int changeSpeed(int who);
	int setAcceleration(int who, int value);
	int setBraking(int who, int value);
	
	// Turnout control
	int setStraight(int who);
	int setTurned(int who);
	int throwSwitch(int who);
	SwitchState getState(int who);
	
private:
	Track * track;
	Turnout * turnouts;
	char * type;
	bool emergency;
};

#endif
