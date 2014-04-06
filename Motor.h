/*
 * Motor.h
 *
 *  Created on: Mar 11, 2014
 *      Author: jdc9622, Jenny Zhen
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <signal.h>
#include <stdint.h>

/**
* Motor class simulates the motor opening and closing a garage door.
* It is represented by a thread performing operations based on input that is 
* given through the Input class, sent to the Controller class.
**/
class Motor {
public:
	Motor();
	virtual ~Motor();
	void openDoor();
	void reOpenDoor();
	void closeDoor();
	void stopDoor();
	struct sigaction sa;

private:
	uintptr_t portAHandle;
	uintptr_t portBHandle;
	uintptr_t directionHandle;
};

#endif /* MOTOR_H_ */
