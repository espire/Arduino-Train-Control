#include <AFMotor.h>
#include <stdlib.h>
#include "turnout.h"

char * input = "00";

float acceleration = 1; // how quickly the train increases speed to its target speed
float drag = 1; // how quickly the train decreases speed to its target speed

int targetSpeed = 0; // how fast would we like to go?
int nextSpeed = 0; // how fast will we set the motor next
int motorSpeed = 0; // how fast is the motor going this moment?

// milliseconds since last time we acted on the motor's speed
long timer = 0;

// motor 2 for train control at 2kHz. 2kHz happens to make the trains whine less.
AF_DCMotor motor(2, MOTOR12_2KHZ);

// left-turn and right-turn turnouts on motors 3 and 4
Turnout leftTurnout(3);
Turnout rightTurnout(4);

long counter = 0;
boolean running = false;

void setup() {
  Serial.begin(9600);
}



void checkThrottle() {
  if(Serial.available()) {
  
      // read 2 characters from serial
      Serial.println("Available: " + (String)Serial.available());
      input[0] = Serial.read();
      input[1] = Serial.read();
      Serial.flush();
      
      Serial.println("Input was: " + (String)(input[0]) + (String)(input[1]));
      
      targetSpeed = atoi(input);
      
      if(targetSpeed > 99)
        targetSpeed = 99;
        
      if(targetSpeed < 10)
        targetSpeed = 0;

      Serial.println("Target speed set to " + (String)targetSpeed);
    }
}

void setNextSpeed() {
  if(targetSpeed == 0) {
    nextSpeed = 0;
  }
  else if(targetSpeed > motorSpeed) {
    nextSpeed = motorSpeed + acceleration;
    Serial.println("Accelerating... next speed set to " + (String)nextSpeed);
  }
  else if(targetSpeed < motorSpeed) {
    nextSpeed = motorSpeed - drag;
    Serial.println("Coasting... next speed set to " + (String)nextSpeed);
  }
  else {
    // targetSpeed == motorSpeed, all is well
  }
}

void changeSpeed() {
  motorSpeed = nextSpeed;
  if(nextSpeed > 0) {
    motor.setSpeed(nextSpeed);
    motor.run(FORWARD);
  }
  else {
    motor.setSpeed(0);
    motor.run(RELEASE);
  }
}

void loop() {
  if(millis() > timer + 100) {
    
  checkThrottle();
  
  setNextSpeed();
  
  changeSpeed();

  timer = millis();
  }
}
