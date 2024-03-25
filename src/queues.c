#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "queues.h"

List* highPriorityReadyQueue;
List* normalPriorityReadyQueue;
List* lowPriorityReadyQueue;

void initializeQueues()
{
    highPriorityReadyQueue = List_create();
    normalPriorityReadyQueue = List_create();
    lowPriorityReadyQueue = List_create();

    if (!highPriorityReadyQueue || !normalPriorityReadyQueue || !lowPriorityReadyQueue)
    {
        printf("Failed to initialize queues\n");
    }
}


// Function to add a process to the ready queue based on priority
int addProcessToReadyQueue(PCB* pcb) {
    List* targetQueue;

    switch (pcb->priority) {
        case 0:
            targetQueue = highPriorityReadyQueue;
            break;
        case 1:
            targetQueue = normalPriorityReadyQueue;
            break;
        case 2:
            targetQueue = lowPriorityReadyQueue;
            break;
        default:
            return -1;
    }

    return List_append(targetQueue, pcb);
}

// Comparator function to match PCB by PID
// bool matchPCBByPID(void* pcb, void* pid) {
//     PCB* pPCB = (PCB*)pcb;
//     int* pPID = (int*)pid;
//     return (pPCB->processID == *pPID) ? true : false;
// }

// Function to remove a process from its ready queue based on PID
// void removeProcessFromReadyQueue(int pid) {
//     PCB* pcbToRemove = NULL;
//     List* queues[] = {readyQueueHigh, readyQueueMedium, readyQueueLow};
//     int queueCount = sizeof(queues) / sizeof(queues[0]);

//     // Search for the PCB in each queue
//     for (int i = 0; i < queueCount; i++) {
//         List_first(queues[i]); // Reset current to the first item for searching
//         pcbToRemove = (PCB*)List_search(queues[i], matchPCBByPID, &pid);
//         if (pcbToRemove != NULL) {
//             // Found the PCB, now remove it
//             List_remove(queues[i]); // This removes the current item, which is our PCB
//             break;
//         }
//     }

//     // Optionally handle the case where the PCB was not found
//     if (pcbToRemove == NULL) {
//         fprintf(stderr, "Process with PID %d not found in any ready queue.\n", pid);
//         exit(EXIT_FAILURE);
//     }
// }

