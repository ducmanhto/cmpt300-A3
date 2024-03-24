#ifndef _QUEUE_H
#define _QUEUE_H

#include "list.h"

typedef enum {
    RUNNING,
    READY,
    BLOCKED
} ProcessState;

typedef struct PCB_s {
    int processID;
    int priority; // 0, 1, or 2
    ProcessState state;
    char* message; // Place to store messages sent or replied to this process.
} PCB;

void initializeProcessControl();
void addProcessToReadyQueue(PCB* pcb);
bool matchPCBByPID(void* pcb, void* pid);
void removeProcessFromReadyQueue(int pid);

#endif