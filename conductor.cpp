#include "conductor.h"

/**
 * Conductor
 * The conductor makes it easier to control many physical railroad elements.
 * Though it has no intelligence, it could be extended to have some.
 */

Conductor::Conductor(Track * track, Turnout * turnouts, char * type) :
	track(track),
	turnouts(turnouts),
	type(type) {
	emergency = false;
}

/** 
 * Tells a track to set its throttle. If who is 0, try to set every track.
 * @return the number of throttles succesfully set.
 */
int Conductor::setThrottle(int who, int value) {
	int set = 0;
	
	// if the emergency brake was on, disable it now.
	if (emergency) {
		emergencyBrake();
	}
	
	for (int i = 0; i < 4; i++) {
		if (type[i] == 't' && (who - 1 == i || who == 0)) {
			track[i].setThrottle(value);
			set++;
		}
	}
	return set;
}

/**
 * Tells a track to set its next speed. If who is 0, try to set every track.
 * @return the number of speeds successfully set.
 */
int Conductor::findNextSpeed(int who) {
	int set = 0;
	for (int i = 0; i < 4; i++) {
		if (type[i] == 't' && (who - 1 == i || who == 0)) {
			track[i].findNextSpeed();
			set++;
		}
	}	
	return set;
}

/**
 * Changes a track's speed. If who is 0, tries to change every track's speed.
 * @return the number of tracks successfully adjusted.
 */
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

/**
 * Adjust a track's braking coefficient. If who is 0, try to adjust every track.
 * @return the number of tracks successfully adjusted.
 */
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

/**
 * Toggle the emergency brakes for all tracks.
 * The conductor remembers the state of emergency, and ensures that it either
 * is or is not an emergency for all track together.
 * @return whether or not the track is now on emergency brake.
 */
bool Conductor::emergencyBrake() {
	if (emergency) {
		for (int i = 0; i < 4; i++) {
			if (type[i] == 't') {
				track[i].disableEmergency();
			}
		}
		emergency = false;
	}
	else {
		for (int i = 0; i < 4; i++) {
			if (type[i] == 't') {
				track[i].emergencyBrake();
			}
		}
		emergency = true;
	}
	return emergency;
}

/**
 * Sets a turnout straight. If who is 0, tries to straighten every turnout.
 * @return the number of turnouts successfully straightened.
 */
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

/**
 * Set a turnout turned. If who is 0, try to set every turnout.
 * @return the number of turnouts successfully turned.
 */
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


/**
 * Throws a turnout. If who is 0, try to throw every turnout.
 * @return the number of turnouts successfully straightened.
 */
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

/** Get a turnout's switched state. */
SwitchState Conductor::getState(int who) {
	if (type[who - 1] == 'p') {
		return turnouts[who - 1].getState(); 
	}
}
