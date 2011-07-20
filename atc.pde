#include <AFMotor.h>
#include <stdlib.h>
#include "track.h"
#include "turnout.h"

char * input = "000";

// milliseconds since last time we acted on the motor's speed
long timer = 0;

// motor 2 for train control at 2kHz. 2kHz happens to make the trains whine less.
Track track(2,0.6,1);

// left-turn and right-turn turnouts on motors 3 and 4
Turnout leftTurnout(3);
Turnout rightTurnout(4);

void setup() {
	Serial.begin(9600);
}

void loop() {
  if(millis() > timer + 50) {
    if(Serial.available()) {
      // read 2 characters from serial
		Serial.println("Available: " + (String)Serial.available());
		input[0] = Serial.read();
		input[1] = Serial.read();
		input[2] = Serial.read();
		Serial.flush();
		Serial.println("Input was: " + (String)(input[0]) + (String)(input[1]) + (String)(input[2]));
		
		if(input[0] == 'e') {
			track.emergencyBrake();
		}
		else if(input[0] == 'c') {
			track.disableEmergency();
		}
		else {
			track.setThrottle(atoi(input));
		}
	}
	
	track.setNextSpeed();
	track.changeSpeed();
	
	timer = millis();
  }
}