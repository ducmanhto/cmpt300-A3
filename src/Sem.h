#ifndef _SEM_H
#define _SEM_H

#include "list.h"
#include "Queue.h"

typedef struct Semaphore_s {
    int value;
    List* waitQueue; // List of PCBs waiting on this semaphore
} Semaphore;

void initializeSemaphores();
void semaphoreWait(int semIndex, PCB* pcb);
void semaphoreSignal(int semIndex);

#endif



