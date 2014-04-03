/*
 * Input.cpp
 *
 *  Created on: Mar 10, 2014
 *      Author: jdc9622, Jenny Zhen
 */

#include "Input.h"
#include "global.h"
#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <hw/inout.h>
#include <sys/neutrino.h>
#include <sys/mman.h>

using namespace std;

#define SUPPRESS

/**
* Input constructor.
**/
Input::Input() {
}

/**
* Input destructor.
**/
Input::~Input() {
}

/**
* Process user input through standard input.
**/
void* Input::processInput(){

	ThreadCtl(_NTO_TCTL_IO, NULL);

	uintptr_t portAHandle;
	uintptr_t portBHandle;
	uintptr_t directionHandle;

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

	int a = in8(portAHandle);
	int b = in8(portBHandle);
	int c;

	cout << "A: " << a << "\n";
	cout << "B: " << b << "\n";
	cout.flush();

	out8(directionHandle, 0x10);
	out8(portBHandle, 0x00);
	sleep(1);
	out8(portBHandle, 0x09);

	a = in8(portAHandle);
	b = in8(portBHandle);
	c = in8(directionHandle);

	cout << "A: " << a << "\n";
	cout << "B: " << b << "\n";
	cout << "C: " << c;
	cout.flush();

	while(true){
		string input;
		#ifndef SUPPRESS
		cout << "Please enter a command: ";
		#endif
		getline(cin, input);

		if(input == "m" || input == "i" || input == "r"){
			commands.push(input);
		}

		else{
			cout << "Incorrect input.\n";
		}

		sleep(1);
	}
}
