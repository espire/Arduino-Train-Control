#include <AFMotor.h>
#include <stdlib.h>
#include "track.h"
#include "turnout.h"

// raw input from serial, such as "t -120"
char input[7] = {'\0'};
// the first character from input, used to specify what the command is
char command = ' ';
// the parameter associated with the command
float value = 0;

// milliseconds since the last time we acted on the motor's speed
long timer = 0;

// motor 2 for train control at 2kHz. 2kHz happens to make the trains whine less than other frequencies.
Track track(2,0.6,1);

// left-turn and right-turn turnouts on motors 3 and 4
Turnout leftTurnout(3);
Turnout rightTurnout(4);

void setup() {
	Serial.begin(9600);
	Serial.flush();
}

void loop() {
  if(millis() > timer + 50) {
    if(Serial.available()) {
	
		Serial.println("Available: " + (String)Serial.available());
		
		for(int i = 0; i < 6 && Serial.available(); i++) {
			input[i] = Serial.read();
			input[i+1] = '\0';
		}
		Serial.println("Input was: " + (String)input);
		
		command = input[0];
		
		// shift the input array 2 to the left, leaving only the parameter
		for(int i = 0; i < 4; i++) {
			input[i] = input[i+2];
		}
		
		value = atof(input);
		
		switch(command) {
			case 't':
			track.setThrottle(value); break;
			case 'a':
			track.setAcceleration(value); break;
			case 'b':
			track.setBraking(value); break;
			case 'e':
			track.emergencyBrake(); break;
			case 'c':
			track.disableEmergency(); break;
		}
		
		Serial.flush();
	}
	
	track.setNextSpeed();
	track.changeSpeed();
	
	timer = millis();
  }
}