#ifndef turnout_h
#define turnout_h

#include <AFMotor.h>
#include <WProgram.h>
#include <stdlib.h>

enum SwitchState {
  STRAIGHT,
  TURNED
};

class Turnout : public AF_DCMotor {
  public:
    Turnout(uint8_t);
    void setStraight();
    void setTurned();
    SwitchState throwSwitch();
    SwitchState getState();
  private:
    uint8_t _motorNum;
    SwitchState state;
    
    //AF_DCMotor * _motor;
};

#endif
