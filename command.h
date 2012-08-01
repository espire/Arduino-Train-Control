#ifndef command_h
#define command_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// An Arduiono Train Control Command(tm) (ATCC).

class Command {
public:
	char action;
	int who;
	float value;
};

#endif
