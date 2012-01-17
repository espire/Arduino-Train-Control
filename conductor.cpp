#include "conductor.h"

// Conductor
// The conductor makes it easier to control physical railroad elements.
// Though it has ~no intelligence, it could be extended to have some.

// The conductor does not create its own layout elements.
Conductor::Conductor(Track * track, Turnout * turnouts, char * type) :
	track(track),
	turnouts(turnouts),
	type(type)  {
}

// tells a track to set its throttle
// if who is 0, try every track.
// returns the number of throttles succesfully set.
int Conductor::setThrottle(int who, int value) {
	int set = 0;
	for (int i = 0; i < 4; i++) {
		if (type[i] == 't' && (who - 1 == i || who == 0)) {
			track[i].setThrottle(value);
			set++;
		}
	}
	return set;
}

// tells a track to set its throttle
// if who is 0, try every track.
// returns the number of speeds successfully set.
int Conductor::setNextSpeed(int who) {
	int set = 0;
	for (int i = 0; i < 4; i++) {
		if (type[i] == 't' && (who - 1 == i || who == 0)) {
			track[i].setNextSpeed();
			set++;
		}
	}	
	return set;
}

// change tracks' speeds
// if who is 0, try every track.
// returns the number of tracks successfully adjusted.
int Conductor::changeSpeed(int who) {
	int set = 0;
	for (int i = 0; i < 4; i++) {
		if (type[i] == 't' && (who - 1 == i || who == 0)) {
			track[i].changeSpeed();
			set++;
		}
	}
	return set;
}

// change tracks' accelerations
// if who is 0, try every track.
// returns the number of tracks successfully adjusted.
int Conductor::setAcceleration(int who, int value) {
	int set = 0;
		for (int i = 0; i < 4; i++) {
			if (type[i] == 't' && (who - 1 == i || who == 0)) {
			track[i].setAcceleration(value);
			set++;
		}
	}
	return set;
}

// change tracks' braking
// if who is 0, try every track.
// returns the number of tracks successfully adjusted.
int Conductor::setBraking(int who, int value) {
	int set = 0;
	for (int i = 0; i < 4; i++) {
		if (type[i] == 't' && (who - 1 == i || who == 0)) {
			track[i].setBraking(value);
			set++;
		}
	}	
	return set;
}

// toggle the emergency brakes
// the conductor remembers the state of emergency.
int Conductor::emergencyBrake() {
	int set = 0;
	if (emergency) {
		for (int i = 0; i < 4; i++) {
			if (type[i] == 't') {
				track[i].disableEmergency();
				set++;
			}
		}
		emergency = 0;
	}
	else {
		for (int i = 0; i < 4; i++) {
			if (type[i] == 't') {
				track[i].disableEmergency();
				set++;
			}
		}
		emergency = 1;
	}
	return set;
}

// set a turnout straight
// if who is 0, try every turnout.
// returns the number of turnouts successfully straightened.
int Conductor::setStraight(int who) {
	int set = 0;
	for (int i = 0; i < 4; i++) {
		if (type[i] == 'p' && (who - 1 == i || who == 0)) {
			turnouts[i].setStraight();
			set++;
		}
	}
	return set;
}

// set a turnout turned
// if who is 0, try every turnout.
// returns the number of turnouts successfully turned.
int Conductor::setTurned(int who) {
	int set = 0;
	for (int i = 0; i < 4; i++) {
		if (type[i] == 'p' && (who - 1 == i || who == 0)) {
			turnouts[i].setTurned();
			set++;
		}
	}
	return set;
}


// set a turnout straight
// if who is 0, try every turnout.
// returns the number of turnouts successfully straightened.
int Conductor::throwSwitch(int who) {
	int set = 0;
	for (int i = 0; i < 4; i++) {
		if (type[i] == 'p' && (who - 1 == i || who == 0)) {
			turnouts[i].throwSwitch();
			set++;
		}
	}
	return set;
}

// get a turnout's state
SwitchState Conductor::getState(int who) {
	if (type[who - 1] == 'p') {
		return turnouts[who - 1].getState(); 
	}
}
