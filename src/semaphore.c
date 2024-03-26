#include "list.h"
#include "queues.h"
#include "semaphore.h"
#include <stdio.h>
#include <stdlib.h>

// Global semaphore list
Semaphore semaphores[5]; 

// Initialize the semaphores (N)
bool initializeSemaphore(int semID, int initialValue) {
    if (semID < 0 || semID >= sizeof(semaphores) / sizeof(semaphores[0])) {
        return false; // Invalid semaphore ID
    }
    Semaphore* sem = &semaphores[semID];
    if (sem->waitQueue != NULL) {
        return false; // Semaphore already initialized
    }
    sem->value = initialValue;
    sem->waitQueue = List_create();
    return true;
}

// Wait on a semaphore (P)
int semaphoreWait(int semID, PCB* pcb) {
    if (semID < 0 || semID >= sizeof(semaphores) / sizeof(semaphores[0])) {
        printf("Invalid semaphore ID.\n");
        exit(-1);
    }

    Semaphore* sem = &semaphores[semID];
    sem->value--;
    if (sem->value < 0) {
        // Add PCB to the wait queue
        List_append(sem->waitQueue, pcb);
        pcb->state = BLOCKED; // Change the process state to BLOCKED
    }
    
    return sem->value;
}

// Signal a semaphore (V)
int semaphoreSignal(int semID) {
    if (semID < 0 || semID >= sizeof(semaphores) / sizeof(semaphores[0])) {
        printf("Invalid semaphore ID.\n");
        exit(-1);
    }

    Semaphore* sem = &semaphores[semID];
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

    return sem->value;
}