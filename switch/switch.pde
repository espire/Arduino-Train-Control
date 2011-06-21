#include <AFMotor.h>
#include <stdlib.h>

AF_DCMotor motor(3); // create motor #3, 2KHz pwm

void setup() {
	Serial.begin(9600);
	
	motor.setSpeed(255);
	
}

void loop() {
	motor.run(FORWARD);
	delay(25);
	motor.run(RELEASE);
	delay(3000);
	motor.run(BACKWARD);
	delay(25);
	motor.run(RELEASE);
	delay(3000);
}