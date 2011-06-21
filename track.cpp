#include "track.h"

Track::Track(uint8_t motorNum, uint8_t freq) : AF_DCMotor(motorNum, freq) {
	_motorNum = motorNum;
}