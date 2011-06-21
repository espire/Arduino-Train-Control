/*
 * WiiChuk Train Controller
 * Ken Olsen (648.ken@gmail.com)
 * http://www.dawson-station.blogspot.com
 * Jan 2010
 *
 * Uses LadyAda motor shield (www.ladyada.net/make/mshield/)
 * and Tod E. Kurt's brilliant WiiMote Connector and libary:  
 * http://todbot.com/blog/2008/02/18/wiichuck-wii-nunchuck-adapter-available/
 *
 * Track leads connected to M1 on motor shield
 * 
 * WiiMote Connector Pins:  
 *        A2 -
 *        A3 +
 *        A4 d
 *        A5 c
 */

#include <Wire.h>
#include "nunchuck_funcs.h"
#include <AFMotor.h>

int debug = true;  // controls if Serial.printing or not
AF_DCMotor motor(1, MOTOR12_2KHZ); // create motor #1

/*
  "For motors 1 and 2 you can choose 
  MOTOR12_64KHZ, MOTOR12_8KHZ, MOTOR12_2KHZ, or MOTOR12_1KHZ. 
  A high speed like 64KHz wont be audible but a low speed 
  like 1KHz will use less power. Motors 3 & 4 are only possible 
  to run at 1KHz and will ignore any setting given. "
  (My engine seems to sounds better with MOTOR12_2KHZ))
*/

byte accx,accy,accz,zbut,cbut,joyx,joyy,joyxMid; // hold Chuck readings
int trainDir = FORWARD; // 1 = FORWARD, 2 = BACKWARD
int throttle = 0;       // adjusted joystick reading
int speedLimit = 125;   // max motor set speed (up to 255)
int speedSet = 0;       // current motor set speed
int deadBand = 10;      // for joystick
float curSpeed = 0;     // current motor speed
float diff = 0;         // change in speed based on accel/momentum
int drag = 50;          // lower = faster speed response
int stallSpeed = 20;    // speed needed to start train movement
int brakeDrag = 10;     // lower = slows faster, must be >0
int rollDrag = 50;      // higher is lower
int cruise = 0;         // cruise control to hold speed?


void setup()
{
    Serial.begin(9600); // for printing to monitor
    nunchuck_setpowerpins();  // setup Chuk pins (if A2,A3 = gnd/pwr)
    nunchuck_init(); // send the initilization handshake
    readChuk();     // throw away first reading (not sure why needed)
    readChuk(); 
    joyxMid = joyx;  // assumes joystick centered on powerup
    if (debug){      // print intial readings
      Serial.print("\nWiiChuk Train Controller Ver.2\n");
      Serial.print("  speedLimit = "); Serial.println(speedLimit);
      Serial.print("  stallSpeed = "); Serial.println(stallSpeed);
      Serial.print("  rollDrag = "); Serial.println(rollDrag);
      Serial.print("  brakeDrag = "); Serial.println(brakeDrag);      
      Serial.print("\nCHUK reads:  accx: "); Serial.print((byte)accx,DEC);
      Serial.print("  joyx: "); Serial.print((byte)joyx,DEC);
      Serial.print("  cbut: "); Serial.print((byte)cbut,DEC);
      Serial.print("  zbut: "); Serial.println((byte)zbut,DEC);
      Serial.println("\nWaiting for joystick movement . . .");
      do{ // wait for joystick movement
        readChuk();
        delay(100);
      } while (abs(joyx - joyxMid) < deadBand);
    }
    motor.setSpeed(0); 
    motor.run(trainDir);
}

void loop()
{
  readChuk();
  if (joyx == 255) {// reset the connection
    nunchuck_init();
    Serial.println("CHUK RESET!");
    delay(500); // just because
    readChuk(); // get new readings
    readChuk(); // discard first set
  }
  if (accx != 0 ){ // Chuk plugged in
    if (zbut) { // brakes on, cruise off!
      drag = brakeDrag; // increase drag
      cruise = 0;       // turn off cruise
    } 
    else{ drag = rollDrag;} // normal momentum
    if (cbut) { // turn on cruise control 
      cruise = 1;
    } 
    throttle = joyx - joyxMid; // desired throttle
  
    if (throttle < -deadBand && trainDir == FORWARD){ // slow down
      speedSet = 0;
      cruise = 0;  // turn off cruise control
      drag = brakeDrag;
      if (curSpeed < deadBand){ // switch direction
        curSpeed = 0;
        trainDir = BACKWARD;
        motor.run(trainDir);
        Serial.println("switch direction backward");
      }
    }
    else if (throttle > deadBand && trainDir == BACKWARD){ // slow down
      speedSet = 0;
      cruise = 0;  // turn off cruise control
      drag = brakeDrag;
      if (curSpeed < deadBand){ // switch direction
        curSpeed = 0;
        trainDir = FORWARD;
        motor.run(trainDir);
        Serial.println("switch direction forward");
      }
    }
    else {
      if (abs(throttle) < deadBand){
        speedSet = 0;}
      else { // speed up
        speedSet = abs(throttle * speedLimit / 100); 
      }
    }
    diff = (speedSet - curSpeed) / (drag);  // determin change

    if (curSpeed < stallSpeed){ // handle slow speeds
      if (curSpeed >= speedSet){ // slowing, jump to zero
        curSpeed = 0;
      }
      else {
        curSpeed = stallSpeed;  // speedup, jump to stallSpeed
      }
    }
    else{ // implement drag
      if (!cruise || diff > 0){ 
        curSpeed = curSpeed + diff;
      }
    }
    
    motor.setSpeed(abs(curSpeed));
    delay(100); // don't need rapid change
    if (debug){ // show what you want to see
      //Serial.print("accx: "); Serial.print((byte)accx,DEC);
      Serial.print("  joyx: "); Serial.print((byte)joyx,DEC);
      //Serial.print("  zbut: "); Serial.print((byte)zbut,DEC);
      //Serial.print("  trainDir: "); Serial.print(trainDir);
      Serial.print("  throttle: "); Serial.print(throttle);    
      Serial.print("  speedSet: "); Serial.print(speedSet);
      Serial.print("  curSpeed: "); Serial.print(curSpeed);
      Serial.print("  diff: "); Serial.print(diff);
      Serial.print("  cruise: "); Serial.println(cruise);
    }
  }
  else{
    Serial.println("WHERE IS CHUK?");
    delay(1000); // wait before checking again
  }
}


void readChuk(){ // check Chuk readings
  nunchuck_get_data(); // from nunchuck_funcs.h
  delay(1); // not sure why needed, but keeps first data set clean
  accx  = nunchuck_accelx(); // 70 - 125 - 182
  //accy  = nunchuck_accely(); // ranges from approx 65 - 112 - 173
  //accz  = nunchuck_accelz(); // ranges from approx ??  ??
  joyx = nunchuck_joyx();  //34 - 137 - 234
  //joyy = nunchuck_joyy();  //34 - 129 - 217
  zbut = nunchuck_zbutton();
  cbut = nunchuck_cbutton(); 
}
