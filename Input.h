/*
 * Input.h
 *
 *  Created on: Mar 10, 2014
 *      Author: jdc9622, Jenny Zhen
 */

#ifndef INPUT_H_
#define INPUT_H_

/**
* Class Input receives user input through key presses.
* Simulates a remote control.
**/
class Input {
public:
	Input();
	virtual ~Input();
	void* processInput();

private:
	const static int DATA_BASE_ADDRESS = 0x280;
	const static int DATA_PORT_A = DATA_BASE_ADDRESS + 0x08;
	const static int DATA_PORT_B = DATA_BASE_ADDRESS + 0x09;
	const static int DATA_DIRECTION = DATA_BASE_ADDRESS + 0xb;
};

#endif /* INPUT_H_ */
