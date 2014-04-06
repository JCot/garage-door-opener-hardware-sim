/*
 * global.h
 *
 *  Created on: Mar 10, 2014
 *      Author: jdc9622, Jenny Zhen
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include <string>

using namespace std;

typedef struct{
	bool buttonPressed;
	bool doorClosed;
	bool doorOpen;
	bool motorUp;
	bool motorDown;
	bool irInterrupted;
	bool irBeamOn;
	bool motorOvercurrent;
	bool interrupted;
	bool doorOpening;
	bool doorClosing;
	bool interruptMovement;
	string lastCommand;
	int secondsPassed;
}SIGNALS;

extern SIGNALS signals;
extern pthread_mutex_t signals_mutex;
extern sem_t commands_semaphore;
extern pthread_cond_t done;
extern pthread_t input;
extern pthread_t scanner;
extern pthread_t motorThread;
extern queue <string> commands;

const static int DATA_BASE_ADDRESS = 0x280;
const static int DATA_PORT_A = DATA_BASE_ADDRESS + 0x08;
const static int DATA_PORT_B = DATA_BASE_ADDRESS + 0x09;
const static int DATA_DIRECTION = DATA_BASE_ADDRESS + 0xb;

#endif /* GLOBAL_H_ */
