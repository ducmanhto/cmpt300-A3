#ifndef _QUEUE_H
#define _QUEUE_H

#include "pcb.h"
#include "list.h"

void initializeQueues();
int addProcessToReadyQueue(PCB* pcb);
// bool matchPCBByPID(void* pcb, void* pid);
// void removeProcessFromReadyQueue(int pid);

#endif