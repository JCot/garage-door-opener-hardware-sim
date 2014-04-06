/*
 * Motor.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: jdc9622, Jenny Zhen
 */

#include "Motor.h"
#include "global.h"
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <hw/inout.h>
#include <sys/neutrino.h>
#include <sys/mman.h>

using namespace std;

#define DEBUG
/**
* If a signal is received while the motor thread is not sleeping, the 
movement of the motor gets interrupted/stopped.
**/
static void sigintHandler(int sig){
	#ifdef DEBUG_V
	cout << "Received signal: " << sig << endl;
	#endif
	
	
	// If we get this interrupt while not sleeping, we need another way to 
	// indicate that we should stop.
	signals.interruptMovement = true;
	cout.flush();
	return;
}

/**
* Motor constructor.
**/
Motor::Motor() {
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = sigintHandler;
	signal(SIGUSR1, sigintHandler);

	ThreadCtl(_NTO_TCTL_IO, NULL);

	portAHandle;
	portBHandle;
	directionHandle;

	portAHandle = mmap_device_io(1, DATA_PORT_A);
	portBHandle = mmap_device_io(1, DATA_PORT_B);
	directionHandle = mmap_device_io(1, DATA_DIRECTION);

	if(portAHandle == MAP_DEVICE_FAILED){
		perror("Failed to map control register");
	}

	if(portBHandle == MAP_DEVICE_FAILED){
		perror("Failed to map control register");
	}

	if(directionHandle == MAP_DEVICE_FAILED){
		perror("Failed to map control register");
	}
}

/**
* Motor destructor.
**/
Motor::~Motor() {
}

/**
* Open the garage door.
* The controller will raise the door:
* - When the user presses the remote pushbutton and the door is in the 
*   full closed position.
* - When the user presses the remote pushbutton and the door is stopped 
*   and the door was lowering prior to stopping.
* - If the door is lowering and a motor overcurrent indication is received.
* - If the door is lowering and an infrared beam interruption is recieved.
**/
void Motor::openDoor(){
	signals.doorClosed = false;
	signals.interrupted = false;
	signals.doorOpening = true;
	signals.doorClosing = false;

	struct timespec tim;
	tim.tv_sec = 1;
	tim.tv_nsec = 0;

	cout << "I am opening the door.\n";
	//cout << "\tOpening for: " << (10 - signals.secondsPassed) << " seconds.\n";
	cout.flush();

	int i = 1;
	out8(portBHandle, 0x09);
	while(signals.secondsPassed < 10){
		if(nanosleep(&tim, NULL) == -1 || signals.interruptMovement){
			#ifdef DEBUG_V
			cout << "I was interrupted; returning!\n";
			#endif
			signals.interruptMovement = false;
			out8(portBHandle, 0x08);
			return;
		}
		signals.secondsPassed++;
		//cout << "\t\tOpening for " << i++ << " seconds...\n";
		//cout.flush();
	}
	
	out8(portBHandle, 0x08);
	cout << "Door opened.\n";
	cout.flush();
	signals.doorOpen = true;
	signals.doorOpening = false;
}

/**
 * Temporary (hopefully) fix for when the door stops opening a second early
 * when opening from a non full closed position.
 */
void Motor::reOpenDoor(){
	signals.secondsPassed--;
	openDoor();
}

/**
 * Same as reOpenDoor except it's to close the door.
 */
void Motor::reCloseDoor(){
	signals.secondsPassed++;
	closeDoor();
}

/**
* Close the garage door.
* The controller will lower the door:
* - When the user presses the remote pushbutton and the door is in the full 
*   open position.
* - When the user presses the remote pushbutton and the door is stopped and 
*   the door was raising prior to stopping. 
**/
void Motor::closeDoor(){
	signals.doorOpen = false;
	signals.interrupted = false;
	signals.doorClosing = true;
	signals.doorOpening = false;
	signals.irBeamOn = true;
	
	struct timespec tim;
	tim.tv_sec = 1;
	tim.tv_nsec = 0;

	cout << "I am closing the door.\n\tI am turning on the infrared beam.\n";
	//cout << "\tClosing for: " << signals.secondsPassed << " seconds.\n";
	cout.flush();

	out8(portBHandle, 0xE);
	int i = 1;
	while(signals.secondsPassed > 0){
		if(nanosleep(&tim, NULL) == -1 || signals.interruptMovement){
			#ifdef DEBUG_V
			cout << "I was interrupted; returning!\n";
			#endif
			signals.interruptMovement = false;
			signals.irBeamOn = false;
			out8(portBHandle, 0x08);
			return;
		}
		//cout << "\t\tClosing for " << i++ << " seconds...\n";
		//cout.flush();
		signals.secondsPassed--;
	}

	out8(portBHandle, 0x08);
	cout << "Door closed.\n";
	cout.flush();
	signals.doorClosed = true;
	signals.irBeamOn = false;
	signals.doorClosing = false;
}

/**
* Stop the movement of the door.
* The controller will stop lowering the door:
* - When the full closed position is reached.
* - If the user presses the remote pushbutton while the door is moving.
* - If the door is raising and a motor overcurrent indication is received.
**/
void Motor::stopDoor(){
	cout << "I am stopping the door.\n";
	cout.flush();

	out8(portBHandle, 0x08);

	cout << "Door stopped.\n";
	cout.flush();
}
