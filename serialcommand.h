#ifndef serialcommand_h
#define serialcommand_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <stdlib.h>
#include "command.h"

class SerialCommand {
public:
	SerialCommand();
	void read(Command & command);
	void write(Command command);
	
	// Raw input from serial, such as "t 2 -100"
	char input[9];
};

#endif
