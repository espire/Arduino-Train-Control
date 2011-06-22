#ifndef turnout_h
#define turnout_h

#include <AFMotor.h>
#include <WProgram.h>
#include <stdlib.h>

enum SwitchState {
	STRAIGHT,
	TURNED,
	UNKNOWN
};

class Turnout : public AF_DCMotor {
  public:
    Turnout(uint8_t);
    void setStraight();
    void setTurned();
    SwitchState throwSwitch();
    SwitchState getState();
  private:
    uint8_t motorNum;
    SwitchState state;
};

#endif
