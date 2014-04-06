/*
 * global.cpp
 *
 *  Created on: Mar 10, 2014
 *      Author: jdc9622, Jenny Zhen
 */

#include "global.h"
#include <pthread.h>
#include <queue>

using namespace std;

SIGNALS signals{false, true, false, false, false, false, false, 
false, false, false, false, false, "", 0};

pthread_mutex_t signals_mutex;
sem_t commands_semaphore;
pthread_cond_t done;
pthread_t input;
pthread_t scanner;
pthread_t motorThread;
queue <string> commands;
//const static int DATA_BASE_ADDRESS = 0x280;
//const static int DATA_PORT_A = DATA_BASE_ADDRESS + 0x08;
//const static int DATA_PORT_B = DATA_BASE_ADDRESS + 0x09;
//const static int DATA_DIRECTION = DATA_BASE_ADDRESS + 0xb;
