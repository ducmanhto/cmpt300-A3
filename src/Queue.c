#include "list.h"
#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

// Declaration of global queues
List* readyQueueHigh; // Priority 0
List* readyQueueMedium; // Priority 1
List* readyQueueLow; // Priority 2
List* sendQueue; // Processes waiting for a reply
List* receiveQueue; // Processes waiting to receive a message

void initializeProcessControl() {
    // Create the ready queues for different priorities
    readyQueueHigh = List_create();
    readyQueueMedium = List_create();
    readyQueueLow = List_create();
    
    // Check for creation failure (in case the list limit is reached)
    if (!readyQueueHigh || !readyQueueMedium || !readyQueueLow) {
        fprintf(stderr, "Failed to create one or more ready queues.\n");
        exit(EXIT_FAILURE); 
    }

    // Create the queues for processes waiting on send/receive operations
    sendQueue = List_create();
    receiveQueue = List_create();

    // Check for creation failure
    if (!sendQueue || !receiveQueue) {
        fprintf(stderr, "Failed to create send or receive queue.\n");
        exit(EXIT_FAILURE);
    }
}

void addProcessToReadyQueue(PCB* pcb) {
    List* targetQueue;
    switch (pcb->priority) {
        case 0:
            targetQueue = readyQueueHigh;
            break;
        case 1:
            targetQueue = readyQueueMedium;
            break;
        case 2:
            targetQueue = readyQueueLow;
            break;
        default:
            // Handle invalid priority
            return;
    }
    List_append(targetQueue, pcb);
}



// Comparator function to match PCB by PID
bool matchPCBByPID(void* pcb, void* pid) {
    PCB* pPCB = (PCB*)pcb;
    int* pPID = (int*)pid;
    return (pPCB->processID == *pPID) ? true : false;
}

// Function to remove a process from its ready queue based on PID
void removeProcessFromReadyQueue(int pid) {
    PCB* pcbToRemove = NULL;
    List* queues[] = {readyQueueHigh, readyQueueMedium, readyQueueLow};
    int queueCount = sizeof(queues) / sizeof(queues[0]);

    // Search for the PCB in each queue
    for (int i = 0; i < queueCount; i++) {
        List_first(queues[i]); // Reset current to the first item for searching
        pcbToRemove = (PCB*)List_search(queues[i], matchPCBByPID, &pid);
        if (pcbToRemove != NULL) {
            // Found the PCB, now remove it
            List_remove(queues[i]); // This removes the current item, which is our PCB
            break;
        }
    }

    // Optionally handle the case where the PCB was not found
    if (pcbToRemove == NULL) {
        fprintf(stderr, "Process with PID %d not found in any ready queue.\n", pid);
        exit(EXIT_FAILURE);
    }
}

