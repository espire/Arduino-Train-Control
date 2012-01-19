#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <stdlib.h>
#include <AFMotor.h>
#include "track.h"
#include "turnout.h"
#include "conductor.h"
#include "command.h"
#include "serialcommand.h"

// milliseconds since the last time we acted on input
long timer = 0;

// Read ATCCs into here
Command command;

// We will get ATCCs from serial
SerialCommand serialcommand;

// arrays for track and turnouts. we have overloaded the motors.
Track track[4] = {
	Track(1,0.6,1),
	Track(2,0.6,1),
	Track(3,0.6,1),
	Track(4,0.6,1)
};
Turnout turnouts[4] = {
	Turnout(1),
	Turnout(2),
	Turnout(3),
	Turnout(4)
};

// running a turnout as track can burn out its motor. be careful!
// if the turnout's back is hot, you're probably doing something wrong.
// this array prevents commands from running on the wrong type of object.
// p is a turnout, t is track.
// if you don't set properly, it will not save you D:
char type[4] = {'p', 't', '0', '0'};

// the Conductor manages these motors for us
Conductor conductor(track, turnouts, type);

void setup() {
	Serial.begin(9600);
	Serial.println("ATC started.");
	
	// straighten the turnouts
	// it's a good idea to make sure they're straight after this.
	// if they're not, swap its wires.
	conductor.setStraight(0);
}

void loop() {
	if(millis() > timer + 100) {
		if(Serial.available()) {
			Serial.println();
			Serial.print("Available: ");
			Serial.println((String)Serial.available());
			
			serialcommand.read(command);
			
			// do stuff
			switch(command.action) {
				case 't':
				conductor.setThrottle(command.who, command.value); break;
				case 'a':
				conductor.setAcceleration(command.who, command.value); break;
				case 'b':
				conductor.setBraking(command.who, command.value); break;
				case 'e':
				conductor.emergencyBrake(); break;
				case 'p':
				conductor.throwSwitch(command.who); break;
				case 's':
				Serial.print("Current current reading: ");
				Serial.println(analogRead(0));
				break;
			}
		}
		
		// do more stuff
		conductor.setNextSpeed(0);
		conductor.changeSpeed(0);
	
	timer = millis();
	}
}
