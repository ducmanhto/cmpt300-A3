#ifndef _QUEUE_H
#define _QUEUE_H

#include "pcb.h"
#include "list.h"

void initializeQueues();
int addProcessToReadyQueue(PCB* pcb);
void Queue_print();
// bool matchPCBByPID(void* pcb, void* pid);
// void removeProcessFromReadyQueue(int pid);

#endif