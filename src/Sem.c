#include "list.h"
#include "Queue.h"
#include "Sem.h"
#include <stdio.h>
#include <stdlib.h>

// Global semaphore list
Semaphore semaphores[5]; 

// Initialize the semaphores
void initializeSemaphores() {
    for (int i = 0; i < 5; i++) {
        semaphores[i].value = 0;
        semaphores[i].waitQueue = List_create();
        if (!semaphores[i].waitQueue) {
            fprintf(stderr, "Failed to create semaphore waiting queue.\n");
            exit(EXIT_FAILURE);
        }
    }
}

// Wait on a semaphore
void semaphoreWait(int semIndex, PCB* pcb) {
    Semaphore* sem = &semaphores[semIndex];
    sem->value--;
    if (sem->value < 0) {
        // Add PCB to the wait queue
        List_append(sem->waitQueue, pcb);
        // Change the process state to BLOCKED
        pcb->state = BLOCKED;
    }
}

// Signal a semaphore
void semaphoreSignal(int semIndex) {
    Semaphore* sem = &semaphores[semIndex];
    sem->value++;
    if (sem->value <= 0) {
        PCB* pcb = (PCB*)List_remove(sem->waitQueue);
        if (pcb != NULL) {
            // Move PCB back to a ready queue based on its priority
            addProcessToReadyQueue(pcb);
            // Change the process state to READY
            pcb->state = READY;
        }
    }
}