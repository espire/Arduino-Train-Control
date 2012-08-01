#include "serialcommand.h"

/**
 * SerialCommand
 * A serial interface for controlling the railroad.
 * Uses Arduino Train Control Commands(tm).
 * An ATCC is an object: [(char) command, (int 0-4) recipient, (float) value]
 */

SerialCommand::SerialCommand() {
	Serial.begin(9600);
}

/**
 * Read a command from serial and parse it into an ATCC.
 */
void SerialCommand::read(Command & command) {
	// Read 8 characters into input.
	for (int i = 0; i < 8 && Serial.available(); i++) {
		input[i] = Serial.read();
		input[i + 1] = '\0';
	}
	
	Serial.print("Input was: ");
	Serial.println((String)input);
	
	// Find the command and recipient.
	command.action = input[0];
	char * who = (char *) input[2];
	command.who = atoi(who);
	
	// Shift the input array 4 to the left, then get the parameter.
	for (int i = 0; i < 4; i++) {
		input[i] = input[i+4];
	}
	command.value = atof(input);
}

/**
 * Given an ATCC, write the command to serial.
 */
void SerialCommand::write(Command command) {
}
