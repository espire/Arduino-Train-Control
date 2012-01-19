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
	
	// track control
	int setThrottle(int who, int value);
	int setNextSpeed(int who);
	int emergencyBrake();
	int disableEmergency();
	int changeSpeed(int who);
	int setAcceleration(int who, int value);
	int setBraking(int who, int value);
	
	// turnout control
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