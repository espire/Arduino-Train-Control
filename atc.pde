#include <WProgram.h>
#include <stdlib.h>
#include <AFMotor.h>
#include "track.h"
#include "turnout.h"

// raw input from serial, such as "t -120"
char input[20];

// the first character from input, used to specify what the command is
char command = ' ';

// the third character from input, used to specify who recieves the command
// 1-4 designate a motor; a designates all the motors
char * whos = "0";
int who;

// the parameter associated with the command, such as speed
float value = 0;

// milliseconds since the last time we acted on input
long timer = 0;

// arrays for track and turnouts. we have overloaded the motors.
Track tracks[4] = {
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

void setup() {
	Serial.begin(9600);
	Serial.println("ATC started.");
	Serial.flush();
	
	// straighten the turnouts
	// it's a good idea to make sure they're straight after this.
	// if they're not, swap its wires.
	for (int i = 0; i < 4; i++) {
		if (type[i] == 'p') {
			turnouts[i].setStraight();
		}
	}
}

void loop() {
	//Serial.println("ATC running!");
	if(millis() > timer + 50) {
		//Serial.print("Current current reading: ");
		//Serial.println(analogRead(0));
		if(Serial.available()) {
			Serial.print("Available: ");
			Serial.println((String)Serial.available());
		
			// read 19 characters into input
			for(int i = 0; i < 18 && Serial.available(); i++) {
				input[i] = Serial.read();
				input[i+1] = '\0';
			}
			Serial.print("Input was: ");
			Serial.println((String)input);
			
			// find the command and recipient
			command = input[0];
			whos[0] = input[2];
			who = atoi(whos);
			
			// shift the input array 4 to the left, leaving only the parameter
			for(int i = 0; i < 4; i++) {
				input[i] = input[i+4];
			}
			
			value = atof(input);
		
			// do stuff
			switch(command) {
				case 't':
				if (type[who - 1] == 't') {
					tracks[who - 1].setThrottle(value);
				}
				break;
				case 'a':
				if (type[who - 1] == 't') {
					tracks[who - 1].setAcceleration(value);
				}
				break;
				case 'b':
				if (type[who - 1] == 't') {
					tracks[who - 1].setBraking(value);
				}
				break;
				case 'e':
				for (int i = 0; i < 4; i++) {
					if (type[i] == 't') {
						tracks[i].emergencyBrake();
					}
				}
				break;
				case 'c':
				for (int i = 0; i < 4; i++) {
					if (type[who - 1] == 't') {
						tracks[who - 1].disableEmergency();
					}
				}
				break;
				case 'p':
				if (type[who - 1] == 'p') {
					turnouts[who - 1].throwSwitch();
				}
				break;
				case 's':
				Serial.print("Current current reading: ");
				Serial.println(analogRead(0));
				break;
			}
			Serial.flush();
		}
		
		// do more stuff
		for (int i = 0; i < 4; i++) {
			if (type[i] == 't') {
				tracks[i].setNextSpeed();
				tracks[i].changeSpeed();
			}
		}
	
	timer = millis();
	}
}
