#include "serialcommand.h"

// SerialCommand
// A serial interface for controlling the railroad.
// Uses Arduino Train Control Commands(tm).
// An ATCC is an object: [(char) command, (int 0-4) recipient, (float) value]

SerialCommand::SerialCommand() {
	Serial.begin(9600);
	
	whos = "0";
}

void SerialCommand::read(Command & command) {
	// read 8 characters into input
	for(int i = 0; i < 8 && Serial.available(); i++) {
		input[i] = Serial.read();
		input[i+1] = '\0';
	}
	
	Serial.print("Input was: ");
	Serial.print((String)input);
	
	// find the command and recipient
	command.action = input[0];
	whos[0] = input[2];
	command.who = atoi(whos);
	
	// shift the input array 4 to the left, leaving only the parameter
	for(int i = 0; i < 4; i++) {
		input[i] = input[i+4];
	}
	
	command.value = atof(input);
}

void SerialCommand::write(Command command) {
}