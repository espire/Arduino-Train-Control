#include <AFMotor.h>
#include <stdlib.h>
#include "track.h"
#include "turnout.h"

char input[5] = {'\0'};


// milliseconds since the last time we acted on the motor's speed
long timer = 0;

// motor 2 for train control at 2kHz. 2kHz happens to make the trains whine less.
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
		for(int i=0; i<4 && Serial.available(); i++) {
			input[i] = Serial.read();
			input[i+1] = '\0';
		}
		Serial.flush();
		Serial.println("Input was: " + (String)input);
		
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