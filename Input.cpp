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

	if(!runAsSoftwareSim){
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

		out8(directionHandle, 0x10);
		out8(portBHandle, 0x00);
		sleep(1);
		out8(portBHandle, 0x08);

		while(true){
			int digitalInput = in8(portAHandle);

			//Depending on machine first 3 bits may be 0 or 1. If bits are 1 need to add 224 to check to get desired result.
			if(digitalInput == 240 || digitalInput == 241 || digitalInput == 242){
				cout.flush();
				commands.push("r");
			}

			else if(digitalInput == 232){
				commands.push("m");
			}

			else if(digitalInput == 228){
				commands.push("i");
			}

			sleep(1);
		}
	}

	else{
		while(true){
			#ifndef SUPPRESS
			cout << "Please enter a command: ";
			cout.flush();
			#endif

			string input;
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
}
