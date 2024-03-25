#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pcb.h"
#include "queues.h"
#include "list.h"

extern List* highPriorityReadyQueue;
extern List* normalPriorityReadyQueue;
extern List* lowPriorityReadyQueue;
// extern Semaphore semaphores[5];

// PCB* currentPCB = NULL;

// void selectNextProcessToRun() {
//     if (List_count(readyQueueHigh) != 0) {
//         currentPCB = (PCB*)List_remove(readyQueueHigh);
//     } else if (List_count(readyQueueMedium) != 0) {
//         currentPCB = (PCB*)List_remove(readyQueueMedium);
//     } else if (List_count(readyQueueLow) != 0) {
//         currentPCB = (PCB*)List_remove(readyQueueLow);
//     } else {
//         currentPCB = NULL; // No processes are ready to run
//     }
//     if (currentPCB != NULL) {
//         currentPCB->state = RUNNING;
//     }
// }

void handleCreateCommand();

int main() {
    char command;

    initializeQueues();

    while (1)
    {
        printf("\nOperating System Simulation Command Menu\n");
        printf("---------------------------------------\n");
        printf("C - Create a new process\n");

        printf("---------------------------------------\n");
        printf("Enter command: ");
        
        scanf(" %c", &command);

        switch (command)
        {
            case 'C':
                handleCreateCommand();
                break;
            default:
                printf("Unkown command.\n");
        }
    }
    return 0;
}

void handleCreateCommand()
{
    int priority;
    printf("Enter the priority of the new process (0=high, 1=normal, 2=low): ");
    scanf("%d", &priority);

    // Validate the priority input
    if (priority < 0 || priority > 2)
    {
        printf("Invalid priority. Please enter a value between 0 and 2.\n");
        printf("Failed to create a new process.\n");
        return ;
    }

    PCB* newPCB = PCB_create(priority);
    if (newPCB == NULL)
    {
        printf("Failed to create a new process.\n");
    }

    int addedProcessResult = addProcessToReadyQueue(newPCB);
    if (addedProcessResult == -1)
    {
        printf("Failed to add the new process to the ready queue.\n");
        return;
    }

    printf("New process created successfully with PID: %d.\n", newPCB->pid);
}