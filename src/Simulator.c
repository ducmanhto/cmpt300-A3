#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Sem.h"
#include "Queue.h"
#include "list.h"

extern List* readyQueueHigh;
extern List* readyQueueMedium;
extern List* readyQueueLow;
extern Semaphore semaphores[5];

PCB* currentPCB = NULL;

void selectNextProcessToRun() {
    if (!List_isEmpty(readyQueueHigh)) {
        currentPCB = (PCB*)List_remove(readyQueueHigh);
    } else if (!List_isEmpty(readyQueueMedium)) {
        currentPCB = (PCB*)List_remove(readyQueueMedium);
    } else if (!List_isEmpty(readyQueueLow)) {
        currentPCB = (PCB*)List_remove(readyQueueLow);
    } else {
        currentPCB = NULL; // No processes are ready to run
    }
    if (currentPCB != NULL) {
        currentPCB->state = RUNNING;
    }
}

int main() {
    char command;
    int semID, initialValue;
    int processID, priority; // For process creation simulation

    // Initialize the process control system and semaphores
    initializeProcessControl();
    // Additional semaphore initialization as required

    printf("Process and Semaphore operations:\n");
    printf("N <semID> <initialValue> - Initialize semaphore\n");
    printf("P <semID> - Semaphore wait operation\n");
    printf("V <semID> - Semaphore signal operation\n");
    printf("C <processID> <priority> - Add new process\n");
    printf("K <processID> - Remove process from ready queue\n");
    printf("Enter 'Q' to quit.\n");

    while (1) {
        printf("Enter command: ");
        scanf(" %c", &command);

        if (command == 'Q' || command == 'q') {
            break; // Exit loop and program
        }

        switch (command) {
            case 'N':
            case 'n':
                printf("Enter semaphore ID and initial value: ");
                scanf("%d %d", &semID, &initialValue);
                if (initializeSemaphore(semID, initialValue)) {
                    printf("Semaphore %d initialized with value %d.\n", semID, initialValue);
                } else {
                    printf("Error: Failed to initialize semaphore %d.\n", semID);
                }
                break;
            case 'P':
            case 'p':
                printf("Enter semaphore ID for P operation: ");
                scanf("%d", &semID);
                semaphoreWait(semID, currentPCB);
                printf("P operation executed on semaphore %d.\n", semID);
                selectNextProcessToRun(); // Select new currentPCB if necessary
                break;
            case 'V':
            case 'v':
                printf("Enter semaphore ID for V operation: ");
                scanf("%d", &semID);
                semaphoreSignal(semID);
                printf("V operation executed on semaphore %d.\n", semID);
                if (currentPCB == NULL) {
                    selectNextProcessToRun(); // Potentially update currentPCB
                }
                break;
            case 'C':
            case 'c':
                printf("Enter process ID and priority (0-2): ");
                scanf("%d %d", &processID, &priority);
                PCB* newPCB = createProcess(processID, priority); // Assume this function creates and returns a new PCB
                addProcessToReadyQueue(newPCB);
                printf("Process %d with priority %d added.\n", processID, priority);
                if (currentPCB == NULL) {
                    selectNextProcessToRun(); // Update currentPCB if necessary
                }
                break;
            case 'K':
            case 'k':
                printf("Enter process ID to remove: ");
                scanf("%d", &processID);
                removeProcessFromReadyQueue(processID);
                printf("Process %d removed from its ready queue.\n", processID);
                break;
            default:
                printf("Invalid command.\n");
        }
    }

    printf("Program terminated.\n");
    return 0;
}