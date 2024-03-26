#ifndef _SEM_H
#define _SEM_H

#include "list.h"
#include "queues.h"

typedef struct Semaphore_s {
    int value;
    List* waitQueue; // List of PCBs waiting on this semaphore
} Semaphore;

bool initializeSemaphore(int semID, int initialValue);
int semaphoreWait(int semIndex, PCB* pcb);
int semaphoreSignal(int semIndex);

#endif



